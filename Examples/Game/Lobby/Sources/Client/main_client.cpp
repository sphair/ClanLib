
#include "precomp.h"
#include "client.h"

class MainClient
{
public:
	static int main(const std::vector<std::string> &args);
};

int MainClient::main(const std::vector<std::string> &args)
{
	clan::SetupCore setup_core;
	clan::SetupDisplay setup_display;
	clan::SetupGL setup_gl;
	clan::SetupNetwork setup_network;

	clan::ConsoleWindow console("Console", 160, 1000);
	clan::ConsoleLogger logger;

	Client client;
	client.exec();

	return 0;
}

clan::Application app(&MainClient::main);
