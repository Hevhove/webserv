/*
A Response message from a client to a server includes, within the first line of that message,
the method to be applied to the resource, the identifier of the resource, and the protocol version in use. 

Example:
*/

#ifndef CLASS_BADREQUEST_RESPONSE_HPP
# define CLASS_BADREQUEST_RESPONSE_HPP

# include "Response.hpp"

class BadRequestResponse : public Response {
    private:

    public:
        // Constructors
        BadRequestResponse();
		virtual ~BadRequestResponse();
        
        // Public methods
        void        constructResponse(Request& req);
        void        setHeaders();
        void        printResponse(void);
        void        setStatusCode(StatusCode sc);
};

#endif
