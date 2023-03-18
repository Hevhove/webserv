#include "ServerBlock.hpp"
#include "utils.hpp"
#include <ios>

ServerBlock::ServerBlock() {
    _GET_allowed = false;
    _POST_allowed = false;
    _DELETE_allowed = false;
    _client_max_body_size = 100000;
    _root_folder = "/";
    _index_page = "index.html";
}

ServerBlock::ServerBlock(const ServerBlock& other) {
    (void)other;
}

ServerBlock::~ServerBlock() {
	// define some cleaning if needed
}

ServerBlock&	ServerBlock::operator=(const ServerBlock& other) {
    (void)other;
	return *this;
}

// Getters
unsigned long	ServerBlock::getClientMaxBodySize(void) {
    return (_client_max_body_size);
}
std::string		ServerBlock::getListeningPort(void) {
    return (_listening_port);
}
std::string		ServerBlock::getRootFolder(void) {
    return (_root_folder);
}
std::string		ServerBlock::getIndexPage(void) {
    return (_index_page);
}
std::string		ServerBlock::getServerName(void) {
    return (_server_name);
}

// Setters
void	            ServerBlock::setClientMaxBodySize(unsigned long maxBodySize) {
    _client_max_body_size = maxBodySize;
}

void    		    ServerBlock::setListeningPort(std::string listeningPort) {
    _listening_port = listeningPort;
}
void    		    ServerBlock::setServerName(std::string serverName) {
    _server_name = serverName;
}
void    		    ServerBlock::setIndexPage(std::string indexPage) {
    _index_page = indexPage;
}
void    		    ServerBlock::setRootFolder(std::string rootFolder) {
    _root_folder = rootFolder;
}

void    		    ServerBlock::setLimitExceptFlag(std::vector<std::string> tokens) {
    for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
        if (*it == "GET")
            _GET_allowed = true;
        if (*it == "POST")
            _POST_allowed = true;
        if (*it == "DELETE")
            _DELETE_allowed = true;
    }
}

void    		    ServerBlock::setDefaultErrorPage(std::string statusCodeString, std::string filePath) {
    StatusCode sc;

    sc = stringToStatusCode(statusCodeString);
    _default_error_pages.insert(std::make_pair(sc, filePath));
}

void    ServerBlock::setNewLocationBlock(LocationBlock* lb) {
    _locationBlocks.push_back(lb);
}

// Printing
void    ServerBlock::printLocationBlocks(void) {
    for (std::vector<LocationBlock*>::iterator it = _locationBlocks.begin(); it != _locationBlocks.end(); ++it) {
        // Printing Server Blocks
        std::cout << "  LOCATION BLOCK: " << std::endl;
        std::cout << "    URL:                " << (*it)->getURL() << std::endl;
        std::cout << "    Root folder:        " << (*it)->getRootFolder() << std::endl;
        std::cout << "    Index page:         " << (*it)->getIndexPage() << std::endl;
        std::cout << "    Client max body:    " << (*it)->getClientMaxBodySize() << std::endl;
        std::cout << "    LimitExcept GET:    " << std::boolalpha << (*it)->getLimitExceptGET() << std::endl;
        std::cout << "    LimitExcept POST:   " << std::boolalpha << (*it)->getLimitExceptPOST() << std::endl;
        std::cout << "    LimitExcept DELETE: " << std::boolalpha << (*it)->getLimitExceptDELETE() << std::endl;
    }
}