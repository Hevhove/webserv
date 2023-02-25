#include "Request.hpp"
#include "utils.hpp"
#include <sstream>
#include <string>

// Constructors
Request::Request() {
    _unparsed_request = "";
    _raw_body = "";
    _request_method = NOT_SET;
    _body_bytes_read = 0;
}

Request::~Request() {

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

int     Request::parseRequest(char *buf, int bytes_read, Config config) {
    int         start = bytes_read;

    /*
    *   Read in a request byte per byte. Part of the request could be in the next/previous call to send(2)
    *   of the main server loop. For this reason we keep adding into the unparsed request until
    *   there is a fully parsed request, or until a Content-Length body has been received.
    */
    while (start < BUFF_SIZE)
    {
        // First check if all the headers of the current request are parsed!
        if (!headersFullyParsed())
        {
            // std::cout << "currently reading byte i: " << start << std::endl; 
            _unparsed_request += buf[start];
            if (_unparsed_request.find("\r\n\r\n") != std::string::npos) // let's say we found this at byte 200
            {
                // we should now have complete headers in _unparsed_request?
                parseRequestStartLine();
                parseRequestHeaders(config);
                printRequest();
            }
            bytes_read++;
        }
        // If there is a body to parse, execute below code
        else 
        {
            if (_has_body)
            {
                _raw_body += buf[start];
                _body_bytes_read++;
                // std::cout << "current body_bytes_read is" << _body_bytes_read << std::endl;
                bytes_read++;
                if (static_cast<unsigned long>(_body_bytes_read) == _body_length) // keep reading until we have the entire body
                {
                    break ;
                }
            }
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
    
    // trim possible garbage values before the startline from a previous read
    std::string keys[] = {"GET", "POST", "DELETE"};
    std::size_t         pos = _raw_start_line.length();
    for (int i = 0; i < 3; i++)
    {
        std::size_t keyPos = _raw_start_line.find(keys[i]);
        if (keyPos != std::string::npos && keyPos < pos)
            pos = keyPos;
    }
    if (pos != _raw_start_line.length())
        _raw_start_line = _raw_start_line.substr(pos);

    // Remove the startline from the request
    _unparsed_request = _unparsed_request.substr(_unparsed_request.find("\r\n") + 2);
  
    // std::cout << "raw_startline is " << _raw_start_line << std::endl;
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
    if (!ft_is_resource_available("public/www/" + _uri.getPath()) && !hasFileExtension(_uri.getPath(), ".php"))
        throw NotFoundException();

    // Parse HTTP version 
    if (startline_split[2] != "HTTP/1.1")
        throw HttpVersionNotSupportedException();
}

void    Request::parseRequestHeaders(Config config) {
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
        _body_length = std::strtoul(it->second.c_str(), NULL, 0);
		if (_body_length > config.getClientMaxBodySize()) {
			throw Request::BodyTooBigException();
		}
    }

    // for testing purposes, we display our headers:
    // std::cout << "HEADER TEST" << std::endl;
    // for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
    //     std::cout << it->first << ": " << it->second << std::endl;
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
    // std::cout << "parsed path is " <<_uri.getPath() << std::endl;
    // std::cout << "parsed query is " <<_uri.getQuery() << std::endl;

    // TODO: if path does not exist, send 404!
}

void    Request::printRequest(void)
{
    std::cout << std::endl << "---REQUEST PARSED---" << std::endl;
    std::cout << _raw_start_line << std::endl << _raw_headers << std::endl;
    std::cout << "---END OF REQUEST---" << std::endl << std::endl;
}

// Exceptions
const char * Request::BadRequestException::what() const throw () {
    return ("Bad request");
}

const char * Request::NotFoundException::what() const throw () {
    return ("Not found");
}

const char * Request::HttpVersionNotSupportedException::what() const throw () {
    return ("This server only accepts HTTP/1.1 requests");
}

const char * Request::BodyTooBigException::what() const throw () {
	return ("Request body is too long ");
}
