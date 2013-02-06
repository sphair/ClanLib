
#include "precomp.h"
#include "docking_group.h"
#include "docked_frame.h"
#include "dock.h"
#include "workspace.h"

using namespace clan;

DockingGroup::DockingGroup(GUIComponent *parent)
: GUIComponent(parent, "docking-group"), center(0), sizing_mode(not_sizing), sizing_group(0)
{
	func_resized().set(this, &DockingGroup::on_resized);
	func_input_pressed().set(this, &DockingGroup::on_input_pressed);
	func_input_released().set(this, &DockingGroup::on_input_released);
	func_input_pointer_moved().set(this, &DockingGroup::on_input_pointer_moved);
}

void DockingGroup::set_center(Dock *new_center)
{
	center = new_center;
}

bool DockingGroup::is_empty() const
{
	for (size_t i = 0; i < left.size(); i++)
		if (!left[i]->is_empty())
			return false;
	for (size_t i = 0; i < right.size(); i++)
		if (!right[i]->is_empty())
			return false;
	for (size_t i = 0; i < top.size(); i++)
		if (!top[i]->is_empty())
			return false;
	for (size_t i = 0; i < bottom.size(); i++)
		if (!bottom[i]->is_empty())
			return false;
	if (center)
		return center->is_empty();
	else
		return true;
}

void DockingGroup::update_layout()
{
	set_visible(!is_empty(), false);
	on_resized();
	if (center)
		center->update_layout();
	for (size_t i = 0; i < left.size(); i++)
		left[i]->update_layout();
	for (size_t i = 0; i < right.size(); i++)
		right[i]->update_layout();
	for (size_t i = 0; i < top.size(); i++)
		top[i]->update_layout();
	for (size_t i = 0; i < bottom.size(); i++)
		bottom[i]->update_layout();
}

bool DockingGroup::on_input_pressed(const InputEvent &input_event)
{
	if (input_event.id == mouse_left)
	{
		for (size_t i = 0; i < left_grabber_boxes.size(); i++)
		{
			if (left_grabber_boxes[i].contains(input_event.mouse_pos))
			{
				sizing_start_pos = component_to_screen_coords(input_event.mouse_pos);
				sizing_mode = sizing_left;
				sizing_group = left[i];
				sizing_start_size = sizing_group->get_width();
				capture_mouse(true);
				return true;
			}
		}

		for (size_t i = 0; i < right_grabber_boxes.size(); i++)
		{
			if (right_grabber_boxes[i].contains(input_event.mouse_pos))
			{
				sizing_start_pos = component_to_screen_coords(input_event.mouse_pos);
				sizing_mode = sizing_right;
				sizing_group = right[i];
				sizing_start_size = sizing_group->get_width();
				capture_mouse(true);
				return true;
			}
		}

		for (size_t i = 0; i < top_grabber_boxes.size(); i++)
		{
			if (top_grabber_boxes[i].contains(input_event.mouse_pos))
			{
				sizing_start_pos = component_to_screen_coords(input_event.mouse_pos);
				sizing_mode = sizing_top;
				sizing_group = top[i];
				sizing_start_size = sizing_group->get_height();
				capture_mouse(true);
				return true;
			}
		}

		for (size_t i = 0; i < bottom_grabber_boxes.size(); i++)
		{
			if (bottom_grabber_boxes[i].contains(input_event.mouse_pos))
			{
				sizing_start_pos = component_to_screen_coords(input_event.mouse_pos);
				sizing_mode = sizing_bottom;
				sizing_group = bottom[i];
				sizing_start_size = sizing_group->get_height();
				capture_mouse(true);
				return true;
			}
		}

		return false;
	}
	else
	{
		return false;
	}
}

bool DockingGroup::on_input_released(const InputEvent &input_event)
{
	if (input_event.id == mouse_left && sizing_mode != not_sizing)
	{
		sizing_mode = not_sizing;
		sizing_group = 0;
		capture_mouse(false);
		return true;
	}
	else
	{
		return false;
	}
}

bool DockingGroup::on_input_pointer_moved(const InputEvent &input_event)
{
	if (sizing_mode != not_sizing)
	{
		Point cur_pos = component_to_screen_coords(input_event.mouse_pos);
		Point delta(cur_pos.x - sizing_start_pos.x, cur_pos.y - sizing_start_pos.y);
		int size = sizing_start_size;
		Rect box = sizing_group->get_geometry();
		switch (sizing_mode)
		{
		case sizing_left:
			size = max(size + delta.x, 32);
			box.right = box.left + size;
			break;
		case sizing_right:
			size = max(size - delta.x, 32);
			box.left = box.right - size;
			break;
		case sizing_top:
			size = max(size + delta.y, 32);
			box.bottom = box.top + size;
			break;
		case sizing_bottom:
			size = max(size - delta.y, 32);
			box.top = box.bottom - size;
			break;
		}
		sizing_group->set_geometry(box);
		update_layout();
		return true;
	}
	else
	{
		for (size_t i = 0; i < left_grabber_boxes.size(); i++)
		{
			if (left_grabber_boxes[i].contains(input_event.mouse_pos))
			{
				set_cursor(cursor_size_we);
				return true;
			}
		}

		for (size_t i = 0; i < right_grabber_boxes.size(); i++)
		{
			if (right_grabber_boxes[i].contains(input_event.mouse_pos))
			{
				set_cursor(cursor_size_we);
				return true;
			}
		}

		for (size_t i = 0; i < top_grabber_boxes.size(); i++)
		{
			if (top_grabber_boxes[i].contains(input_event.mouse_pos))
			{
				set_cursor(cursor_size_ns);
				return true;
			}
		}

		for (size_t i = 0; i < bottom_grabber_boxes.size(); i++)
		{
			if (bottom_grabber_boxes[i].contains(input_event.mouse_pos))
			{
				set_cursor(cursor_size_ns);
				return true;
			}
		}
		set_cursor(cursor_arrow);
		return true;
	}
}

