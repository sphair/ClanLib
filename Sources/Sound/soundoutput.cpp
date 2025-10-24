/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
#include "API/Sound/soundoutput.h"
#include "API/Sound/soundoutput_description.h"
#include "API/Sound/soundfilter.h"
#include "API/Sound/sound.h"
#include "API/Core/System/thread.h"
#include "soundoutput_generic.h"
#ifdef WIN32
#include "Win32/soundoutput_directsound.h"
#else
#ifdef __APPLE__
#include "MacOSX/soundoutput_macosx.h"
#else

#ifdef HAVE_ALSA_ASOUNDLIB_H
#include "Unix/soundoutput_alsa.h"
#endif
#include "Unix/soundoutput_oss.h"
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_SoundOutput construction:

CL_SoundOutput::CL_SoundOutput() : impl(0)
{
}

CL_SoundOutput::CL_SoundOutput(int mixing_frequency, int latency)
: impl(0)
{
	CL_SoundOutput_Description desc;
	desc.set_mixing_frequency(mixing_frequency);
	desc.set_mixing_latency(latency);
	operator =(CL_SoundOutput(desc));
}

CL_SoundOutput::CL_SoundOutput(const CL_SoundOutput_Description &desc) : impl(0)
{
#ifdef WIN32
	impl = new CL_SoundOutput_DirectSound(desc.get_mixing_frequency(), desc.get_mixing_latency());
#else
#ifdef __APPLE__
	impl = new CL_SoundOutput_MacOSX(desc.get_mixing_frequency(), desc.get_mixing_latency());
#else
#ifdef __linux__

#ifdef HAVE_ALSA_ASOUNDLIB_H
	CL_SoundOutput_alsa *alsa_impl;
	alsa_impl = new CL_SoundOutput_alsa(desc.get_mixing_frequency(), desc.get_mixing_latency());
	if (alsa_impl->handle)
		impl = alsa_impl;
	else
		delete alsa_impl;
	if (!impl)
#endif

#endif
	impl = new CL_SoundOutput_OSS(desc.get_mixing_frequency(), desc.get_mixing_latency());
#endif
#endif
	impl->add_ref();
	CL_Sound::select_output(this);
}

CL_SoundOutput::CL_SoundOutput(const CL_SoundOutput &copy) : impl(copy.impl)
{
	if (impl) impl->add_ref();
	CL_Sound::select_output(this);
}

CL_SoundOutput::~CL_SoundOutput()
{
	if (impl) impl->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundOutput attributes:

const CL_String8 &CL_SoundOutput::get_name() const
{
	CL_MutexSection mutex_lock(&impl->mutex);
	return impl->name;
}

int CL_SoundOutput::get_mixing_frequency() const
{
	CL_MutexSection mutex_lock(&impl->mutex);
	return impl->mixing_frequency;
}

int CL_SoundOutput::get_mixing_latency() const
{
	CL_MutexSection mutex_lock(&impl->mutex);
	return impl->mixing_latency;
}

float CL_SoundOutput::get_global_volume() const
{
	CL_MutexSection mutex_lock(&impl->mutex);
	return impl->volume;
}

float CL_SoundOutput::get_global_pan() const
{
	CL_MutexSection mutex_lock(&impl->mutex);
	return impl->pan;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundOutput operations:

CL_SoundOutput &CL_SoundOutput::operator =(const CL_SoundOutput &copy)
{
	if (impl) impl->release_ref();
	impl = copy.impl;
	if (impl) impl->add_ref();
	CL_Sound::select_output(this);
	return *this;
}

void CL_SoundOutput::stop_all()
{
}
	
void CL_SoundOutput::set_global_volume(float volume)
{
	if (impl)
	{
		CL_MutexSection mutex_lock(&impl->mutex);
		impl->volume = volume;
	}
}

void CL_SoundOutput::set_global_pan(float pan)
{
	if (impl)
	{
		CL_MutexSection mutex_lock(&impl->mutex);
		impl->pan = pan;
	}
}

void CL_SoundOutput::add_filter(CL_SoundFilter *filter, bool delete_filter)
{
	if (impl)
	{
		CL_MutexSection mutex_lock(&impl->mutex);
		impl->filters.push_back(filter);
		impl->delete_filters.push_back(delete_filter);
	}
}

void CL_SoundOutput::remove_filter(CL_SoundFilter *filter)
{
	if (impl)
	{
		CL_MutexSection mutex_lock(&impl->mutex);
		for (std::vector<CL_SoundFilter *>::size_type i=0; i<impl->filters.size(); i++)
		{
			if (impl->filters[i] == filter)
			{
				if (impl->delete_filters[i]) delete impl->filters[i];
				impl->filters.erase(impl->filters.begin()+i);
				impl->delete_filters.erase(impl->delete_filters.begin()+i);
				break;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundOutput implementation:
