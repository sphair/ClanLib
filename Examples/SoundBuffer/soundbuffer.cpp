/*
	Example of playing a soundbuffer from a wave file.
*/

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/sound.h>
#include <ClanLib/vorbis.h>
#include <iostream>

class SoundbufferApp : public CL_ClanApplication
{
public:
	virtual int main(int, char **)
	{
		CL_ConsoleWindow console("SoundbufferApp");
		console.redirect_stdio();

		try
		{
			CL_SetupCore setup_core;
			CL_SetupSound setup_sound;

			CL_SoundOutput output(44100);

			// Test recording:
//			rec_sound();

			// Play some sounds
			play_sound1();
			play_sound2();
//			play_sound3();
			
			// Play some sounds from resource script
//			play_sound3_ex(false);
			
			// Play some sounds from datafile
//			play_sound3_ex(true);

			// Play streamed sound:
//			play_sound4();

			// Play from resource script, reference count test:
//			play_sound5();

			// Play two sounds at once
			play_sound6();

			// Play ogg file:
//			play_sound7();
		}
		catch (CL_Error err)
		{
			std::cout << "error: " << err.message.c_str() << std::endl;

			console.display_close_message();
		}

		return 0;
	}

	void play_sound1()
	{
		std::cout << "play_sound1..." << std::endl;

		// Load a sample from a wave file:
		CL_SoundBuffer soundbuffer("beep.wav");

		// Setup a session handle for the playback:
		CL_SoundBuffer_Session session = soundbuffer.prepare();

		// Start the playback:
		session.play();

		// Wait until the sample finishes:
		while(session.is_playing())
		{
			CL_System::keep_alive(100);
		}
	}

	void play_sound2()
	{
		std::cout << "play_sound2..." << std::endl;

		// Load sample, play it once and unload it when done:
		CL_SoundBuffer soundbuffer("beep.wav");
		soundbuffer.play();

		// Note: if you call this function instead of play_sound() in this
		//       example, you wont actually hear the sample because program
		//       exits right away.
	}

	void play_sound3()
	{
		std::cout << "play_sound3..." << std::endl;

		// This time load from a resource file.
		CL_ResourceManager resources("sound.xml");
		CL_SoundBuffer soundbuffer("beep", &resources);

		// Setup a session handle for the playback:
		CL_SoundBuffer_Session session = soundbuffer.prepare();

		// Start the playback:
		session.play();

		// Wait until the sample finishes:
		while(session.is_playing())
		{
			CL_System::keep_alive(100);
		}
	}

	void play_sound3_ex(bool from_datafile)
	{
		std::cout << "play_sound3_ex..." << std::endl;

		// Use a zip archive as datafile if needed:
		CL_Zip_Archive *zip_file = 0;
		bool delete_provider = false;

		if (from_datafile)
		{
			// todo: create zip archive on the fly here, or include it with example?

			zip_file = new CL_Zip_Archive("sound.zip");
			delete_provider = true;
		}

		// This time load from a resource file.
		CL_ResourceManager resources("sound.xml", zip_file, delete_provider);

		// Do a multiple resource load test:
		for (int i=0; i<3; i++)
		{
			CL_SoundBuffer soundbuffer("beep", &resources);

			// Setup a session handle for the playback:
			CL_SoundBuffer_Session session = soundbuffer.prepare();

			// Start the playback:
			session.play();

			// Wait until the sample finishes:
			while(session.is_playing())
			{
				CL_System::keep_alive();
				CL_System::sleep(100);
			}
		}
	}

	void play_sound4()
	{
		std::cout << "play_sound4..." << std::endl;

		// Load sample as streaming:
		CL_SoundBuffer soundbuffer("beep.wav", true);

		// Setup a session handle for the playback:
		CL_SoundBuffer_Session session = soundbuffer.prepare();

		// Start the playback:
		session.play();

		// Wait until the sample finishes:
		while(session.is_playing())
		{
			CL_System::keep_alive(100);
		}
	}

	void play_sound5()
	{
		std::cout << "play_sound5..." << std::endl;

		// Reference counting test:
		CL_ResourceManager resources("sound.xml");

		for (int i=0; i<5; i++)
		{
			CL_SoundBuffer soundbuffer("beep", &resources);

			// Setup a session handle for the playback:
			CL_SoundBuffer_Session session = soundbuffer.prepare();

			// Start the playback:
			session.play();

			// Wait until the sample finishes:
			while(session.is_playing())
			{
				CL_System::keep_alive();
				CL_System::sleep(100);
			}
		}
	}

	void play_sound6()
	{
		std::cout << "play_sound6..." << std::endl;

		// Load a sample from a wave file:
		CL_SoundBuffer soundbuffer1("gun.wav");
		CL_SoundBuffer soundbuffer2("explode.wav");

		// Setup a session handle for the playback:
		CL_SoundBuffer_Session session1 = soundbuffer1.prepare();
		CL_SoundBuffer_Session session2 = soundbuffer2.prepare();

		// Start the playback:
		session1.play();
		session2.play();

		// Wait until one of the sounds finishes:
		while(session1.is_playing() || session2.is_playing())
		{
			CL_System::keep_alive(100);
		}
	}

	void play_sound7()
	{
		std::cout << "play_sound7..." << std::endl;

		CL_SetupVorbis setup_vorbis;
		CL_SoundBuffer soundbuffer("KeepHopeAlive.ogg");
		CL_SoundBuffer_Session session = soundbuffer.play();
		while (session.is_playing()) CL_System::keep_alive(100);
	}

	void rec_sound()
	{
/*		std::cout << "rec_sound..." << std::endl;
		CL_StreamSoundProvider *mic = CL_Sound::get_microphone();
		if (mic == 0) return; // no mic support.

		CL_StreamSoundProvider_Session *mic_session = mic->begin_session();

		std::cout << "recording..." << std::endl;
		mic_session->play();
		CL_System::sleep(10000);
		mic_session->stop();

		// Get for 10 secs of audio data:
		unsigned char buffer[22050*4*10];
		int received = mic_session->get_data(buffer, 22050*4*10);

		mic->end_session(mic_session);

		// Put it into a raw sample and play it:
		CL_SoundBuffer sample(
			new CL_Sample_RawData(buffer, received, 2, true),
			true);

		std::cout << "playing back recording..." << std::endl;
		CL_SoundBuffer_Session session = sample.play();

		// Wait until the sample finishes:
		while(session.is_playing())
		{
			CL_System::keep_alive(100);
		} */
	}
} app;
