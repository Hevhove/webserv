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

# include "URI.hpp"
# include "utils.hpp"
#include <string>

typedef enum StatusCode {
    // 2xx (successful)
    OK = 200,
    CREATED = 201,
    NO_CONTENT = 204,

    // 4xx (client error)
    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    LENGTH_REQUIRED = 411,
    CONTENT_TOO_LARGE = 413,
    UNSUPPORTED_MEDIA_TYPE = 415,

    // 5xx (server error)
    INTERNAL_SERVER_ERROR = 500,
    BAD_GATEWAY = 502,
    HTTP_VERSION_NOT_SUPPORTED = 505
} StatusCode;

class Response {
    private:
        // to construct for sending
        std::string                         _raw_status_line;
        std::string                         _raw_headers;
        std::string                         _raw_body;

        // 
        std::string                         _http_version;
        StatusCode                          _status_code;
        std::string                         _status_string;
        std::map<std::string, std::string>  _headers;
        
    public:
        // Constructors
        Response();
		virtual ~Response();
		Response(const Response& src);
		Response& operator=(const Response& rhs);

};

#endif
