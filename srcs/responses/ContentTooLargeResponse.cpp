#include "ContentTooLargeResponse.hpp"

ContentTooLargeResponse::ContentTooLargeResponse() {

}

ContentTooLargeResponse::~ContentTooLargeResponse() {

}

void 	ContentTooLargeResponse::setHeaders() {
	setDateHeader();
	setContentTypeHeader();
}

void	ContentTooLargeResponse::constructResponse(Request& req) {
	(void)req;
	this->_status_code = CONTENT_TOO_LARGE;
	this->_raw_status_line = "HTTP/1.1 413 Content Too Large\r\n";
	setHeaders();
	setRawHeaders();
	this->_raw_body = "<html><body>Error 413: Content Too Large</body></html>\r\n";
	setRawResponse();
}
