#include "GetResponse.hpp"

// Constructors
GetResponse::GetResponse() {

}

GetResponse::~GetResponse() {

}

GetResponse::GetResponse(const GetResponse& src) {
    (void)src;
}

GetResponse& GetResponse::operator=(const GetResponse& rhs) {
    (void)rhs;
	return (*this);
}

// Public methods
void    GetResponse::setRawBody() {
    std::ifstream   file(_resource);
    std::string     content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    _raw_body = content;
}

void    GetResponse::setHeaders() {
    setDateHeader();
    setContentLengthHeader();
    setConnectionHeader();
    setContentTypeHeader();
    // add more headers if desired below...
    // add here...
}

void    GetResponse::constructResponse(Request& req) {
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
