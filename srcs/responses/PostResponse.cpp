#include "PostResponse.hpp"
#include <fstream>
#include <string>

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
// std::string parseQuery(Request& req) {
//     std::string query;
//
// }
//

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
    exit(-1);
}


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
    
    // execute the php...
    executePostResponse(req);

    // set the headers
    setHeaders();
    setRawHeaders();

    // include the body
    setRawBody();
    setRawResponse();
    //printResponse();
}
