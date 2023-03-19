#include "Server.hpp"
#include <iterator>
#include <ostream>
#include <sys/poll.h>

// CONSTRUCTORS
// Server::Server() {
//     // Below is currently not being called.
// 	this->_fd_count = 0;
// 	this->_fd_size = MAXEVENTS;
// 	_pfds = new pollfd[_fd_size];

//     // Initialize the listening ports from the config
// 	_listenSocket.initListenSocket(_config.getListeningPort());
// 	_pfds[0].fd = _listenSocket.getSocketFD();
// 	_pfds[0].events = POLLIN | POLLOUT;
// 	_fd_count++;
// }

Server::Server(Config* conf) {
	this->_fd_count = 0;
	this->_fd_size = MAXEVENTS;
	_pfds = new pollfd[_fd_size];

    _config = conf;
    // Initialize the listening ports from the config
    std::cout << _config->_server_blocks[0] << std::endl;
    int i = 0;
    for (std::vector<ServerBlock*>::iterator it = _config->_server_blocks.begin(); it != _config->_server_blocks.end(); ++it) {
        std::cout << *it << std::endl;
        std::cout << "LOL" << std::endl;
        sleep(1);
        std::cout << "launching a server on port " << (*it)->getListeningPort() << std::endl;
        std::cout << "at pfds[" << i << "]" << std::endl;
        // Add a listening socket to the list
        // TODO: memory clean
        Socket* listenSock = new Socket;
        listenSock->setPortFD((*it)->getListeningPort());
        // Add relevant serverblock to listening socket!
        listenSock->setServerBlock(*it);
        _listenSockets.push_back(listenSock);
      	_listenSockets.back()->initListenSocket((*it)->getListeningPort().c_str());
    	_pfds[i].fd = _listenSockets.back()->getSocketFD();
	    _pfds[i].events = POLLIN | POLLOUT;
	    _fd_count++;
        i++;
    }
    std::cout << "Number of servers launched is: " << i << std::endl;
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

// Server::Server(const Server& src) {
// 	this->_listenSocket = src._listenSocket;
// 	this->_hints = src._hints;
// 	this->_servinfo = src._servinfo;
// }

Server&	Server::operator=(const Server& rhs) {
	this->_listenSocket = rhs._listenSocket;
	this->_hints = rhs._hints;
	this->_servinfo = rhs._servinfo;
	return (*this);
}

Socket*    Server::retrieveListeningSocket(int fd) {
    for (std::vector<Socket*>::iterator it = _listenSockets.begin(); it != _listenSockets.end(); it++){
        if (fd == (*it)->getSocketFD())
            return (*it) ;
    }
    return (NULL);
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
        // std::cout << "POLL RETURN IS " << poll_count << std::endl;
		for (int i = 0; i < _fd_count; i++)
		{
            // std::cout << "Events on pfds[" << i << "].revents: " << _pfds[i].revents << std::endl;
            // If a client sends a POLLHUP signal, it is asking to close the connection
            if (_pfds[i].revents & POLLHUP)
            {
                dropConnection(i);
            }
            // Check if descriptor has data available for reading
            else if (_pfds[i].revents & POLLIN)
			{
                Socket* listenSock = retrieveListeningSocket(_pfds[i].fd);
                if (listenSock != NULL)
                {
					handleNewConnection(listenSock);
                    break ;
                }
				else
                {
					readFromExistingConnection(i);
                }
            }
            // Or if a descriptor is ready for writing, and a parsed request is ready!
            else if (_pfds[i].revents & POLLOUT)
            {
                /*
                    Because of keep-alive connection, this event will trigger a lot...
                    as long as client keeps the connection open!
                */
                respondToExistingConnection(i);
            }
		}
	}
}

// PRIVATE METHODS
void    Server::respondToExistingConnection(int i) {
    std::string     response;

    try {
        response = _connections[_pfds[i].fd]->getRawResponse();
    } catch (std::exception& e) {
        // TODO
    }
    if (response.empty())
        return ;
    // std::cout << "response to be sent: " << response << std::endl;

    // Cast response to a buffer
    char* buffer = new char[response.size()];
    std::memcpy(buffer, response.c_str(), response.size());
    int bytes_sent = send(_pfds[i].fd, buffer, response.size(), 0);

    // std::cout << "bytes sent on " << _pfds[i].fd << " is " << bytes_sent << std::endl;

    // Temporary hack for redirection issue:
    if (response.find("302 Found") != std::string::npos
        || response.find("404 Not Found") != std::string::npos
        || response.find("204 No Content") != std::string::npos
        || response.find("413 Content Too Large") != std::string::npos
		)
        dropConnection(i);
    delete[] buffer;

    // TODO: implement a throw exception for code below
    if (bytes_sent < 0)
        std::cout << "some error sending" << std::endl;
}

void	Server::readFromExistingConnection(int i) {
	int	    nbytes;
	char    buf[BUFF_SIZE];

	memset(buf, 0, BUFF_SIZE);
    std::cout << "reading from pfds[" << i << "]" << &_pfds[i] << std::endl;
    nbytes = recv(_pfds[i].fd, buf, sizeof(buf), 0);
	if (nbytes <= 0)
	{
		if (nbytes == 0)
			std::cout << "---SOCKET " << _connections[_pfds[i].fd]->getSocketFD() << " HUNG UP---" << std::endl;
		else if (nbytes < 0)
			std::cout << "server: recv error" << std::endl;
		dropConnection(i);
	}
	else
	{
        _connections[_pfds[i].fd]->handleRequest(buf);
        memset(buf, 0, BUFF_SIZE);
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
    _connections.insert(std::make_pair(newfd, new_conn));
}

void	Server::handleNewConnection(Socket* listenSock) {
	socklen_t				addrlen;
	struct sockaddr_storage	remote_addr;
	char					remoteIP[INET_ADDRSTRLEN];
    Connection*             new_connection = new Connection(_config);

	addrlen = sizeof(remote_addr);
    try {
        // Set the socket for the connection
	    new_connection->setSocketFD(accept(listenSock->getSocketFD(), (struct sockaddr *)&remote_addr, &addrlen));
        // Add server block config to this connection
        new_connection->setServerBlock(listenSock->getServerBlock());
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

	// add connection to list of existing connections in pfds
    addConnection(new_connection->getSocketFD(), new_connection);
	std::cout << "---NEW CONNECTION: " << inet_ntop(remote_addr.ss_family, get_in_addr((struct sockaddr*)&remote_addr), remoteIP, INET_ADDRSTRLEN);
	std::cout << " on socket " << new_connection->getSocketFD();
    std::cout << " over port " << new_connection->getServerBlock()->getListeningPort() << "---" << std::endl;
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
    std::cout << "---CONNECTION DROPPED---" << std::endl;
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
