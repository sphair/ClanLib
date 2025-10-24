
#include <ClanLib/application.h>
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

#include "app.h"
#include "program.h"

// This is the Program class that is called by CL_ClanApplication
int Program::main(const std::vector<CL_String> &args)
{
	// Initialize ClanLib base components
	CL_SetupCore setup_core;

	// Initialize the ClanLib display component
	CL_SetupDisplay setup_display;

	// Initilize the OpenGL drivers
	CL_SetupGL setup_gl;

	// Start the Application
	App app;
	int retval = app.start(args);
	return retval;
}

CL_ClanApplication app(&Program::main);

