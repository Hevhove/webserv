#ifndef CLASS_CONFIG_HPP 
# define CLASS_CONFIG_HPP

# include "utils.hpp"

class Config {
    private:
        int _port;

    public:
        Config();
        Config(const char *fpath)
        ~Config();
        Config(const Config& src);
        Config& operator=(const Config& rhs);

    // Exceptions
    class ConfigWrongFormat : public std::exception {
        const char *what() const throw();
    };
}

#endif
