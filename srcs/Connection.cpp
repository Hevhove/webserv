#include "Connection.hpp"
#include "utils.hpp"

// Constructors
Connection::Connection() {

}

Connection::~Connection() {

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
        _request.parseRequest(buf);
    } catch (std::exception e) {
        std::cout << "Error parsing request on socket " << this->getSocketFD() << ": " << e.what() << std::endl;
    }
    // testing:
    std::cout << "request was parsed: " << std::endl;
    _request.printRequest();

    // Now we need to generate a response to the HTTP request!
    _response.constructResponse();
    _response.sendResponse();
}

// Private Member Functions
