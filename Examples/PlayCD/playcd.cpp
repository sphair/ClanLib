/*
	Example of playing a cd.
*/

#include <ClanLib/application.h>
#include <ClanLib/core.h>
#include <ClanLib/sound.h>

class CDPlayApp : public CL_ClanApplication
{
public:
	virtual int main(int, char **)
	{
		// make sure there is some "visible" output
		CL_ConsoleWindow console("My console");
		console.redirect_stdio();

		try
		{
			CL_SetupCore setup_core;
		
			// Setup Sound
			CL_SetupSound setup_sound;

			// Get the first drive in the system
			if( CL_CDDrive::get_drives().empty() )
			{
				throw CL_Error("Error, couldn't find any cd drives");
			}

			CL_CDDrive &mycd = CL_CDDrive::get_drives().front();

			// open the cd
			// mycd->init();

			std::cout << "Playing tracks 7 and 8" << std::endl;

			//play tracks 7 and 8
			mycd.play_tracks(7,8);

			// you can exit right now.. the system will continue
			// playing the cd.. If not, you need the following while
			
			// if you stop the CD by hand, this loop will exit (because the CD
			// won't be playing anymore
			while(mycd.is_playing())
			{
				CL_System::keep_alive();
				CL_System::sleep(10);
			} 

			// stop the cd
			mycd.stop();
		}
		catch(CL_Error err)
		{
			std::cout << err.message << std::endl;
		}

		// Display console close message and wait for a key
		console.display_close_message();

		return 0;
	}
} app;
