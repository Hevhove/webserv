#ifndef CLASS_SERVER_HPP
# define CLASS_SERVER_HPP
 
# include "Socket.hpp"
# include "Connection.hpp"
# include "utils.hpp"

class Server {
	private:
		// Vars
		Socket				        _listenSocket;
        Socket				        _currSocket; // TODO : check if delete needed
		struct addrinfo		        _hints;
		struct addrinfo*	        _servinfo;
		int					        _fd_count;
		int					        _fd_size;
		struct pollfd   	    	*_pfds;
        std::map<int, Connection*>  _connections; 

		// Methods : Private
		void				addConnection(int newfd, Connection* new_conn);
		void				dropConnection(int i);
		void			    handleNewConnection(void);
		void			    handleExistingConnection(int i);
        void                respondToExistingConnection(int i);
		void*			    get_in_addr(struct sockaddr *sa);

	public:
		// Constructors
		Server();
		virtual ~Server();
		Server(const Server& src);
		Server& operator=(const Server& rhs);

		// Methods : Public
		void	run(void);

		// Exceptions
		class PollException : public std::exception {
			const char * what () const throw();
		};
		class AcceptConnectionFailure : public std::exception {
			const char * what () const throw();
		};
};

#endif
