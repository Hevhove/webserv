#include "Config.hpp"

Config::Config() {
	_client_max_body_size = 1024; // in the future is going to come from the config file
}

Config::~Config() {
	// define some cleaning if needed
}
