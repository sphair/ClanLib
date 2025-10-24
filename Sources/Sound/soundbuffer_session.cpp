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
#include "API/Sound/soundbuffer_session.h"
#include "API/Sound/soundprovider_session.h"
#include "API/Sound/soundfilter.h"
#include "soundbuffer_session_generic.h"
#include "soundoutput_generic.h"
#include <cmath>

/////////////////////////////////////////////////////////////////////////////
// CL_SoundBuffer_Session construction:

CL_SoundBuffer_Session::CL_SoundBuffer_Session()
{
}

CL_SoundBuffer_Session::CL_SoundBuffer_Session(const CL_SoundBuffer_Session &copy) : impl(copy.impl)
{
}

CL_SoundBuffer_Session::CL_SoundBuffer_Session(const CL_MutexSharedPtr<CL_SoundBuffer_Session_Generic> &impl) : impl(impl)
{
}

CL_SoundBuffer_Session::~CL_SoundBuffer_Session()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundBuffer_Session attributes:

int CL_SoundBuffer_Session::get_position() const
{
	CL_MutexSection mutex_lock(&impl->mutex);
	return impl->provider_session->get_position();
}

float CL_SoundBuffer_Session::get_position_relative() const
{
	CL_MutexSection mutex_lock(&impl->mutex);
	int position = impl->provider_session->get_position();
	int length = impl->provider_session->get_num_samples();
	if (length == 0) return 1.0f;
	return position / (float) length;
}

int CL_SoundBuffer_Session::get_length() const
{
	if (!impl) return 0;
	CL_MutexSection mutex_lock(&impl->mutex);
	return impl->provider_session->get_num_samples();
}

int CL_SoundBuffer_Session::get_frequency() const
{
	CL_MutexSection mutex_lock(&impl->mutex);
	return impl->provider_session->get_frequency();
}

float CL_SoundBuffer_Session::get_volume() const
{
	CL_MutexSection mutex_lock(&impl->mutex);
	return impl->volume;
}

float CL_SoundBuffer_Session::get_pan() const
{
	CL_MutexSection mutex_lock(&impl->mutex);
	return impl->pan;
}

float CL_SoundBuffer_Session::get_speedfactor() const
{
	CL_MutexSection mutex_lock(&impl->mutex);
	return impl->speedfactor;
}

bool CL_SoundBuffer_Session::is_playing()
{
	if (!impl) return false;
	CL_MutexSection mutex_lock(&impl->mutex);
	return impl->playing;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundBuffer_Session operations:

bool CL_SoundBuffer_Session::set_position(int new_pos)
{
	CL_MutexSection mutex_lock(&impl->mutex);
	if (impl->provider_session->set_position(new_pos))
	{
		// instantly update position here?
		return true;
	}
	return false;
}

bool CL_SoundBuffer_Session::set_position_relative(float new_pos)
{
	int length = get_length();
	return set_position((int) (new_pos * length));
}

bool CL_SoundBuffer_Session::set_frequency(int new_freq)
{
	CL_MutexSection mutex_lock(&impl->mutex);
//	if (!impl->provider_session->set_frequency(new_freq)) return false;
	return true;
}

void CL_SoundBuffer_Session::set_volume(float new_volume)
{
	CL_MutexSection mutex_lock(&impl->mutex);
	impl->volume = new_volume;
}

void CL_SoundBuffer_Session::set_pan(float new_pan)
{
	CL_MutexSection mutex_lock(&impl->mutex);
	impl->pan = new_pan;
}

void CL_SoundBuffer_Session::set_speedfactor(float new_speedfactor)
{
	CL_MutexSection mutex_lock(&impl->mutex);
	impl->speedfactor = fabs(new_speedfactor);
}

void CL_SoundBuffer_Session::play()
{
	CL_MutexSection mutex_lock(&impl->mutex);
	if (impl->playing) return;
	if (impl->provider_session->play())
	{
		impl->playing = true;
		CL_SoundOutput_Generic *output = impl->output;
		mutex_lock.leave();
		output->play_session(impl);
	}
}

void CL_SoundBuffer_Session::stop()
{
	if (!impl) return;
	CL_MutexSection mutex_lock(&impl->mutex);
	if (!impl->playing) return;
	CL_SoundOutput_Generic *output = impl->output;
	mutex_lock.leave();
	output->stop_session(impl);
	mutex_lock.enter();
	impl->playing = false;
	impl->provider_session->stop();
}

void CL_SoundBuffer_Session::set_looping(bool loop)
{
	CL_MutexSection mutex_lock(&impl->mutex);
	impl->looping = loop;
	impl->provider_session->set_looping(loop);
}

void CL_SoundBuffer_Session::add_filter(CL_SoundFilter *filter, bool delete_filter)
{
	CL_MutexSection mutex_lock(&impl->mutex);
	impl->filters.push_back(filter);
	impl->delete_filters.push_back(delete_filter);
}


void CL_SoundBuffer_Session::remove_all_filters()
{
	if (!impl) return;

	CL_MutexSection mutex_lock(&impl->mutex);
	for (std::vector<CL_SoundFilter *>::size_type i=0; i<impl->filters.size(); i++)
	{
		if (impl->delete_filters[i]) delete impl->filters[i];
	}

	impl->filters.clear();
	impl->delete_filters.clear();
}

void CL_SoundBuffer_Session::remove_filter(CL_SoundFilter *filter)
{
	CL_MutexSection mutex_lock(&impl->mutex);
	for (std::vector<CL_SoundFilter *>::size_type i=0; i<impl->filters.size(); i++)
	{
		if (impl->filters[i] == filter)
		{
			if (impl->delete_filters[i]) delete impl->filters[i];
			impl->filters.erase(impl->filters.begin()+i);
			impl->delete_filters.erase(impl->delete_filters.begin()+i);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundBuffer_Session implementation:

