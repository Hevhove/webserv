/*
A client sends an HTTP request to the server in the form of a request message,
beginning with a method, URI, and protocol version, followed by MIME-like header fields containing request modifiers,
client information, and payload metadata, an empty line to indicate the end of the header section, and finally the payload body (if any).

Example:
  GET /hello.txt HTTP/1.1
  User-Agent: curl/7.16.3 libcurl/7.16.3 OpenSSL/0.9.7l zlib/1.2.3
  Host: www.example.com
  Accept: *//*
*/

#ifndef CLASS_REQUEST_HPP
# define CLASS_REQUEST_HPP

# include "URI.hpp"
# include "utils.hpp"
#include <string>

typedef enum RequestMethod {
    GET = 0,
    POST,
    DELETE
} RequestMethod;

class Request {
    private:
        // Parsing
        std::string			_unparsed_request;		// Request text that hasn't been analyzed
	    std::string			_raw_start_line; 		// The complete request line such as: `GET / HTTP/1.1`
	    std::string			_raw_headers;           // Raw headers (general headers, response/request headers, entity headers)
	    std::string			_raw_body;              // HTTP Message Body
        int                 _count; 

        // HTTP
        RequestMethod                       _request_method; 
        URI                                 _uri; // the parsed path with scheme:[//authority]path[?query][#fragment]
        std::map<std::string, std::string>  _headers;

        // Private member functions
        void                parseRequestStartLine(void);
        void                parseRequestHeaders(void);
        void                parseRequestBody(void);
        void                parseURI(std::string uri);

    public:
        // Constructors
        Request();
		virtual ~Request();
		Request(const Request& src);
		Request&            operator=(const Request& rhs);

        // Getters
        std::string     getUnparsedRequest(void);
        std::string     getRawStartline(void);
        std::string     getRawHeaders(void);
        std::string     getRawBody(void);
        RequestMethod   getRequestMethod(void);
        URI&            getURI(void);
        std::map<std::string, std::string>  getHeaders(void);
        
        // Public member functions
        void                parseRequest(char buf[BUFF_SIZE]);
        void                printRequest(void);
   
        // Exceptions
       	class ParsingFailure : public std::exception {
        public: 
            const char * what () const throw();
		};
        class HTTPVersionMismatch : public std::exception {
        public:
			const char * what () const throw();
		};
};

#endif
