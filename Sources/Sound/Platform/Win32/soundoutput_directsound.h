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
*/

#pragma once

#include "../../soundoutput_impl.h"
#include <dsound.h>

#ifdef __MINGW32__
#define _Pre_null_
#endif

namespace clan
{
	class SoundOutput_DirectSound : public SoundOutput_Impl
	{
	public:
		SoundOutput_DirectSound(int mixing_frequency, int mixing_latency = 50);
		~SoundOutput_DirectSound();

		LPDIRECTSOUND directsound;
		LPDIRECTSOUNDBUFFER soundbuffer;
		int frag_size;
		HANDLE sleep_event;
		HWND hwnd;
		LPDIRECTSOUNDNOTIFY notify;
		bool has_sound;
		int last_write_pos;

		/// \brief Called when we have no samples to play - and wants to tell the sound card
		/// \brief about this possible event.
		virtual void silence();

		/// \brief Returns the buffer size used by device (returned as number of [stereo] samples).
		virtual int get_fragment_size();

		/// \brief Writes a fragment to the sound card.
		virtual void write_fragment(float *data);

		/// \brief Waits until output source isn't full anymore.
		virtual void wait();

	private:
		void release_resources();
		void create_directsound_object();
		void set_cooperative_level();
		void set_fragment_size();
		int get_fragment_count() const;
		int get_bytes_per_sample() const;
		int get_buffer_size() const;
		void create_sound_buffer();
		void verify_sound_buffer_capabilities();
		void clear_sound_buffer();
		void create_notify_event();
		void retrieve_notify_interface();
		void set_notify_positions();
		void play_sound_buffer();
		int find_fragment_write_position();
		void write_to_sound_buffer(int write_pos, const float *data, int size);

		typedef HRESULT(WINAPI *FuncDirectSoundCreate)(_In_opt_ LPCGUID pcGuidDevice, LPDIRECTSOUND *ppDS, _Pre_null_ LPUNKNOWN pUnkOuter);

		static std::recursive_mutex dsound_mutex;
		static HMODULE dsound_dll;
		static FuncDirectSoundCreate directsound_create;
	};
}
