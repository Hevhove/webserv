#ifndef CLASS_CONFIG_HPP
# define CLASS_CONFIG_HPP

# include "utils.hpp"

class Config {
private:
	unsigned long	_client_max_body_size;
	const char*		_listening_port;
	std::map<StatusCode, std::string>	_default_error_pages; 

public:
	Config();
	Config(const Config& other);
	~Config();

	Config&	operator=(const Config& other);

	unsigned long	getClientMaxBodySize(void);
	const char*		getListeningPort(void);
	std::string		getDefaultErrorPage(StatusCode status_code);
};

#endif
