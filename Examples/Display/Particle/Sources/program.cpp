/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Mark Page
*/

#include "precomp.h"
#include "program.h"
#include "particle.h"

int Program::main(const std::vector<std::string> &args)
{
	try
	{
		// Initialize ClanLib base components
		clan::SetupCore setup_core;

		// Initialize the ClanLib display component
		clan::SetupDisplay setup_display;

		// We support all display targets
		clan::SetupGL setup_gl;
#ifdef WIN32
		clan::SetupD3D setup_d3d;
#endif
		clan::SetupLegacyGL setup_legacy_gl;


		// Start the Application
		Particle app;
		int retval = app.start(args);
		return retval;
	}
	catch(clan::Exception &exception)
	{
		// Create a console window for text-output if not available
		clan::ConsoleWindow console("Console", 80, 160);
		clan::Console::write_line("Exception caught: " + exception.get_message_and_stack_trace());
		console.display_close_message();

		return -1;
	}
}

// Instantiate Application, informing it where the Program is located
clan::Application app(&Program::main);
