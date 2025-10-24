/*
	The minimum requirements needed by every ClanLib application.

	Note that the try/catch is not required but should be added 
	to every ClanLib application.
*/

#include <ClanLib/core.h>
#include <ClanLib/application.h>

class MinimumApp : public CL_ClanApplication
{
public:
	virtual int main(int, char **)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		try
		{
		 	CL_SetupCore setup_core;
		}
		catch (CL_Error err)
		{
			std::cout << "Exception caught: " << err.message.c_str() << std::endl;

			// Display console close message and wait for a key
			console.display_close_message();
		}

		return 0;
	}
} app;
