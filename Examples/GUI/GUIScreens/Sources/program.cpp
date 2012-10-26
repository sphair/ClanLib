
#include "precomp.h"
#include "program.h"
#include "example.h"

CL_ClanApplication clanapp(&Program::main);

int Program::main(const std::vector<CL_String> &args)
{
	CL_SetupCore setup_core;
	CL_SetupDisplay setup_display;
	CL_SetupGL setup_gl;
	CL_SetupGUI setup_gui;

	try
	{
		Example example;
		return example.exec();
	}
	catch(CL_Exception &exception)
	{
		CL_ConsoleWindow console("Console", 80, 160);
		CL_Console::write_line("Error: " + exception.get_message_and_stack_trace());
		console.display_close_message();

		return -1;
	}
}
