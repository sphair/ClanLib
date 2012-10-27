
#include "precomp.h"
#include "program.h"
#include <ClanLib/application.h>

using namespace clan;

Application clanapp(&Program::main);

int Program::main(const std::vector<std::string> &args)
{
	SetupCore setup_core;
	SetupDisplay setup_display;
	SetupGL setup_gl;
	SetupGUI setup_gui;

	return 0;
}
