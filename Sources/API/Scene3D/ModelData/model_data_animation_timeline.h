/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

#include "../../Core/Math/cl_math.h"

namespace clan
{

template<typename Type>
class ModelDataAnimationTimeline
{
public:
	/*enum InterpolationType
	{
		linear_interpolation
	};
	InterpolationType interpolation;*/
	std::vector<float> timestamps;
	std::vector<Type> values;

	Type get_value(float timestamp)
	{
		size_t index, index2;
		float t = find_animation_indices(timestamp, index, index2);
		return mix(values[index], values[index2], t);
	}

	float find_animation_indices(float timestamp, size_t &index, size_t &index2)
	{
		if (timestamps.empty())
		{
			index = 0;
			index2 = 0;
			return 0.0f;
		}

		index = binary_search(timestamp);
		index2 = min(index + 1, timestamps.size() - 1);

		float start = timestamps[index];
		float end = timestamps[index2];
		if (start != end)
			return clamp((timestamp - start) / (end - start), 0.0f, 1.0f);
		else
			return 0.0f;
	}

private:
	size_t binary_search(float timestamp)
	{
		size_t imin = 0;
		size_t imax = timestamps.size() - 1;
		while (imin < imax)
		{
			size_t imid = imin + (imax - imin) / 2;
			if (timestamps[imid] > timestamp)
				imax = imid;
			else
				imin = imid + 1;
		}
		if (imin < timestamps.size() - 1)
			return imin - 1;
		else
			return imin;
	}
};

template<>
inline Quaternionf ModelDataAnimationTimeline<Quaternionf>::get_value(float timestamp)
{
	size_t index, index2;
	float t = find_animation_indices(timestamp, index, index2);
	return Quaternionf::lerp(values[index], values[index2], t);
}

template<>
inline Mat4f ModelDataAnimationTimeline<Mat4f>::get_value(float timestamp)
{
	size_t index, index2;
	float t = find_animation_indices(timestamp, index, index2);
	return values[index];
}

}

