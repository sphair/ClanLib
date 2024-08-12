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
*/

#pragma once

#include "../../soundoutput_impl.h"
#include "API/Core/System/databuffer.h"
#include <AudioToolbox/AudioToolbox.h>

namespace clan
{

class SoundOutput_MacOSX : public SoundOutput_Impl
{
public:
	SoundOutput_MacOSX(int mixing_frequency, int mixing_latency);
	~SoundOutput_MacOSX() override;

	/// \brief Called when we have no samples to play - and wants to tell the soundcard
	/// \brief about this possible event.
	void silence() override;

	/// \brief Returns the buffer size used by device (returned as num [stereo] samples).
	int get_fragment_size() override;

	/// \brief Writes a fragment to the soundcard.
	void write_fragment(float *data) override;

	/// \brief Waits until output source isn't full anymore.
	void wait() override;

    /// \brief Called by the mixer thread when it starts
    void mixer_thread_starting() override;

    /// \brief Called by the mixer thread when it stops
    void mixer_thread_stopping() override;

private:
    void audio_queue_callback(AudioQueueRef queue, AudioQueueBufferRef buffer);
    static void static_audio_queue_callback(void *userdata, AudioQueueRef queue, AudioQueueBufferRef buffer);

    static const int fragment_buffer_count = 4;
    int frequency, latency;
    int fragment_size;
    int next_fragment, read_cursor;
    int fragments_available;
    DataBuffer fragment_data;

    AudioStreamBasicDescription audio_format;
    AudioQueueRef audio_queue;
    AudioQueueBufferRef audio_buffers[fragment_buffer_count];

};

}

