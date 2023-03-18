#include "NotOkResponse.hpp"

// Constructors
NotOkResponse::NotOkResponse() {

}

NotOkResponse::~NotOkResponse() {

}

void    NotOkResponse::setHeaders() {
    setDateHeader();
    setContentTypeHeader();
    // add more headers if desired below...
    // add here...
}

void	NotOkResponse::constructResponse(Request& req) {
	(void)req;
}

void    NotOkResponse::constructResponseWithBody(Request& req, std::string raw_body) {
    this->_status_code = HTTP_VERSION_NOT_SUPPORTED;
	switch (req.getStatusCode()) {
		case HTTP_VERSION_NOT_SUPPORTED:
			_raw_status_line = "HTTP/1.1 505 HTTP Version Not Supported\r\n";
			break;
		case CONTENT_TOO_LARGE:
			_raw_status_line = "HTTP/1.1 413 Content Too Large\r\n";
			break;
		case NOT_FOUND:
			_raw_status_line = "HTTP/1.1 404 Not Found\r\n";
			break;
		case INTERNAL_SERVER_ERROR:
			_raw_status_line = "HTTP/1.1 500 HTTP Version Not Supported\r\n";
			break;
		default:
			// ?
			break;
	}
    setHeaders();
    setRawHeaders();
    _raw_body = raw_body;
    setRawResponse();
}
