#include "PostResponse.hpp"

// Constructors
PostResponse::PostResponse() {

}

PostResponse::~PostResponse() {

}

PostResponse::PostResponse(const PostResponse& src) {
    (void)src;
}

PostResponse& PostResponse::operator=(const PostResponse& rhs) {
    (void)rhs;
	return (*this);
}

// Public methods
void    PostResponse::setRawBody() {
    std::ifstream   file(_resource);
    std::string     content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    _raw_body = content;
}

void    PostResponse::setHeaders() {
    setDateHeader();
    setContentLengthHeader();
    setConnectionHeader();
    setContentTypeHeader();
    // add more headers if desired below...
    // add here...
}

void    PostResponse::constructResponse(Request& req) {
    std::string path = (req.getURI()).getPath();   
    
    setResource(req.getURI().getPath());
    _raw_status_line = _http_version + " 200 OK" + "\r\n"; 
       
    // set the headers
    setHeaders();
    setRawHeaders();

    // include the body
    setRawBody();
    //printResponse();
}
