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
#include "tool_window.h"
#include "workspace.h"
#include "API/GameIDE/Workspace/dockable_component.h"

namespace clan
{

ToolWindow::ToolWindow(DockableComponent *dockable, Rect window_geometry)
: GUIComponent(Workspace::instance(), get_top_level_description(dockable->get_title(), window_geometry), "tool-window"), dockable(dockable), is_moving(false)
{
	part_header = GUIThemePart(this, "header");
	part_button_area = GUIThemePart(this, "button-area");
	func_close().set(this, &ToolWindow::on_close);
	func_resized().set(this, &ToolWindow::on_resized);
	func_render().set(this, &ToolWindow::on_render);
	func_input_pressed().set(this, &ToolWindow::on_input_pressed);
	func_input_released().set(this, &ToolWindow::on_input_released);
	func_input_doubleclick().set(this, &ToolWindow::on_input_doubleclick);
	func_input_pointer_moved().set(this, &ToolWindow::on_input_pointer_moved);
	button_close = new PushButton(this);
	button_close->func_clicked().set(this, &ToolWindow::on_button_close_clicked);
	dockable->set_parent_component(this);
	dockable->set_visible(true, false);
	set_visible(true, false);
	on_resized();
	request_repaint();
}

void ToolWindow::update_layout_boxes()
{
	resize_box = get_size();
	resize_content_box = get_content_box();
	int header_height = part_header.get_css_height();
	header_box = Rect(resize_content_box.left, resize_content_box.top, resize_content_box.right, min(resize_content_box.top + header_height, resize_content_box.bottom));
	int button_area_width = part_button_area.get_css_width();
	button_area_box = Rect(max(header_box.right - button_area_width, header_box.left), header_box.top, header_box.right, header_box.bottom);
	button_area_content_box = part_button_area.get_content_box(button_area_box);
	dockable_box = Rect(resize_content_box.left, header_box.bottom, resize_content_box.right, resize_content_box.bottom);
}

void ToolWindow::on_render(Canvas &canvas, const Rect &update_box)
{
	update_layout_boxes();
	part_header.render_box(canvas, header_box);
	part_button_area.render_box(canvas, button_area_box);
	part_header.render_text(canvas, dockable->get_title(), part_header.get_content_box(header_box));
}

void ToolWindow::on_resized()
{
	update_layout_boxes();
	button_close->set_geometry(button_area_content_box);
	dockable->set_geometry(dockable_box);
}

bool ToolWindow::on_close()
{
	dockable->hide();
	return true;
}

void ToolWindow::on_button_close_clicked()
{
	on_close();
}

bool ToolWindow::on_input_pressed(const InputEvent &input_event)
{
	if (input_event.id == mouse_left && header_box.contains(input_event.mouse_pos))
	{
		dock_icons_overlay.reset(new DockIconsOverlay());
		last_move_pos = component_to_screen_coords(input_event.mouse_pos);
		capture_mouse(true);
		is_moving = true;
		return true;
	}
	else
	{
		return false;
	}
}

bool ToolWindow::on_input_released(const InputEvent &input_event)
{
	if (input_event.id == mouse_left && is_moving)
	{
		capture_mouse(false);
		is_moving = false;
		Dock *dock = dock_icons_overlay->try_dock_at(component_to_screen_coords(input_event.mouse_pos));
		dock_icons_overlay.reset();
		if (dock)
			dockable->show_as_docked(dock);
		return true;
	}
	else
	{
		return false;
	}
}

bool ToolWindow::on_input_doubleclick(const InputEvent &input_event)
{
	if (input_event.id == mouse_left)
	{
		if(header_box.contains(input_event.mouse_pos))
		{
			Dock *dock = Workspace::instance()->find_dock(dockable->get_last_docked_id());

			if(dock)
				dockable->show_as_docked(dock);
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool ToolWindow::on_input_pointer_moved(const InputEvent &input_event)
{
	if (is_moving)
	{
		Point new_pos = component_to_screen_coords(input_event.mouse_pos);
		Point delta = new_pos - last_move_pos;
		last_move_pos = new_pos;
		Rect window_geometry = get_window_geometry();
		window_geometry.translate(delta);
		set_window_geometry(window_geometry);
		dock_icons_overlay->set_cursor_pos(new_pos);
		return true;
	}
	else
	{
		return false;
	}
}

GUITopLevelDescription ToolWindow::get_top_level_description(const std::string &title, const Rect &box)
{
	GUITopLevelDescription desc;
	desc.set_position(box, false);
	desc.set_title(title);
	desc.set_allow_resize(true);
	desc.show_caption(false);
	desc.set_tool_window(true);
	desc.set_visible(false);
	return desc;
}

}
