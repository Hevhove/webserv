#ifndef CLASS_ServerBlock_HPP
# define CLASS_ServerBlock_HPP

# include "utils.hpp"
# include "LocationBlock.hpp"
# include <string>

class ServerBlock {
private:
	unsigned long   _client_max_body_size;
	std::string		_listening_port;
    std::string     _root_folder;
    std::string     _index_page;
    std::string     _server_name;


	std::map<StatusCode, std::string>	_error_pages;
    std::vector<LocationBlock*>          _locationBlocks; // TODO: memory leak check

public:
	ServerBlock();
	ServerBlock(const ServerBlock& other);
	~ServerBlock();

	ServerBlock&	operator=(const ServerBlock& other);

    // Allowed methods
    bool            _GET_allowed;
    bool            _POST_allowed;
    bool            _DELETE_allowed;


    // Member Functions
    void        printLocationBlocks(void);
    std::string getErrorPath(StatusCode sc);

    // Getters
	unsigned long	getClientMaxBodySize(void);
	std::string		getListeningPort(void);
	std::string		getRootFolder(void);
	std::string		getIndexPage(void);
	std::string		getServerName(void);
    std::vector<LocationBlock*>& getLocationBlocks(void);

    // Setters
	void	            setClientMaxBodySize(unsigned long maxBodySize);
	void    		    setListeningPort(std::string listeningPort);
	void    		    setServerName(std::string serverName);
	void    		    setIndexPage(std::string indexPage);
	void    		    setRootFolder(std::string rootFolder);
	void    		    setLimitExceptFlag(std::vector<std::string> tokens);
	void    		    setDefaultErrorPage(std::string statusCode, std::string filePath);
    void                setNewLocationBlock(LocationBlock* lb);
};

#endif
