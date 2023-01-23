#include "Socket.hpp"

// Constructors
Socket::Socket() {

}

Socket::~Socket() {

}

Socket::Socket(const Socket& src) {
	this->_sockfd = src._sockfd;
	this->_sin_size = src._sin_size;
}

Socket& Socket::operator=(const Socket& rhs) {
	this->_sockfd = rhs._sockfd;
	this->_sin_size = rhs._sin_size;
	return (*this);
}

// Helper functions
static void	*get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
	{
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// Methods
void	Socket::createSocket(int ai_family, int ai_socktype, int ai_protocol) {
	int yes = 1;

	if ((_sockfd = socket(ai_family, ai_socktype, ai_protocol)) < 0)
		throw Socket::SocketFailureException();
	if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
		throw Socket::SocketSetOptException();
	std::cout << "listenSocket set up successfully" << std::endl;
}

void	Socket::bindSocket(struct sockaddr* ai_addr, socklen_t ai_addrlen) {
	if (bind(_sockfd, ai_addr, ai_addrlen) < 0)
		throw Socket::SocketBindException();
	std::cout << "bindSocket worked successfully" << std::endl;
}

void	Socket::listenOnSocket(void) {
	if (listen(_sockfd, BACKLOG) < 0)
		throw Socket::SocketListenException();
	std::cout << "Now listening on socket #" << _sockfd << " for incoming connections" << std::endl;
}

void	Socket::acceptConnection(int listenSock) {
	if ((_sockfd = accept(listenSock, (struct sockaddr*)&_addr, &_sin_size)) < 0)
		throw Socket::SocketAcceptException();
}

void	Socket::printConnection(void) {
	char s[INET6_ADDRSTRLEN];

	inet_ntop(_addr.ss_family, get_in_addr((struct sockaddr *)&_addr), s, sizeof(s));
	std::cout << "server received connection from: " << s << std::endl;
}

void	Socket::initListenSocket(const char* port) {
	struct addrinfo	hints;
	struct addrinfo	*ai;
	struct addrinfo *p;

	// Get a socket for us and bind it
	memset(&hints, 0, sizeof(hints)); // change to ft_memset later?
	hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo(NULL, port, &hints, &ai) != 0)
		throw SocketInitException();
	for (p = ai; p != NULL; p = p->ai_next)
	{
		try {
			createSocket(p->ai_family, p->ai_socktype, p->ai_protocol);
		} catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
			continue ;
		}
		try {
			bindSocket(p->ai_addr, p->ai_addrlen);
		} catch (std::exception& e) {
            close(_sockfd); // TODO: add close protection
			std::cerr << e.what() << std::endl;
			continue ;
		}
		break ;
	}
	freeaddrinfo(ai);
	if (p == NULL)
		throw Socket::SocketBindException();
	listenOnSocket();
}

void    Socket::closeSocket(void) {
    int ret;

    ret = close(this->getSocketFD());
    if (ret < 0)
        throw SocketCloseException();
    else
        std::cout << "---SOCKET " << this->getSocketFD() << " CLOSED SUCCESSFULLY---" << std::endl;
}

// Getters
int		Socket::getSocketFD(void) {
	return (this->_sockfd);
}

// Setters
void    Socket::setSocketFD(int fd) {
    if (fd < 0)
        throw SocketAcceptException();
    this->_sockfd = fd;
}

// Exceptions
const char * Socket::SocketInitException::what() const throw ()
{
    return ("Socket failed to initialize");
}

const char * Socket::SocketFailureException::what() const throw ()
{
    return ("Socket failed to launch");
}

const char * Socket::SocketSetOptException::what() const throw ()
{
    return ("Socket failed to set options");
}

const char * Socket::SocketBindException::what() const throw ()
{
    return ("Socket could not bind to address");
}

const char * Socket::SocketListenException::what() const throw ()
{
    return ("Socket listen on this address");
}

const char * Socket::SocketAcceptException::what() const throw ()
{
    return ("Could not create new socket to accept connection");
}

const char * Socket::SocketCloseException::what() const throw ()
{
    return ("Socket failed to close properly");
}
