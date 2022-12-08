/*
A uniform resource identifier (URI) syntax defines its structure, allowing a program to understand it. 
Here’s the generic syntax of a URI: scheme:[//authority]path[?query][#fragment]

Example:

https://www.example.co.uk:443/blog/article/search?docid=720&hl=en#dayone

*/

#ifndef CLASS_URI_HPP
# define CLASS_URI_HPP

# include <iostream>

class URI {
    private:

    public:
        std::string     hostname;
        int             port;
        std::string     path;
        std::string     query;
        std::string     fragment;

};

#endif