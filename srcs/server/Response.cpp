#include "Response.hpp"
#include "Request.hpp"
#include "utils.hpp"
#include <fstream>
#include <string>

// Constructors
Response::Response() {
    _http_version = "HTTP/1.1";
}

Response::~Response() {

}

// Public methods
void    Response::printResponse(void) {
    std::cout << "PRINTING RESPONSE: " << std::endl;
    std::cout << _raw_status_line << std::endl << _raw_headers << std::endl << _raw_body << std::endl;
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

void    Response::setDateHeader(void) {
    time_t now = time(NULL);
    struct tm* timeinfo = gmtime(&now);
    char buffer[128];

    strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S %Z", timeinfo);
    std::string time_str(buffer);
    _headers.insert(std::make_pair("Date", time_str));

}

void    Response::setContentLengthHeader() {
    std::stringstream ss;

    ss << _raw_body.length();
    std::string content_length = ss.str();
    _headers.insert(std::make_pair("Content-Length", ss.str()));
}

void    Response::setConnectionHeader(std::string type) {
    _headers.insert(std::make_pair("Connection", type));
}

void    Response::setContentTypeHeader(void) {
    if (hasFileExtension(_resource, ".html") || _status_code == BAD_REQUEST || _status_code == INTERNAL_SERVER_ERROR)
        _headers.insert(std::make_pair("Content-Type", "text/html"));
    else if (hasFileExtension(_resource, ".css"))
        _headers.insert(std::make_pair("Content-Type", "text/css"));
    else if (hasFileExtension(_resource, ".ico"))
        _headers.insert(std::make_pair("Content-Type", "image/x-icon"));
    else if (hasFileExtension(_resource, ".jpeg"))
        _headers.insert(std::make_pair("Content-Type", "image/jpeg"));
    else if (hasFileExtension(_resource, ".png"))
        _headers.insert(std::make_pair("Content-Type", "image/png"));
    else if (hasFileExtension(_resource, ".json"))
        _headers.insert(std::make_pair("Content-Type", "application/json"));
}

void    Response::setCacheControl(const char* type) {
    _headers.insert(std::make_pair("Cache-Control", type));
}

void    Response::setRetryAfter(int sec) {
   std::stringstream ss;

    ss << sec;
    std::string seconds = ss.str();
    _headers.insert(std::make_pair("Retry-After", seconds));
}

void 	Response::setHost(const char* server_name) {
    _headers.insert(std::make_pair("Host", server_name));
}

void    Response::setResource(Request& req) {
    _resource = req.getResource();
    return ;
    // TODO: fix pathing in this function!
    // if (path[path.size() - 1] == '/')
    //     _resource = "public/www" + path + "index.html";
    // else if (hasFileExtension(path, ".php"))
    // {
    //     _resource = path.substr(1, std::string::npos);
    // }
    // else
    //     _resource = "public/www" + path;
    // // std::cout << "resource is " << _resource << std::endl;
    // if (!ft_is_resource_available(_resource))
    // {
    //     // TODO send a response with resource not available! --> SET UP ERROR PAGES!
    //     std::cout << "resource not available!" << _resource << std::endl;
    //     exit(-1);
    // }
}

void    Response::setRawResponse(void) {
    _raw_response = _raw_status_line + _raw_headers + _raw_body;
}

std::string Response::getRawResponse(void) {
    return (_raw_response);
}

// Setters
void    Response::setStatusCode(StatusCode sc) {
    _status_code = sc;
}
