/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
**    Harry Storbacka
*/

#include "GUI/precomp.h"
#include "menu_modal_loop.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_window_manager.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_message_activation_change.h"
#include "API/GUI/gui_message_focus_change.h"
#include "API/GUI/gui_message_close.h"
#include "API/GUI/Components/menubar.h"
#include "API/Display/Window/input_device.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "popupmenu_window.h"
#include "menubar_impl.h"

CL_MenuModalLoop::CL_MenuModalLoop(CL_GUIComponent *owner, CL_PopupMenu menu, const CL_Point &pos)
: owner(owner), menubar(0), popup_window(0), destroy_self_flag(false)
{
	CL_GUIManager manager = owner->get_gui_manager();
	popup_window = new CL_PopupMenuWindow(menu, pos, owner, &manager);
	popup_window->set_focus();
}

CL_MenuModalLoop::CL_MenuModalLoop(CL_GUIComponent *owner, CL_MenuBar *menubar, int show_index)
: owner(owner), menubar(menubar), popup_window(0), destroy_self_flag(false)
{
	if (show_index != -1)
	{
		CL_PopupMenu menu = menubar->get_menu(show_index);
		CL_Rect menu_rect = menubar->get_menu_rect(show_index);
		CL_Point pos(menu_rect.left, menu_rect.bottom);

		CL_GUIManager manager = owner->get_gui_manager();
		popup_window = new CL_PopupMenuWindow(menu, pos, menubar, &manager);
		popup_window->set_focus();
		menubar_shown_menu_index = show_index;
		menubar->impl->selected_index = menubar_shown_menu_index;
		menubar->invalidate_rect();
	}
}

CL_MenuModalLoop::~CL_MenuModalLoop()
{
	if (popup_window)
		delete popup_window;
}

void CL_MenuModalLoop::exec()
{
	destroy_self_flag = false;

	CL_GUIManager gui_manager = owner->get_gui_manager();
	owner->capture_mouse(true);

	CL_AcceleratorTable accel_table;
	exec_handler(accel_table, true);
}

void CL_MenuModalLoop::start()
{
	destroy_self_flag = true;

	owner->capture_mouse(true);

	CL_GUIManager gui_manager = owner->get_gui_manager();
	old_func_exec_handler = gui_manager.func_exec_handler();

	gui_manager.func_exec_handler().set(this, &CL_MenuModalLoop::exec_handler);

}

int CL_MenuModalLoop::exec_handler(CL_AcceleratorTable &accel_table, bool loop_until_complete)
{
	CL_GUIManager gui_manager = owner->get_gui_manager();

	while (!gui_manager.get_exit_flag() && !(popup_window == 0 && menubar == 0))
	{
		CL_GUIMessage message;
		if (loop_until_complete)
		{
			message = gui_manager.peek_message(true);
			if (message.is_null())
				break;
		}
		else
		{
			message = gui_manager.peek_message(false);
			if (message.is_null())
				return 0;
		}

		// Abort if user decides to do something else:
		if (message.is_type(CL_GUIMessage_ActivationChange::get_type_name()))
			break;
		if (message.is_type(CL_GUIMessage_FocusChange::get_type_name()))
			break;
		if (message.is_type(CL_GUIMessage_Close::get_type_name()))
			break;

		// Pass normal messages to their target windows as usual.
		// Our modal loop only captures input to our menu system.
		if (!message.is_type(CL_GUIMessage_Input::get_type_name()))
		{
			message = gui_manager.get_message();
			gui_manager.dispatch_message(message);
			continue;
		}

		if (is_mouse_click_through(CL_GUIMessage_Input(message)))
			break;

		message = gui_manager.get_message();
		process_message(CL_GUIMessage_Input(message));

		gui_manager.get_window_manager()->update();

	}

	owner->capture_mouse(false);
	int exit_code = -1;
	if (gui_manager.get_exit_flag())
	{
		exit_code = gui_manager.get_exit_code();
		gui_manager.clear_exit_flag();
	}

	if (menubar)
	{
		menubar->impl->selected_index = -1;
		menubar->invalidate_rect();
	}

	if (popup_window)
	{
		CL_PopupMenuItem pmi = popup_window->get_clicked_item();

		delete popup_window;
		popup_window = 0;

		if (!pmi.is_null() && !pmi.func_clicked().is_null())
			pmi.func_clicked().invoke();
	}

	if (destroy_self_flag)
	{
		gui_manager.func_exec_handler() = old_func_exec_handler;
		delete this;
	}
	return exit_code;

}
bool CL_MenuModalLoop::is_mouse_click_through(CL_GUIMessage_Input message_input)
{
	// check if mouse clicks fall outside any of our window areas (popup menus + menubar):
	CL_InputEvent input_event = message_input.get_event();
	if (input_event.device.get_type() == CL_InputDevice::pointer &&
		input_event.type == CL_InputEvent::pressed)
	{
		CL_Point screen_pos = owner->component_to_screen_coords(input_event.mouse_pos);
		if (get_menubar_screen_rect().contains(screen_pos))
			return false;
		CL_PopupMenuWindow *cur = popup_window;
		while (cur)
		{
			if (get_popup_window_screen_rect(cur).contains(screen_pos))
				return false;
			cur = cur->get_child_popup();
		}
		return true;
	}
	return false;
}

