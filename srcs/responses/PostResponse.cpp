#include "PostResponse.hpp"
#include <fstream>
#include <string>

// Constructors
PostResponse::PostResponse() {

}

PostResponse::~PostResponse() {

}

PostResponse::PostResponse(const PostResponse& src) : Response(src) {
    (void)src;
}

PostResponse& PostResponse::operator=(const PostResponse& rhs) {
    (void)rhs;
	return (*this);
}

// Public methods
void    PostResponse::executePostResponse(Request& req) {
    std::ofstream   tmpFile;
    const char*     file_path = "tmp/entry.txt";
    std::string     command;
    // int             ret;
    
    // debugging check
    std::cout << "raw POST body is " << req.getRawBody() << std::endl;
    
    // write the body of the POST request to a tmp file
    tmpFile.open(file_path);
    tmpFile << req.getRawBody();
    tmpFile.close();
    
    // execute the php script with the contents of the file
    command = "php " + _resource + " " + file_path; 
    std::system(command.c_str());

    // delete the tmp file
    remove(file_path);
    // TODO: check return value ret and throw error if fail...
}

// void    PostResponse::setRawBody() {
//     std::ifstream   file(_resource);
//     std::string     content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
//
//     _raw_body = content;
// }

void    PostResponse::setLocationHeader(void) {
    _headers.insert(std::make_pair("Location", "/index.html"));
}

void    PostResponse::setHeaders(void) {
    setDateHeader();
    // setContentLengthHeader();
    setConnectionHeader();
    // setContentTypeHeader();
    setLocationHeader();
    setCacheControl("no-cache");
    // add more headers if desired below...
    // add here...
}

void    PostResponse::constructResponse(Request& req) {
    std::string path = (req.getURI()).getPath();   
    
    setResource(req.getURI().getPath());
    _raw_status_line = _http_version + " 302 Found" + "\r\n"; 

    // execute the php...
    executePostResponse(req);

    // set the headers
    setHeaders();
    setRawHeaders();

    // include the body
    //setRawBody();
    setRawResponse();
    std::cout << "post response is: " << std::endl;
    std::cout << _raw_status_line << _raw_headers << _raw_body << std::endl;
    //printResponse();
}
