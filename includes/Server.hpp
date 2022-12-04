#ifndef CLASS_SERVER_HPP
# define CLASS_SERVER_HPP

# include "Socket.hpp"
# include <signal.h>
# include <stdio.h>
# include <cstring>
# include <unistd.h>
# include <time.h>
# include <sys/wait.h>
# include <stdlib.h>

# define PORT "3490"

class Server {
	private:
		// Vars
		Socket				listenSocket;
		Socket				currSocket;
		struct addrinfo		hints;
		struct addrinfo*	servinfo;
		struct sigaction	sa;

		// Methods
		void				setup(void);

	public:
		// Constructors
		Server();
		virtual ~Server();
		Server(const Server& src);
		Server& operator=(const Server& rhs);

		// Methods
		void	run(void);
};

#endif