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
**    
*/

#include <ClanLib/core.h>
using namespace clan;

class MyCommandLine
{
public:
	enum { ARG_ARG = 301 };

	CommandLine argp;

	MyCommandLine(int argc, char** argv) 
	{
		argp.add_usage("[OPTIONS]... [FILES]...");
		argp.add_usage("foobar [OPTIONS]... [FILES]...");
	 
		argp.add_doc("This program does nothing usefull..");
	 
		argp.add_group ("Options that want a file:");
		argp.add_option('c',	  "config", "FILE", "Config the app");
		argp.add_option('f',	  "file",	"FILE", "Load a file");

		argp.add_group ("Options that don't want a file:");
		argp.add_option('z',	  "zero",	"",	  "Zero Args");
		argp.add_option('a',	  "",		 "",	  "short a");
		argp.add_option('h',	  "help",	"",	  "help");

		argp.add_group ("Options without a short arg:");
		argp.add_option(ARG_ARG, "arg",	 "",	  "long a");
	 
		argp.add_doc("For more info have a look at FOobar does nothing usefull..\n"
						 "Examples can be found at blubbark...\n"
						 "....");

		argp.parse_args(argc, argv);
	}
  
	void read_options() 
	{
		while (argp.next())
		{
			switch (argp.get_key()) 
			{
			case 'h':
				argp.print_help();
				break;
			case 'f':
				Console::write_line("file: " + argp.get_argument());
				break;
			case 'c':
				Console::write_line("config: " + argp.get_argument());
				break;
			case 'a':
				Console::write_line("a: " + argp.get_argument());
				break;
			case 'z':
				Console::write_line("zero: " + argp.get_argument());
				break;
			case ARG_ARG:
				Console::write_line("arg: " + argp.get_argument());
				break;
			case CommandLine::REST_ARG:
				Console::write_line("REST: " + argp.get_argument());
				break;
			default:
				Console::write_line(string_format("GOT: %1", argp.get_key()) + " " + argp.get_argument());
				break;
			}
		}
	}
};

int main(int argc, char** argv)
{
	try 
	{
		SetupCore setup_core;
		ConsoleWindow console("Console");

		Console::write_line("(Call this program with \"-h\" for help)");
		MyCommandLine args(argc, argv);
		args.read_options();

		console.display_close_message();
	} 
	catch(Exception &exception)
	{
		ConsoleWindow console("Console");
		Console::write_line("Exception caught: " + exception.get_message_and_stack_trace());
		console.display_close_message();
		return 1;
	}
	return 0;
}

