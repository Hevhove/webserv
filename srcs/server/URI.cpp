#include "URI.hpp"

URI::URI() {
    //
}

URI::~URI() {
    // 
}

URI::URI(const URI& src) {
    (void)src;
}

URI& URI::operator=(const URI& rhs) {
    (void)rhs;
	return (*this);
}

// Member functions

// Getters
std::string&    URI::getPath(void) {
    return (_path);
}   

std::string&    URI::getQuery(void) {
    return (_query);
}  

// std::string&    URI::getFragment(void) {
//     return (_fragment);
// } 

// Setters
void    URI::setPath(std::string path) {
    this->_path = path; 
} 
void    URI::setQuery(std::string query) {
    this->_query = query;
}  

// void    URI::setFragment(std::string fragment) {
//     this->_fragment = fragment;
// } 

