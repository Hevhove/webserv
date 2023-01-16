#include "DeleteResponse.hpp"

// Constructors
DeleteResponse::DeleteResponse() {

}

DeleteResponse::~DeleteResponse() {

}

DeleteResponse::DeleteResponse(const DeleteResponse& src) : Response(src) {
    (void)src;
}

DeleteResponse& DeleteResponse::operator=(const DeleteResponse& rhs) {
    (void)rhs;
	return (*this);
}

// Public methods
void    DeleteResponse::executeDeleteResponse(Request& req) {
    std::ofstream   tmpFile;
    const char*     file_path = "tmp/delete.txt";
    std::string     command;
    // int             ret;
    
    // debugging check
    std::cout << "raw DELETE body is " << req.getRawBody() << std::endl;
    
    // write the body of the POST request to a tmp file
    tmpFile.open(file_path);
    tmpFile << req.getRawBody();
    tmpFile.close();
    
    // execute the php script with the contents of the file
    command = "php " + _resource + " " + file_path; 
    std::cout << "full command to be exec is: " << command << std::endl;
    std::system(command.c_str());

    // delete the tmp file
    remove(file_path);
    // TODO: check return value ret and throw error if fail...
}

// void    DeleteResponse::setRawBody() {
//     std::ifstream   file(_resource);
//     std::string     content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
//
//     _raw_body = content;
// }

void    DeleteResponse::setLocationHeader(void) {
    _headers.insert(std::make_pair("Location", "/index.html"));
}

void    DeleteResponse::setHeaders(void) {
    setDateHeader();
    // setContentLengthHeader();
    setConnectionHeader();
    // setContentTypeHeader();
    setLocationHeader();
    setCacheControl("no-cache");
    // add more headers if desired below...
    // add here...
}

void    DeleteResponse::constructResponse(Request& req) {
    std::string path = (req.getURI()).getPath();   
    
    setResource(req.getURI().getPath());
    _raw_status_line = _http_version + " 302 Found" + "\r\n"; 

    // execute the php...
    executeDeleteResponse(req);

    // set the headers
    setHeaders();
    setRawHeaders();

    // include the body
    //setRawBody();
    setRawResponse();
    std::cout << "delete response is: " << std::endl;
    std::cout << _raw_status_line << _raw_headers << _raw_body << std::endl;
    //printResponse();
}
