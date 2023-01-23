/*
A Response message from a client to a server includes, within the first line of that message,
the method to be applied to the resource, the identifier of the resource, and the protocol version in use. 

Example:
    HTTP/1.1 200 OK
    Date: Mon, 27 Jul 2009 12:28:53 GMT
    Server: Apache
    Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT
    ETag: "34aa387-d-1568eb00"
    Accept-Ranges: bytes
    Content-Length: 14
    Vary: Accept-Encoding
    Content-Type: text/plain

    Hello World!
*/

#ifndef CLASS_GET_RESPONSE_HPP
# define CLASS_GET_RESPONSE_HPP

# include "Response.hpp"

class GetResponse : public Response {
    private:
    
        // Private methods
        void    setHeaders();
        void    setRawBody();

    public:
        // Constructors
        GetResponse();
		virtual ~GetResponse();
		GetResponse(const GetResponse& src);
		GetResponse& operator=(const GetResponse& rhs);
        
        // Public methods
        void        constructResponse(Request& req);
        void        printResponse(void);
        void        setStatusCode(StatusCode sc);
};

#endif
