#include "Config.hpp"

Config::Config() {
	_client_max_body_size = 1024; // in the future is going to come from the config file
}

Config::Config(const Config& other) {
	_client_max_body_size = other._client_max_body_size;
}

Config::~Config() {
	// define some cleaning if needed
}


// Getters
unsigned long	Config::getClientMaxBodySize(void) {
	return _client_max_body_size;
}
