#include "Server.hpp"
#include <iterator>
#include <ostream>
#include <sys/poll.h>

// CONSTRUCTORS
Server::Server() {
	this->_fd_count = 0;
	this->_fd_size = MAXEVENTS;
	_pfds = new pollfd[_fd_size];
	_listenSocket.initListenSocket(PORT);
	_pfds[0].fd = _listenSocket.getSocketFD();
	_pfds[0].events = POLLIN | POLLOUT;
	_fd_count++;
}

Server::~Server() {
	delete[] _pfds;
    std::map<int, Connection*>::iterator it;
    for (it = _connections.begin(); it != _connections.end(); ++it)
    {
        delete it->second;
    }
    std::cout << "server: Shutting down" << std::endl;
}

Server::Server(const Server& src) {
	this->_listenSocket = src._listenSocket;
	this->_currSocket = src._currSocket;
	this->_hints = src._hints;
	this->_servinfo = src._servinfo;
}

Server&	Server::operator=(const Server& rhs) {
	this->_listenSocket = rhs._listenSocket;
	this->_currSocket = rhs._currSocket;
	this->_hints = rhs._hints;
	this->_servinfo = rhs._servinfo;
	return (*this);
}

// PUBLIC METHODS
void	Server::run(void) {
	int poll_count;

	std::cout << "Launching server..." << std::endl;
	while (1)
	{
		if ((poll_count = poll(_pfds, _fd_count, -1)) < 0)
			throw PollException();
		// Run through the existing connections looking for data to read
        std::cout << "POLL RETURN IS " << poll_count << std::endl;
		for (int i = 0; i < _fd_count; i++)
		{
            std::cout << "Events on pfds[" << i << "].revents: " << _pfds[i].revents << std::endl;
            std::cout << "event flags are: " << _pfds[i].events << std::endl;
            // Check if descriptor has data available for reading
            if (_pfds[i].revents & POLLIN)
			{
				if (_pfds[i].fd == _listenSocket.getSocketFD())
                {
					handleNewConnection();
                    break ;
                }
				else
                {
					handleExistingConnection(i);
                }
            }
            else if (_pfds[i].revents & POLLOUT)
            {
                respondToExistingConnection(i);
                // right now just drop the connection, or we could reset the req/res to keep using the same
                dropConnection(i);
            }
		}
	}
}

// PRIVATE METHODS
void    Server::respondToExistingConnection(int i) {
    std::string     response;

    std::map<int, Connection*>::iterator it;
    for (it = _connections.begin(); it != _connections.end(); it++)
        {
            std::cout << it->first    // string (key)
                << ':'
                << it->second   // string's value 
                << std::endl;
        }

    response = _connections[_pfds[i].fd]->getRawResponse();
    if (response.length() == 0)
        return ;
    std::cout << "current raw response!!! : " << std::endl;
    std::cout << response.c_str() << std::endl;
    int bytes_sent = send(_pfds[i].fd, response.c_str(), response.size(), 0);
    std::cout << "bytes sent is " << bytes_sent << std::endl;
    if (bytes_sent < 0)
        std::cout << "some error sending" << std::endl;
    // while (bytes_sent < response.size())
    // {
    //     int n = send(_pfds[i].fd, response.c_str() + bytes_sent, response.size() - bytes_sent, 0);
    //     if (n < 0)
    //     {
    //         // TODO: throw some kind of error!
    //         break ;
    //     }
    //     bytes_sent += n;
    // }
}

void	Server::handleExistingConnection(int i) {
	int	    nbytes;
	char    buf[BUFF_SIZE];    // Buffer for client data

	memset(buf, 0, BUFF_SIZE);
    nbytes = recv(_pfds[i].fd, buf, sizeof(buf), 0);
	if (nbytes <= 0)
	{
		if (nbytes == 0)
			std::cout << "server: Socket " << _connections[_pfds[i].fd]->getSocketFD() << " hung up" << std::endl;
		else if (nbytes < 0)
			std::cout << "server: recv error" << std::endl;
		dropConnection(i);
	}
	else
	{
        _connections[_pfds[i].fd]->handleRequest(buf);
        memset(buf, 0, BUFF_SIZE);
        // display all the connections currently connected to the server
        // std::map<int, Connection*>::iterator it;
        // std::cout << "List of current connections: " << std::endl;
        // for (it = _connections.begin(); it != _connections.end(); it++)
        // {
        //     std::cout << it->first << ':' << it->second->getSocketFD() << std::endl;
        // }
	}
}

void	Server::addConnection(int newfd, Connection* new_conn) {
    // This function adds a new file descriptor to the set of pfds
	// if we don't have room, we need to realloc
	if (_fd_count == _fd_size)
	{
		_fd_size *= 2;
		struct pollfd *pfds_new = new pollfd[_fd_size];
		for (int i = 0; i < _fd_count; i++)
		{
			pfds_new[i].fd = _pfds[i].fd;
			pfds_new[i].events = _pfds[i].events;
			pfds_new[i].revents = _pfds[i].revents;
		}
		delete[] _pfds;
		_pfds = pfds_new;
	}
	_pfds[_fd_count].fd = newfd;
	_pfds[_fd_count].events = POLLIN | POLLOUT;
	_fd_count++;
    // Add a connection to the list of connections to the server
    std::cout << "adding a connection on socket with fd " << newfd << std::endl;
    _connections.insert(std::make_pair(newfd, new_conn));
}

void	Server::handleNewConnection(void) {
	socklen_t				addrlen;
	struct sockaddr_storage	remote_addr;
	char					remoteIP[INET_ADDRSTRLEN];
    Connection*             new_connection = new Connection;

	addrlen = sizeof(remote_addr);
    try {
	    new_connection->setSocketFD(accept(_listenSocket.getSocketFD(), (struct sockaddr *)&remote_addr, &addrlen));
    } catch (std::exception e) {
        std::cerr << e.what() << std::endl;
    }

	// add connection to list of existing connections in pfds
    addConnection(new_connection->getSocketFD(), new_connection);
	std::cout << "server: new connection from " << inet_ntop(remote_addr.ss_family, get_in_addr((struct sockaddr*)&remote_addr), remoteIP, INET_ADDRSTRLEN);
	std::cout << " on socket " << new_connection->getSocketFD() << std::endl;
}

void	Server::dropConnection(int i) {

    // Close the socket
    _connections[_pfds[i].fd]->closeSocket();
    // Remove from the map _connections.erase(i), careful for memory leak!
    // std::cout << "_pfds[i].fd is " << _pfds[i].fd << std::endl;
    std::map<int, Connection*>::iterator it = _connections.find(_pfds[i].fd);
    if (it != _connections.end())
    {
        delete it->second;
        _connections.erase(it);
    }
    //
	// Remove from the pfds list by copying the file descriptor of the last one over it. 
    // then reduce the number of file descriptors
    if (i != _fd_count - 1)
    {
        _pfds[i] = _pfds[_fd_count - 1];
    }
	_fd_count--;
    std::cout << "connection dropped  " << std::endl;
}

void* Server::get_in_addr(struct sockaddr *sa) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
}

// EXCEPTIONS
const char * Server::PollException::what() const throw ()
{
    return ("Failed to operate poll function");
}

const char * Server::AcceptConnectionFailure::what() const throw ()
{
    return ("Could not accept new connection");
}
