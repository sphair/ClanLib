#include <ClanLib/core.h>
#include <ClanLib/application.h>

class DirectoryTest : public CL_ClanApplication
{
public:
	virtual int main(int argc, char **argv)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		std::string pathname;
		std::string pattern;

		if (argc == 1)
		{
			pathname = "";
		}
		else if (argc == 2)
		{
			pathname = argv[1];
		}
		else if (argc == 3)
		{
			pathname = argv[1];
			pattern = argv[2];
		}
		else
		{
			std::cout << "Usage: " << argv[0] << " [PATHNAME] [PATTERN]" << std::endl;
			exit (EXIT_FAILURE);
		}
		
		try
		{
			// Initialize ClanLib base components
			CL_SetupCore setup_core;

			CL_DirectoryScanner scanner;

			bool scan_successfull;
			if (pattern.empty ())
				scan_successfull = scanner.scan (pathname);
			else
				scan_successfull = scanner.scan (pathname, pattern);

			if (scan_successfull)
			{
				std::cout << "Searching in " << scanner.get_directory_path();
				if (!pattern.empty ())
					std::cout << " using pattern '" << pattern << "'" << std::endl;
				else
					std::cout << std::endl;

				while (scanner.next())
				{
					std::cout << "Found : (" << scanner.get_name() << ")";
			
					if (scanner.is_directory())
						std::cout << " == directory";

					if (scanner.is_hidden())
						std::cout << " [hidden]";
			
					std::cout << std::endl;
				}
			}
			else
			{
				std::cout << "Directory scan for '" << pathname << "' failed." << std::endl;
			}

			console.wait_for_key();
		}
		catch(CL_Error error)
		{
			std::cout << "Exception caught : " << error.message.c_str() << std::endl;

			// Display console close message and wait for a key
			console.display_close_message();

			return -1;
		}

		return 0;
	}
} my_app;