void CL_MenuModalLoop::process_message(CL_GUIMessage_Input message_input)
{
	CL_InputEvent input_event = message_input.get_event();
	CL_Point screen_pos = owner->component_to_screen_coords(input_event.mouse_pos);

	if (input_event.device.get_type() == CL_InputDevice::pointer)
	{
		if (get_menubar_screen_rect().contains(screen_pos))
		{
			process_menubar_mouse_message(message_input);
		}

		// Notify all popup's about mouse movement:
		// (this is easier to implement than track enter/leave)
		CL_PopupMenuWindow *cur = popup_window;
		while (cur)
		{
			process_popup_mouse_message(message_input, cur);
			cur = cur->get_child_popup();
		}

/*		else if (popup_window)
		{
			// Find last popup:
			CL_PopupMenuWindow *cur = popup_window;
			while (cur->get_child_popup())
				cur = cur->get_child_popup();

			// Search which window cursor hovers above:
			while (cur)
			{
				if (get_popup_window_screen_rect(cur).contains(screen_pos))
				{
					process_popup_mouse_message(message_input, cur);
					break;
				}
				cur = cur->get_owner_popup();
			}
		}
*/
	}
	else if (input_event.device.get_type() == CL_InputDevice::keyboard)
	{
		switch (input_event.type)
		{
		case CL_InputEvent::pressed:
			on_key_pressed(input_event);
			break;
		case CL_InputEvent::released:
			on_key_released(input_event);
			break;
		}
	}
}

void CL_MenuModalLoop::process_menubar_mouse_message(CL_GUIMessage_Input message_input)
{
	CL_InputEvent input_event = message_input.get_event();
	CL_Point screen_pos = owner->component_to_screen_coords(input_event.mouse_pos);
	input_event.mouse_pos = menubar->screen_to_component_coords(screen_pos);

	switch (input_event.type)
	{
	case CL_InputEvent::pressed:
		menubar_on_mouse_pressed(input_event);
		break;
	case CL_InputEvent::released:
		menubar_on_mouse_released(input_event);
		break;
	case CL_InputEvent::pointer_moved:
		menubar_on_mouse_moved(input_event);
		break;
	}
}

void CL_MenuModalLoop::menubar_on_mouse_pressed(CL_InputEvent input_event)
{
}

void CL_MenuModalLoop::menubar_on_mouse_released(CL_InputEvent input_event)
{
}

/*
#include "API/GL/opengl.h"
void test(CL_GraphicContext &gc, const CL_Rect &rect, CL_Callback_v2<CL_GraphicContext &, const CL_Rect &> old_cb)
{
	CL_OpenGL::set_active(gc);
	old_cb.invoke(gc, rect);
}
*/

void CL_MenuModalLoop::menubar_on_mouse_moved(CL_InputEvent input_event)
{
	int mouse_over_index = menubar->get_index(input_event.mouse_pos);
	if (mouse_over_index != -1 && mouse_over_index != menubar_shown_menu_index)
	{
		CL_PopupMenu mouse_over_menu = menubar->get_menu(mouse_over_index);
		CL_Point pos = menubar->get_menu_rect(mouse_over_index).get_bottom_left();

		CL_GUIManager manager = menubar->get_gui_manager();
		delete popup_window;
		popup_window = new CL_PopupMenuWindow(mouse_over_menu, pos, menubar, &manager);
//		popup_window->func_render().set(&test, popup_window->func_render());
		menubar_shown_menu_index = mouse_over_index;
		menubar->impl->selected_index = menubar_shown_menu_index;
		menubar->invalidate_rect();
	}
}

void CL_MenuModalLoop::process_popup_mouse_message(CL_GUIMessage_Input message_input, CL_PopupMenuWindow *popup)
{
	CL_InputEvent input_event = message_input.get_event();
	CL_Point screen_pos = owner->component_to_screen_coords(input_event.mouse_pos);
	CL_Point component_pos = popup->screen_to_component_coords(screen_pos);

	CL_InputEvent new_event = input_event;
	new_event.mouse_pos = component_pos;

	CL_GUIMessage_Input message;
	message.set_event(new_event);
	message.set_target(popup);
	owner->get_gui_manager().dispatch_message(message);
}

