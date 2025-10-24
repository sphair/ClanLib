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
				std::cout << "file: " << argp.get_argument() << std::endl;
				break;
			case 'c':
				std::cout << "config: " << argp.get_argument() << std::endl;
				break;
			case 'a':
				std::cout << "a" << std::endl;
				break;
			case 'z':
				std::cout << "zero" << std::endl;
				break;
			case ARG_ARG:
				std::cout << "arg" << std::endl;
				break;
			case CL_CommandLine::REST_ARG:
				std::cout << "rest: " << argp.get_argument() << std::endl;
				break;
			default:
				std::cout << "Got " << argp.get_key() << " " << argp.get_argument() << std::endl;
				break;
			}
		}
	}
};

int main(int argc, char** argv)
{
	try 
	{
		MyCommandLine args(argc, argv);
		args.read_options();
	} 
	catch (CL_Error& err) 
	{
		std::cout << argv[0] << err.message << std::endl;
	}
	return 0;
}