void DockingGroup::on_resized()
{
	Rect client_box = get_size();
	Rect center_box(
		client_box.left + get_left_preferred_width(),
		client_box.top + get_top_preferred_height(),
		client_box.right - get_right_preferred_width(),
		client_box.bottom - get_bottom_preferred_height());
	center_box.right = max(center_box.left, center_box.right);
	center_box.bottom = max(center_box.top, center_box.bottom);
	if (center)
		center->set_geometry(center_box);
	set_left_geometry(client_box, center_box);
	set_right_geometry(client_box, center_box);
	set_top_geometry(client_box);
	set_bottom_geometry(client_box);
}

int DockingGroup::get_left_preferred_width()
{
	int width = 0;
	for (size_t i = 0; i < left.size(); i++)
	{
		if (!left[i]->is_empty())
			width += border_width + left[i]->get_width();
	}
	return width;
}

int DockingGroup::get_right_preferred_width()
{
	int width = 0;
	for (size_t i = 0; i < right.size(); i++)
	{
		if (!right[i]->is_empty())
			width += border_width + right[i]->get_width();
	}
	return width;
}

int DockingGroup::get_top_preferred_height()
{
	int height = 0;
	for (size_t i = 0; i < top.size(); i++)
	{
		if (!top[i]->is_empty())
			height += border_height + top[i]->get_height();
	}
	return height;
}

int DockingGroup::get_bottom_preferred_height()
{
	int height = 0;
	for (size_t i = 0; i < bottom.size(); i++)
	{
		if (!bottom[i]->is_empty())
			height += border_height + bottom[i]->get_height();
	}
	return height;
}

void DockingGroup::set_left_geometry(Rect client_box, Rect center_box)
{
	left_grabber_boxes.clear();
	int x = client_box.left;
	for (size_t i = 0; i < left.size(); i++)
	{
		if (!left[i]->is_empty())
		{
			int width = left[i]->get_width();
			left[i]->set_geometry(Rect(x, center_box.top, x + width, center_box.bottom));
			x += width;
			left_grabber_boxes.push_back(Rect(x, center_box.top, x + border_width, center_box.bottom));
			x += border_width;
		}
	}
}

void DockingGroup::set_top_geometry(Rect client_box)
{
	top_grabber_boxes.clear();
	int y = client_box.top;
	for (size_t i = 0; i < top.size(); i++)
	{
		if (!top[i]->is_empty())
		{
			int height = top[i]->get_height();
			top[i]->set_geometry(Rect(client_box.left, y, client_box.right, y + height));
			y += height;
			top_grabber_boxes.push_back(Rect(client_box.left, y, client_box.right, y + border_height));
			y += border_height;
		}
	}
}

void DockingGroup::set_right_geometry(Rect client_box, Rect center_box)
{
	right_grabber_boxes.clear();
	int x = client_box.right;
	for (size_t i = 0; i < right.size(); i++)
	{
		if (!right[i]->is_empty())
		{
			int width = right[i]->get_width();
			right[i]->set_geometry(Rect(x - width, center_box.top, x, center_box.bottom));
			x -= width;
			right_grabber_boxes.push_back(Rect(x - border_width, center_box.top, x, center_box.bottom));
			x -= border_width;
		}
	}
}

void DockingGroup::set_bottom_geometry(Rect client_box)
{
	bottom_grabber_boxes.clear();
	int y = client_box.bottom;
	for (size_t i = 0; i < bottom.size(); i++)
	{
		if (!bottom[i]->is_empty())
		{
			int height = bottom[i]->get_height();
			bottom[i]->set_geometry(Rect(client_box.left, y - height, client_box.right, y));
			y -= height;
			bottom_grabber_boxes.push_back(Rect(client_box.left, y - border_height, client_box.right, y));
			y -= border_height;
		}
	}
}

Dock *DockingGroup::find_dock(int dock_id)
{
	if(center)
		if(center->get_id() == dock_id)
			return center;

	for (size_t i = 0; i < left.size(); i++)
	{
		Dock *dock = left[i]->find_dock(dock_id);
		if(dock)
			return dock;
	}
	for (size_t i = 0; i < right.size(); i++)
	{
		Dock *dock = right[i]->find_dock(dock_id);
		if(dock)
			return dock;
	}
	for (size_t i = 0; i < top.size(); i++)
	{
		Dock *dock = top[i]->find_dock(dock_id);
		if(dock)
			return dock;
	}
	for (size_t i = 0; i < bottom.size(); i++)
	{
		Dock *dock = bottom[i]->find_dock(dock_id);
		if(dock)
			return dock;
	}

	return 0;
}
