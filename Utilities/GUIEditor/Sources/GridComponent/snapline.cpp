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
**    Kenneth Gangstoe
*/

#include "precomp.h"
#include "snapline.h"
#include <cstdlib>
#include <climits>

SnapLine::SnapLine(SnapLineType type, int offset, SnapLinePriority priority)
: type(type), offset(offset), priority(priority)
{
}

bool SnapLine::is_horizontal() const
{
	return (type == type_top || type == type_bottom || type == type_horizontal || type == type_baseline);
}

bool SnapLine::is_vertical() const
{
	return (type == type_left || type == type_right || type == type_vertical);
}

int SnapLine::get_offset() const
{
	return offset;
}

SnapLine::SnapLinePriority SnapLine::get_priority() const
{
	return priority;
}

SnapLine::SnapLineType SnapLine::get_type() const
{
	return type;
}

bool SnapLine::should_snap(SnapLine line1, SnapLine line2)
{
	if (line1.get_type() != line2.get_type())
	{
		return false;
	}
	return true;
}

clan::Vec2i SnapLine::snap(
	const clan::Rect &source_rect, 
	const std::vector<SnapLine> &source_snaplines, 
	const std::vector<SnapLineTarget> &targets)
{
	const int snap_distance = 5;

	clan::Vec2i delta;

	for (size_t source_snapline_index = 0; source_snapline_index < source_snaplines.size(); source_snapline_index++)
	{
		SnapLine source_snapline = source_snaplines[source_snapline_index];

		int source_position = source_snapline.get_offset();
		if(source_snapline.is_horizontal())
			source_position += source_rect.top + delta.y;
		else
			source_position += source_rect.left + delta.x;

		int distance = find_closest_snapdistance(source_snapline, source_position, targets);
		if(abs(distance) <= snap_distance)
		{
			if(source_snapline.is_horizontal())
				delta.y -= distance;
			else
				delta.x -= distance;
		}
	}

	return delta;
}

int SnapLine::find_closest_snapdistance(const SnapLine &source_snapline, int source_position, const std::vector<SnapLineTarget> &targets)
{
	int max_range = INT_MAX;
	
	for(size_t t = 0; t < targets.size(); ++t)
	{
		for(size_t s = 0; s < targets[t].snaplines.size(); ++s)
		{
			SnapLine target_snapline = targets[t].snaplines[s];

			if(SnapLine::should_snap(target_snapline, source_snapline))
			{
				clan::Rect target_rect = targets[t].rect;

				int target_position = target_snapline.get_offset();
				if(source_snapline.is_horizontal())
					target_position += target_rect.top;
				else
					target_position += target_rect.left;

				int distance = source_position - target_position;
				if(abs(distance) < abs(max_range))
				{
					max_range = distance;
				}
			}
		}
	}

	return max_range;
}
