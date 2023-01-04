/*
A uniform resource identifier (URI) syntax defines its structure, allowing a program to understand it. 
Here’s the generic syntax of a URI: [scheme]:[//authority]hostname[:port][path][?query][#fragment]

Example:

https://www.example.co.uk:443/blog/article/search?docid=720&hl=en#dayone

*/

#ifndef CLASS_URI_HPP
# define CLASS_URI_HPP

# include <iostream>

class URI {
    private:
        // std::string     _scheme;
        // std::string     _authority;
        // std::string     _hostname;
        // int             _port;
        std::string     _path;
        std::string     _query;
        // std::string     _fragment;

    public:
        // Constructors
        URI();
		virtual ~URI();
		URI(const URI& src);
		URI&            operator=(const URI& rhs);

        // Getters
        // std::string&    getScheme(void);    
        // std::string&    getAuthority(void);    
        // std::string&    getHostName(void);    
        // std::string&    getPort(void);    
        std::string&    getPath(void);    
        std::string&    getQuery(void);    
        // std::string&    getFragment(void);    

        // Setters
        // std::string&    setScheme(void);    
        // std::string&    setAuthority(void);    
        // std::string&    setHostName(void);    
        // std::string&    setPort(void);    
        void    setPath(std::string path); 
        void    setQuery(std::string query);  
        // void    setFragment(std::string fragment); 

        // Member functions
};

#endif
