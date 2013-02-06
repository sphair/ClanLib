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

class Workspace;
class DockableComponent;
class Dock;
class DockingGroup;

class DockIconsOverlay : public clan::GUIComponent
{
public:
	DockIconsOverlay();
	~DockIconsOverlay();
	void set_cursor_pos(clan::Point screen_pos);
	Dock *try_dock_at(clan::Point screen_pos);

private:
	enum Place { place_left, place_right, place_top, place_bottom, place_center };
	struct DockIconArea
	{
		DockIconArea(DockIconsOverlay *overlay, DockingGroup *group, Place place);
		clan::GUIThemePart theme_part;
		clan::Rect group_box;
		clan::Rect icon_box;
		clan::Rect hint_box;
		Place place;
	};

	void set_current_hint(int new_hint);
	void on_render(clan::Canvas &canvas, const clan::Rect &update_box);
	static clan::GUITopLevelDescription get_top_level_description();

	std::vector<DockIconArea> areas;
	clan::GUIThemePart part_dock_up;
	clan::GUIThemePart part_dock_down;
	clan::GUIThemePart part_dock_left;
	clan::GUIThemePart part_dock_right;
	clan::GUIThemePart part_dock_center;
	int current_hint;
};
