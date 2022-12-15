#include "Server.hpp"

// CONSTRUCTORS
Server::Server() {
	this->fd_count = 0;
	this->fd_size = MAXEVENTS;
	pfds = new pollfd[fd_size];

	listenSocket.initListenSocket();
	pfds[0].fd = listenSocket.getSocketFD();
	pfds[0].events = POLLIN;
	fd_count++;
}

Server::~Server() {
	delete[] pfds;
	std::cout << "server: Shutting down" << std::endl;
}

Server::Server(const Server& src) {
	this->listenSocket = src.listenSocket;
	this->currSocket = src.currSocket;
	this->hints = src.hints;
	this->servinfo = src.servinfo;
}

Server&	Server::operator=(const Server& rhs) {
	this->listenSocket = rhs.listenSocket;
	this->currSocket = rhs.currSocket;
	this->hints = rhs.hints;
	this->servinfo = rhs.servinfo;
	return (*this);
}

// PUBLIC METHODS
void	Server::run(void) {
	int poll_count;

	std::cout << "Launching server..." << std::endl;
	while (1)
	{
		if ((poll_count = poll(pfds, fd_count, -1)) < 0)
		{
			std::cout << "heh" << std::endl;
			throw PollException();
		}
		// Run through the existing connections looking for data to read
		for (int i = 0; i < fd_count; i++)
		{
			// Check if descriptor has data available
			if (pfds[i].revents & POLLIN)
			{
				if (pfds[i].fd == listenSocket.getSocketFD())
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

	sender_fd = pfds[i].fd;
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
	if (fd_count == fd_size)
	{
		fd_size *= 2;
		struct pollfd *pfds_new = new pollfd[fd_size];
		for (int i = 0; i < fd_count; i++)
		{
			pfds_new[i].fd = pfds[i].fd;
			pfds_new[i].events = pfds[i].events;
			pfds_new[i].revents = pfds[i].revents;
		}
		delete[] pfds;
		pfds = pfds_new;
	}
	pfds[fd_count].fd = newfd;
	pfds[fd_count].events = POLLIN;
	fd_count++;
}

void	Server::dropConnection(int i) {
	// copy the last over this one.
	pfds[i] = pfds[fd_count - 1];
	fd_count--;
}

void* Server::get_in_addr(struct sockaddr *sa)
{
    return &(((struct sockaddr_in*)sa)->sin_addr);
}

void	Server::handleConnection(void) {
	socklen_t				addrlen;
	int						newfd;
	struct sockaddr_storage	remote_addr;
	char					remoteIP[INET_ADDRSTRLEN];

	addrlen = sizeof(remote_addr);
	newfd = accept(listenSocket.getSocketFD(), (struct sockaddr *)&remote_addr, &addrlen);
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
