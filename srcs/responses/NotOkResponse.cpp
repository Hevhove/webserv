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
void    NotOkResponse::setRawBody() {
    std::ifstream   file(_resource.c_str());
    std::string     content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    std::cout << _resource << std::endl;
    _raw_body = content;
    setContentLengthHeader();
    // std::cout << "raw body after processing is : " << _raw_body << std::endl;
}

void	NotOkResponse::constructResponse(Request& req) {
	(void)req;
}

void    NotOkResponse::constructDefaultResponseWithBody(Request& req, std::string raw_body) {
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

static std::string statusToString(StatusCode sc) {
    switch (sc) {
        case BAD_REQUEST:
            return "400 Bad Request";
        case UNAUTHORIZED:
            return "401 Unauthorized";
        case NOT_FOUND:
            return "404 Not Found";
        case LENGTH_REQUIRED:
            return "411 Length Required";
        case CONTENT_TOO_LARGE:
            return "413 Content Too Large";
        case INTERNAL_SERVER_ERROR:
            return "500 Internal Server Error";
        case HTTP_VERSION_NOT_SUPPORTED:
            return "505 HTTP Version Not Supported";
        default:
            return "Unknown";
    }
}

void        NotOkResponse::constructConfigResponse(Request& req, std::string filePath) {
    _resource = filePath;
    _raw_status_line = _http_version + " " + statusToString(req.getStatusCode()) + "\r\n";
    setHeaders();
    setRawHeaders();
    setRawBody();
    setRawResponse();
    //printResponse();
}