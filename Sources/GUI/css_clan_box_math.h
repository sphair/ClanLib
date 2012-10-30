/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

#include "API/CSSLayout/css_box_properties.h"

namespace clan
{

class CSSClanBoxMath
{
public:
	std::vector<float> used_min_lengths;
	std::vector<float> used_max_lengths;
	std::vector<float> used_expand_weights;
	std::vector<float> used_shrink_weights;
	std::vector<float> used_lengths;

	void adjust(float used_content_length)
	{
		clamp_lengths();
		float available_space = find_sum(used_lengths) - used_content_length;
		if (available_space > 0.0f)
			expand_lengths(available_space);
		else if (available_space < -0.0f)
			shrink_lengths(available_space);
	}

	void clamp_lengths()
	{
		for (size_t i = 0; i < used_lengths.size(); i++)
		{
			used_lengths[i] = clamp(used_lengths[i], used_min_lengths[i], used_max_lengths[i]);
		}
	}

	void expand_lengths(float available_space)
	{
		float weight_sum = find_sum(used_expand_weights);
		while (weight_sum != 0.0f)
		{
			float new_weight_sum = 0.0f;
			float expand_length_sum = 0.0f;
			for (size_t i = 0; i < used_lengths.size(); i++)
			{
				float expand_length = available_space * used_expand_weights[i] / weight_sum;
				if (used_lengths[i] + expand_length >= used_max_lengths[i])
				{
					expand_length = used_max_lengths[i] - used_lengths[i];
				}
				else
				{
					new_weight_sum += used_expand_weights[i];
				}
				expand_length_sum += expand_length;
			}
			available_space -= expand_length_sum;
			if (weight_sum == new_weight_sum)
				break;
			weight_sum = new_weight_sum;
		}
	}

	void shrink_lengths(float available_space)
	{
		float weight_sum = find_sum(used_shrink_weights);
		while (weight_sum != 0.0f)
		{
			float new_weight_sum = 0.0f;
			float shrink_length_sum = 0.0f;
			for (size_t i = 0; i < used_lengths.size(); i++)
			{
				float shrink_length = available_space * used_shrink_weights[i] / weight_sum;
				if (used_lengths[i] + shrink_length <= used_min_lengths[i])
				{
					shrink_length = used_min_lengths[i] - used_lengths[i];
				}
				else
				{
					new_weight_sum += used_shrink_weights[i];
				}
				shrink_length_sum += shrink_length;
			}
			available_space -= shrink_length_sum;
			if (weight_sum == new_weight_sum)
				break;
			weight_sum = new_weight_sum;
		}
	}

	float find_sum(const std::vector<float> &vec)
	{
		float sum = 0.0f;
		for (size_t i = 0; i < vec.size(); i++)
			sum += vec[i];
		return sum;
	}
};

}
