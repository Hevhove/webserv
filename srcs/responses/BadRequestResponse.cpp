#include "BadRequestResponse.hpp"

// Constructors
BadRequestResponse::BadRequestResponse() {

}

BadRequestResponse::~BadRequestResponse() {

}

void    BadRequestResponse::setHeaders() {
    setDateHeader();
    setContentTypeHeader();
    // add more headers if desired below...
    // add here...
}

void    BadRequestResponse::constructResponse(Request& req) {
    (void)req;
    this->_status_code = BAD_REQUEST;
    this->_raw_status_line = "HTTP/1.1 400 Bad Request\r\n"; 
    setHeaders();
    setRawHeaders();
    this->_raw_body = "<html><body>Bad Request</body></html>\r\n";
    setRawResponse();
}
