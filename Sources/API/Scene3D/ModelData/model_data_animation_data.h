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

#include "model_data_animation_timeline.h"

namespace clan
{

/// \brief Stores different values for different animation timelines
///
/// Animatable properties are stored in timelines. There is one timeline
/// per animation supported by the model.
///
/// For properties that are not to be animated, a single timeline is used
/// with a single value stored in it.
template<typename Type>
class ModelDataAnimationData
{
public:
	std::vector<ModelDataAnimationTimeline<Type> > timelines;

	bool has_multiple_values() const
	{
		return (!timelines.empty() && timelines[0].values.size() > 1) || timelines.size() > 1;
	}

	Type get_value(int animation_index, float timestamp) const
	{
		if (timelines.empty())
			return Type();

		int timeline_index = min(animation_index, (int)timelines.size() - 1);
		return timelines[timeline_index].get_value(timestamp);
	}

	Type get_single_value() const
	{
		return get_value(0, 0.0f);
	}

	void set_single_value(Type value)
	{
		timelines.resize(1);
		timelines[0].timestamps.resize(1);
		timelines[0].timestamps[0] = 0.0f;
		timelines[0].values.resize(1);
		timelines[0].values[0] = value;
	}
};

}