CL_Rect CL_MenuModalLoop::get_menubar_screen_rect()
{
	if (menubar == 0)
		return CL_Rect();

	CL_Rect rect_menubar(CL_Point(0,0), menubar->get_geometry().get_size());
	CL_Point top_left = menubar->component_to_screen_coords(CL_Point(rect_menubar.left, rect_menubar.top));
	CL_Point bottom_right = menubar->component_to_screen_coords(CL_Point(rect_menubar.right, rect_menubar.bottom));
	CL_Rect screen_rect_menubar = CL_Rect(top_left.x, top_left.y, bottom_right.x, bottom_right.y);
	return screen_rect_menubar;
}

CL_Rect CL_MenuModalLoop::get_popup_window_screen_rect(CL_PopupMenuWindow *popup_window)
{
	if (popup_window == 0)
		return CL_Rect();

	CL_Rect rect_popup_window(CL_Point(0,0), popup_window->get_geometry().get_size());
	CL_Point top_left = popup_window->component_to_screen_coords(CL_Point(rect_popup_window.left, rect_popup_window.top));
	CL_Point bottom_right = popup_window->component_to_screen_coords(CL_Point(rect_popup_window.right, rect_popup_window.bottom));
	CL_Rect screen_rect_popup_window = CL_Rect(top_left.x, top_left.y, bottom_right.x, bottom_right.y);
	return screen_rect_popup_window;
}

void CL_MenuModalLoop::on_key_pressed(CL_InputEvent &input_event)
{
	if (popup_window == 0)
	{
		on_menubar_key_pressed(input_event);
	}
	else
	{
		CL_PopupMenuWindow *cur_window = popup_window;
		while (cur_window->get_child_popup())
			cur_window = cur_window->get_child_popup();
		on_popupmenu_key_pressed(input_event, cur_window);
	}
}

void CL_MenuModalLoop::on_key_released(CL_InputEvent &input_event)
{
	if (popup_window == 0)
		on_menubar_key_released(input_event);

	if (popup_window == 0)
		return;

	CL_PopupMenuWindow *cur_window = popup_window;
	while (cur_window->get_child_popup())
		cur_window = cur_window->get_child_popup();
	on_popupmenu_key_released(input_event, cur_window);
}

void CL_MenuModalLoop::on_menubar_key_pressed(CL_InputEvent &input_event)
{
	int old_selected_menu_index = menubar->impl->selected_index;

	if (input_event.id == CL_KEY_ESCAPE)
	{
		owner->exit_with_code(0);
	}
	else if (input_event.id == CL_KEY_LEFT)
	{
		menubar->impl->selected_index--;
		if (menubar->impl->selected_index < 0)
			menubar->impl->selected_index = 0;
		menubar->invalidate_rect();
	}
	else if (input_event.id == CL_KEY_RIGHT)
	{
		menubar->impl->selected_index++;
		if (menubar->impl->selected_index == menubar->impl->menus.size())
			menubar->impl->selected_index = menubar->impl->menus.size()-1;
		menubar->invalidate_rect();
	}

	if (old_selected_menu_index != menubar->impl->selected_index)
	{
		CL_PopupMenu new_menu = menubar->get_menu(menubar->impl->selected_index);
		CL_GUIManager manager = menubar->get_gui_manager();
		delete popup_window;
		CL_Rect rect = menubar->get_menu_rect(menubar->impl->selected_index);
		popup_window = new CL_PopupMenuWindow(new_menu, rect.get_bottom_left(), menubar, &manager);
	}
}

void CL_MenuModalLoop::on_menubar_key_released(CL_InputEvent &input_event)
{
}

void CL_MenuModalLoop::on_popupmenu_key_pressed(CL_InputEvent &input_event, CL_PopupMenuWindow *popup)
{
	bool consumed = false;

	if (popup)
	{
		CL_GUIMessage_Input input_msg;
		input_msg.set_event(input_event);
		input_msg.set_target(popup);
		owner->get_gui_manager().dispatch_message(input_msg);
		consumed = input_msg.is_consumed();
	}
	if (!consumed && menubar && (input_event.id == CL_KEY_LEFT || input_event.id == CL_KEY_RIGHT))
	{
		on_menubar_key_pressed(input_event);
	}
}

void CL_MenuModalLoop::on_popupmenu_key_released(CL_InputEvent &input_event, CL_PopupMenuWindow *popup)
{
	if (input_event.id == CL_KEY_ESCAPE)
	{
		popup->get_root_popup()->exit_with_code(0);
	}
	else 
	{
		CL_GUIMessage_Input input_msg;
		input_msg.set_event(input_event);
		input_msg.set_target(popup);
		owner->get_gui_manager().dispatch_message(input_msg);
	}
}
