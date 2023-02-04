#ifndef CLASS_CONFIG_HPP
# define CLASS_CONFIG_HPP

class Config {
private:
	unsigned long _client_max_body_size;

public:
	Config();
	Config(const Config& other);
	~Config();

	unsigned long	getClientMaxBodySize(void);
};

#endif
