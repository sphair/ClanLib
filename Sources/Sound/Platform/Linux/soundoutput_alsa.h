/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
**    Hans de Goede
*/

#pragma once

#ifdef __linux__

#include "../../soundoutput_impl.h"
#include <alsa/asoundlib.h>

namespace clan
{

class SoundOutput_alsa : public SoundOutput_Impl
{
//! Construction:
public:
	SoundOutput_alsa(int mixing_frequency, int mixing_latency);
	
	~SoundOutput_alsa();

//! Attributes:
public:
	snd_pcm_t *handle;
	snd_pcm_uframes_t frames_in_period;
	snd_pcm_uframes_t frames_in_buffer;

//! Operations:
public:
	//: Called when we have no samples to play - and wants to tell the soundcard
	//: about this possible event.
	virtual void silence() override;

	//: Returns true if all fragments are filled with data.
	virtual bool is_full();

	//: Returns the buffer size used by device (returned as num [stereo] samples).
	virtual int get_fragment_size() override;

	//: Writes a fragment to the soundcard.
	virtual void write_fragment(float *data) override;

	//: Waits until output source isn't full anymore.
	virtual void wait() override;

//! Implementation:
private:
};

}

#endif

