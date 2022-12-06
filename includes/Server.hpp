#ifndef CLASS_SERVER_HPP
# define CLASS_SERVER_HPP

# include "Socket.hpp"
# include "utils.hpp"
# include <signal.h>
# include <stdio.h>
# include <cstring>
# include <time.h>
# include <sys/wait.h>
# include <stdlib.h>
# include <poll.h>

# define MAXEVENTS 10

class Server {
	private:
		// Vars
		Socket				listenSocket;
		Socket				currSocket;
		struct addrinfo		hints;
		struct addrinfo*	servinfo;
		struct sigaction	sa;
		int					fd_count;
		int					fd_size;
		struct pollfd		*pfds;


		// Methods : TODO: review private vs public methods
		void				addConnection(int newfd);
		void				dropConnection(int i);

	public:
		// Constructors
		Server();
		virtual ~Server();
		Server(const Server& src);
		Server& operator=(const Server& rhs);

		// Methods
		void	run(void);
		void	handleConnection(void);
		void	handleRequest(int i);

		// Exceptions
		class PollException : public std::exception {
			const char * what () const throw();
		};
		class AcceptConnectionFailure : public std::exception {
			const char * what () const throw();
		};

};

#endif