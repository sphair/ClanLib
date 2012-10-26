/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

#include "API/Sound/SoundProviders/soundprovider.h"
#include "API/Sound/SoundProviders/soundprovider_session.h"
#include "API/Core/System/thread.h"
#include "API/Core/System/mutex.h"
#include <dsound.h>

#if DIRECTSOUND_VERSION < 0x0800
#error Found DirectX headers older than 8.0. Please download a newer directx, and make sure its FIRST in the include path and library path (Tools->Options->Directories in MSVC).
#endif

namespace clan
{

class SoundProvider_Recorder_DirectSound_Session : public SoundProvider_Session, Runnable
{
/// \name Construction
/// \{

public:
	SoundProvider_Recorder_DirectSound_Session(int frequency);

	virtual ~SoundProvider_Recorder_DirectSound_Session();


/// \}
/// \name Attributes
/// \{

public:
	virtual int get_num_samples() const;

	virtual int get_frequency() const;

	virtual int get_num_channels() const;

	virtual int get_position() const;


/// \}
/// \name Operations
/// \{

public:
	virtual bool eof() const;

	virtual void stop();

	virtual bool play();

	virtual bool set_position(int pos);

	virtual bool set_end_position(int pos) { return false; }

	virtual int get_data(float **data_ptr, int data_requested);


/// \}
/// \name Implementation
/// \{

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

	Mutex mutex;

	volatile bool stop_thread;

	Thread thread;
/// \}
};

}
