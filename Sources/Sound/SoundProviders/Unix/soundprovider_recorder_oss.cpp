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

#include "Sound/precomp.h"
#include "API/Sound/soundformat.h"
#include <API/Core/System/error.h>
#include "API/Core/System/cl_assert.h"
#include "API/Core/System/system.h"
#include "API/Core/System/log.h"
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


/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Recorder_OSS_Session construction:

CL_SoundProvider_Recorder_OSS_Session::CL_SoundProvider_Recorder_OSS_Session(int frequency) :
	frequency(frequency), position(0)
{

	dev_dsp_fd = open(DEFAULT_DSP, O_RDONLY);
	if (dev_dsp_fd == -1)
	{
		frag_size = frequency/2;
		return;
//		throw CL_Error("Could not open " + DEFAULT_DSP + ". No sound will be available.");
	}

#ifndef USE_DRIVER_FRAGSIZE
	int frag_settings = 0x0003000b; // 0xMMMMSSSS
		// (where MMMM = num fragments, SSSS = fragment size)

	if (ioctl(dev_dsp_fd, SNDCTL_DSP_SETFRAGMENT, &frag_settings))
	{
		CL_Log::log("debug", "ClanSound: Failed to set soundcard fragment size. Sound may have a long latency.");
	}

#endif

	format = AFMT_S16_NE;
	ioctl(dev_dsp_fd, SNDCTL_DSP_SETFMT, &format);
	if (format != AFMT_S16_NE)
	{
		close(dev_dsp_fd);
		throw CL_Error("Requires 16 bit soundcard. No sound input will be available.");
	}

	int stereo = 0;
	ioctl(dev_dsp_fd, SNDCTL_DSP_STEREO, &stereo);
	if (stereo != 0)
	{
		close(dev_dsp_fd);
		throw CL_Error("ClanSound: Requires 16 bit mono input. No sound will be available.");
	}

	int speed = frequency;
	ioctl(dev_dsp_fd, SNDCTL_DSP_SPEED, &speed);

	float percent_wrong = speed / (float) frequency;
	if (percent_wrong < 0.90 || percent_wrong > 1.10)
	{
		close(dev_dsp_fd);
		throw CL_Error("ClanSound: Mixing rate is not supported by soundcard.");
	}

	// Try to improve mixing performance by using the same mixing buffer size
	// as the sound device does:
	int err = ioctl(dev_dsp_fd, SNDCTL_DSP_GETBLKSIZE, &frag_size);
	if (err == -1)
	{
		CL_Log::log("debug", "ClanSound: Warning, Couldn't get sound device blocksize. Using 0.25 sec mixing buffer.");
		frag_size = frequency/4; // 0.25 sec mixing buffer used.
	}

}

CL_SoundProvider_Recorder_OSS_Session::~CL_SoundProvider_Recorder_OSS_Session()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Recorder_OSS_Session attributes:

int CL_SoundProvider_Recorder_OSS_Session::get_num_samples() const
{
	return 1;
}

int CL_SoundProvider_Recorder_OSS_Session::get_frequency() const
{

	return frequency;
}

CL_SoundFormat CL_SoundProvider_Recorder_OSS_Session::get_format() const
{
	return (CL_SoundFormat)format;
}

int CL_SoundProvider_Recorder_OSS_Session::get_num_channels() const
{
	return 1;
}

int CL_SoundProvider_Recorder_OSS_Session::get_position() const
{
	return position;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Recorder_OSS_Session operations:

bool CL_SoundProvider_Recorder_OSS_Session::eof() const
{
	return false;
}

void CL_SoundProvider_Recorder_OSS_Session::stop()
{
}

bool CL_SoundProvider_Recorder_OSS_Session::play()
{
	return true;
}

bool CL_SoundProvider_Recorder_OSS_Session::set_position(int pos)
{
	position = pos;
	return true;
}

int CL_SoundProvider_Recorder_OSS_Session::get_data(void **data_ptr, int data_requested)
{
	int len;
	audio_buf_info info;
	// todo: read samples, and only travel position by amount read.
	int err = ioctl(dev_dsp_fd, SNDCTL_DSP_GETISPACE, &info);
	if (err == -1)
	{
		CL_Log::log("debug", "ClanSound: fragments free not supported by device!?");
		position = 0;
	}
	if(info.fragments == 0)
	{
		return 0;
	}
	else
	{
		len = read(dev_dsp_fd, data_ptr[0], data_requested);
	}

	position += len;
	return position;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Recorder_OSS_Session implementation:
