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

#include "grid_edit_state_handler.h"
#include "GridComponent/snapline.h"

class GridEditStateObjectSizing : public GridEditStateHandler
{
public:
	GridEditStateObjectSizing();

	bool on_input_pressed(const InputEvent &input_event);
	bool on_input_released(const InputEvent &input_event);
	bool on_input_doubleclick(const InputEvent &input_event);
	bool on_input_pointer_moved(const InputEvent &input_event);

private:
	void resize_to(const Point &mouse_pos, bool perform_snap);
	Rect resize_rect(Rect geometry, Vec2i delta) const;
	std::vector<SnapLine> get_filtered_snaplines(const std::vector<SnapLine> &original_snaplines);

	size_t primary_object_index;
	Point start;
	std::vector<Rect> start_geometry;
	Vec2i dir;
};
