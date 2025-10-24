#include <ClanLib/core.h>
#include <ClanLib/vorbis.h>
#include <ClanLib/sound.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>

class CL_DistortFilter : public CL_SoundFilter
{
public:
	CL_DistortFilter(int distortion) : m_distortion(distortion)
	{
		rand_map = new int[2048];
		for (int i=0;i<2048;i++)
		{
			rand_map[i] = (rand()%distortion) - (distortion/2);
		}
		rand_lim = 1024+rand()%1024;
		rand_pos = rand()%1024;
	}
	
	~CL_DistortFilter()
	{
		delete[] rand_map;
	}
	
	virtual void filter(int **sample_data, int num_samples, int channels)
	{
		for (int c=0;c<channels;c++)
		{
			for (int i=0;i<num_samples;i++)
			{
				sample_data[c][i] = sample_data[c][i]+rand_map[rand_pos++];
				if (rand_pos >= rand_lim)
				{
					rand_lim = 1024+rand()%1024;
					rand_pos = rand()%1024;
				}
			}
		}
	}
	
private:
	int m_distortion;
	int *rand_map;
	int rand_lim;
	int rand_pos;
};

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
			CL_SetupDisplay setup_display;
			CL_SetupGL setup_gl;
			CL_SetupVorbis setup_vorbis;

			CL_SoundOutput sound_output(44100);

			CL_DisplayWindow window("SoundFilters", 320, 200);
			
			// Load ogg file into a soundbuffer
			CL_SoundBuffer sample("cheer1.ogg");

			CL_FadeFilter fade(1.0f);
			CL_InverseEchoFilter inverseecho(128*1024);
			CL_DistortFilter distort(10000);
			CL_EchoFilter echo(8*1024);

			CL_SoundFilter *current_filter = 0;
			int current_key = 0;

			// Prepare playback session
			CL_SoundBuffer_Session playback(sample.prepare());
			playback.set_looping(true);
			
			std::cout << "Playing vorbis session...Press 1-4 to apply filters, 5 to remove" << std::endl;

			// Play it!
			playback.play();
			
			// Loop until done playing the ogg file
			while (playback.is_playing() && !CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
			{
				CL_Display::clear();
				
				if(CL_Keyboard::get_keycode(CL_KEY_1) && current_key != CL_KEY_1)
				{
					current_key = CL_KEY_1;
					std::cout << "Applying fade filter" << std::endl;

					if(current_filter)
						playback.remove_filter(current_filter);
					current_filter = &fade;
					fade.set_volume(1.0f);
					fade.fade_to_volume(0.0f, 3*1000); // fade out, duration 3 seconds.
					playback.add_filter(current_filter);
				}
				if(CL_Keyboard::get_keycode(CL_KEY_2) && current_key != CL_KEY_2)
				{
					current_key = CL_KEY_2;
					std::cout << "Applying inverse echo filter" << std::endl;

					if(current_filter)
						playback.remove_filter(current_filter);
					current_filter = &inverseecho;
					playback.add_filter(current_filter);
				}
				if(CL_Keyboard::get_keycode(CL_KEY_3) && current_key != CL_KEY_3)
				{
					current_key = CL_KEY_3;
					std::cout << "Applying distort filter" << std::endl;

					if(current_filter)
						playback.remove_filter(current_filter);
					current_filter = &distort;
					playback.add_filter(current_filter);
				}
				if(CL_Keyboard::get_keycode(CL_KEY_4) && current_key != CL_KEY_4)
				{
					current_key = CL_KEY_4;
					std::cout << "Applying echo filter" << std::endl;

					if(current_filter)
						playback.remove_filter(current_filter);
					current_filter = &echo;
					playback.add_filter(current_filter);
				}
				if(CL_Keyboard::get_keycode(CL_KEY_5) && current_key != CL_KEY_5)
				{
					current_key = CL_KEY_5;
					if(current_filter)
					{
						std::cout << "Removing any filter" << std::endl;

						playback.remove_filter(current_filter);
						current_filter = NULL;
					}
				}
				
				CL_Display::flip();
				
				// Make sure the system keeps responding
				CL_System::sleep(10);
				
				// Handle system events
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
