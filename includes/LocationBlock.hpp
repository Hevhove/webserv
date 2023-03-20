#ifndef CLASS_LocationBlock_HPP
# define CLASS_LocationBlock_HPP

# include "utils.hpp"

class LocationBlock {
private:
    std::string     _url;
	unsigned long   _client_max_body_size;
    std::string     _root_folder;
    std::string     _index_page;

    // Flags for Limit Except
    bool            _GET_allowed;
    bool            _POST_allowed;
    bool            _DELETE_allowed;

public:
	LocationBlock();
	LocationBlock(const LocationBlock& other);
	~LocationBlock();

	LocationBlock&	operator=(const LocationBlock& other);

    // Getters
	std::string		getURL(void);
	unsigned long	getClientMaxBodySize(void);
	std::string		getRootFolder(void);
	std::string		getIndexPage(void);
	bool		    getLimitExceptGET(void);
	bool		    getLimitExceptPOST(void);
	bool    		getLimitExceptDELETE(void);

    // Setters
    void                setURL(std::string URL);
	void	            setClientMaxBodySize(unsigned long maxBodySize);
	void    		    setIndexPage(std::string indexPage);
	void    		    setRootFolder(std::string rootFolder);
	void    		    setLimitExceptFlag(std::vector<std::string> tokens);
};

#endif
