#include "webserv.hpp"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: ./webserv [configuration file]" << std::endl;
		return (1);
	}
    // Config check and parsing
	Config config;
    try {
        std::string fileName(argv[1]);
        config.checkConfig(fileName);
        // config.parseConfig();
    } catch (std::exception& e){
        std::cerr << "Failure to load config file: " << e.what() << std::endl;
        exit(-1);
    }
    config.printConfig();
    std::cout << config._server_blocks[0] << std::endl;

    // Server loading
	Server server(&config);
	try {
		server.run();
	} catch (std::exception& e) {
		std::cerr << "Stopping webserver: " << e.what() << std::endl;
	}
	return (0);
}
