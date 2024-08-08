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
#include "soundoutput_impl.h"
#include "soundbuffer_session_impl.h"
#include "API/Sound/soundfilter.h"
#include <algorithm>
#include "API/Sound/sound_sse.h"

namespace clan
{
	std::recursive_mutex SoundOutput_Impl::singleton_mutex;
	SoundOutput_Impl *SoundOutput_Impl::instance = nullptr;

	SoundOutput_Impl::SoundOutput_Impl(int mixing_frequency, int latency)
		: mixing_frequency(mixing_frequency), mixing_latency(latency), volume(1.0f),
		pan(0.0f), mix_buffer_size(0)
	{
		mix_buffers[0] = nullptr;
		mix_buffers[1] = nullptr;
		temp_buffers[0] = nullptr;
		temp_buffers[1] = nullptr;
		stereo_buffer = nullptr;

		std::unique_lock<std::recursive_mutex> lock(singleton_mutex);
		if (instance)
			throw Exception("Only a single instance of SoundOutput is allowed");
		instance = this;

	}

	SoundOutput_Impl::~SoundOutput_Impl()
	{
		SoundSSE::aligned_free(stereo_buffer);
		SoundSSE::aligned_free(mix_buffers[0]);
		SoundSSE::aligned_free(mix_buffers[1]);
		SoundSSE::aligned_free(temp_buffers[0]);
		SoundSSE::aligned_free(temp_buffers[1]);

		std::unique_lock<std::recursive_mutex> lock(singleton_mutex);
		instance = nullptr;
	}

	void SoundOutput_Impl::play_session(SoundBuffer_Session &session)
	{
		std::unique_lock<std::recursive_mutex> mutex_lock(mutex);
		sessions.push_back(session);
	}

	void SoundOutput_Impl::stop_session(SoundBuffer_Session &session)
	{
		std::unique_lock<std::recursive_mutex> mutex_lock(mutex);

		for (auto it = sessions.begin(); it != sessions.end(); ++it)
		{
			if (session.impl.get() == it->impl.get())
			{
				sessions.erase(it);
				break;
			}
		}
	}

	void SoundOutput_Impl::stop_all()
	{
		sessions.clear();
	}

	void SoundOutput_Impl::start_mixer_thread()
	{
		stop_flag = false;
		thread = std::thread(&SoundOutput_Impl::mixer_thread, this);
		//	thread.set_priority(cl_priority_highest);
	}

	void SoundOutput_Impl::stop_mixer_thread()
	{
		std::unique_lock<std::recursive_mutex> mutex_lock(mutex);
		stop_flag = true;
		mutex_lock.unlock();
		thread.join();
		thread = std::thread();
	}

	void SoundOutput_Impl::mix_fragment()
	{
		resize_mix_buffers();
		clear_mix_buffers();
		fill_mix_buffers();
		filter_mix_buffers();
		apply_master_volume_on_mix_buffers();
		clamp_mix_buffers();
		SoundSSE::pack_float_stereo(mix_buffers, mix_buffer_size, stereo_buffer);
	}

	void SoundOutput_Impl::mixer_thread()
	{
		mixer_thread_starting();

		while (if_continue_mixing())
		{
			// Mix some audio:
			mix_fragment();

			// Send mixed data to sound card:
			write_fragment(stereo_buffer);

			// Wait for sound card to want more:
			wait();
		}

		mixer_thread_stopping();
	}

	bool SoundOutput_Impl::if_continue_mixing()
	{
		return !stop_flag;
	}

	void SoundOutput_Impl::resize_mix_buffers()
	{
		if (get_fragment_size() != mix_buffer_size)
		{
			SoundSSE::aligned_free(stereo_buffer); stereo_buffer = nullptr;
			SoundSSE::aligned_free(mix_buffers[0]); mix_buffers[0] = nullptr;
			SoundSSE::aligned_free(mix_buffers[1]); mix_buffers[1] = nullptr;
			SoundSSE::aligned_free(temp_buffers[0]); temp_buffers[0] = nullptr;
			SoundSSE::aligned_free(temp_buffers[1]); temp_buffers[1] = nullptr;

			mix_buffer_size = get_fragment_size();
			//if (mix_buffer_size & 3)
			//	throw Exception("Fragment size must be a multiple of 4");

			mix_buffers[0] = (float *)SoundSSE::aligned_alloc(sizeof(float) * mix_buffer_size);
			mix_buffers[1] = (float *)SoundSSE::aligned_alloc(sizeof(float) * mix_buffer_size);
			temp_buffers[0] = (float *)SoundSSE::aligned_alloc(sizeof(float) * mix_buffer_size);
			temp_buffers[1] = (float *)SoundSSE::aligned_alloc(sizeof(float) * mix_buffer_size);
			stereo_buffer = (float *)SoundSSE::aligned_alloc(sizeof(float) * mix_buffer_size * 2);
			SoundSSE::set_float(stereo_buffer, mix_buffer_size * 2, 0.0f);
		}
	}

	void SoundOutput_Impl::clear_mix_buffers()
	{
		// Clear channel mixing buffers:
		SoundSSE::set_float(mix_buffers[0], mix_buffer_size, 0.0f);
		SoundSSE::set_float(mix_buffers[1], mix_buffer_size, 0.0f);
	}

	void SoundOutput_Impl::fill_mix_buffers()
	{
		std::unique_lock<std::recursive_mutex> mutex_lock(mutex);
		std::vector< SoundBuffer_Session > ended_sessions;
		std::vector< SoundBuffer_Session >::iterator it;
		for (it = sessions.begin(); it != sessions.end(); ++it)
		{
			SoundBuffer_Session session = *it;
			bool playing = session.impl->mix_to(mix_buffers, temp_buffers, mix_buffer_size, 2);
			if (!playing) ended_sessions.push_back(session);
		}

		// Release any sessions pending for removal:
		int size_ended_sessions = ended_sessions.size();
		for (int i = 0; i < size_ended_sessions; i++) stop_session(ended_sessions[i]);
	}

	void SoundOutput_Impl::filter_mix_buffers()
	{
		// Apply global filters to mixing buffers:
		std::unique_lock<std::recursive_mutex> mutex_lock(mutex);
		int size_filters = filters.size();
		int i;
		for (i = 0; i < size_filters; i++)
		{
			filters[i].filter(mix_buffers, mix_buffer_size, 2);
		}
	}

	void SoundOutput_Impl::apply_master_volume_on_mix_buffers()
	{
		// Calculate volume on left and right channel:
		float left_pan = 1 - pan;
		float right_pan = 1 + pan;
		if (left_pan < 0.0f) left_pan = 0.0f;
		if (left_pan > 1.0f) left_pan = 1.0f;
		if (right_pan < 0.0f) right_pan = 0.0f;
		if (right_pan > 1.0f) right_pan = 1.0f;
		if (volume < 0.0f) volume = 0.0f;
		if (volume > 1.0f) volume = 1.0f;

		float left_volume = volume * left_pan;
		float right_volume = volume * right_pan;

		SoundSSE::multiply_float(mix_buffers[0], mix_buffer_size, left_volume);
		SoundSSE::multiply_float(mix_buffers[1], mix_buffer_size, right_volume);
	}

	void SoundOutput_Impl::clamp_mix_buffers()
	{
		// Make sure values stay inside 16 bit range:
		for (auto & elem : mix_buffers)
		{
			for (int k = 0; k < mix_buffer_size; k++)
			{
				if (elem[k] > 1.0f)  elem[k] = 1.0f;
				else if (elem[k] < -1.0f) elem[k] = -1.0f;
			}
		}
	}
}
