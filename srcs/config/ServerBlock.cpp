#include "ServerBlock.hpp"
#include "utils.hpp"
#include <ios>

ServerBlock::ServerBlock() {
    _GET_allowed = true;
    _POST_allowed = true;
    _DELETE_allowed = true;
    _client_max_body_size = 100000;
    _root_folder = "public/www";
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
    int get_check = 0;
    int post_check = 0;
    int delete_check = 0;

    for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
        if (*it == "GET")
        {
            _GET_allowed = true;
            get_check = 1;
        }
        if (*it == "POST")
        {
            _POST_allowed = true;
            post_check = 1;
        }
        if (*it == "DELETE")
        {
            _DELETE_allowed = true;
            delete_check = 1;
        }
    }
    if (get_check == 0)
        _GET_allowed = false;
    if (post_check == 0)
        _POST_allowed = false;
    if (delete_check == 0)
        _DELETE_allowed = false;
}

void    		    ServerBlock::setDefaultErrorPage(std::string statusCodeString, std::string filePath) {
    StatusCode sc;

    sc = stringToStatusCode(statusCodeString);
    _error_pages.insert(std::make_pair(sc, filePath));
}

void    ServerBlock::setNewLocationBlock(LocationBlock* lb) {
    _locationBlocks.push_back(lb);
}

// Member Functions
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

std::string ServerBlock::getErrorPath(StatusCode sc) {
    std::string filePath;
    std::map<StatusCode, std::string>::iterator it = _error_pages.find(sc);

    // returns the string of the file location OR empty if not found
    if (it != _error_pages.end())
        return (it->second);
    else
        return std::string();
}
std::vector<LocationBlock*>& ServerBlock::getLocationBlocks(void) {
    return (_locationBlocks);
}