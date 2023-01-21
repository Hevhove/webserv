#include "GetResponse.hpp"
#include "utils.hpp"
#include <cstddef>
#include <fstream>
#include <string>

// Constructors
GetResponse::GetResponse() {

}

GetResponse::~GetResponse() {

}

GetResponse::GetResponse(const GetResponse& src) : Response(src) {
    (void)src;
}

GetResponse& GetResponse::operator=(const GetResponse& rhs) {
    (void)rhs;
	return (*this);
}

std::string processPhp(std::string res) {
    std::string command;
    std::string result;

    command = "php " + res;
    FILE *in = popen(command.c_str(), "r");

    if (in) {
        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), in)) {
            result += buffer;
        }
        pclose(in);
    }
    return (result);
}

// Public methods
void    GetResponse::setRawBody() {
    std::ifstream   file(_resource.c_str());
    std::string     content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    //
    // _raw_body = content;
    // std::cout << "resource is " << _resource << std::endl;
    // _raw_body = processPhpInHtml(_resource);
    //_raw_body = executePhpBlock(content);
    if (hasFileExtension(_resource, ".html") || hasFileExtension(_resource, ".php"))
    {
        _raw_body = processPhp(_resource);
    }
    else
    {
        _raw_body = content;
    }
    setContentLengthHeader();
    // std::cout << "raw body after processing is : " << _raw_body << std::endl;
}

void    GetResponse::setHeaders() {
    setDateHeader();
    //setContentLengthHeader();
    setConnectionHeader();
    setContentTypeHeader();
    setRetryAfter(2);
    // add more headers if desired below...
    // add here...
}

void    GetResponse::constructResponse(Request& req) {
    std::string path = (req.getURI()).getPath();   
    
    setResource(req.getURI().getPath());
    _raw_status_line = _http_version + " 200 OK" + "\r\n"; 
       
    // set the headers
    setHeaders();

    // include the body
    setRawBody();
    setRawHeaders();
    setRawResponse();
    //printResponse();
}
