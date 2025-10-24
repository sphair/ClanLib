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
**    Lih-Hern (Lf3T-Hn4D)
**    (if your name is missing here, please add it)
*/

#include <iostream>
#include <conio.h>

#include <string>

#ifdef WIN32
#include <windows.h>
#endif

#include "iniparser.h"
#include "vc++toolkit.h"

bool read_config();
void wait_for_key();
int main(int argc, char *argv[]);

std::string input_lib_dir;
std::string input_include_dir;
std::string output_lib_dir;
std::string output_include_dir;
bool use_opengl;
bool use_network;
bool use_clansound;
int compiler;

bool read_config()
{
	dictionary *config = iniparser_new("Setup/CodeBlocks/config");
	if (!config) return false;

	input_lib_dir = iniparser_getstring(config, "dirs:input_lib", "");
	input_include_dir = iniparser_getstring(config, "dirs:input_include", "");
	output_lib_dir = iniparser_getstring(config, "dirs:output_lib", "");
	output_include_dir = iniparser_getstring(config, "dirs:output_include", "");

	use_opengl = iniparser_getboolean(config, "use:opengl", false);
	use_network = iniparser_getboolean(config, "use:network", false);
	use_clansound = iniparser_getboolean(config, "use:clansound", false);

	compiler = iniparser_getint(config, "others:compiler", 1);

	iniparser_free(config);

	return true;
}

void wait_for_key()
{
#ifdef WIN32
  #ifdef __BORLANDC__
	std::string foo;
	cin >> foo;
  #else
	while (!kbhit()) Sleep(250);
  #endif
#endif
}

int main(int argc, char *argv[])
{
	std::cout << "===============================================" << std::endl;
	std::cout << "- ClanLib :: Code::Blocks Workspace Generator -" << std::endl;
	std::cout << "===============================================" << std::endl;
	std::cout << "This application generates the Workspace" << std::endl;
	std::cout << "and Project files for Code::Blocks" << std::endl << std::endl;
	std::cout << "Options are read from Setup/CodeBlocks/config" << std::endl;
	std::cout << "Please refer to Setup/CodeBlocks/config.sample for list of options." << std::endl << std::endl;

	if (!read_config())
	{
		std::cout << "Setup/CodeBlocks/config not found!!!" << std::endl;
		std::cout << "You did not create a config file! Please refer to the sample file" << std::endl;
		std::cout << "to find out how to create your config file." << std::endl;
	}
	else
	{
		std::cout << "Setup/CodeBlocks/config found! ^_^" << std::endl;
		std::cout << "input_lib_dir = " << input_lib_dir << std::endl;
		std::cout << "input_include_dir = " << input_include_dir << std::endl;
		std::cout << "output_lib_dir = " << output_lib_dir << std::endl;
		std::cout << "output_include_dir = " << output_include_dir << std::endl;
		std::cout << "using opengl... " << (use_opengl?"yes":"no") << std::endl;
		std::cout << "using network... " << (use_network?"yes":"no") << std::endl;
		std::cout << "using clansound... " << (use_clansound?"yes":"no") << std::endl;
		std::cout << "compiler: ";

		/*Workspace wrk_clanlib;
		Workspace wrk_examples;
		Workspace wrk_tests;
		Workspace wrk_utilities;*/

		std::list<std::string> defines_list;
		if (use_opengl) defines_list.push_back("USE_OPENGL");
		if (use_network) defines_list.push_back("USE_NETWORK");
		if (use_clansound) defines_list.push_back("USE_CLANSOUND");

		switch (compiler)
		{
			case 1:
				std::cout << "Visual C++ Toolkit 2003" << std::endl;
				setup_vc_clanlib_workspace(defines_list, input_lib_dir, input_include_dir, output_lib_dir, output_include_dir);

				break;

			default:
				std::cout << "Unknown or unsupported - " << compiler << std::endl;
				std::cout << "Error!!! Unable to generate project files. Unknown compiler type." << std::endl;

				#ifdef WIN32
					std::cout << std::endl << "(press any key to close this console window)";
					std::cout.flush();

					wait_for_key();
				#endif

				return 0;
		}
	}

#ifdef WIN32
	std::cout << std::endl << "(press any key to close this console window)";
	std::cout.flush();

	wait_for_key();
#endif

	return 0;
}
