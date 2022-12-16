#include "Connection.hpp"

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
