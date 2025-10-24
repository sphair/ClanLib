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

#ifndef header_soundrecorder_directsound
#define header_soundrecorder_directsound

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Sound/soundprovider.h"
#include "API/Sound/soundprovider_session.h"
#include "API/Core/System/thread.h"
#include "API/Core/System/mutex.h"
#include <dsound.h>

class CL_SoundProvider_Recorder_DirectSound_Session : public CL_SoundProvider_Session, CL_Runnable
{
//! Construction:
public:
	CL_SoundProvider_Recorder_DirectSound_Session(int frequency);

	virtual ~CL_SoundProvider_Recorder_DirectSound_Session();

//! Attributes:
public:
	virtual int get_num_samples() const;

	virtual int get_frequency() const;

	virtual CL_SoundFormat get_format() const;

	virtual int get_num_channels() const;

	virtual int get_position() const;

//! Operations:
public:
	virtual bool eof() const;
	
	virtual void stop();

	virtual bool play();
		
	virtual bool set_position(int pos);
	
	virtual int get_data(void **data_ptr, int data_requested);

//! Implementation:
private:
	virtual void run();

	int frequency;

	int position;

	LPDIRECTSOUNDCAPTURE8 dsoundcapture;

	LPDIRECTSOUNDCAPTUREBUFFER capturebuffer;

	LPDIRECTSOUNDNOTIFY8 notify;

	HANDLE notify_event;

	int bytes_per_sample;

	int frag_size, num_fragments;

	char *buffer;

	int buffer_size;

	CL_Mutex mutex;

	volatile bool stop_thread;

	CL_Thread thread;
};

#endif
