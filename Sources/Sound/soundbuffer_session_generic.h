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

#ifndef header_soundbuffer_session_generic
#define header_soundbuffer_session_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include <vector>
#include "API/Core/System/mutexsharedptr.h"
#include "API/Core/System/mutex.h"
#include "API/Sound/soundformat.h"

class CL_SoundFilter;
class CL_SoundBuffer_Generic;
class CL_SoundProvider_Session;
class CL_SoundOutput_Generic;

class CL_SoundBuffer_Session_Generic
{
//! Construction:
public:
	CL_SoundBuffer_Session_Generic(
		CL_MutexSharedPtr<CL_SoundBuffer_Generic> soundbuffer,
		bool looping,
		CL_SoundOutput_Generic *output);
	
	virtual ~CL_SoundBuffer_Session_Generic();

//! Attributes:
public:
	CL_MutexSharedPtr<CL_SoundBuffer_Generic> soundbuffer;

	CL_SoundProvider_Session *provider_session;

	CL_SoundOutput_Generic *output;

	float volume;
	
	float pan;

	bool looping;
	
	bool playing;
	
	float speedfactor;

	std::vector<CL_SoundFilter *> filters;
	
	std::vector<bool> delete_filters;
	
	mutable CL_Mutex mutex;

//! Operations:
public:
	bool mix_to(int **sample_data, int **temp_data, int num_samples, int channels);

//! Implementation:
private:
	//: Fills temporary buffers with data from provider.
	void get_data();

	//: Temporary channel buffers containing sound data in provider frequency.
	int **buffer_data;

	//: Size of temporary channel buffers.
	int num_buffer_samples;

	//: Number of temporary channel buffers;
	int num_buffer_channels;

	//: Current playback position in temporary buffers.
	double buffer_position;

	//: Number of samples currently written to buffer_data.
	int buffer_samples_written;
};

#endif
