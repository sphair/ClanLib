/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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

#include <vector>
#include <list>
#include <memory>
#include <mutex>
#include <thread>
#include <atomic>

namespace clan
{
	class SoundFilter;
	class SoundBuffer_Session_Impl;
	class SoundBuffer_Session;

	class SoundOutput_Impl
	{
	public:
		SoundOutput_Impl(int mixing_frequency, int mixing_latency);
		virtual ~SoundOutput_Impl();

		void play_session(SoundBuffer_Session &session);
		void stop_session(SoundBuffer_Session &session);

	protected:
		std::string name;
		int mixing_frequency;
		int mixing_latency;
		float volume;
		float pan;
		std::vector<SoundFilter> filters;
		std::thread thread;
		std::atomic_bool stop_flag;
		std::vector< SoundBuffer_Session > sessions;

		int mix_buffer_size;
		float *mix_buffers[2];
		float *temp_buffers[2];
		float *stereo_buffer;

		/// \brief Called when we have no samples to play - and wants to tell the soundcard
		/// \brief about this possible event.
		virtual void silence() = 0;

		/// \brief Returns the buffer size used by device (returned as num [stereo] samples).
		virtual int get_fragment_size() = 0;

		/// \brief Writes a fragment to the soundcard.
		virtual void write_fragment(float *data) = 0;

		/// \brief Waits until output source isn't full anymore.
		virtual void wait() = 0;

		/// \brief Called by the mixer thread when it starts
		virtual void mixer_thread_starting() { }

		/// \brief Called by the mixer thread when it stops
		virtual void mixer_thread_stopping() { }

		/// \brief Starts a thread and call mix_fragment() and wait() continueously.
		void start_mixer_thread();

		/// \brief Stops the mixer thread.
		void stop_mixer_thread();

		/// \brief Mixes a single fragment and stores the result in stereo_buffer.
		void mix_fragment();

	private:
		/// \brief Worker thread for output device. Mixes the audio and sends it to write_fragment.
		void mixer_thread();

		/// \brief Returns true if the mixer thread should continue mixing fragments
		bool if_continue_mixing();

		/// \brief Ensures the mixing buffers match the fragment size
		void resize_mix_buffers();

		/// \brief Clears the content of the mixing buffers
		void clear_mix_buffers();

		/// \brief Mixes soundbuffer sessions into the mixing buffers
		void fill_mix_buffers();

		/// \brief Applies filters to the mixing buffers
		void filter_mix_buffers();

		/// \brief Apply master volume and panning to mix buffers
		void apply_master_volume_on_mix_buffers();

		/// \brief Clamp mixing buffer values to the -1 to 1 range
		void clamp_mix_buffers();

		static std::recursive_mutex singleton_mutex;
		static SoundOutput_Impl *instance;

		mutable std::recursive_mutex mutex;

		friend class SoundOutput;
	};
}
