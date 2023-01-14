#include "Connection.hpp"

// Constructors
Connection::Connection() {

}

Connection::~Connection() {
    // TODO: clear all of the Responses allocated on the heap
}

Connection::Connection(const Connection& src) {
    (void)src;
}

Connection& Connection::operator=(const Connection& rhs) {
    (void)rhs;
	return (*this);
}

// Public Member Functions
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

std::string    Connection::getRawResponse(void) {
    std::string response;

    try {
        if (_request.getRequestMethod() == NOT_SET)
        {
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

// Private Member Functions 
