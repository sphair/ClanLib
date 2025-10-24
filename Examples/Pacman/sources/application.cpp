/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    Magnus Norddahl
**    (if your name is missing here, please add it)
*/

#include "precomp.h"
#include "application.h"
#include "world.h"

Application app;

int Application::main(int argc, char** argv)
{
	CL_ConsoleWindow console("Debug console window", 80, 25);
	console.redirect_stdio();

	try
	{
		std::cout << "ClanLib Pacman game, written by Magnus Norddahl" << std::endl;
		
		CL_SetupCore setup_core;
		CL_SetupDisplay setup_display;
		CL_SetupGL setup_gl;
		CL_SetupSound setup_sound;
		CL_SetupMikMod setup_mikmod;

		CL_SoundOutput sound_output(44100);
		CL_DisplayWindow window("The ClanLib Pacman game!", 640, 480, false);

		CL_ResourceManager resources("pacman.xml");

		World world(&resources);

		// Connect the Window close event
		CL_Slot slot_quit = window.sig_window_close().connect(&world, &World::on_quit);

		// Enter the amazing pacman world
		world.run(window);
	}
	catch (CL_Error err)
	{
		std::cout << "Exeception caught: " << err.message.c_str() << std::endl;
		console.wait_for_key();
	}
	
	return 0;
}
