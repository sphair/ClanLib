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

#include "dock_icons_overlay.h"

namespace clan
{

class Workspace;
class DockableComponent;

class ToolWindow : public GUIComponent
{
public:
	ToolWindow(DockableComponent *dockable, Rect window_geometry);
	DockableComponent *get_dockable() { return dockable; }

private:
	void update_layout_boxes();
	void on_render(Canvas &canvas, const Rect &update_box);
	void on_resized();
	bool on_close();
	void on_button_close_clicked();
	bool on_input_pressed(const InputEvent &input_event);
	bool on_input_released(const InputEvent &input_event);
	bool on_input_doubleclick(const InputEvent &input_event);
	bool on_input_pointer_moved(const InputEvent &input_event);
	static GUITopLevelDescription get_top_level_description(const std::string &title, const Rect &box);

	DockableComponent *dockable;
	PushButton *button_close;
	GUIThemePart part_header;
	GUIThemePart part_button_area;
	Rect resize_box;
	Rect resize_content_box;
	Rect header_box;
	Rect button_area_box;
	Rect button_area_content_box;
	Rect dockable_box;
	bool is_moving;
	Point last_move_pos;
	std::unique_ptr<DockIconsOverlay> dock_icons_overlay;
};

}

