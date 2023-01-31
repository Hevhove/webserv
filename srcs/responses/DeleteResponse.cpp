#include "DeleteResponse.hpp"
#include <string>

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
// void    DeleteResponse::executeDeleteResponse(Request& req) {
//     std::ofstream   tmpFile;
//     const char*     file_path = "tmp/delete.txt";
//     std::string     command;
//     // int             ret;
//     
//     // debugging check
//     std::cout << "raw DELETE body is " << req.getRawBody() << std::endl;
//     
//     // write the body of the POST request to a tmp file
//     tmpFile.open(file_path);
//     tmpFile << req.getRawBody();
//     tmpFile.close();
//     
//     // execute the php script with the contents of the file
//     command = "php " + _resource + " " + file_path; 
//     std::cout << "full command to be exec is: " << command << std::endl;
//     std::system(command.c_str());
//
//     // delete the tmp file
//     remove(file_path);
//     // TODO: check return value ret and throw error if fail...
// }

std::string getResourceID(std::string resource) {
    std::string numberStr;
    
    std::string::size_type pos = resource.rfind('/');
    if (pos != std::string::npos) {
        ++pos;
    }
    std::string::size_type endPos = resource.find(".jpeg", pos);
    if (endPos != std::string::npos) {
    numberStr = resource.substr(pos, endPos - pos);
    }
    return numberStr;
}

void    DeleteResponse::executeDeleteResponse(Request& req) {
    std::ofstream   tmpFile;
    std::string     file_path = "tmp/delete-entry.txt";
    std::string     command;
    std::string     resource_id; 

    // get resource id
    (void)req;
    resource_id = getResourceID(_resource);

    // write the body of the POST request to a tmp file
    tmpFile.open(file_path.c_str());
    // std::cout << "DELETE BODY" << req.getRawBody() << std::endl;
    tmpFile << "_method=DELETE&id=" + resource_id;
    tmpFile.close();
    
    // execute the php script with the contents of the file
    command = "php cgi-bin/delete-entry.php " + file_path; 
    std::system(command.c_str());

    // grab the id from the tmpFile
    // delete the relevant entry from the folder public/www/images
    std::string remove_path = "public/www/images/" + resource_id + ".jpeg";
    remove(remove_path.c_str());
    // TODO: check return of above path! does file not exist, what then?

    // delete the tmp file
    remove(file_path.c_str());
    // TODO: check return value ret and throw error if fail...
}

void    DeleteResponse::setLocationHeader(void) {
    _headers.insert(std::make_pair("Location", "/"));
}

void    DeleteResponse::setHeaders(void) {
    setDateHeader();
    // setContentLengthHeader();
    setConnectionHeader("close");
    // setContentTypeHeader();
    setLocationHeader();
    setCacheControl("no-cache");
    // add more headers if desired below...
    // add here...
}

void    DeleteResponse::constructResponse(Request& req) {
    std::string path = (req.getURI()).getPath();   
    
    setResource(req.getURI().getPath());
    _raw_status_line = _http_version + " 204 No Content" + "\r\n"; 

    // execute the php
    executeDeleteResponse(req);

    // set the headers
    setHeaders();
    setRawHeaders();

    // set response
    setRawResponse();
    std::cout << "delete response is: " << std::endl;
    std::cout << _raw_status_line << _raw_headers << _raw_body << std::endl;
    //printResponse();
}
