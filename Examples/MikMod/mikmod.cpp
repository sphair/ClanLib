#include <ClanLib/core.h>
#include <ClanLib/mikmod.h>
#include <ClanLib/sound.h>
#include <ClanLib/application.h>

class MyApp : public CL_ClanApplication
{
public:
	virtual int main(int argc, char** argv)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		try
		{
			CL_SetupCore setup_core;
			CL_SetupSound setup_sound;
			CL_SetupMikMod setup_mikmod;

			CL_SoundOutput sound_output(44100);

			CL_SoundBuffer sample("clanbeat.xm");
			CL_SoundBuffer_Session playback(sample.prepare());

			std::cout << "Playing module session..." << std::endl;
			playback.play();
		
			while (playback.is_playing())
			{
				CL_System::sleep(10);
				CL_System::keep_alive();
			}
		}
		catch (CL_Error err)
		{
			std::cout << "error: " << err.message.c_str() << std::endl;

			// Display console close message and wait for a key
			console.display_close_message();

			return -1;
		}

		return 0;
	}
} app;
