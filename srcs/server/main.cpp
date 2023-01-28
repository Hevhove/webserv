#include "utils.hpp"
#include "Server.hpp"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: ./webserv [configuration file]" << std::endl;
		return (1);
	}
	Server server;
	// Use argv[1] to load config file onto server
	// server.load_config(argv[1]);
	(void)argv;
	try {
		server.run();
	} catch (std::exception & e) {
		std::cerr << "Stopping webserver: " << e.what() << std::endl;
	}
	return (0);
}
