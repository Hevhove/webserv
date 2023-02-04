/*
    Class for our Server using poll(2).

    The server maintains a map of connections, which themselves have a list of pairs of Requests and Responses.
    When calling the poll(2) function, it monitors a set of file descriptors on a bunch of possible events.
    
    The bitmask events are the following:
    POLLIN (bit 0, value 1): This bit is set if there is data available to be read on the file descriptor.
    POLLPRI (bit 1, value 2): This bit is set if there is "out-of-band" data available to be read on the file descriptor. This data is typically used for urgent data that should be handled with higher priority than regular data.
    POLLOUT (bit 2, value 4): This bit is set if the file descriptor is ready for writing.
    POLLERR (bit 3, value 8): This bit is set if an error occurred on the file descriptor.
    POLLHUP (bit 4, value 16): This bit is set if the file descriptor has been "hung up" or disconnected. This can happen, for example, when the other end of a socket connection has closed the connection.
    POLLNVAL (bit 5, value 32): This bit is set if the file descriptor is not a valid open file descriptor.
*/
#ifndef CLASS_SERVER_HPP
# define CLASS_SERVER_HPP
 
# include "Socket.hpp"
# include "Connection.hpp"
# include "utils.hpp"
# include "Config.hpp"

class Server {
	private:
		// Vars
		Socket				        _listenSocket;
		struct addrinfo		        _hints;
		struct addrinfo*	        _servinfo;
		int					        _fd_count;
		int					        _fd_size;
		struct pollfd   	    	*_pfds;
		Config						_config;	
        std::map<int, Connection*>  _connections; 

		// Methods : Private
		void				addConnection(int newfd, Connection* new_conn);
		void				dropConnection(int i);
		void			    handleNewConnection(void);
		void			    readFromExistingConnection(int i);
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
		void	load_config(const char *fpath);

		// Exceptions
		class PollException : public std::exception {
			const char * what () const throw();
		};
		class AcceptConnectionFailure : public std::exception {
			const char * what () const throw();
		};
};

#endif
