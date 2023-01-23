#include "InternalServerResponse.hpp"

// Constructors
InternalServerResponse::InternalServerResponse() {

}

InternalServerResponse::~InternalServerResponse() {

}

InternalServerResponse::InternalServerResponse(const InternalServerResponse& src) : Response(src) {
    (void)src;
}

InternalServerResponse& InternalServerResponse::operator=(const InternalServerResponse& rhs) {
    (void)rhs;
	return (*this);
}

void    InternalServerResponse::setHeaders() {
    setDateHeader();
    setContentTypeHeader();
    // add more headers if desired below...
    // add here...
}

void    InternalServerResponse::constructResponse(Request& req) {
    (void)req;
    this->_status_code = INTERNAL_SERVER_ERROR;
    this->_raw_status_line = "HTTP/1.1 500 HTTP Version Not Supported\r\n"; 
    setHeaders();
    setRawHeaders();
    this->_raw_body = "<html><body>Internal Server Error</body></html>\r\n";
    setRawResponse();
}
