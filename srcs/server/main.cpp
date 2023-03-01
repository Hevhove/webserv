#include "webserv.hpp"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: ./webserv [configuration file]" << std::endl;
		return (1);
	}
	(void)argv;
	// Parse config file and store all server configs in an array of Config
	// For each Config, start a server with its own specific config
	Config config;
	Server server = Server(config);
	try {
		server.run();
	} catch (std::exception & e) {
		std::cerr << "Stopping webserver: " << e.what() << std::endl;
	}
	return (0);
}
