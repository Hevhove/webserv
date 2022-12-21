#include "Request.hpp"

// Constructors
Request::Request() {

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
