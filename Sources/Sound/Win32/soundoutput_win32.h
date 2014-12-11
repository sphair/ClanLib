/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

#include "../soundoutput_impl.h"
#include "API/Core/System/comptr.h"
#include "API/Core/System/databuffer.h"
#include <mmdeviceapi.h>
#include <audioclient.h>

namespace clan
{

class SoundOutput_Win32 : public SoundOutput_Impl
{
/// \name Construction
/// \{
public:
	SoundOutput_Win32(int mixing_frequency, int mixing_latency = 50);
	~SoundOutput_Win32();
/// \}

/// \name Attributes
/// \{
public:
/// \}

/// \name Operations
/// \{
public:
	/// \brief Called when we have no samples to play - and wants to tell the sound card
	/// \brief about this possible event.
	virtual void silence();

	/// \brief Returns the buffer size used by device (returned as number of [stereo] samples).
	virtual int get_fragment_size();

	/// \brief Writes a fragment to the sound card.
	virtual void write_fragment(float *data);

	/// \brief Waits until output source isn't full anymore.
	virtual void wait();
/// \}

/// \name Implementation
/// \{
private:
	ComPtr<IMMDevice> mmdevice;
	ComPtr<IAudioClient> audio_client;
	ComPtr<IAudioRenderClient> audio_render_client;
	DataBuffer next_fragment;
	HANDLE audio_buffer_ready_event;
	bool is_playing;
	UINT32 fragment_size;
	int wait_timeout;
	int write_pos;
/// \}
};

}
