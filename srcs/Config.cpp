#include "Config.hpp"
// might need a Config instance for each server {} scope

Config::Config() {
    // This would get the default; it can go wrong so it must go 
    // through all the checks as well
}

Config::Config(const char *fpath) {
    // open file -> error can occur
    // check format -> error can occur
    // check content -> error can occur
    // get content -> can be stored in a hasmap (for example)
    // save content in Config instance
}

// Private methdos to implement
// - check format -> open parenthesys, missing ; (?), ...
// - check content -> check if minimal information is there, ... ??

Config::~Config() {
    // need to free whatever is on the heap
}

Config::Config(const Config& rhs) {

}

Config& Config::operator=(const Config& rhs) {

}

const char * Config::ConfigWrongFormat::what() const throw() {
    return ("Configuration files is not well formatted");
}
