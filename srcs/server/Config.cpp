#include "Config.hpp"

Config::Config() {
	_client_max_body_size = 25000; // in the future is going to come from the config file
    _listening_port = "3490";
}

Config::Config(const Config& other) {
	_client_max_body_size = other._client_max_body_size;
}

Config::~Config() {
	// define some cleaning if needed
}

Config&	Config::operator=(const Config& other) {
	this->_client_max_body_size = other._client_max_body_size;
	return *this;
}

// Getters
unsigned long	Config::getClientMaxBodySize(void) {
	return _client_max_body_size;
}

const char *	Config::getListeningPort(void) {
	return _listening_port;
}
