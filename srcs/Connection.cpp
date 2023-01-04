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
        //std::cout << "buff test" << buf << std::endl;
        _request.parseRequest(buf);
    } catch (Request::ParsingFailure e) {
        std::cout << "To be implemented: bad request 400 resp " << this->getSocketFD() << ": " << e.what() << std::endl;
        // send a BAD_REQUEST 400 back to t
    } catch (Request::HTTPVersionMismatch f) {
        std::cout << "To be implemented: bad request 505 resp " << this->getSocketFD() << ": " << f.what() << std::endl;
        // send a 505
    } catch (std::exception g) {
        std::cout << "other type of parsing errors" << std::endl;
    }
    // testing:
    std::cout << "request was parsed: " << std::endl;
    _request.printRequest();

    // if the request was parsed and was valid, do what was asked!
    // possible functions here (POST, DELETE): uploadAsset, deleteAsset, ..
    // For GET requests, we only need to construct the body and send it

    // after executing the request, generate a response
    _response.constructResponse(_request, _request.getRequestMethod());
}

std::string    Connection::getRawResponse(void) {
    std::string response;

    response = _response.getRawResponse();
    return (response);
}

// Private Member Functions 
