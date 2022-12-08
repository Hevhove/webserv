#ifndef CLASS_SOCKET_HPP
# define CLASS_SOCKET_HPP

# include "utils.hpp"

class Socket {
	private:
		int						sockfd;
		socklen_t				sin_size;
		struct sockaddr_storage	addr;

	public:
		// Constructors
		Socket();
		virtual ~Socket();
		Socket(const Socket& src);
		Socket& operator=(const Socket& rhs);

		// Methods
		void	createSocket(int ai_family, int ai_socktype, int ai_protocol);
		void	bindSocket(struct sockaddr* ai_addr, socklen_t ai_addrlen);
		void	listenOnSocket(void);
		void	acceptConnection(int listenSock);
		void	printConnection(void);
		void	initListenSocket(void);

		// Getters
		int		getSocketFD(void);

		// Exceptions
		class SocketInitException : public std::exception {
			const char * what () const throw();
		};
		class SocketFailureException : public std::exception {
			const char * what () const throw();
		};
		class SocketSetOptException : public std::exception {
			const char * what () const throw();
		};
		class SocketBindException : public std::exception {
			const char * what () const throw();
		};
		class SocketListenException : public std::exception {
			const char * what () const throw();
		};
		class SocketAcceptException : public std::exception {
			const char * what () const throw();
		};
};

#endif