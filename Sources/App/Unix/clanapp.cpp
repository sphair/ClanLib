/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
*/

#include <cstdlib>
#include <iostream>
#include "API/App/clanapp.h"
#include "API/Core/System/exception.h"
#include "API/Core/System/console_window.h"
#include "API/Core/Text/console.h"


int main(int argc, char **argv)
{
	if (clan::Application::main == nullptr)
	{
		std::cout << "ClanLib: No global Application instance!" << std::endl;
		return 255;
	}

	std::vector<std::string> args;
	for (int i = 0; i < argc; i++)
		args.push_back(argv[i]);
	// Call clanapp main:
	int retval;
	if (clan::Application::enable_catch_exceptions)
	{
		try
		{
			retval = clan::Application::main(args);
		}
		catch(clan::Exception &exception)
		{
			// Create a console window for text-output if not available
			std::string console_name("Console");
			if (!args.empty())
				console_name = args[0];

			clan::ConsoleWindow console(console_name, 80, 160);
			clan::Console::write_line("Exception caught: " + exception.get_message_and_stack_trace());
			console.display_close_message();

			retval = -1;
		}

	}
	else
	{
		retval = clan::Application::main(args);
	}

	return retval;
}

clan::Application::MainFunction *clan::Application::main = nullptr;
bool clan::Application::enable_catch_exceptions = true;

