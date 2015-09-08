/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
#include "API/Sound/SoundProviders/soundprovider_raw.h"
#include "soundprovider_raw_impl.h"
#include "soundprovider_raw_session.h"

namespace clan
{
	SoundProvider_Raw::SoundProvider_Raw(
		void *sound_data,
		int num_samples,
		int bytes_per_sample,
		bool stereo,
		int frequency) : impl(std::make_shared<SoundProvider_Raw_Impl>())
	{
		int data_size = num_samples * bytes_per_sample;
		if (stereo) data_size *= 2;

		impl->sound_data = new unsigned char[data_size];
		memcpy(impl->sound_data, sound_data, data_size);
		impl->num_samples = num_samples;
		impl->bytes_per_sample = bytes_per_sample;
		impl->stereo = stereo;
		impl->frequency = frequency;
	}

	SoundProvider_Raw::~SoundProvider_Raw()
	{
	}

	SoundProvider_Session *SoundProvider_Raw::begin_session()
	{
		return new SoundProvider_Raw_Session(*this);
	}

	void SoundProvider_Raw::end_session(SoundProvider_Session *session)
	{
		delete session;
	}
}
