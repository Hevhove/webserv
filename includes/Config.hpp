#ifndef CLASS_CONFIG_HPP
# define CLASS_CONFIG_HPP

# include "utils.hpp"
# include "ServerBlock.hpp"

class Config {
private:

    // Legacy below, leave for now.
	unsigned long   _client_max_body_size;
	const char*		_listening_port;
    //const char*     _root_folder;
    // const char*     _index_page;
    // const char*     _server_name;
	std::map<StatusCode, std::string>	_default_error_pages;

public:
    std::vector<ServerBlock*>    _server_blocks;

	Config();
	Config(const Config& other);
	~Config();

	Config&	operator=(const Config& other);

    // Parsing
    bool    checkConfig(std::string confFile);
    bool    checkDirectives(std::string fileName);
    void    parseLocationBlock(std::ifstream& file, std::vector<std::string> tokens, ServerBlock* sb);
    void    printConfig(void);
    //void    parseConfig(void);

    // Getters
    std::vector<ServerBlock*>& getServerBlocks(void);

    // Legacy
	unsigned long	getClientMaxBodySize(void);
	const char*		getListeningPort(void);
	std::string		getDefaultErrorPage(StatusCode status_code);

    // Exceptions
    class FileDoesNotExistException : public std::exception {
        const char * what () const throw();
    };
    class DirectiveDoesNotExistException : public std::exception {
        const char * what () const throw();
    };
    class MissingBracketsException : public std::exception {
        const char * what () const throw();
    };
};

#endif
