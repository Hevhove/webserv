#include "Config.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdlib>

// Constructors
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
    (void)other;
}

Config::~Config() {
	// define some cleaning if needed
}

Config&	Config::operator=(const Config& other) {
	this->_client_max_body_size = other._client_max_body_size;
	// how to copy a map? -> for _default_error_pages
	return *this;
}

// Member functions
bool checkBlocks(std::string fileName) {
    std::ifstream file(fileName.c_str());
    if (!file.is_open()) {
        return false;
    }

    int open_braces_count = 0;
    int close_braces_count = 0;
    std::string line;

    while (std::getline(file, line)) {
        std::string::iterator iter = line.begin();
        std::string::iterator end = line.end();
        while (iter != end) {
            char& c = *iter;
            if (c == '{') {
                open_braces_count++;
            } else if (c == '}') {
                close_braces_count++;
            }
            ++iter;
        }
    }
    return open_braces_count == close_braces_count;
}
std::string trim(std::string s) {
    std::string::size_type start = s.find_first_not_of(" \t\r\n");
    std::string::size_type end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

void    parseServerBlockLine(ServerBlock* currServerBlock, std::vector<std::string> tokens)
{
    if (tokens.at(0) == "listen")
    {
        currServerBlock->setListeningPort(tokens.at(1));
    }
    if (tokens.at(0) == "client_max_body_size")
    {
        currServerBlock->setClientMaxBodySize(strtoul(tokens.at(1).c_str(), NULL, 10));
    }
    else if (tokens.at(0) == "server_name")
    {
        currServerBlock->setServerName(tokens.at(1));
    }
    else if (tokens.at(0) == "index")
    {
        currServerBlock->setIndexPage(tokens.at(1));
    }
    else if (tokens.at(0) == "root")
    {
        currServerBlock->setRootFolder(tokens.at(1));
    }
    else if (tokens.at(0) == "error_pages")
    {
        currServerBlock->setDefaultErrorPage(tokens.at(1), tokens.at(2));
    }
    else if (tokens.at(0) == "limit_except")
    {
        currServerBlock->setLimitExceptFlag(tokens);
    }
}

void    parseLocationBlockLine(LocationBlock* currLocationBlock, std::vector<std::string> tokens) {
    if (tokens.at(0) == "index")
    {
        currLocationBlock->setIndexPage(tokens.at(1));
    }
    else if (tokens.at(0) == "root")
    {
        currLocationBlock->setRootFolder(tokens.at(1));
    }
    else if (tokens.at(0) == "limit_except")
    {
        currLocationBlock->setLimitExceptFlag(tokens);
    }
    else if (tokens.at(0) == "client_max_body_size")
    {
        currLocationBlock->setClientMaxBodySize(strtoul(tokens.at(1).c_str(), NULL, 10));
    }
}

void    Config::parseLocationBlock(std::ifstream& file, std::vector<std::string> tokens, ServerBlock* sb) {
    // Add a locationblock to the serverblock
    LocationBlock* lb = new LocationBlock;
    sb->setNewLocationBlock(lb);
    lb->setURL(tokens.at(1));

    // Set allowed keys for inside location block
    std::vector<std::string> allowedKeys;
    allowedKeys.push_back("index");
    allowedKeys.push_back("root");
    allowedKeys.push_back("limit_except");
    allowedKeys.push_back("client_max_body_size");

    // Parse the Location Block
    std::string line;
    while (std::getline(file, line)) {
        // Remove leading and trailing whitespace
        line = trim(line);
        // Ignore empty lines and lines starting with comments or brackets
        if (line.empty()){
            std::cout << "EMPTY" << std::endl;
        }
        if (line.empty() || line[0] == '#') {
            continue;
        }
        if (line[0] == '}')
            return ;
        // Split the line into tokens based on whitespace
        std::stringstream ss(line);
        std::vector<std::string> location_tokens;
        std::string location_token;
        while (std::getline(ss, location_token, ' ')) {
            // Remove leading and trailing whitespace from each token
            location_token = trim(location_token);
            if (!location_token.empty()) {
                location_tokens.push_back(location_token);
            }
        }
        // Check if the first token is allowed
        if (std::find(allowedKeys.begin(), allowedKeys.end(), location_tokens[0]) == allowedKeys.end()) {
            std::cout << "Unrecognized Location token: " << location_tokens[0] << std::endl;
            throw DirectiveDoesNotExistException();
        }
        if (line.find("}") == std::string::npos)
        {
            parseLocationBlockLine(lb, location_tokens);
        }
        else // if "}" then we are at end of location block
            return ;
    }
}

bool Config::checkDirectives(std::string fileName) {

    std::ifstream file(fileName.c_str());
    if (!file.is_open()) {
        return false;
    }

    std::vector<std::string> allowedKeys;
    allowedKeys.push_back("http");
    allowedKeys.push_back("server");
    allowedKeys.push_back("listen");
    allowedKeys.push_back("server_name");
    allowedKeys.push_back("index");
    allowedKeys.push_back("root");
    allowedKeys.push_back("location");
    allowedKeys.push_back("limit_except");
    allowedKeys.push_back("client_max_body_size");
    allowedKeys.push_back("error_pages");

    std::string line;
    while (std::getline(file, line)) {
        // Remove leading and trailing whitespace
        line = trim(line);

        // Ignore empty lines and lines starting with comments or brackets
        if (line.empty()){
            std::cout << "EMPTY" << std::endl;
        }
        if (line.empty() || line[0] == '#' || line[0] == '{' || line[0] == '}') {
            continue;
        }

        // Split the line into tokens based on whitespace
        std::stringstream ss(line);
        std::vector<std::string> tokens;
        std::string token;
        while (std::getline(ss, token, ' ')) {
            // Remove leading and trailing whitespace from each token
            token = trim(token);
            if (!token.empty()) {
                tokens.push_back(token);
            }
        }

        // Check if the first token is allowed
        if (std::find(allowedKeys.begin(), allowedKeys.end(), tokens[0]) == allowedKeys.end()) {
            std::cout << "Unrecognized token: " << tokens[0] << std::endl;
            return false;
        }
        // Parse other tokens
        if (tokens.at(0) == "server")
        {
            ServerBlock* sb = new ServerBlock();
            _server_blocks.push_back(sb);
        }
        if (tokens.at(0) != "http" && tokens.at(0) != "server")
        {
            if (tokens.at(0) == "location")
            {
                parseLocationBlock(file, tokens, _server_blocks.back());
            }
            else
                parseServerBlockLine(_server_blocks.back(), tokens);
        }
        // Check if there are any extra tokens on the line
        // if (tokens.size() > 1) {
        //     std::cout << "Extra tokens on line: " << line << std::endl;
        //     return false;
        // }
    }

    return true;
}

bool Config::checkConfig(std::string fileName) {
    // Block checks
    if (!checkBlocks(fileName))
        throw MissingBracketsException();

    // Check directive names;
    if (!checkDirectives(fileName))
        throw DirectiveDoesNotExistException();

    return true;
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

// TODO: duplicate ports and server_names checks

void    Config::printConfig(void) {
    std::cout << "** PRINTING PARSED CONFIG **" << std::endl;
    for (std::vector<ServerBlock*>::iterator it = _server_blocks.begin(); it != _server_blocks.end(); ++it) {
        // Printing Server Blocks
        std::cout << "SERVER BLOCK:       " << std::endl;
        std::cout << "  Server name:      " << (*it)->getServerName() << std::endl;
        std::cout << "  Listening port:   " << (*it)->getListeningPort() << std::endl;
        std::cout << "  Root folder:      " << (*it)->getRootFolder() << std::endl;
        std::cout << "  Index page:       " << (*it)->getIndexPage() << std::endl;
        std::cout << "  Client max body:  " << (*it)->getClientMaxBodySize() << std::endl;
        // Printing Nested Location Blocks
        (*it)->printLocationBlocks();
        std::cout << std::endl;
    }
}

// Exceptions
const char * Config::FileDoesNotExistException::what() const throw ()
{
    return ("Failed to open file");
}

const char * Config::DirectiveDoesNotExistException::what() const throw ()
{
    return ("A directive does not exist");
}

const char * Config::MissingBracketsException::what() const throw ()
{
    return ("Brackets are missing");
}
