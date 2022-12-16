#ifndef UTILS_HPP
# define UTILS_HPP

// Macros
# define PORT "3490"
# define BACKLOG 10
# define BUFF_SIZE 1000
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

// IO
# include <unistd.h>
# include <stdlib.h>
# include <iostream>
# include <stdio.h>

// Strings
# include <string>
# include <cstring>
# include <string.h>

// Time
# include <time.h>

#endif
