#include "Server.hpp"

// CONSTRUCTORS
Server::Server() {
	this->_fd_count = 0;
	this->_fd_size = MAXEVENTS;
	_pfds = new pollfd[_fd_size];

	_listenSocket.initListenSocket();
	_pfds[0].fd = _listenSocket.getSocketFD();
	_pfds[0].events = POLLIN;
	_fd_count++;
}

Server::~Server() {
	delete[] _pfds;
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
		{
			std::cout << "heh" << std::endl;
			throw PollException();
		}
		// Run through the existing connections looking for data to read
		for (int i = 0; i < _fd_count; i++)
		{
			// Check if descriptor has data available
			if (_pfds[i].revents & POLLIN)
			{
				if (_pfds[i].fd == _listenSocket.getSocketFD())
					handleConnection();
				else
					handleRequest(i);
			}
		}
	}
}

// PRIVATE METHODS
void	Server::handleRequest(int i) {
	int	nbytes;
	int sender_fd;
	char buf[BUFF_SIZE];    // Buffer for client data

	sender_fd = _pfds[i].fd;
	memset(buf, 0, BUFF_SIZE);
	nbytes = recv(sender_fd, buf, sizeof(buf), 0);
	if (nbytes <= 0)
	{
		if (nbytes == 0)
			std::cout << "server: Socket " << sender_fd << " hung up" << std::endl;
		else if (nbytes < 0)
			std::cout << "server: recv error" << std::endl;
		close(sender_fd);
		dropConnection(i);
	}
	else
	{
		// we got some data hurrah!
		// TODO: parsing -> put inside of a request class
		// But for now, let's just print what we received
		std::cout << "Received: " << buf << std::endl;
	}
}

void	Server::addConnection(int newfd) {
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
	_pfds[_fd_count].events = POLLIN;
	_fd_count++;
}

void	Server::dropConnection(int i) {
	// copy the last over this one.
	_pfds[i] = _pfds[_fd_count - 1];
	_fd_count--;
}

void* Server::get_in_addr(struct sockaddr *sa) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
}

void	Server::handleConnection(void) {
	socklen_t				addrlen;
	int						newfd;
	struct sockaddr_storage	remote_addr;
	char					remoteIP[INET_ADDRSTRLEN];

	addrlen = sizeof(remote_addr);
	newfd = accept(_listenSocket.getSocketFD(), (struct sockaddr *)&remote_addr, &addrlen);
	try {
		if (newfd < 0)
			throw AcceptConnectionFailure();
	} catch (std::exception& e){
		std::cerr << e.what() << std::endl;
		return ;
	}
	// add connection to list of existing connections
	addConnection(newfd);
	std::cout << "server: new connection from " << inet_ntop(remote_addr.ss_family, get_in_addr((struct sockaddr*)&remote_addr), remoteIP, INET_ADDRSTRLEN);
	std::cout << " on socket " << newfd << std::endl;
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
