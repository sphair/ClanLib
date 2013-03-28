
#include "precomp.h"
#include "program.h"
#include <ClanLib/application.h>

using namespace clan;

Application clanapp(&Program::main);

int Program::main(const std::vector<std::string> &args)
{
	SetupCore setup_core;

	ConsoleWindow window("SWRender Compiler Output", 80, 25);

	window.display_close_message();
	return 0;
}
