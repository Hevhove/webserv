#include "Socket.hpp"

// Constructors
Socket::Socket() {

}

Socket::~Socket() {

}

Socket::Socket(const Socket& src) {
	this->sockfd = src.sockfd;
	this->sin_size = src.sin_size;
}

Socket& Socket::operator=(const Socket& rhs) {
	this->sockfd = rhs.sockfd;
	this->sin_size = rhs.sin_size;
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

	if ((sockfd = socket(ai_family, ai_socktype, ai_protocol)) < 0)
		throw Socket::SocketFailureException();
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
		throw Socket::SocketSetOptException();
}

void	Socket::bindSocket(struct sockaddr* ai_addr, socklen_t ai_addrlen) {
	if (bind(sockfd, ai_addr, ai_addrlen) < 0)
		throw Socket::SocketBindException();
}

void	Socket::listenOnSocket(void) {
	if (listen(sockfd, BACKLOG) < 0)
		throw Socket::SocketListenException();
}

void	Socket::acceptConnection(int listenSock) {
	if ((sockfd = accept(listenSock, (struct sockaddr*)&addr, &sin_size)) < 0)
		throw Socket::SocketAcceptException();
}

void	Socket::printConnection(void) {
	char s[INET6_ADDRSTRLEN];

	inet_ntop(addr.ss_family, get_in_addr((struct sockaddr *)&addr), s, sizeof(s));
	std::cout << "server received connection from: " << s << std::endl;
}



// Getters
int		Socket::getSocketFD(void) {
	return (this->sockfd);
}


// Exceptions
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