#ifndef CLASS_CONFIG_HPP
# define CLASS_CONFIG_HPP

class Config {
private:
	unsigned long	_client_max_body_size;
	const char*		_listening_port;

public:
	Config();
	Config(const Config& other);
	~Config();

	Config&	operator=(const Config& other);

	unsigned long	getClientMaxBodySize(void);
	const char*		getListeningPort(void);
};

#endif
