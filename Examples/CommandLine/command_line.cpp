#include <ClanLib/core.h>

class MyCommandLine
{
public:
	enum { ARG_ARG = 301 };

	CL_CommandLine argp;

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
				CL_Console::write_line("file: " + argp.get_argument());
				break;
			case 'c':
				CL_Console::write_line("config: " + argp.get_argument());
				break;
			case 'a':
				CL_Console::write_line("a: " + argp.get_argument());
				break;
			case 'z':
				CL_Console::write_line("zero: " + argp.get_argument());
				break;
			case ARG_ARG:
				CL_Console::write_line("arg: " + argp.get_argument());
				break;
			case CL_CommandLine::REST_ARG:
				CL_Console::write_line("REST: " + argp.get_argument());
				break;
			default:
				CL_Console::write_line(cl_format("GOT: %1", argp.get_key()) + " " + argp.get_argument());
				break;
			}
		}
	}
};

int main(int argc, char** argv)
{
	CL_SetupCore setup_core;
	CL_ConsoleWindow console("Console");

	CL_Console::write_line("(Call this program with \"-h\" for help)");

	try 
	{
		MyCommandLine args(argc, argv);
		args.read_options();
	} 
	catch (CL_Exception& exception) 
	{
		CL_Console::write_line("Exception caught:");
		CL_Console::write_line(exception.message);
	}

	console.display_close_message();

	return 0;
}

