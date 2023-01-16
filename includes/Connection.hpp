#ifndef CLASS_CONNECTION_HPP
# define CLASS_CONNECTION_HPP

# include "Request.hpp"
# include "Response.hpp"
# include "GetResponse.hpp"
# include "PostResponse.hpp"
# include "BadRequestResponse.hpp"
# include "HttpVersionResponse.hpp"
# include "InternalServerResponse.hpp"
# include "utils.hpp"
# include "Socket.hpp"

class Connection : public Socket {
	private:
        Request         _request;
        Response*       _response;

        // Private Member functions
        void        parse_header();
        void        parse_body();

	public:
		// Constructors
		Connection();
		virtual ~Connection();
		Connection(const Connection& src);
		Connection& operator=(const Connection& rhs);

		// Member functions
        void            handleRequest(char buf[BUFF_SIZE]);
        std::string     getRawResponse(void);
        Request&        getRequest(void);

};

#endif

