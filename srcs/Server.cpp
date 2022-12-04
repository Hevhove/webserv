#include "Server.hpp"

// CONSTRUCTORS
Server::Server() {
	;// TODO
}

Server::~Server() {
	;// TODO
}

Server::Server(const Server& src) {
	this->listenSocket = src.listenSocket;
	this->currSocket = src.currSocket;
	this->hints = src.hints;
	this->servinfo = src.servinfo;
	this->sa = src.sa;
}

Server&	Server::operator=(const Server& rhs) {
	this->listenSocket = rhs.listenSocket;
	this->currSocket = rhs.currSocket;
	this->hints = rhs.hints;
	this->servinfo = rhs.servinfo;
	this->sa = rhs.sa;
	return (*this);
}

// HELPER FUNCTIONS
void	sigchld_handler(int s)
{
	(void)s; // WHY???????????????????????????????????
	// waitpid() might overwrite errno, so we save it and restore it:
	//int saved_errno = errno;

	while (waitpid(-1, NULL, WNOHANG) > 0);
	//errno = saved_errno;
}

// METHODS
void	Server::run(void) {
	setup();
}

void	Server::setup(void) {
	int				err_val;
	struct addrinfo *p;
	char	recv_buff[BUFF_SIZE]; // 1 kiloByte

	// Fill the hints struct with information for possible connections
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;			// Accept IPv4 connections
	hints.ai_socktype = SOCK_STREAM;	// Accept over TCP (not UDP)
	hints.ai_flags = AI_PASSIVE;		// Assign the address of my local host to the socket structures.
	if ((err_val = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
	{
		std::cerr << "error: getaddrinfo: " << gai_strerror(err_val) << std::endl;
		exit(-1); // CHANGE THISS!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	}
	std::cout << servinfo->ai_canonname << std::endl;
	// If everything worked, servinfo is a linked list with sockaddr we can use
	int count = -1;
	for (p = servinfo; p != NULL; p = p->ai_next)
	{
		count++;
		try {
			listenSocket.createSocket(p->ai_family, p->ai_socktype, p->ai_protocol);
		} catch (std::exception& e) {
			std::cerr << "struct addrinfo #"<< count << ": " << e.what() << std::endl;
			continue ;
		}
		try {
			listenSocket.bindSocket(p->ai_addr, p->ai_addrlen);
		} catch (std::exception& e) {
			std::cerr << "struct addrinfo #"<< count << ": " << e.what() << std::endl;
			std::cout << p->ai_next << std::endl;
			continue ;
		}
		break ;
	}
	// When the loop is complete, we should have a socket bound to our address, we can free now
	freeaddrinfo(servinfo);
	if (p == NULL)
		throw Socket::SocketBindException();
	listenSocket.listenOnSocket();

	sa.sa_handler = sigchld_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) < 0)
	{
		perror("sigaction");
		exit(1);
	}
	std::cout << "server: waiting for connections..." << std::endl;
	while (1)
	{
		try {
			currSocket.acceptConnection(listenSocket.getSocketFD());
		} catch (std::exception& e){
			continue ; // If connection accept fails, just try again later
		}
		currSocket.printConnection();
		// Not sure how large to make the recv buffer... Explanation from link below
		// https://stackoverflow.com/questions/2862071/how-large-should-my-recv-buffer-be-when-calling-recv-in-the-socket-library
		memset(recv_buff, 0, BUFF_SIZE);
		recv(currSocket.getSocketFD(), recv_buff, BUFF_SIZE, 0);
		time_t timetoday;
		time(&timetoday);
		std::cout << "TIMESTAMP: " << std::asctime(localtime(&timetoday)) << std::endl;
		std::cout << "Received: " << recv_buff << std::endl;
		// Server response should be here:
		// server.sendResponse();
		// Fork and send HTTP/1.1 200 OK\r\n ... etc
		close(currSocket.getSocketFD());
	}
	std::cout << "Hurray we made it!" << std::endl;
}