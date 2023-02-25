#include "HttpVersionResponse.hpp"

// Constructors
HttpVersionResponse::HttpVersionResponse() {

}

HttpVersionResponse::~HttpVersionResponse() {

}

void    HttpVersionResponse::setHeaders() {
    setDateHeader();
    setContentTypeHeader();
    // add more headers if desired below...
    // add here...
}

void    HttpVersionResponse::constructResponse(Request& req) {
    (void)req;
    this->_status_code = HTTP_VERSION_NOT_SUPPORTED;
    this->_raw_status_line = "HTTP/1.1 505 HTTP Version Not Supported\r\n"; 
    setHeaders();
    setRawHeaders();
    this->_raw_body = "<html><body>HTTP Version Not Supported</body></html>\r\n";
    setRawResponse();
}
