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
**    Mark Page
*/

#include "Sound/precomp.h"
#include "fadefilter_provider.h"

namespace clan
{
	FadeFilterProvider::FadeFilterProvider(float initial_volume)
	{
		if (initial_volume < 0.0f)
			initial_volume = 0.0f;
		if (initial_volume > 1.0f)
			initial_volume = 1.0f;

		cur_volume = initial_volume;
		new_volume = initial_volume;
		speed = 0;
	}

	FadeFilterProvider::~FadeFilterProvider()
	{
	}

	float FadeFilterProvider::get_volume() const
	{
		return cur_volume;
	}

	void FadeFilterProvider::set_volume(float new_volume)
	{
		if (new_volume < 0.0f)
			new_volume = 0.0f;
		if (new_volume > 1.0f)
			new_volume = 1.0f;

		cur_volume = new_volume;
		speed = 0.0f;
	}

	void FadeFilterProvider::fade_to_volume(float volume, int duration)
	{
		if (volume < 0.0f)
			volume = 0.0f;
		if (volume > 1.0f)
			volume = 1.0f;

		new_volume = volume;

		float delta_volume = new_volume - cur_volume;
		speed = delta_volume / 22.05f / duration;
	}

	void FadeFilterProvider::filter(float **sample_data, int num_samples, int channels)
	{
		for (int i = 0; i < num_samples; i++)
		{
			for (int j = 0; j < channels; j++)
			{
				sample_data[j][i] = (sample_data[j][i] * cur_volume);
			}

			// change volume for every second sample (because data is in stereo).
			cur_volume += speed;
			if (
				(speed > 0 && cur_volume > new_volume) ||
				(speed < 0 && cur_volume < new_volume))
			{
				cur_volume = new_volume;
				speed = 0;
			}
		}
	}
}
