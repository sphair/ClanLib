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
#include "soundoutput_oss.h"
#include <API/Core/System/error.h>
#include "API/Core/System/cl_assert.h"
#include "API/Core/System/system.h"
#include "API/Core/System/log.h"
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
// CL_SoundOutput_OSS construction:

CL_SoundOutput_OSS::CL_SoundOutput_OSS(int mixing_frequency) :
	CL_SoundOutput_Generic(mixing_frequency), dev_dsp_fd(-1), frag_size(0)
{
	dev_dsp_fd = open(DEFAULT_DSP, O_WRONLY|O_NONBLOCK);
	if (dev_dsp_fd == -1)
	{
		has_sound = false;
		frag_size = mixing_frequency/2;
		return;
//		throw CL_Error("Could not open " + DEFAULT_DSP + ". No sound will be available.");
	}
	fcntl(dev_dsp_fd, F_SETFL, fcntl(dev_dsp_fd, F_GETFL) &~ O_NONBLOCK);
	
#ifndef USE_DRIVER_FRAGSIZE
	int frag_settings = 0x0003000b; // 0xMMMMSSSS
		// (where MMMM = num fragments, SSSS = fragment size)

	if (ioctl(dev_dsp_fd, SNDCTL_DSP_SETFRAGMENT, &frag_settings))
	{
		CL_Log::log("debug", "ClanSound: Failed to set soundcard fragment size. Sound may have a long latency.");
	}

#endif
	
	int format = AFMT_S16_NE;
	ioctl(dev_dsp_fd, SNDCTL_DSP_SETFMT, &format);
	if (format != AFMT_S16_NE)
	{
		close(dev_dsp_fd);
		throw CL_Error("Requires 16 bit soundcard. No sound will be available.");
	}
	
	int stereo = 1;
	ioctl(dev_dsp_fd, SNDCTL_DSP_STEREO, &stereo);
	if (stereo != 1)
	{
		close(dev_dsp_fd);
		throw CL_Error("ClanSound: Requires 16 bit stereo capable soundcard. No sound will be available.");
	}
	
	int speed = mixing_frequency;
	ioctl(dev_dsp_fd, SNDCTL_DSP_SPEED, &speed);

	float percent_wrong = speed / (float) mixing_frequency;
	if (percent_wrong < 0.90 || percent_wrong > 1.10)
	{
		close(dev_dsp_fd);
		throw CL_Error("ClanSound: Mixing rate (22.05 kHz) not supported by soundcard.");
	}
	
	// Try to improve mixing performance by using the same mixing buffer size
	// as the sound device does:
	int err = ioctl(dev_dsp_fd, SNDCTL_DSP_GETBLKSIZE, &frag_size);
	if (err == -1)
	{
		CL_Log::log("debug", "ClanSound: Warning, Couldn't get sound device blocksize. Using 0.25 sec mixing buffer.");
		frag_size = mixing_frequency/2; // 0.25 sec mixing buffer used.
	}

	audio_buf_info info;
	ioctl(dev_dsp_fd, SNDCTL_DSP_GETOSPACE, &info);

	start_mixer_thread();
}
	
CL_SoundOutput_OSS::~CL_SoundOutput_OSS()
{
	if (has_sound) stop_mixer_thread();
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundOutput_OSS attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_SoundOutput_OSS operations:

void CL_SoundOutput_OSS::silence()
{
	ioctl(dev_dsp_fd, SNDCTL_DSP_POST, 0);
}

bool CL_SoundOutput_OSS::is_full()
{
	if (!has_sound) return false;
	audio_buf_info info;
	int err = ioctl(dev_dsp_fd, SNDCTL_DSP_GETOSPACE, &info);
	if (err == -1)
	{
		CL_Log::log("debug", "ClanSound: fragments free not supported by device!?");
		return false; // not supported by device!?
	}

	if (info.fragments == 0) return true;
	return false;
}

int CL_SoundOutput_OSS::get_fragment_size()
{
	return frag_size/4;
}

void CL_SoundOutput_OSS::write_fragment(short *data)
{
	write(dev_dsp_fd, data, frag_size);
}

void CL_SoundOutput_OSS::wait()
{
	if(!has_sound)
	{
		CL_System::sleep(100);
		return;
	}

	fd_set wfds;

	FD_ZERO(&wfds);
	FD_SET(dev_dsp_fd, &wfds);

	/*int retval =*/ select(dev_dsp_fd+1, NULL, &wfds, NULL, NULL);
//	cl_assert(retval != -1);
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundOutput_OSS implementation:
