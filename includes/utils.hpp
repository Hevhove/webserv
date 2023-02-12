#ifndef UTILS_HPP
# define UTILS_HPP

// Macros
# define PORT "3490" // to be changed later with config file
# define BACKLOG 10
# define BUFF_SIZE 2000
# define MAXEVENTS 10

// Libraries
// Networking
# include <poll.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/wait.h>
# include <netdb.h>
# include <arpa/inet.h>

// Containers
# include <vector>
# include <map>

// Error management
# include <errno.h>
# include <exception>
# include <cstddef>

// IO
# include <unistd.h>
# include <stdlib.h>
# include <iostream>
# include <stdio.h>
# include <fstream>
# include <fcntl.h>

// Strings
# include <string>
# include <cstring>
# include <string.h>
# include <sstream>

// Algo
# include <algorithm>

// Time
# include <ctime>
# include <time.h>

// Functions
std::vector<std::string>    ft_split(const std::string &str, char delim);
bool                        ft_is_non_whitespace(int ch); 
bool                        ft_is_resource_available(std::string resource);
bool                        hasFileExtension(const std::string& s, const std::string& ext);

// enum
typedef enum StatusCode {
    // 2xx (successful)
    OK = 200, // successful GET request
    CREATED = 201, // successful POST request
    NO_CONTENT = 204, // successful DELETE request

    // 3xx (redirections)
    FOUND = 302, // used after successful POST, reload main table

    // 4xx (client error)
    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    LENGTH_REQUIRED = 411,
    CONTENT_TOO_LARGE = 413,
    UNSUPPORTED_MEDIA_TYPE = 415,

    // 5xx (server error)
    INTERNAL_SERVER_ERROR = 500,
    HTTP_VERSION_NOT_SUPPORTED = 505
} StatusCode;

typedef enum RequestMethod {
    GET = 0,
    POST,
    DELETE,
    NOT_SET
} RequestMethod;

#endif
