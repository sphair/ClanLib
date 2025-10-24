/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
*/

#pragma once


#if _MSC_VER > 1000
#pragma once
#endif

#include <vector>
#include <list>
#include "API/Core/Text/string_types.h"
#include "API/Core/System/thread.h"
#include "API/Core/System/mutex.h"
#include "API/Core/System/event.h"
#include "API/Core/System/sharedptr.h"

class CL_SoundFilter;
class CL_SoundBuffer_Session_Generic;

class CL_SoundOutput_Generic
{
/// \name Construction
/// \{

public:
	CL_SoundOutput_Generic(int mixing_frequency, int mixing_latency);

	virtual ~CL_SoundOutput_Generic();


/// \}
/// \name Attributes
/// \{

public:
	CL_String8 name;

	int mixing_frequency;

	int mixing_latency;

	float volume;

	float pan;

	std::vector<CL_SoundFilter *> filters;

	std::vector<bool> delete_filters;

	CL_Thread thread;

	CL_Event stop_mixer;

	std::list< CL_SharedPtr<CL_SoundBuffer_Session_Generic> > sessions;

	CL_Mutex mutex;

	int mix_buffer_size;

	int *mix_buffers[2];

	int *temp_buffers[2];

	short *stereo_buffer;


/// \}
/// \name Operations
/// \{

public:
	void add_ref();

	void release_ref();

	void play_session(CL_SharedPtr<CL_SoundBuffer_Session_Generic> session);

	void stop_session(CL_SharedPtr<CL_SoundBuffer_Session_Generic> session);

protected:
	/// \brief Called when we have no samples to play - and wants to tell the soundcard
	/// \brief about this possible event.
	virtual void silence() = 0;

	/// \brief Returns the buffer size used by device (returned as num [stereo] samples).
	virtual int get_fragment_size() = 0;

	/// \brief Writes a fragment to the soundcard.
	virtual void write_fragment(short *data) = 0;

	/// \brief Waits until output source isn't full anymore.
	virtual void wait() = 0;

	/// \brief Starts a thread and call mix_fragment() and wait() continueously.
	void start_mixer_thread();

	/// \brief Stops the mixer thread.
	void stop_mixer_thread();

	/// \brief Mixes a single fragment and stores the result in stereo_buffer.
	void mix_fragment();


/// \}
/// \name Implementation
/// \{

private:
	/// \brief Worker thread for output device. Mixes the audio and sends it to write_fragment.
	void mixer_thread();

	int ref_count;
/// \}
};


