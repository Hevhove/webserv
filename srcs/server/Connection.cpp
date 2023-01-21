#include "Connection.hpp"
#include <string>

// Constructors
Connection::Connection() {

}

Connection::~Connection() {
    // TODO: clear all of the Responses allocated on the heap
}

Connection::Connection(const Connection& src) : Socket(src) {
    (void)src;
}

Connection& Connection::operator=(const Connection& rhs) {
    (void)rhs;
	return (*this);
}

// Public Member Functions
void    Connection::handleRequest2(char buf[BUFF_SIZE]) {
    // we have something in the buffer now, we don't know what...
    // how do we know which connection we are on right now?
    //
    // Make a pair of a request pointer, and NULL (for now...)
    // ONLY IF
    //      1. The current requestResponseList is empty
    //      2. The previous request exists was fully parsed
    //          - A no-body request ends with \r\n\r\n
    //          - A bodied request has reached content-length
    //
    // We also need to keep parsing the above buffer until we reach the end of it...
    int bytes_checked = 0;
    while (bytes_checked < BUFF_SIZE)
    {
        // If the previous request was full, and we have not read the entire buffer yet,
        // we need to allocate more space to put requests
        std::cout << "size is " << requestResponseList.size() << std::endl; 
        if (requestResponseList.size() == 0
            || requestResponseList[requestResponseList.size() - 1].first->isFullyParsed()) // this means both body and header fully parsed!
        {
            std::cout << "adding new pair to reqreslist" << std::endl;
            Request* req = new Request;
            std::pair<Request*, Response*> pair = std::make_pair(req, nullptr);
            requestResponseList.push_back(pair);
        }
        std::cout << "LOL3" << std::endl;
        Request* last_req = requestResponseList[requestResponseList.size() - 1].first;
        std::cout << "LOL4" << std::endl;
        // while we have not read BUFF_SIZE bytes, there is more data to read...
        bytes_checked = last_req->parseRequest2(buf, bytes_checked);
        std::cout << "current bytes checked is: " << bytes_checked << std::endl;
    }
}

void Connection::handleRequest(char buf[BUFF_SIZE]) {
    // First, we parse the current request and check for parsing errors
    // If there is an error, send back an error response
    try {
        _request.parseRequest(buf);
    } catch (Request::BadRequestException& e) {
        std::cout << "To be implemented: bad request 400 resp " << this->getSocketFD() << ": " << e.what() << std::endl;
        _response = new BadRequestResponse();
        return ;
    } catch (Request::HttpVersionNotSupportedException& e) {
        std::cout << "To be implemented: bad request 505 resp " << this->getSocketFD() << ": " << e.what() << std::endl;
        _response = new HttpVersionResponse(); 
        return ;
    } catch (std::exception& e) {
        std::cout << "other type of parsing errors" << std::endl;
        _response = new InternalServerResponse();
    }
    // testing:
    std::cout << "request was parsed: " << std::endl;
    _request.printRequest();
}

std::string     Connection::getRawResponse2(void) {
    std::string response;
    // firs thing to do is loop over the req/resp list
    for (unsigned int i = 0; i < requestResponseList.size(); i++)
    {
        Request* req = requestResponseList[i].first;
        if (req->isFullyParsed())
        {
            try
            {
                if (req->getRequestMethod() == GET)
                {
                    requestResponseList[i].second = new GetResponse();
                }
                else if (req->getRequestMethod() == POST)
                {
                    requestResponseList[i].second = new PostResponse();
                }
                // if (req->getRequestMethod() == DELETE)
                // {
                //     requestResponseList[i].second = new DeleteResponse();
                // }
            } catch (std::exception& e) {
                // TODO
            }
            requestResponseList[i].second->constructResponse(*req);
            response = requestResponseList[i].second->getRawResponse();
            // clean up now?
            std::vector<std::pair<Request*, Response*> >::iterator it = requestResponseList.begin() + i;
            delete it->first;
            delete it->second;
            requestResponseList.erase(it);
            
            // return the response string
            // std::cout << "Response is " << response << std::endl;
            return (response);
        }
    }
    // TODO: double check: if no request was fully parsed, this returns empty str? What happens?
    return response;
}

std::string    Connection::getRawResponse(void) {
    std::string response;

    try {
        if (_request.getRequestMethod() == NOT_SET)
        {
            // here: if request wasn't set, we need to prompt the client to try again, so that we are ready!
            response += "HTTP/1.1 503 Service Unavailable\r\n";
            response += "Retry-After: 0.5\r\n\r\n";
            std::cout << "returning response of: " << response << std::endl;
            return response;
        }
        else if (_request.getRequestMethod() == GET)
            _response = new GetResponse();
        else if (_request.getRequestMethod() == POST)
        {
            _response = new PostResponse();
        }
        // else if (_request.getRequestMethod() == DELETE)
        //     _response = new DeleteResponse();
        _response->constructResponse(_request);
    } catch (std::exception& e) {
        // TODO
    }

    response = _response->getRawResponse();
    return (response);
}

Request&        Connection::getRequest(void) {
    return (_request);
}

// Private Member Functions 
