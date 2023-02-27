/*
A Response message from a client to a server includes, within the first line of that message,
the method to be applied to the resource, the identifier of the resource, and the protocol version in use.

Example:
*/

#ifndef CLASS_CONTENTTOOLARGE_RESPONSE_HPP
# define CLASS_CONTENTTOOLARGE_RESPONSE_HPP

# include "Response.hpp"

class ContentTooLargeResponse : public Response {
    private:

    public:
        // Constructors
        ContentTooLargeResponse();
		virtual ~ContentTooLargeResponse();
		ContentTooLargeResponse(const ContentTooLargeResponse& src);
		ContentTooLargeResponse& operator=(const ContentTooLargeResponse& rhs);

        // Public methods
        void        constructResponse(Request& req);
        void        setHeaders();
        void        printResponse(void);
        void        setStatusCode(StatusCode sc);
};

#endif
