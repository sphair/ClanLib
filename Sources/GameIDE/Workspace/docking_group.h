/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

namespace clan
{

class Dock;

class DockingGroup : public GUIComponent
{
public:
	DockingGroup(GUIComponent *parent);

	Dock *get_center() const { return center; }
	void set_center(Dock *center);
	void update_layout();

	Dock *find_dock(int dock_id);

	std::vector<DockingGroup *> left;
	std::vector<DockingGroup *> right;
	std::vector<DockingGroup *> top;
	std::vector<DockingGroup *> bottom;
	bool is_empty() const;

private:
	void on_resized();
	bool on_input_pressed(const InputEvent &input_event);
	bool on_input_released(const InputEvent &input_event);
	bool on_input_pointer_moved(const InputEvent &input_event);
	int get_left_preferred_width();
	int get_right_preferred_width();
	int get_top_preferred_height();
	int get_bottom_preferred_height();
	void set_left_geometry(Rect client_box, Rect center_box);
	void set_right_geometry(Rect client_box, Rect center_box);
	void set_top_geometry(Rect client_box);
	void set_bottom_geometry(Rect client_box);

	Dock *center;
	std::vector<Rect> left_grabber_boxes;
	std::vector<Rect> right_grabber_boxes;
	std::vector<Rect> top_grabber_boxes;
	std::vector<Rect> bottom_grabber_boxes;
	static const int border_width = 5;
	static const int border_height = 5;
	enum SizingMode
	{
		not_sizing,
		sizing_left,
		sizing_right,
		sizing_top,
		sizing_bottom
	};
	SizingMode sizing_mode;
	DockingGroup *sizing_group;
	Point sizing_start_pos;
	int sizing_start_size;
};

}
