/*
A Response message from a server to client on a DELETE request can be :
    - 204: No Content
    - 302: Found
    - 400: Bad Request
    - 411: Length Required
    - 413: Content too large
    - 415: Unsupported media type
    - 500: Internal Server Error
    - 505: HTTP Version Not Supported

Example:

HTTP/1.1 204 No Content\r\n
Server: MyServer\r\n
Date: Mon, 14 Jan 2023 12:00:00 GMT\r\n
Content-Length: 0\r\n
\r\n

*/
#ifndef CLASS_DELETE_RESPONSE_HPP
# define CLASS_DELETE_RESPONSE_HPP

# include "Response.hpp"

class DeleteResponse : public Response {
    private:
        // Private methods
        void    setHeaders();
        void    setRawBody();
        void    setLocationHeader(void);

    public:
        // Constructors
        DeleteResponse();
		virtual ~DeleteResponse();

        // Public methods
        void        constructResponse(Request& req);
        void  	  	constructDefaultResponseWithBody(Request& req, std::string raw_body);
        void        constructConfigResponse(Request& req, std::string filePath);
        void        executeDeleteResponse(Request& req);
        void        printResponse(void);
        void        setStatusCode(StatusCode sc);
};

#endif
