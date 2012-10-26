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
*/

#include "Sound/precomp.h"
#include "API/Sound/soundformat.h"
#include "API/Core/System/exception.h"
#include "API/Core/System/system.h"
#include "API/Core/Text/logger.h"
#include "soundprovider_recorder_oss.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#ifdef HAVE_MACHINE_SOUNDCARD_H
#include <machine/soundcard.h>
#endif
#ifdef HAVE_SYS_SOUNDCARD_H
#include <sys/soundcard.h>
#endif
#ifdef HAVE_SOUNDCARD_H
#include <soundcard.h>
#endif
#ifdef __CYGWIN__
#include <sys/select.h>
#endif

#define DEFAULT_DSP "/dev/dsp"

#include "API/Sound/sound_sse.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// SoundProvider_Recorder_OSS_Session construction:

SoundProvider_Recorder_OSS_Session::SoundProvider_Recorder_OSS_Session(int frequency) :
	frequency(frequency), position(0)
{

	dev_dsp_fd = open(DEFAULT_DSP, O_RDONLY);
	if (dev_dsp_fd == -1)
	{
		frag_size = frequency/2;
		return;
//		throw Error("Could not open " + DEFAULT_DSP + ". No sound will be available.");
	}

#ifndef USE_DRIVER_FRAGSIZE
	int frag_settings = 0x0003000b; // 0xMMMMSSSS
		// (where MMMM = num fragments, SSSS = fragment size)

	if (ioctl(dev_dsp_fd, SNDCTL_DSP_SETFRAGMENT, &frag_settings))
	{
		cl_log_event("debug", "ClanSound: Failed to set soundcard fragment size. Sound may have a long latency.");
	}

#endif

	format = AFMT_S16_NE;
	ioctl(dev_dsp_fd, SNDCTL_DSP_SETFMT, &format);
	if (format != AFMT_S16_NE)
	{
		close(dev_dsp_fd);
		throw Exception("Requires 16 bit soundcard. No sound input will be available.");
	}

	int stereo = 0;
	ioctl(dev_dsp_fd, SNDCTL_DSP_STEREO, &stereo);
	if (stereo != 0)
	{
		close(dev_dsp_fd);
		throw Exception("ClanSound: Requires 16 bit mono input. No sound will be available.");
	}

	int speed = frequency;
	ioctl(dev_dsp_fd, SNDCTL_DSP_SPEED, &speed);

	float percent_wrong = speed / (float) frequency;
	if (percent_wrong < 0.90 || percent_wrong > 1.10)
	{
		close(dev_dsp_fd);
		throw Exception("ClanSound: Mixing rate is not supported by soundcard.");
	}

	// Try to improve mixing performance by using the same mixing buffer size
	// as the sound device does:
	int err = ioctl(dev_dsp_fd, SNDCTL_DSP_GETBLKSIZE, &frag_size);
	if (err == -1)
	{
		cl_log_event("debug", "ClanSound: Warning, Couldn't get sound device blocksize. Using 0.25 sec mixing buffer.");
		frag_size = frequency/4; // 0.25 sec mixing buffer used.
	}

}

SoundProvider_Recorder_OSS_Session::~SoundProvider_Recorder_OSS_Session()
{
}

/////////////////////////////////////////////////////////////////////////////
// SoundProvider_Recorder_OSS_Session attributes:

int SoundProvider_Recorder_OSS_Session::get_num_samples() const
{
	return 1;
}

int SoundProvider_Recorder_OSS_Session::get_frequency() const
{

	return frequency;
}

SoundFormat SoundProvider_Recorder_OSS_Session::get_format() const
{
	return sf_16bit_signed;
}

int SoundProvider_Recorder_OSS_Session::get_num_channels() const
{
	return 1;
}

int SoundProvider_Recorder_OSS_Session::get_position() const
{
	return position;
}

/////////////////////////////////////////////////////////////////////////////
// SoundProvider_Recorder_OSS_Session operations:

bool SoundProvider_Recorder_OSS_Session::eof() const
{
	return false;
}

void SoundProvider_Recorder_OSS_Session::stop()
{
}

bool SoundProvider_Recorder_OSS_Session::play()
{
	return true;
}

bool SoundProvider_Recorder_OSS_Session::set_position(int pos)
{
	position = pos;
	return true;
}

int SoundProvider_Recorder_OSS_Session::get_data(float **data_ptr, int data_requested)
{

	int len;
	audio_buf_info info;
	// todo: read samples, and only travel position by amount read.
	int err = ioctl(dev_dsp_fd, SNDCTL_DSP_GETISPACE, &info);
	if (err == -1)
	{
		cl_log_event("debug", "ClanSound: fragments free not supported by device!?");
		position = 0;
	}
	if(info.fragments == 0)
	{
		return 0;
	}
	else
	{
		std::vector<char> buffer;
		buffer.resize(data_requested);
		len = read(dev_dsp_fd, &buffer[0], data_requested);
		if (len>0)
			SoundSSE::unpack_16bit_mono((short *) &buffer[0], len/2, data_ptr[0]);
	}

	position += len;
	return position;
}

/////////////////////////////////////////////////////////////////////////////
// SoundProvider_Recorder_OSS_Session implementation:

}
