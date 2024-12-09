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

#include "Sound/precomp.h"
#include "API/Sound/soundbuffer_session.h"
#include "API/Sound/SoundProviders/soundprovider_session.h"
#include "API/Sound/soundfilter.h"
#include "soundbuffer_session_impl.h"
#include "soundoutput_impl.h"

namespace clan
{
	SoundBuffer_Session::SoundBuffer_Session()
	{
	}

	SoundBuffer_Session::SoundBuffer_Session(SoundBuffer &soundbuffer, bool looping, SoundOutput &output)
		: impl(std::make_shared<SoundBuffer_Session_Impl>(soundbuffer, looping, output))
	{
	}

	SoundBuffer_Session::~SoundBuffer_Session()
	{
	}

	void SoundBuffer_Session::throw_if_null() const
	{
		if (!impl)
			throw Exception("SoundBuffer_Session is null");
	}

	int SoundBuffer_Session::get_position() const
	{
		if (impl)
		{
			std::unique_lock<std::recursive_mutex> mutex_lock(impl->mutex);
			return impl->provider_session->get_position();
		}
		else
		{
			return 0;
		}
	}

	float SoundBuffer_Session::get_position_relative() const
	{
		if (impl)
		{
			std::unique_lock<std::recursive_mutex> mutex_lock(impl->mutex);
			int position = impl->provider_session->get_position();
			int length = impl->provider_session->get_num_samples();
			if (length == 0) return 1.0f;
			return position / (float)length;
		}
		else
		{
			return 0.0f;
		}
	}

	int SoundBuffer_Session::get_length() const
	{
		if (impl)
		{
			std::unique_lock<std::recursive_mutex> mutex_lock(impl->mutex);
			return impl->provider_session->get_num_samples();
		}
		else
		{
			return 0;
		}
	}

	int SoundBuffer_Session::get_frequency() const
	{
		if (impl)
		{
			return impl->frequency;
		}
		else
		{
			return 0;
		}
	}

	float SoundBuffer_Session::get_volume() const
	{
		if (impl)
		{
			std::unique_lock<std::recursive_mutex> mutex_lock(impl->mutex);
			return impl->volume;
		}
		else
		{
			return 0.0f;
		}
	}

	float SoundBuffer_Session::get_pan() const
	{
		if (impl)
		{
			std::unique_lock<std::recursive_mutex> mutex_lock(impl->mutex);
			return impl->pan;
		}
		else
		{
			return 0.0f;
		}
	}

	bool SoundBuffer_Session::get_looping() const
	{
		if (impl)
		{
			return impl->looping;
		}
		else
		{
			return false;
		}
	}

	const std::vector<SoundFilter>& SoundBuffer_Session::get_filters() const
	{
		return impl->filters;
	}

	bool SoundBuffer_Session::is_playing()
	{
		if (impl)
		{
			std::unique_lock<std::recursive_mutex> mutex_lock(impl->mutex);
			return impl->playing;
		}
		else
		{
			return false;
		}
	}

	bool SoundBuffer_Session::set_position(int new_pos)
	{
		if (impl)
		{
			std::unique_lock<std::recursive_mutex> mutex_lock(impl->mutex);
			if (impl->provider_session->set_position(new_pos))
			{
				// instantly update position here?
				return true;
			}
			return false;
		}
		else
		{
			return false;
		}
	}

	bool SoundBuffer_Session::set_end_position(int new_pos)
	{
		if (impl)
		{
			std::unique_lock<std::recursive_mutex> mutex_lock(impl->mutex);
			if (impl->provider_session->set_end_position(new_pos))
			{
				// instantly update end position here?
				return true;
			}
			return false;
		}
		else
		{
			return false;
		}
	}

	bool SoundBuffer_Session::set_position_relative(float new_pos)
	{
		if (impl)
		{
			int length = get_length();
			return set_position((int)(new_pos * length));
		}
		else
		{
			return false;
		}
	}

	void SoundBuffer_Session::set_volume(float new_volume)
	{
		if (impl)
			impl->volume = new_volume;
	}

	void SoundBuffer_Session::set_frequency(int new_frequency)
	{
		if (impl)
			impl->frequency = new_frequency;
	}

	void SoundBuffer_Session::set_pan(float new_pan)
	{
		if (impl)
			impl->pan = new_pan;
	}

	void SoundBuffer_Session::play()
	{
		if (impl)
		{
			std::unique_lock<std::recursive_mutex> mutex_lock(impl->mutex);
			if (impl->playing) return;
			if (impl->provider_session->play())
			{
				impl->playing = true;
				mutex_lock.unlock();
				impl->output.impl->play_session(*this);
			}
		}
	}

	void SoundBuffer_Session::stop()
	{
		if (impl)
		{
			std::unique_lock<std::recursive_mutex> mutex_lock(impl->mutex);
			if (!impl->playing) return;
			mutex_lock.unlock();
			impl->output.impl->stop_session(*this);
			mutex_lock.lock();
			impl->playing = false;
			impl->provider_session->stop();
		}
	}

	void SoundBuffer_Session::set_looping(bool loop)
	{
		if (impl)
		{
			std::unique_lock<std::recursive_mutex> mutex_lock(impl->mutex);
			impl->looping = loop;
			impl->provider_session->set_looping(loop);
		}
	}

	void SoundBuffer_Session::add_filter(SoundFilter &filter)
	{
		if (impl)
		{
			std::unique_lock<std::recursive_mutex> mutex_lock(impl->mutex);
			impl->filters.push_back(filter);
		}
	}

	void SoundBuffer_Session::remove_filter(SoundFilter &filter)
	{
		if (impl)
		{
			std::unique_lock<std::recursive_mutex> mutex_lock(impl->mutex);
			for (std::vector<SoundFilter>::size_type i = 0; i < impl->filters.size(); i++)
			{
				if (impl->filters[i] == filter)
				{
					impl->filters.erase(impl->filters.begin() + i);
				}
			}
		}
	}
}
