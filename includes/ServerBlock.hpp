#ifndef CLASS_ServerBlock_HPP
# define CLASS_ServerBlock_HPP

# include "utils.hpp"
# include "LocationBlock.hpp"

class ServerBlock {
private:
	unsigned long   _client_max_body_size;
	std::string		_listening_port;
    std::string     _root_folder;
    std::string     _index_page;
    std::string     _server_name;
    bool            _GET_allowed;
    bool            _POST_allowed;
    bool            _DELETE_allowed;
	std::map<StatusCode, std::string>	_default_error_pages;
    std::vector<LocationBlock*>          _locationBlocks; // TODO: memory leak check

public:
	ServerBlock();
	ServerBlock(const ServerBlock& other);
	~ServerBlock();

	ServerBlock&	operator=(const ServerBlock& other);

    // Member Functions
    void    printLocationBlocks(void);

    // Getters
	unsigned long	getClientMaxBodySize(void);
	std::string		getListeningPort(void);
	std::string		getRootFolder(void);
	std::string		getIndexPage(void);
	std::string		getServerName(void);

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
