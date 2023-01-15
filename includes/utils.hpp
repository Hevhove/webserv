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

#endif
