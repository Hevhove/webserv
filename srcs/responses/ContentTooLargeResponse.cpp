#include "ContentTooLargeResponse.hpp"

// Constructors
ContentTooLargeResponse::ContentTooLargeResponse() {

}

ContentTooLargeResponse::~ContentTooLargeResponse() {

}

ContentTooLargeResponse::ContentTooLargeResponse(const ContentTooLargeResponse& src) : Response(src) {
    (void)src;
}

ContentTooLargeResponse& ContentTooLargeResponse::operator=(const ContentTooLargeResponse& rhs) {
    (void)rhs;
	return (*this);
}

void    ContentTooLargeResponse::setHeaders() {
    setDateHeader();
    setContentTypeHeader();
    // add more headers if desired below...
    // add here...
}

void    ContentTooLargeResponse::constructResponse(Request& req) {
    (void)req;
    this->_status_code = CONTENT_TOO_LARGE;
    this->_raw_status_line = "HTTP/1.1 413 Content Too Large\r\n";
    setHeaders();
    setRawHeaders();
    this->_raw_body = "<html><body>Content Too Large</body></html>\r\n";
    setRawResponse();
}
