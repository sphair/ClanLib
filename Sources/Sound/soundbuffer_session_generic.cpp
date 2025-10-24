/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    (if your name is missing here, please add it)
*/

#include "Sound/precomp.h"
#include "soundbuffer_session_generic.h"
#include "soundbuffer_generic.h"
#include "soundoutput_generic.h"
#include "API/Sound/soundfilter.h"
#include "API/Sound/soundprovider.h"
#include "API/Sound/soundprovider_session.h"
#include "API/Core/System/cl_assert.h"
#include <cstring>

/////////////////////////////////////////////////////////////////////////////
//! Construction:

CL_SoundBuffer_Session_Generic::CL_SoundBuffer_Session_Generic(
	CL_MutexSharedPtr<CL_SoundBuffer_Generic> soundbuffer,
	bool looping,
	CL_SoundOutput_Generic *output)
:
	soundbuffer(soundbuffer), provider_session(0), output(output), volume(1.0f),
	pan(0.0f), looping(looping), playing(false), speedfactor(1.0f)
{
	volume = soundbuffer->volume;
	pan = soundbuffer->pan;
	provider_session = soundbuffer->provider->begin_session();
	provider_session->set_looping(looping);

	num_buffer_samples = 16*1024;
	num_buffer_channels = provider_session->get_num_channels();
	buffer_position = 0.0;
	buffer_samples_written = 0;

	buffer_data = new int*[num_buffer_channels];
	for (int i=0; i<num_buffer_channels; i++) buffer_data[i] = new int[num_buffer_samples];
}
	
CL_SoundBuffer_Session_Generic::~CL_SoundBuffer_Session_Generic()
{
	for (std::vector<CL_SoundFilter *>::size_type i=0; i<filters.size(); i++)
		if (delete_filters[i]) delete filters[i];
		
	if (soundbuffer && provider_session)
	{
		soundbuffer->provider->end_session(provider_session);
	}

	for (int j=0; j < num_buffer_channels; ++j) delete[] buffer_data[j];
	delete[] buffer_data;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundBuffer_Session_Generic attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_SoundBuffer_Session_Generic operations:

bool CL_SoundBuffer_Session_Generic::mix_to(int **sample_data, int **temp_data, int num_samples, int num_channels)
{
	CL_MutexSection mutex_lock(&mutex);

	// Clear working buffers:
	int chan;
	for (chan = 0; chan < num_channels; chan++)
	{
		memset(temp_data[chan], 0, sizeof(int) * num_samples);
	}

	// Convert from session frequency to mixer frequency:
	// This is done by copying data from the temporary session buffers (buffer_data) to
	// the temporary mixing buffers (temp_data), and if buffer_data is exhausted, calling
	// get_data() to fill it with new data from the soundprovider session object.
	double speed = (provider_session->get_frequency() / double(output->mixing_frequency)) * double(speedfactor);
	for (int i = 0; i < num_samples; i++)
	{
		if (buffer_position < buffer_samples_written)
		{
			for (int chan = 0; chan < num_buffer_channels; chan++)
			{
				temp_data[chan][i] = buffer_data[chan][int(buffer_position)];
			}
		}
		else
		{
			if (provider_session->eof())
			{
				playing = false;
				break;
			}

			// Out of data, get more from provider:
			buffer_position -= buffer_samples_written;
			get_data();
			i--;
			continue;
		}

		buffer_position += speed;
	}

	// Run filters on data:
	for (std::vector<CL_SoundFilter *>::size_type index_filter = 0; index_filter < filters.size(); index_filter++)
	{
		filters[index_filter]->filter(temp_data, num_samples, num_buffer_channels);
	}

	// Calculate volume on each channel:
	int channel_volume[2] = { int (128-pan*128), int (128+pan*128) };
	if (channel_volume[0] < 0) channel_volume[0] = 0;
	if (channel_volume[0] > 128) channel_volume[0] = 128;
	if (channel_volume[1] < 0) channel_volume[1] = 0;
	if (channel_volume[1] > 128) channel_volume[1] = 128;
	int vol = (int) (volume*128);
	channel_volume[0] *= vol;
	channel_volume[1] *= vol;

	// Finally mix temp buffers into main mixing channels:
	if (num_buffer_channels == 1)
	{
		// If its a mono stream, play it in all channels:
		for (int chan = 0; chan < num_channels; chan++)
		{
			for (int i = 0; i < num_samples; i++)
			{
				sample_data[chan][i] += temp_data[0][i]*channel_volume[chan]/16384;
			}
		}
	}
	else
	{
		if (num_channels > num_buffer_channels) num_channels = num_buffer_channels;

		for (int chan = 0; chan < num_channels; chan++)
		{
			for (int i = 0; i < num_samples; i++)
			{
				sample_data[chan][i] += temp_data[chan][i]*channel_volume[chan]/16384;
			}
		}
	}

	return playing;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundBuffer_Session_Generic implementation:

void CL_SoundBuffer_Session_Generic::get_data()
{
	int num_session_channels = provider_session->get_num_channels();
	cl_assert(num_session_channels == num_buffer_channels);

	// Copy stream data to working buffer:
	int samples_left = num_buffer_samples;
	while (samples_left > 0)
	{
		int written = provider_session->get_data((void **) buffer_data, samples_left);
		samples_left -= written;

		// This is a bit ugly, but get_data() returns the data in either
		// 8 or 16 bit, and our temp buffer is 32 bit. To correctly fill
		// the buffer, we need to move the channel pointers accordingly
		// to 8 bit samples or 16 bit samples:
		for (int chan = 0; chan < num_session_channels; chan++)
		{
			if (provider_session->get_format() == sf_8bit_signed)
			{
				char **buffer_data8 = (char **) buffer_data;
				buffer_data8[chan] += written;
			}
			else if (provider_session->get_format() == sf_16bit_signed)
			{
				short **buffer_data16 = (short **) buffer_data;
				buffer_data16[chan] += written;
			}
		}

		if (samples_left > 0 && provider_session->eof())
		{
			// Reached end of stream, loop if enabled.
			if (looping)
			{
				// Try to loop back to beginning:
				if (provider_session->set_position(0) == false)
				{
					// It failed. Stop playing then.
					break;
				}

				// Start playing again if it stopped by reaching eof.
				provider_session->play();
			}
			else
			{
				break;
			}
		}
		else if (written == 0)
		{
			// No more data to get from provider for some reason.
			break;
		}
	}

	buffer_samples_written = num_buffer_samples - samples_left;

	// Convert from native format (8 or 16 bit) to 32 bit:
	for (int chan = 0; chan < num_session_channels; chan++)
	{
		if (provider_session->get_format() == sf_8bit_signed)
		{
			// Move pointer back to starting position:
			char **buffer_data8 = (char **) buffer_data;
			buffer_data8[chan] -= buffer_samples_written;

			// Align data to 32 bit:
			for (int i=buffer_samples_written-1; i>=0; i--)
			{
				buffer_data[chan][i] = buffer_data8[chan][i]*256;
			}
		}
		else if (provider_session->get_format() == sf_16bit_signed)
		{
			// Move pointer back to starting position:
			short **buffer_data16 = (short **) buffer_data;
			buffer_data16[chan] -= buffer_samples_written;

			// Align data to 32 bit:
			for (int i=buffer_samples_written-1; i>=0; i--)
			{
				buffer_data[chan][i] = buffer_data16[chan][i];
			}
		}
	}
}
