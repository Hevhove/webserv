#ifndef CLASS_CONNECTION_HPP
# define CLASS_CONNECTION_HPP

# include "Request.hpp"
# include "Response.hpp"
# include "GetResponse.hpp"
# include "PostResponse.hpp"
# include "DeleteResponse.hpp"
# include "BadRequestResponse.hpp"
# include "NotFoundResponse.hpp"
# include "HttpVersionResponse.hpp"
# include "InternalServerResponse.hpp"
# include "utils.hpp"
# include "Socket.hpp"
# include "Config.hpp"

class Connection : public Socket {
	private:
        std::vector<std::pair<Request*, Response*> >	requestResponseList;
		Config											_config;

        // Private Member functions
        void        parse_header();
        void        parse_body();

	public:
		
        // Constructors
		Connection();
		Connection(Config config);
		virtual ~Connection();
		Connection(const Connection& src);
		Connection& operator=(const Connection& rhs);

		// Member functions
        void            handleRequest(char buf[BUFF_SIZE]);
        std::string     getRawResponse(void);
};

#endif
