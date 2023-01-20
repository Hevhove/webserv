#include "Request.hpp"
#include <sstream>
#include <string>

// Constructors
Request::Request() {
    _unparsed_request = "";
    _raw_body = "";
    _count = 0;
    _request_method = NOT_SET;
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

// Getters
std::string     Request::getUnparsedRequest(void) {
    return _unparsed_request;
}

std::string     Request::getRawStartline(void) {
    return (_raw_start_line);
}
        
std::string     Request::getRawHeaders(void) {
    return (_raw_headers);
}


std::string     Request::getRawBody(void) {
    return (_raw_body);
}


RequestMethod   Request::getRequestMethod(void) {
    return (_request_method);
}

URI&             Request::getURI(void) {
     return (_uri);
}


std::map<std::string, std::string>  Request::getHeaders(void) {
    return (_headers);
}

// Public Member Functions
void    Request::parseRequest(char *buf)
{
    _unparsed_request += buf;
    if (_count++ == 0)
    {
        parseRequestStartLine();
        parseRequestHeaders();
    }
    // If the method has content-length, keep adding content to the body until content-length is received
    std::map<std::string, std::string>::iterator it = _headers.find("Content-Length");
    if (it != _headers.end()) // if there is a content-length, we have to parse a body
    {
        parseRequestBody();
    }
}

bool     Request::headersFullyParsed() {
    if (_unparsed_request.find("\r\n\r\n") != std::string::npos)
        return (true);
    else
        return (false);
}

bool     Request::isFullyParsed() {
    if (headersFullyParsed() && !_has_body)
        return (true);
    else if (headersFullyParsed() && _has_body && _raw_body.length() == _body_length)
    {
        return (true);
    }
    else
        return (false);
}

int     Request::parseRequest2(char *buf, int bytes_read) {
    //int         to_read = BUFF_SIZE - bytes_read;
    int         start = bytes_read;
    // are the entire headers already read in?
    // does the request already have a content-length specified?
    while (start < BUFF_SIZE)
    {
        if (!headersFullyParsed())
        {
            std::cout << "currently reading byte i: " << start << std::endl; 
            _unparsed_request += buf[start];
            if (_unparsed_request.find("\r\n\r\n") != std::string::npos) // let's say we found this at byte 200
            {
                // we should now have complete headers in _unparsed_request?
                parseRequestStartLine(); // REWRITE
                parseRequestHeaders(); // REWRITE
                std::cout << "a request was fully parsed, with following data: " << _unparsed_request << std::endl;
            }
            bytes_read++;
        }
        else // header of current req have been fully parsed. Is there a body to parse further?
        {
            // check if there is a body to parse...
            if (_has_body)
            {
                static int body_bytes_read;
                _raw_body += buf[start];
                body_bytes_read++;
                std::cout << "current body_bytes_read is" << body_bytes_read << std::endl;
                bytes_read++;
                if (static_cast<unsigned long>(body_bytes_read) == _body_length) // keep reading until we have the entire body
                {
                    std::cout << "full body of current req was read!" << std::endl;
                    break ;
                }
            }
            else
                break ; // if there is no body, and headers have been parsed, we are good too
        }
        start++;
    }
    return (bytes_read);
}

// Private Member functions
void    Request::parseRequestStartLine(void) {
    std::vector<std::string> startline_split;

    // The first line of the request should indicate METHOD, resource and HTTP tag
    _raw_start_line = _unparsed_request.substr(0, _unparsed_request.find("\r\n"));
    _unparsed_request = _unparsed_request.substr(_unparsed_request.find("\r\n") + 2);
   
    startline_split = ft_split(_raw_start_line, ' ');
    if (startline_split.size() != 3)
        throw BadRequestException();

    // Parse the RequestMethod
    if (startline_split[0] == "GET")
        _request_method = GET;
    else if (startline_split[0] == "POST")
        _request_method = POST;
    else if (startline_split[0] == "DELETE")
        _request_method = DELETE;
    else
        throw BadRequestException(); // do we need to empty buffers etc here? 
    
    // Parse the URI 
    parseURI(startline_split[1]); 

    // Parse HTTP version 
    if (startline_split[2] != "HTTP/1.1")
        throw HttpVersionNotSupportedException();
}

void    Request::parseRequestHeaders(void) {
    std::string         line;

    // Select only the raw headers | NOT SURE IF THIS LINE STILL NEEDED
    _raw_headers = _unparsed_request.substr(0, _unparsed_request.find("\r\n\r\n"));
    std::stringstream   ss(_raw_headers); 
    
    while (std::getline(ss, line, '\r'))
    {
        std::stringstream   ls(line);
        std::string         key;
        std::string         value;

        // get the key before the colon, trim leading and trailing whitespace
        std::getline(ls, key, ':');
        key.erase(key.begin(), std::find_if(key.begin(), key.end(), ft_is_non_whitespace));
        key.erase(std::find_if(key.rbegin(), key.rend(), ft_is_non_whitespace).base(), key.end());

        // get the value after the colon until the newline, trim leading and trailing whitespace  
        std::getline(ls, value);
        value.erase(value.begin(), std::find_if(value.begin(), value.end(), ft_is_non_whitespace));
        value.erase(std::find_if(value.rbegin(), value.rend(), ft_is_non_whitespace).base(), value.end());
       
        // insert the key and value into the map
        _headers.insert(std::make_pair(key, value));
    }
    // check if there is a body to parse...
    std::map<std::string, std::string>::iterator it = _headers.find("Content-Length");
    if (it != _headers.end())
    {
        _has_body = true;
        _body_length = std::stoul(it->second, NULL, 0);
    }

    // for testing purposes, we display our headers:
    // std::cout << "HEADER TEST" << std::endl;
    // for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
    //     std::cout << it->first << ": " << it->second << std::endl;
    // std::cout << "-----------------------------" << std::endl;

    // if we are dealing with a post request, we need still need the data coming after the headers, so we move the string forward 
    // if (_request_method == POST) 
    //     _unparsed_request = _unparsed_request.substr(_unparsed_request.find("\r\n\r\n") + 4);
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
    // check if it exists? if not, send 404...
    //
}

void    Request::printRequest(void)
{
    std::cout << _raw_start_line << std::endl << _raw_headers << std::endl << _raw_body << std::endl;
}

// Exceptions
const char * Request::BadRequestException::what() const throw () {
    return ("Bad request");
}

const char * Request::HttpVersionNotSupportedException::what() const throw () {
    return ("This server only accepts HTTP/1.1 requests");
}
