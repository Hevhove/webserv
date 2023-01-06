#include "Connection.hpp"
#include "utils.hpp"

// Constructors
Connection::Connection() {

}

Connection::~Connection() {
    // TODO: clear all of the Responses allocated on the heap
    delete _response;
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
    // If we are in the beginning of receiving the request, the header
    // should be included with details on type of request, else reject
    try {
        //std::cout << "buff test" << buf << std::endl;
        _request.parseRequest(buf);
    } catch (Request::BadRequestException& e) {
        std::cout << "To be implemented: bad request 400 resp " << this->getSocketFD() << ": " << e.what() << std::endl;
        _response = new BadRequestResponse();
        return ;
    } catch (Request::HttpVersionNotSupportedException& f) {
        std::cout << "To be implemented: bad request 505 resp " << this->getSocketFD() << ": " << f.what() << std::endl;
        // _response = new HttpVersionResponse(); 
    } catch (std::exception& g) {
        std::cout << "other type of parsing errors" << std::endl;
    }
    // itesting:
    std::cout << "request was parsed3: " << std::endl;
    _request.printRequest();

    try {
        if (_request.getRequestMethod() == GET)
            _response = new GetResponse();
        else if (_request.getRequestMethod() == POST)
            _response = new PostResponse();
    // else if (_request.getRequestMethod() == DELETE)
    //     _response = new DeleteResponse();
        _response->constructResponse(_request);
    } catch (std::exception& h) {

    }
}

std::string    Connection::getRawResponse(void) {
    std::string response;

    response = _response->getRawResponse();
    return (response);
}

// Private Member Functions 
