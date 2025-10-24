
#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/sound.h>

class SinusStream : public CL_SoundProvider_Session
{
public:
	SinusStream()
	{
		pos = 0;
		playing = false;
		stream_length = 60*44100; // play sound for 60 seconds.
	}
	
	virtual bool eof() const
	{
		return (pos >= stream_length); // only stream for four seconds.
	}
	
	virtual void stop()
	{
		playing = false;
	}

	virtual bool play()
	{
		playing = true;
		return true;
	}

	virtual int get_position() const
	{
		return pos;
	}

	virtual bool set_position(int new_pos)
	{
		pos = new_pos;
		return true;
	}

	virtual int get_num_samples() const
	{
		return stream_length;
	}
	
	virtual int get_data(void **data_ptr, int samples_req)
	{
		if (samples_req + pos > stream_length)
		// playing beyond the end of stream data
		{
			samples_req = stream_length - pos;
			if (samples_req < 0)
			{
				stop();
				return 0;
			}
		}
		
		short *channel1 = (short *) data_ptr[0];

		// Generate required audio samples
		for (int i=0; i<samples_req; i++)
		{
			channel1[i] = (short) (sin(((pos+i)*440*3.14)/44100)*32000);
		}
	
		pos += samples_req;

		return samples_req; // return the amount samples given.
	}

	virtual int get_frequency() const
	{
		return 44100;
	}

	virtual CL_SoundFormat get_format() const
	{
		return sf_16bit_signed;
	}

	virtual int get_num_channels() const
	{
		return 1;
	}

private:
	int pos;
	bool playing;
	
	int stream_length;
};

class SinusStreamProvider : public CL_SoundProvider
{
public:
	virtual CL_SoundProvider_Session *begin_session()
	{
		return new SinusStream();
	}

	virtual void end_session(CL_SoundProvider_Session *session)
	{
		delete session;
	}
};

class App : public CL_ClanApplication
{
public:
	virtual int main(int argc, char **argv)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		try
		{
			CL_SetupCore setup_core;
			CL_SetupSound setup_sound;
			
			CL_SoundOutput output(44100);

			// Create the soundbuffer with the SinusStreamProvider

			CL_SoundBuffer buffer(
				new SinusStreamProvider,
				true);

			// Prepare a soundbuffer session
			CL_SoundBuffer_Session session(buffer.prepare());

			// Start playing
			session.play();
			
			// Wait some time
			CL_System::keep_alive(5000);

			// Stop playback
			session.stop();
		}
		catch (CL_Error err)
		{
			std::cout << "Error: " << err.message.c_str() << std::endl;

			// Display console close message and wait for a key
			console.display_close_message();

			return -1;
		}
		return 0;
	}
} app;
