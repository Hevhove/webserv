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

# include "AMessage.hpp"
# include "URI.hpp"

class Request : public AMessage {
    private:
        std::string     _unparsed_path;
        URI             _uri; // the parsed path with scheme:[//authority]path[?query][#fragment]
        

    public:
        // Constructors
        Request();
		virtual ~Request();
		Request(const Request& src);
		Request& operator=(const Request& rhs);

};

#endif