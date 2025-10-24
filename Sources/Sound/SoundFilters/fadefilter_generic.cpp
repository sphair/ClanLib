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

#ifdef _MSC_VER
#pragma warning (disable:4786)
#endif

#include "fadefilter_generic.h"

CL_FadeFilter::CL_FadeFilter(float initial_volume)
{
	if(initial_volume < 0.0f)
		initial_volume = 0.0f;
	if(initial_volume > 1.0f)
		initial_volume = 1.0f;

	impl = new CL_FadeFilter_Generic;
	impl->cur_volume = initial_volume;
	impl->new_volume = initial_volume;
	impl->speed = 0;
}

CL_FadeFilter::~CL_FadeFilter()
{
	delete impl;
}

float CL_FadeFilter::get_volume() const
{
	return impl->cur_volume;
}

void CL_FadeFilter::set_volume(float new_volume)
{
	if(new_volume < 0.0f)
		new_volume = 0.0f;
	if(new_volume > 1.0f)
		new_volume = 1.0f;

	impl->cur_volume = new_volume;
	impl->speed = 0.0f;
}

void CL_FadeFilter::fade_to_volume(float new_volume, int duration)
{
	if(new_volume < 0.0f)
		new_volume = 0.0f;
	if(new_volume > 1.0f)
		new_volume = 1.0f;

	impl->new_volume = new_volume;

	float delta_volume = impl->new_volume - impl->cur_volume;
	impl->speed = delta_volume / 22.05f / duration;
}

void CL_FadeFilter::filter(int **sample_data, int num_samples, int channels)
{
	if (impl->speed == 0 && impl->cur_volume == 1)
	{
		return; //no reason to waste processor power here, we've already faded to our target
	}

	float &cur_volume = impl->cur_volume;
	float &new_volume = impl->new_volume;
	float &speed = impl->speed;

	for (int i=0; i<num_samples; i++)
	{
		for (int j=0; j<channels; j++)
		{
			sample_data[j][i] = (int) (sample_data[j][i] * cur_volume);
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
