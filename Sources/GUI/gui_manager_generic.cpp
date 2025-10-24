/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "precomp.h"
#include "API/Core/System/system.h"
#include "API/GUI/component.h"
#include "API/Display/display.h"
#include "API/Display/keys.h"
#include "API/Display/keyboard.h"
#include "API/Display/mouse.h"
#include "gui_manager_generic.h"
#include "component_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GUIManager_Generic construction:

CL_GUIManager_Generic::CL_GUIManager_Generic(CL_GUIManager *_owner)
:
	owner(_owner),
	input_enabled(true),
	current_tab_id(0),
	comp_focus(_owner),
	comp_capture_mouse(NULL),
	comp_mouse_over(NULL),
	focus_component(NULL)
{
	owner->set_gui_manager(owner);

	slots.connect(CL_Keyboard::sig_key_down(),
		this, &CL_GUIManager_Generic::on_input_down);
	slots.connect(CL_Keyboard::sig_key_up(),
		this, &CL_GUIManager_Generic::on_input_up);
	slots.connect(CL_Mouse::sig_key_down(),
		this, &CL_GUIManager_Generic::on_input_down);
	slots.connect(CL_Mouse::sig_key_up(),
		this, &CL_GUIManager_Generic::on_input_up);
	slots.connect(CL_Mouse::sig_move(),
		this, &CL_GUIManager_Generic::on_mouse_move);
	slots.connect(CL_Mouse::sig_key_dblclk(),
		this, &CL_GUIManager_Generic::on_mouse_dblclk);
	slots.connect(CL_Display::sig_resize(),
		this, &CL_GUIManager_Generic::on_resize);
	slots.connect(CL_Display::sig_paint(),
		this, &CL_GUIManager_Generic::on_paint);
}

