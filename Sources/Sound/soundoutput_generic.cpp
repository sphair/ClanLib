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
#include "soundoutput_generic.h"
#include "soundbuffer_session_generic.h"
#include "API/Core/System/threadfunc_v0.h"
#include "API/Sound/soundfilter.h"
#include <cstring>

/////////////////////////////////////////////////////////////////////////////
// CL_SoundOutput_Generic construction:

CL_SoundOutput_Generic::CL_SoundOutput_Generic(int mixing_frequency) :
	mixing_frequency(mixing_frequency), volume(1.0f), pan(0.0f), mix_buffer_size(0), has_sound(true), ref_count(0)
{
	mix_buffers[0] = 0;
	mix_buffers[1] = 0;
	temp_buffers[0] = 0;
	temp_buffers[1] = 0;
	stereo_buffer = 0;
}

CL_SoundOutput_Generic::~CL_SoundOutput_Generic()
{
	int size = filters.size();
	for (int i=0; i<size; i++) if (delete_filters[i]) delete filters[i];

	delete[] stereo_buffer;
	delete[] mix_buffers[0];
	delete[] mix_buffers[1];
	delete[] temp_buffers[0];
	delete[] temp_buffers[1];
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundOutput_Generic attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_SoundOutput_Generic operations:

void CL_SoundOutput_Generic::add_ref()
{
	ref_count++;
}

void CL_SoundOutput_Generic::release_ref()
{
	if (--ref_count == 0)
		delete this;
}

void CL_SoundOutput_Generic::play_session(CL_MutexSharedPtr<CL_SoundBuffer_Session_Generic> session)
{
	CL_MutexSection mutex_lock(&mutex);
	sessions.push_back(session);
}

void CL_SoundOutput_Generic::stop_session(CL_MutexSharedPtr<CL_SoundBuffer_Session_Generic> session)
{
	CL_MutexSection mutex_lock(&mutex);
	sessions.remove(session);
}

void CL_SoundOutput_Generic::start_mixer_thread()
{
	thread = CL_Thread(new CL_ThreadFunc_Runnable_v0<CL_SoundOutput_Generic>(this, &CL_SoundOutput_Generic::mixer_thread));
	thread.start();
	thread.set_priority(cl_priority_highest);
}

void CL_SoundOutput_Generic::stop_mixer_thread()
{
	CL_MutexSection mutex_lock(&mutex);
	stop_mixer.set_flag();
	mutex_lock.leave();
	if (thread.is_initialized()) 
	{
		thread.wait();
	}
	thread = CL_Thread();
}

void CL_SoundOutput_Generic::mix_fragment()
{
	if (get_fragment_size() != mix_buffer_size)
	{
		delete[] stereo_buffer; stereo_buffer = 0;
		delete[] mix_buffers[0]; mix_buffers[0] = 0;
		delete[] mix_buffers[1]; mix_buffers[1] = 0;
		delete[] temp_buffers[0]; temp_buffers[0] = 0;
		delete[] temp_buffers[1]; temp_buffers[1] = 0;

		mix_buffer_size = get_fragment_size();
		mix_buffers[0] = new int[mix_buffer_size];
		mix_buffers[1] = new int[mix_buffer_size];
		temp_buffers[0] = new int[mix_buffer_size];
		temp_buffers[1] = new int[mix_buffer_size];
		stereo_buffer = new short[mix_buffer_size*2];
		memset(stereo_buffer, 0, sizeof(short)*mix_buffer_size*2);
	}

	// Clear channel mixing buffers:
	memset(mix_buffers[0], 0, sizeof(int)*mix_buffer_size);
	memset(mix_buffers[1], 0, sizeof(int)*mix_buffer_size);

	// Mix playing soundbuffers into mixing buffers:
	CL_MutexSection mutex_lock(&mutex);
	std::vector< CL_MutexSharedPtr<CL_SoundBuffer_Session_Generic> > ended_sessions;
	std::list< CL_MutexSharedPtr<CL_SoundBuffer_Session_Generic> >::iterator it;
	for (it = sessions.begin(); it != sessions.end(); ++it)
	{
		CL_MutexSharedPtr<CL_SoundBuffer_Session_Generic> session = *it;
		bool playing = session->mix_to(mix_buffers, temp_buffers, mix_buffer_size, 2);
		if (!playing) ended_sessions.push_back(session);
	}

	// Apply global filters to mixing buffers:
	int size_filters = filters.size();
	int i;
	for (i = 0; i < size_filters; i++)
	{
		filters[i]->filter(mix_buffers, mix_buffer_size, 2);
	}

	// Release any sessions pending for removal:
	int size_ended_sessions = ended_sessions.size();
	for (i = 0; i < size_ended_sessions; i++) stop_session(ended_sessions[i]);
	mutex_lock.leave();

	// Make sure values stay inside 16 bit range:
	for (int chan = 0; chan < 2; chan++)
	{
		for (int k=0; k<mix_buffer_size; k++)
		{
			if      (mix_buffers[chan][k] > 32767)  mix_buffers[chan][k] = 32767;
			else if (mix_buffers[chan][k] < -32767) mix_buffers[chan][k] = -32767;
			else                                    mix_buffers[chan][k] = mix_buffers[chan][k];
		}
	}

	// Calculate volume on left and right channel:
	double left_pan = 1-pan;
	double right_pan = 1+pan;
	if (left_pan < 0.0) left_pan = 0.0;
	if (left_pan > 1.0) left_pan = 1.0;
	if (right_pan < 0.0) right_pan = 0.0;
	if (right_pan > 1.0) right_pan = 1.0;
	if (volume < 0.0) volume = 0.0;
	if (volume > 1.0) volume = 1.0;

	double left_volume = volume * left_pan;
	double right_volume = volume * right_pan;

	// Interleave channels before sending them to sound card:
	short *ptr = stereo_buffer;
	for (int k=0; k<mix_buffer_size; k++)
	{
		*(ptr++) = short(mix_buffers[0][k]*left_volume);
		*(ptr++) = short(mix_buffers[1][k]*right_volume);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundOutput_Generic implementation:

void CL_SoundOutput_Generic::mixer_thread()
{
	while (true)
	{
		CL_MutexSection mutex_lock(&mutex);
		if (stop_mixer.get_flag()) break;
		mutex_lock.leave();

		// Mix some audio:
		mix_fragment();

		// Send mixed data to sound card:
		write_fragment(stereo_buffer);

		// Wait for soundcard to want more:
		wait();
	}
}
