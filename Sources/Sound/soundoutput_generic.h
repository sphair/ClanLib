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

#ifndef header_soundoutput_generic
#define header_soundoutput_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include <vector>
#include <string>
#include <list>
#include "API/Core/System/thread.h"
#include "API/Core/System/mutex.h"
#include "API/Core/System/event_trigger.h"
#include "API/Core/System/mutexsharedptr.h"

class CL_SoundFilter;
class CL_SoundBuffer_Session_Generic;

class CL_SoundOutput_Generic
{
//! Construction:
public:
	CL_SoundOutput_Generic(int mixing_frequency);
	
	virtual ~CL_SoundOutput_Generic();

//! Attributes:
public:
	std::string name;

	int mixing_frequency;

	float volume;

	float pan;

	std::vector<CL_SoundFilter *> filters;

	std::vector<bool> delete_filters;

	CL_Thread thread;

	CL_EventTrigger stop_mixer;

	std::list< CL_MutexSharedPtr<CL_SoundBuffer_Session_Generic> > sessions;

	CL_Mutex mutex;

	int mix_buffer_size;

	int *mix_buffers[2];

	int *temp_buffers[2];

	short *stereo_buffer;

	bool has_sound;

//! Operations:
public:
	void add_ref();
	
	void release_ref();

	void play_session(CL_MutexSharedPtr<CL_SoundBuffer_Session_Generic> session);

	void stop_session(CL_MutexSharedPtr<CL_SoundBuffer_Session_Generic> session);

protected:
	//: Called when we have no samples to play - and wants to tell the soundcard
	//: about this possible event.
	virtual void silence() = 0;

	//: Returns the buffer size used by device (returned as num [stereo] samples).
	virtual int get_fragment_size() = 0;

	//: Writes a fragment to the soundcard.
	virtual void write_fragment(short *data) = 0;

	//: Waits until output source isn't full anymore.
	virtual void wait() = 0;

	//: Starts a thread and call mix_fragment() and wait() continueously.
	void start_mixer_thread();

	//: Stops the mixer thread.
	void stop_mixer_thread();

	//: Mixes a single fragment and stores the result in stereo_buffer.
	void mix_fragment();

//! Implementation:
private:
	//: Worker thread for output device. Mixes the audio and sends it to write_fragment.
	void mixer_thread();

	int ref_count;
};

#endif
