#include "NotFoundResponse.hpp"

// Constructors
NotFoundResponse::NotFoundResponse() {

}

NotFoundResponse::~NotFoundResponse() {

}

void    NotFoundResponse::setHeaders() {
    setDateHeader();
    setContentTypeHeader();
    // add more headers if desired below...
    // add here...
}

void    NotFoundResponse::constructResponse(Request& req) {
    (void)req;
    this->_status_code = BAD_REQUEST;
    this->_raw_status_line = "HTTP/1.1 404 Not Found\r\n"; 
    setHeaders();
    setRawHeaders();
    this->_raw_body = "<html><body>Error 404: Not Found</body></html>\r\n";
    setRawResponse();
}