CL_GUIManager_Generic::~CL_GUIManager_Generic()
{
	disable_input();
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIManager_Generic attributes:

CL_Component *CL_GUIManager_Generic::get_focus()
{
	return comp_focus;
}

CL_Component *CL_GUIManager_Generic::get_capture()
{
	return comp_capture_mouse;
}

bool CL_GUIManager_Generic::is_input_enabled()
{
	return input_enabled;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIManager_Generic operations:

void CL_GUIManager_Generic::set_focus(CL_Component *component)
{
	if (component == 0) return;
	if (comp_focus == component) return;

	CL_Component *old_comp_focus = comp_focus;

	comp_focus = component;
	if(old_comp_focus)
		old_comp_focus->sig_lost_focus()();
	if(comp_focus)
		comp_focus->sig_got_focus()();
	
	sig_focus_changed(comp_focus);
	owner->update();
}

void CL_GUIManager_Generic::quit()
{
	quit_run = true;
}

void CL_GUIManager_Generic::run(CL_Component *modal_level)
{
	modal_stack.push(modal_level);

	try
	{
		quit_run = false;
		while (!quit_run)
		{
			if (update_trigger.get_flag())
			{
				update_trigger.reset();
				show();
				CL_Display::flip();
			}
			CL_System::keep_alive(10);
		}
		quit_run = false;
	}
	catch (...)
	{
		modal_stack.pop();
		throw;
	}

	modal_stack.pop();
}

void CL_GUIManager_Generic::show()
{
	owner->paint();
}

void CL_GUIManager_Generic::enable_input()
{
	input_enabled = true;
}

void CL_GUIManager_Generic::disable_input()
{
	input_enabled = false;
}

void CL_GUIManager_Generic::gui_capture_mouse(CL_Component *component)
{
	comp_capture_mouse = component;
}

void CL_GUIManager_Generic::gui_release_mouse()
{
	comp_capture_mouse = NULL;
}

void CL_GUIManager_Generic::component_deleted(CL_Component *component)
{
	// First try to move focus to someone else:
	if (component->get_parent()) owner->set_focus(component->get_parent());

	// Make sure we dont crash no matter what:
	if (comp_focus == component) comp_focus = 0;
	if (comp_capture_mouse == component) comp_capture_mouse = 0;
	if (comp_mouse_over == component) comp_mouse_over = 0;
	if (focus_component == component) focus_component = 0;
}

CL_Component *CL_GUIManager_Generic::find_focus_component(const CL_InputEvent &key)
{
	CL_Component *focus_component = comp_focus;

	if (comp_capture_mouse)
	{
		focus_component = comp_capture_mouse;
	}
	else if (key.device.get_type() == CL_InputDevice::mouse)
	{
	focus_component = owner->get_component_at(
			(int)(key.mouse_pos.x - owner->get_position().left),
			(int)(key.mouse_pos.y - owner->get_position().top));
	}

	// If we are in a modal run, make sure input is mapped to modal tree:
	if (get_modal_component())
	{
		if (focus_component == get_modal_component() ||
			get_modal_component()->has_child(focus_component))
		{
		}
		else
		{
			focus_component = get_modal_component();
		}
	}

	return focus_component;
}

void CL_GUIManager_Generic::on_input_down(const CL_InputEvent &key)
{
	if (!input_enabled) return;

	// Transform from screen coordinates to GUI coordinate.
	int mouse_x = (int) key.mouse_pos.x;
	int mouse_y = (int) key.mouse_pos.y;
	owner->sig_transform_coords()(mouse_x, mouse_y);

	CL_InputEvent transformed_key = key;
	transformed_key.mouse_pos.x = mouse_x;
	transformed_key.mouse_pos.y = mouse_y;

	// Figure out what our focus component is (whom to send message):
	focus_component = find_focus_component(transformed_key);
	if (!focus_component)
		return;

	// If its a mouse click, make the clicked component get focus.
	if (key.device.get_type() == CL_InputDevice::mouse)
		if (focus_component->is_focusable())
			set_focus(focus_component);

	// If its a tab click, pass on keyboard input to next component.
	if (key.id == CL_KEY_TAB)
	{
		if(key.device.get_keycode(CL_KEY_LSHIFT) || key.device.get_keycode(CL_KEY_RSHIFT))
			focus_component = tab_previous();
		else
			focus_component = tab_next();
	}

	if (!focus_component)
		return;

	// If component is disabled, do not send event:
	if (focus_component->is_enabled() == false || focus_component->is_visible() == false)
		return;

	// Make coordinates relative to focus component.
	transformed_key.mouse_pos.x -= focus_component->get_screen_rect().left;
	transformed_key.mouse_pos.y -= focus_component->get_screen_rect().top;

	// Send input down event:
	focus_component->sig_input_down()(transformed_key);
	if (focus_component == 0) return; // component was deleted.

	// Send key down event:
	if(key.device.get_type() == CL_InputDevice::keyboard)
		focus_component->sig_key_down()(transformed_key);

	// Send mouse down event:
	if(key.device.get_type() == CL_InputDevice::mouse)
		focus_component->sig_mouse_down()(transformed_key);
}

void CL_GUIManager_Generic::on_mouse_dblclk(const CL_InputEvent &key)
{
	if (!input_enabled) return;

	// Transform from screen coordinates to GUI coordinate.
	int mouse_x = (int) key.mouse_pos.x;
	int mouse_y = (int) key.mouse_pos.y;
	owner->sig_transform_coords()(mouse_x, mouse_y);

	CL_InputEvent transformed_key = key;
	transformed_key.mouse_pos.x = mouse_x;
	transformed_key.mouse_pos.y = mouse_y;

	// Figure out what our focus component is (whom to send message):
	focus_component = find_focus_component(transformed_key);
	if (!focus_component)
		return;

	// If its a mouse click, make the clicked component get focus.
	if (focus_component->is_focusable())
		set_focus(focus_component);

	// If component is disabled, do not send event:
	if (focus_component->is_enabled() == false || focus_component->is_visible() == false)
		return;

	// Make coordinates relative to focus component.
	transformed_key.mouse_pos.x -= focus_component->get_screen_rect().left;
	transformed_key.mouse_pos.y -= focus_component->get_screen_rect().top;

	// Send input down event:
	focus_component->sig_input_down()(transformed_key);
	if (focus_component == 0) return; // component was deleted.

	transformed_key.repeat_count = 2;

	// Send mouse down event:
	focus_component->sig_mouse_dblclk()(transformed_key);
}

void CL_GUIManager_Generic::on_input_up(const CL_InputEvent &key)
{
	if (!input_enabled) return;

	// Do not pass tab support down through the gui:
	if (key.id == CL_KEY_TAB) return;

	// Transform from screen coordinates to GUI coordinate.
	int mouse_x = (int) key.mouse_pos.x;
	int mouse_y = (int) key.mouse_pos.y;
	owner->sig_transform_coords()(mouse_x, mouse_y);

	CL_InputEvent transformed_key = key;
	transformed_key.mouse_pos.x = mouse_x;
	transformed_key.mouse_pos.y = mouse_y;

	// Figure out what our focus component is (whom to send message):
	focus_component = find_focus_component(transformed_key);
	if (!focus_component)
		return;

	// Make coordinates relative to focus component.
	transformed_key.mouse_pos.x -= focus_component->get_screen_rect().left;
	transformed_key.mouse_pos.y -= focus_component->get_screen_rect().top;

	// Send input up event:
	focus_component->sig_input_up()(transformed_key);
	if (focus_component == 0) return; // component was deleted.

	// Send key up event:
	if(key.device.get_type() == CL_InputDevice::keyboard)
		focus_component->sig_key_up()(transformed_key);

	// Send mouse up event:
	if(key.device.get_type() == CL_InputDevice::mouse)
		focus_component->sig_mouse_up()(transformed_key);
}

void CL_GUIManager_Generic::on_mouse_move(const CL_InputEvent &key)
{
	if (!input_enabled) return;

	int x = key.mouse_pos.x;
	int y = key.mouse_pos.y;
	
	owner->sig_transform_coords()(x, y);

	CL_Component *over_component = owner->get_component_at(
			x - owner->get_position().left,
			y - owner->get_position().top);

	focus_component = comp_capture_mouse;
	if (!comp_capture_mouse) focus_component = over_component;

/*	CL_Component *focus_component = over_component;
	if (comp_capture_mouse)
		if(!comp_capture_mouse->has_child(over_component))
			focus_component = comp_capture_mouse;
*/	

	if (!focus_component) return;

	// Make coordinates relative to focus component.
	x -= focus_component->get_screen_rect().left;
	y -= focus_component->get_screen_rect().top;

	// Send mouse enter and mouse leave signals:
	do_mouse_enter_leave(x, y, over_component);

	if (focus_component == 0) return; // component was deleted.

	CL_InputEvent new_key = key;
	new_key.mouse_pos.x = x;
	new_key.mouse_pos.y = y;
	focus_component->sig_mouse_move()(new_key);
}

void CL_GUIManager_Generic::do_mouse_enter_leave(int x, int y, CL_Component *new_focus)
{
	CL_Component *old_focus = comp_mouse_over;
	if (x < 0 || y < 0 || x >= new_focus->get_width() || y >= new_focus->get_height()) new_focus = 0;

	// No signals needed if same component kept focus.
	if (old_focus == new_focus) return;

	if (old_focus)
	{
		old_focus->impl->mouse_over = false;
		old_focus->sig_mouse_leave()();
		if (focus_component == 0) return; // component was deleted.
	}
	if (new_focus)
	{
		new_focus->impl->mouse_over = true;
		new_focus->sig_mouse_enter()();
		if (focus_component == 0) return; // component was deleted.
	}

	comp_mouse_over = new_focus;
}

void CL_GUIManager_Generic::on_resize(int width, int height)
{
	owner->set_size(width, height);
}

void CL_GUIManager_Generic::on_paint(const CL_Rect &rect)
{
	owner->update();
}

CL_Component *CL_GUIManager_Generic::tab_next()
{
	// First try to find component by tab id:
	CL_Component *tab_component = owner->find_tab_id(++current_tab_id);
	if (tab_component == 0) // reached end, start from beginning
	{
		current_tab_id = 0;
		tab_component = owner->find_tab_id(current_tab_id);
	}
	if (tab_component)
	{
		set_focus(tab_component);
		return comp_focus;
	}

	if(comp_focus == 0)
		return 0;
		
	// No tab ids - fall back to tree walking:
	if (comp_focus->get_children().empty()) // tab to sibling or uncle
	{
		CL_Component *cur = comp_focus;

		while (true)
		{
			CL_Component *parent = cur->get_parent();
			if (parent == 0) break;

			// Search siblings:
			std::list<CL_Component *>::const_iterator it;
			for (it = parent->get_children().begin(); it != parent->get_children().end(); it++)
			{
				if (*it == cur)
				{
					it++;

					// no more siblings
					if (it == parent->get_children().end()) break;

					// give sibling focus
					set_focus(*it);
					return comp_focus;
				}
			}

			// Was last sibling, continue search in parent (uncles)
			cur = parent;
		}

		// if we are top node (no siblings), and got no children:
		if (cur == comp_focus) return comp_focus;

		// walked through all components. Restart at first component:
		set_focus(owner);
		return comp_focus;
	}
	else // tab to children
	{
		// give first child focus
		set_focus(comp_focus->get_children().front());
		return comp_focus;
	}
}

CL_Component *CL_GUIManager_Generic::tab_previous()
{
	// First try to find component by tab id:
	CL_Component *tab_component = owner->find_tab_id(--current_tab_id);
	if (current_tab_id == -1) // reached beginning, start from end
	{
		current_tab_id = find_highest_tab_id(owner);
		tab_component = owner->find_tab_id(current_tab_id);
	}
	if (tab_component)
	{
		set_focus(tab_component);
		return comp_focus;
	}

	if(comp_focus == 0)
		return 0;

	// No tab ids - fall back to tree walking:
	CL_Component *cur = comp_focus;
	while (true)
	{
		CL_Component *parent = cur->get_parent();
		if (parent == 0) break;

		// Search siblings:
		std::list<CL_Component *>::const_iterator it;
		for (it = parent->get_children().begin(); it != parent->get_children().end(); it++)
		{
			if (*it == cur)
			{
				// no more siblings
				if (it == parent->get_children().begin()) break;

				it--;

				// give sibling focus
				set_focus(*it);
				return comp_focus;
			}
		}

		// Was last sibling, continue search in parent (uncles)
		cur = parent;
	}

	// walked through all components. Restart at end component:
	cur = owner;
	while (!cur->get_children().empty()) cur = cur->get_children().back();
	set_focus(cur);
	return comp_focus;
}

#define cl_max(a,b) ((a > b) ? a : b)

int CL_GUIManager_Generic::find_highest_tab_id(const CL_Component *component)
{
	int id = cl_max(0, component->get_tab_id());
	const std::list<CL_Component *> &children = component->get_children();
	std::list<CL_Component *>::const_iterator it;
	for (it = children.begin(); it != children.end(); it++)
	{
		id = cl_max(id, find_highest_tab_id(*it));
	}
	return id;
}

CL_Component *CL_GUIManager_Generic::get_modal_component() const
{
	if (modal_stack.empty()) return 0;
	return modal_stack.top();
}
