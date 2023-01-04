#include "Response.hpp"
#include "Request.hpp"
#include <fstream>
#include <string>

// Constructors
Response::Response() {
    _http_version = "HTTP/1.1";
}

Response::~Response() {

}

Response::Response(const Response& src) {
    (void)src;
}

Response& Response::operator=(const Response& rhs) {
    (void)rhs;
	return (*this);
}

// Public methods
void    Response::constructResponse(Request& req, RequestMethod req_method) {
    if (req_method == GET)
        constructGETResponse(req);
    else if (req_method == POST)
        constructPOSTResponse(req);
    else if (req_method == DELETE)
        constructDELETEResponse(req);
}

void    Response::printResponse(void) {
    std::cout << "PRINTING RESPONSE: " << std::endl;
    std::cout << _raw_status_line << _raw_headers << _raw_body << std::endl;
}

// Private methods 
void    Response::setRawHeaders() {
    std::stringstream   ss;

    for(std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
    {
        ss << it->first << ": " << it->second << "\r\n";
    }
    ss << "\r\n";
    _raw_headers = ss.str(); 
}

void    Response::setTimeHeader(void) {
    time_t now = time(nullptr); 
    struct tm* timeinfo = gmtime(&now);
    char buffer[128];

    strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S %Z", timeinfo);
    std::string time_str(buffer);
    std::cout << "Current time: " << time_str << std::endl;
    _headers.insert(std::make_pair("Current time", time_str));

}
void    Response::setContentLengthHeader() {
    std::ifstream file(_resource);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::stringstream ss;
    
    ss << content.length();
    std::string content_length = ss.str();
    _headers.insert(std::make_pair("Content-Length", ss.str()));
}

void    Response::setConnectionHeader(void) {
    _headers.insert(std::make_pair("Connection", "close"));
}

void    Response::setHeaders() {
    setTimeHeader();
    setContentLengthHeader();
    setConnectionHeader();
    // add more headers if desired below...
}

void    Response::setRawBody() {
    std::ifstream   file(_resource);
    std::string     content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    _raw_body = content;
}

void    Response::setResource(std::string path) {
    if (path[path.size() - 1] == '/')
        _resource = "public/www" + path + "index.html";
    else
        _resource = "public/www" + path;
    std::cout << "resource is " << _resource << std::endl;
    if (!ft_is_resource_available(_resource))
    {
        // TODO send a response with resource not available!
        std::cout << "resource not available!" << std::endl;
        exit(-1);
    }
    std::cout << "resource exists!" << std::endl;
}

void    Response::constructGETResponse(Request& req) {
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

std::string Response::getRawResponse(void) {
    std::string response;

    response = _raw_status_line + _raw_headers + _raw_body;
    return (response);
}

void    Response::constructPOSTResponse(Request& req) {
    (void)req; 
}

void    Response::constructDELETEResponse(Request& req) {
    (void)req; 
}
