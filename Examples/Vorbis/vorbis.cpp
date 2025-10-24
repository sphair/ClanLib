#include <ClanLib/core.h>
#include <ClanLib/vorbis.h>
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
			CL_SetupVorbis setup_vorbis;

			CL_SoundOutput sound_output(44100);

			// Load ogg file into a soundbuffer
			CL_SoundBuffer sample("cheer1.ogg");

			// Prepare playback session
			CL_SoundBuffer_Session playback = sample.prepare();

			std::cout << "Playing vorbis session..." << std::endl;

			// Play it!
			playback.play();
			
			// Loop until done playing the ogg file
			while (playback.is_playing())
			{
				// Make sure the system keeps responding
				CL_System::sleep(10);
				
				// Handle system events:
				// Exits the loop if ClanLib requests shutdown - for instance if
				// someone closes the window.
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
