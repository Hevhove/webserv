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

#ifndef CLASS_RESPONSE_HPP
# define CLASS_RESPONSE_HPP

# include "Request.hpp"
# include "URI.hpp"
# include "utils.hpp"
# include <string>

class Response {
    protected:
        // to construct for sending
        std::string                         _raw_status_line;
        std::string                         _raw_headers;
        std::string                         _raw_body;
        std::string                         _raw_response;

        // contents 
        std::string                         _http_version;
        StatusCode                          _status_code;
        std::string                         _status_string;
        std::map<std::string, std::string>  _headers;

        std::string                         _resource;
    
        // functions to set headers 
        void    setRawHeaders(void); 
        void    setDateHeader(void); 
        void    setContentLengthHeader(); 
        void    setConnectionHeader(std::string type);
        void    setContentTypeHeader(); 
        void    setCacheControl(const char* type);
        void    setRetryAfter(int sec);

        void    setRawResponse(void);
        void    setResource(std::string path);

    public:
        // Constructors
        Response();
		virtual ~Response();
		Response(const Response& src);
        
        // Public methods
        virtual void    constructResponse(Request& req) = 0;
        virtual void    setHeaders() = 0;

        std::string     getRawResponse(void);
        std::string     getRawResponse2(void);
        void            printResponse(void);
        void            setStatusCode(StatusCode sc);
};

#endif
