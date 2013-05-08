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

#include "GameIDE/precomp.h"
#include "workspace.h"
#include "dock_icons_overlay.h"
#include "docking_group.h"
#include "tab_dock.h"

namespace clan
{

DockIconsOverlay::DockIconsOverlay()
//: GUIComponent(Workspace::instance(), get_top_level_description()), current_hint(-1)
: GUIComponent(Workspace::instance(), "dock-icons-overlay"), current_hint(-1)
{
	set_geometry(Rect(Point(0,0), Workspace::instance()->get_size()));

	func_render().set(this, &DockIconsOverlay::on_render);
	part_dock_up = GUIThemePart(this, "up");
	part_dock_down = GUIThemePart(this, "down");
	part_dock_left = GUIThemePart(this, "left");
	part_dock_right = GUIThemePart(this, "right");
	part_dock_center = GUIThemePart(this, "center");
	set_visible(true, false);
	request_repaint();

	areas.push_back(DockIconArea(this, Workspace::instance()->root_docking_group, place_left));
	areas.push_back(DockIconArea(this, Workspace::instance()->root_docking_group, place_top));
	areas.push_back(DockIconArea(this, Workspace::instance()->root_docking_group, place_right));
	areas.push_back(DockIconArea(this, Workspace::instance()->root_docking_group, place_bottom));
	areas.push_back(DockIconArea(this, Workspace::instance()->root_docking_group, place_center));
}

DockIconsOverlay::~DockIconsOverlay()
{
	get_parent_component()->request_repaint();
}

DockIconsOverlay::DockIconArea::DockIconArea(DockIconsOverlay *overlay, DockingGroup *group, Place place)
: place(place)
{
	group_box = Rect(overlay->screen_to_component_coords(group->component_to_screen_coords(Point(0,0))), group->get_size());
	Rect center_box = Rect(overlay->screen_to_component_coords(group->get_center()->component_to_screen_coords(Point(0,0))), group->get_center()->get_size());
	Size margin(16, 16);
	switch (place)
	{
	case place_left:
		theme_part = overlay->part_dock_left;
		icon_box = Rect(Point(group_box.left + margin.width, group_box.get_center().y - theme_part.get_css_height() / 2), theme_part.get_css_size());
		hint_box = Rect(group_box.left, center_box.top, group_box.left + 300, center_box.bottom);
		break;
	case place_right:
		theme_part = overlay->part_dock_right;
		icon_box = Rect(Point(group_box.right - margin.width - theme_part.get_css_width(), group_box.get_center().y - theme_part.get_css_height() / 2), theme_part.get_css_size());
		hint_box = Rect(group_box.right - 300, center_box.top, group_box.right, center_box.bottom);
		break;
	case place_top:
		theme_part = overlay->part_dock_up;
		icon_box = Rect(Point(group_box.get_center().x - theme_part.get_css_width() / 2, group_box.top + margin.height), theme_part.get_css_size());
		hint_box = Rect(group_box.left, group_box.top, group_box.right, group_box.top + 200);
		break;
	case place_bottom:
		theme_part = overlay->part_dock_down;
		icon_box = Rect(Point(group_box.get_center().x - theme_part.get_css_width() / 2, group_box.bottom - margin.height - theme_part.get_css_height()), theme_part.get_css_size());
		hint_box = Rect(group_box.left, group_box.bottom - 200, group_box.right, group_box.bottom);
		break;
	case place_center:
		theme_part = overlay->part_dock_center;
		icon_box = Rect(Point(center_box.get_center().x - theme_part.get_css_width() / 2, center_box.get_center().y - theme_part.get_css_height() / 2), theme_part.get_css_size());
		hint_box = center_box;
		break;
	default:
		throw Exception("Unknown docking place");
	}
}

Dock *DockIconsOverlay::try_dock_at(Point screen_pos)
{
	Workspace *workspace = Workspace::instance();
	DockingGroup *root_group = workspace->root_docking_group;
	Point pos = screen_to_component_coords(screen_pos);
	for (size_t i = 0; i < areas.size(); i++)
	{
		if (areas[i].icon_box.contains(pos))
		{
			DockingGroup *group = 0;
			switch (areas[i].place)
			{
			default:
			case place_left:
				if (root_group->left.empty())
				{
					root_group->left.push_back(new DockingGroup(root_group));
					root_group->left.front()->set_geometry(Rect(0,0,300,0));
				}
				group = root_group->left.front();
				break;
			case place_right:
				if (root_group->right.empty())
				{
					root_group->right.push_back(new DockingGroup(root_group));
					root_group->right.front()->set_geometry(Rect(0,0,300,0));
				}
				group = root_group->right.front();
				break;
			case place_top:
				if (root_group->top.empty())
				{
					root_group->top.push_back(new DockingGroup(root_group));
					root_group->top.front()->set_geometry(Rect(0,0,0,300));
				}
				group = root_group->top.front();
				break;
			case place_bottom:
				if (root_group->bottom.empty())
				{
					root_group->bottom.push_back(new DockingGroup(root_group));
					root_group->bottom.front()->set_geometry(Rect(0,0,0,300));
				}
				group = root_group->bottom.front();
				break;
			case place_center:
				group = root_group;
				break;
			}
			if (!group->get_center())
				group->set_center(new TabDock(group, workspace->generate_dock_id()));
			return group->get_center();
		}
	}
	return 0;
}

void DockIconsOverlay::set_cursor_pos(Point screen_pos)
{
	Point pos = screen_to_component_coords(screen_pos);
	for (size_t i = 0; i < areas.size(); i++)
	{
		if (areas[i].icon_box.contains(pos))
		{
			set_current_hint(i);
			return;
		}
	}
	set_current_hint(-1);
}

void DockIconsOverlay::set_current_hint(int new_hint)
{
	if (current_hint != new_hint)
	{
		current_hint = new_hint;
		request_repaint();
	}
}

void DockIconsOverlay::on_render(Canvas &canvas, const Rect &update_box)
{
	//gc.clear(Colorf::transparent);
	Colorf c = Colorf::powderblue;
	c.a = 0.8f;

	if (current_hint != -1)
		canvas.fill_rect(areas[current_hint].hint_box, c);

	for (size_t i = 0; i < areas.size(); i++)
		areas[i].theme_part.render_box(canvas, areas[i].icon_box);
}

GUITopLevelDescription DockIconsOverlay::get_top_level_description()
{
	Rect workspace_box(Workspace::instance()->component_to_screen_coords(Point(0,0)), Workspace::instance()->get_size());

	GUITopLevelDescription desc;
	desc.set_position(workspace_box, false);
	desc.set_title("Dock icons overlay");
	desc.set_allow_resize(false);
	desc.show_caption(false);
	desc.set_layered(true);
	desc.set_visible(false);
	desc.set_tool_window(true);
	desc.set_topmost(true);
	return desc;
}

}
