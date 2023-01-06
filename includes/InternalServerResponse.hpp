/*
A Response message from a client to a server includes, within the first line of that message,
the method to be applied to the resource, the identifier of the resource, and the protocol version in use. 

Example:
*/

#ifndef CLASS_INTERNALSERVER_RESPONSE_HPP
# define CLASS_INTERNALSERVER_RESPONSE_HPP

# include "Response.hpp"

class InternalServerResponse : public Response {
    private:

    public:
        // Constructors
        InternalServerResponse();
		virtual ~InternalServerResponse();
		InternalServerResponse(const InternalServerResponse& src);
		InternalServerResponse& operator=(const InternalServerResponse& rhs);

        // Public methods
        void        constructResponse(Request& req);
        void        setHeaders();
        void        printResponse(void);
        void        setStatusCode(StatusCode sc);
};

#endif
