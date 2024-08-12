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

#include "Sound/precomp.h"
#include "soundoutput_alsa.h"
#include <API/Core/System/exception.h>
#include "API/Core/System/system.h"
#include "API/Core/Text/logger.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#ifdef __linux__

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// SoundOutput_alsa construction:

SoundOutput_alsa::SoundOutput_alsa(int mixing_frequency, int mixing_latency) :
	SoundOutput_Impl(mixing_frequency, mixing_latency), frames_in_buffer(4096),
	frames_in_period(1024)
{
	int rc;
	snd_pcm_hw_params_t *hwparams;

	rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
	if (rc < 0)
	{
		log_event("warn", "ClanSound: Couldn't open sound device, disabling sound");
		handle = nullptr;
		return;
	}

	snd_pcm_hw_params_alloca(&hwparams);
	snd_pcm_hw_params_any(handle, hwparams);
	snd_pcm_hw_params_set_access(handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED);
	snd_pcm_hw_params_set_format(handle, hwparams, SND_PCM_FORMAT_FLOAT);
	snd_pcm_hw_params_set_channels(handle, hwparams, 2);
	snd_pcm_hw_params_set_rate_near(handle, hwparams,
				(unsigned int *)&this->mixing_frequency, nullptr);
	snd_pcm_hw_params_set_buffer_size_near(handle, hwparams, &frames_in_buffer);
	frames_in_period = frames_in_buffer / 4;
	snd_pcm_hw_params_set_period_size_near(handle, hwparams, &frames_in_period, nullptr);

	rc = snd_pcm_hw_params(handle, hwparams);
	if (rc < 0)
	{
		log_event("warn", "ClanSound: Couldn't initialize sound device, disabling sound");
		snd_pcm_close(handle);
		handle = nullptr;
		return;
	}

	snd_pcm_hw_params_get_period_size(hwparams, &frames_in_period, nullptr);

	start_mixer_thread();
}

SoundOutput_alsa::~SoundOutput_alsa()
{
	stop_mixer_thread();
	if (handle) {
		snd_pcm_close(handle);
		handle = nullptr;
	}
}

/////////////////////////////////////////////////////////////////////////////
// SoundOutput_alsa operations:

void SoundOutput_alsa::silence()
{
	/* Note: not supported by all hardware! */
	if (handle)
		snd_pcm_pause(handle, 1);
}

bool SoundOutput_alsa::is_full()
{
	int rc;
	snd_pcm_sframes_t delay;

	if (handle == nullptr) return false;

	rc = snd_pcm_delay(handle, &delay);
	if (rc < 0) {
		log_event("debug", "ClanSound: snd_pcm_delay() failed!?");
		return false;
	}

	/* See if there is more then one period free in the buffer */
	return delay > (snd_pcm_sframes_t)(frames_in_buffer - frames_in_period);
}

int SoundOutput_alsa::get_fragment_size()
{
	return frames_in_period;
}

void SoundOutput_alsa::write_fragment(float *data)
{
	snd_pcm_sframes_t rc;

	if (handle == nullptr) return;

	switch(snd_pcm_state(handle)) {
		case SND_PCM_STATE_XRUN:
		case SND_PCM_STATE_SUSPENDED:
			snd_pcm_prepare(handle);
			break;
		case SND_PCM_STATE_PAUSED:
			snd_pcm_pause(handle, 0);
			break;
		default:
			break;
	}

	rc = snd_pcm_writei(handle, data, frames_in_period);
	if (rc < 0)
		log_event("debug", "ClanSound: snd_pcm_writei() failed!");
}

void SoundOutput_alsa::wait()
{
	if(handle == nullptr)
	{
		System::sleep(100);
		return;
	}
	/* wait upto 1 second */
	snd_pcm_wait(handle, 1000);
}

/////////////////////////////////////////////////////////////////////////////
// SoundOutput_alsa implementation:

}

#endif
