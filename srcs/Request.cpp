#include "Request.hpp"
#include <sstream>
#include <string>

// Constructors
Request::Request() {
    _unparsed_request = "";
    _raw_body = "";
}

Request::~Request() {

}

Request::Request(const Request& src) {
    (void)src;
}

Request& Request::operator=(const Request& rhs) {
    (void)rhs;
	return (*this);
}

// Public Member Functions
void    Request::parseRequest(char *buf)
{
    static int i;

    _unparsed_request += buf;
    if (i++ == 0)
    {
        parseRequestStartLine();
        parseRequestHeaders();
    }
    // If the method is a POST request, keep adding content to the body until content-length is received
    // The way of processing the body depends on the "Content-type" header...
    if (_request_method == POST)
        parseRequestBody();
}

// Private Member functions
void    Request::parseRequestStartLine(void) {
    std::vector<std::string> startline_split;

    // The first line of the request should indicate METHOD, resource and HTTP tag
    _raw_start_line = _unparsed_request.substr(0, _unparsed_request.find("\r\n"));
    _unparsed_request = _unparsed_request.substr(_unparsed_request.find("\r\n") + 2);
   
    startline_split = ft_split(_raw_start_line, ' ');
    if (startline_split.size() != 3)
        throw ParsingFailure();

    // Parse the RequestMethod
    if (startline_split[0] == "GET")
        _request_method = GET;
    else if (startline_split[0] == "POST")
        _request_method = POST;
    else if (startline_split[0] == "DELETE")
        _request_method = DELETE;
    else
        throw ParsingFailure(); // do we need to empty buffers etc here? 
    
    // Parse the URI 
    parseURI(startline_split[1]); 

    // Parse HTTP version 
    if (startline_split[2] != "HTTP/1.1")
        throw HTTPVersionMismatch();

    // Testing
    if (_request_method == GET)
        std::cout << "request method is GET" << std::endl;
}

void    Request::parseRequestHeaders(void) {
    std::string         line;

    // Select only the raw headers
    _raw_headers = _unparsed_request.substr(0, _unparsed_request.find("\r\n\r\n"));
    std::stringstream   ss(_raw_headers); 
    
    while (std::getline(ss, line, '\r'))
    {
        std::stringstream   ls(line);
        std::string         key;
        std::string         value;

        // get the key before the colon, trim leading and trailing whitespace
        std::getline(ls, key, ':');
        key.erase(key.begin(), std::find_if(key.begin(), key.end(), is_non_whitespace));
        key.erase(std::find_if(key.rbegin(), key.rend(), is_non_whitespace).base(), key.end());

        // get the value after the colon until the newline, trim leading and trailing whitespace  
        std::getline(ls, value);
        value.erase(value.begin(), std::find_if(value.begin(), value.end(), is_non_whitespace));
        value.erase(std::find_if(value.rbegin(), value.rend(), is_non_whitespace).base(), value.end());
       
        // insert the key and value into the map
        _headers.insert(std::make_pair(key, value));
    }
    // for testing purposes, we display our headers:
 //    std::cout << "HEADER TEST" << std::endl;
 //    for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
 //        std::cout << it->first << ": " << it->second << std::endl;
 // 
    // if we are dealing with a post request, we need still need the data coming after the headers, so we move the string forward 
    if (_request_method == POST) 
        _unparsed_request = _unparsed_request.substr(_unparsed_request.find("\r\n\r\n") + 4);
}

void    Request::parseRequestBody(void) {
    _raw_body += _unparsed_request;
}

void    Request::parseURI(std::string uri) {
    // find the path until the query, or the end of the string
    _uri.setPath(uri.substr(0, uri.find('?')));
    
    // if there is a query, set it
    if (uri.find('?') != std::string::npos)
        _uri.setQuery(uri.substr(uri.find('?') + 1, (uri.find('#'))));
    std::cout << "parsed path is " <<_uri.getPath() << std::endl;
    std::cout << "parsed query is " <<_uri.getQuery() << std::endl;
}

void    Request::printRequest(void)
{
    std::cout << _raw_start_line << _raw_headers << _raw_body << std::endl;
}

// Exceptions
const char * Request::ParsingFailure::what() const throw () {
    return ("Request could not be parsed");
}

const char * Request::HTTPVersionMismatch::what() const throw () {
    return ("This server only accepts HTTP/1.1 requests");
}
