#include "Config.hpp"

Config::Config() {
	_client_max_body_size = 25000; // in the future is going to come from the config file
	_listening_port = "3491";
	_default_error_pages[BAD_REQUEST] = "BAD_REQUEST\r\n";
	_default_error_pages[UNAUTHORIZED] = "UNAUTHORIZED\r\n";
    _default_error_pages[BAD_REQUEST] = "BAD_REQUEST\r\n";
    _default_error_pages[UNAUTHORIZED] = "UNAUTHORIZED\r\n";
    _default_error_pages[FORBIDDEN] = "FORBIDDEN\r\n";
    _default_error_pages[NOT_FOUND] = "NOT_FOUND\r\n\r\n";
    _default_error_pages[LENGTH_REQUIRED] = "LENGTH_REQUIRED\r\n";
    _default_error_pages[CONTENT_TOO_LARGE] = "<html><body>Error 413: Content Too Large - this is a test</body></html>\r\n";
    _default_error_pages[UNSUPPORTED_MEDIA_TYPE] = "UNSUPPORTED_MEDIA_TYPE\r\n";
    _default_error_pages[INTERNAL_SERVER_ERROR] = "INTERNAL_SERVER_ERROR\r\n";
    _default_error_pages[HTTP_VERSION_NOT_SUPPORTED] = "HTTP_VERSION_NOT_SUPPORTED\r\n";
}

Config::Config(const Config& other) {
	_client_max_body_size = other._client_max_body_size;
}

Config::~Config() {
	// define some cleaning if needed
}

Config&	Config::operator=(const Config& other) {
	this->_client_max_body_size = other._client_max_body_size;
	// how to copy a map? -> for _default_error_pages
	return *this;
}

// Getters
unsigned long	Config::getClientMaxBodySize(void) {
	return _client_max_body_size;
}

const char *	Config::getListeningPort(void) {
	return _listening_port;
}

std::string		Config::getDefaultErrorPage(StatusCode status_code)
{
	return _default_error_pages[status_code];
}
