#include "LocationBlock.hpp"
#include "utils.hpp"

LocationBlock::LocationBlock() {
    _GET_allowed = false;
    _POST_allowed = false;
    _DELETE_allowed = false;
    _client_max_body_size = 100000;
    _root_folder = "/";
    _index_page = "index.html";
}

LocationBlock::LocationBlock(const LocationBlock& other) {
    (void)other;
}

LocationBlock::~LocationBlock() {
	// define some cleaning if needed
}

LocationBlock&	LocationBlock::operator=(const LocationBlock& other) {
    (void)other;
	return *this;
}

// Getters
std::string		LocationBlock::getURL(void) {
    return (_url);
}

unsigned long	LocationBlock::getClientMaxBodySize(void) {
    return (_client_max_body_size);
}

std::string		LocationBlock::getRootFolder(void) {
    return (_root_folder);
}

std::string		LocationBlock::getIndexPage(void) {
    return (_index_page);
}

bool		    LocationBlock::getLimitExceptGET(void) {
    return _GET_allowed;
}

bool		    LocationBlock::getLimitExceptPOST(void) {
    return _POST_allowed;
}

bool    		LocationBlock::getLimitExceptDELETE(void) {
    return _DELETE_allowed;
}

// Setters
void                LocationBlock::setURL(std::string URL) {
    _url = URL;
}

void	            LocationBlock::setClientMaxBodySize(unsigned long maxBodySize) {
    _client_max_body_size = maxBodySize;
}

void    		    LocationBlock::setIndexPage(std::string indexPage) {
    _index_page = indexPage;
}
void    		    LocationBlock::setRootFolder(std::string rootFolder) {
    _root_folder = rootFolder;
}

void    		    LocationBlock::setLimitExceptFlag(std::vector<std::string> tokens) {
    for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
        if (*it == "GET")
            _GET_allowed = true;
        if (*it == "POST")
            _POST_allowed = true;
        if (*it == "DELETE")
            _DELETE_allowed = true;
    }
}