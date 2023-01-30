/*
TODO: DOCUMENT A Response message from a client to a server includes, within the first line of that message,
the method to be applied to the resource, the identifier of the resource, and the protocol version in use. 

Example:
*/

#ifndef CLASS_NOTFOUND_RESPONSE_HPP
# define CLASS_NOTFOUND_RESPONSE_HPP

# include "Response.hpp"

class NotFoundResponse : public Response {
    private:

    public:
        // Constructors
        NotFoundResponse();
		virtual ~NotFoundResponse();
		NotFoundResponse(const NotFoundResponse& src);
		NotFoundResponse& operator=(const NotFoundResponse& rhs);
        
        // Public methods
        void        constructResponse(Request& req);
        void        setHeaders();
        void        printResponse(void);
};

#endif
