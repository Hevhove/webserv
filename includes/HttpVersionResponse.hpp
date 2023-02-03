/*
A Response message from a client to a server includes, within the first line of that message,
the method to be applied to the resource, the identifier of the resource, and the protocol version in use. 

Example:
*/

#ifndef CLASS_HTTPVERSION_RESPONSE_HPP
# define CLASS_HTTPVERSION_RESPONSE_HPP

# include "Response.hpp"

class HttpVersionResponse : public Response {
    private:

    public:
        // Constructors
        HttpVersionResponse();
		virtual ~HttpVersionResponse();

        // Public methods
        void        constructResponse(Request& req);
        void        setHeaders();
        void        printResponse(void);
        void        setStatusCode(StatusCode sc);
};

#endif
