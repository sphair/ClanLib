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

CL_MenuModalLoop::CL_MenuModalLoop(CL_GUIManager manager)
: owner(0), menubar(0), running(false), ignore_menubar_mouse_up(true), popup(false)
{
	slot_filter = manager.sig_filter_message().connect(this, &CL_MenuModalLoop::on_filter_message);
}

CL_MenuModalLoop::~CL_MenuModalLoop()
{
}

void CL_MenuModalLoop::start(CL_GUIComponent *owner, CL_PopupMenu menu, const CL_Point &pos)
{
	if (running)
		throw CL_Exception("Menu loop already running!");
	running = true;
	this->owner = owner;
	create_popup_window(menu, pos);
	owner->capture_mouse(true);
	popup = true;
}

void CL_MenuModalLoop::start(CL_GUIComponent *owner, CL_MenuBar *menubar)
{
	if (running)
		throw CL_Exception("Menu loop already running!");
	running = true;
	this->owner = owner;
	this->menubar = menubar;
	create_popup_window(menubar->impl->get_selected_menu(), menubar->impl->get_submenu_screen_pos());
	owner->capture_mouse(true);
}

void CL_MenuModalLoop::end()
{
	if (menubar)
		menubar->impl->clear_selection();

	ignore_menubar_mouse_up = true;
	close_all_popup_windows();
	owner->capture_mouse(false);
	running = false;
	owner = 0;
	menubar = 0;
}

void CL_MenuModalLoop::on_filter_message(CL_GUIMessage &message)
{
	if (running)
	{
		if (message.get_type() == CL_GUIMessage_Input::get_type_name())
		{
			CL_GUIMessage_Input input_message(message);
			on_filter_input_message(input_message);
		}
		else if (message.get_type() == CL_GUIMessage_FocusChange::get_type_name())
		{
			// Check we are not losing focus (FIXME: Required for X11 compatibility - this line is not required for win32)
			if ( ( CL_GUIMessage_FocusChange(message).get_focus_type() == CL_GUIMessage_FocusChange::losing_focus) && (message.get_target() == owner) )
			{
				end();
				message.set_consumed();
			}
		}
	}
}

void CL_MenuModalLoop::on_filter_input_message(CL_GUIMessage_Input &message)
{
	CL_InputEvent e = message.get_event();

	e.mouse_pos = message.get_target()->component_to_screen_coords(e.mouse_pos);
	if (e.device.get_type() == CL_InputDevice::pointer)
	{
		CL_PopupMenuWindow *popup = find_popup_at(e.mouse_pos);
		if (popup)
		{
			on_popup_mouse_input(popup, e);
			message.set_consumed();
		}
		else if (is_above_menubar(e.mouse_pos))
		{
			on_menubar_mouse_input(e);
			message.set_consumed();
		}
		else
		{
			if (e.type == CL_InputEvent::pressed)
			{
				end();
				message.set_consumed();
			}
			else
				message.set_consumed();
		}
	}
	else
	{
		on_keyboard_input(e);
		message.set_consumed();
	}
}

void CL_MenuModalLoop::create_popup_window(CL_PopupMenu menu, const CL_Point &pos)
{
	CL_PopupMenuWindow *window = new CL_PopupMenuWindow(menu, pos, owner);
	popup_windows.push_back(window);
}

void CL_MenuModalLoop::close_all_popup_windows()
{
	while (!popup_windows.empty())
	{
		CL_PopupMenuWindow *window = popup_windows.back();
		if(!window->func_close().is_null())
			window->func_close().invoke();

		popup_windows.pop_back();
		delete window;
	}
}

void CL_MenuModalLoop::close_last_popup_window()
{
	if (!popup_windows.empty())
	{
		CL_PopupMenuWindow *window = popup_windows.back();
		if(!window->func_close().is_null())
			window->func_close().invoke();

		popup_windows.pop_back();
		delete window;
	}
}

void CL_MenuModalLoop::on_popup_mouse_input(CL_PopupMenuWindow *popup, CL_InputEvent e)
{
	e.mouse_pos = popup->screen_to_component_coords(e.mouse_pos);
	
	if (e.type == CL_InputEvent::released)
	{
		CL_PopupMenuItem pmi = popup->get_selected_item();
		if (!pmi.is_null() && !pmi.is_disabled())
		{
			end();
			if (!pmi.is_null())
			{
				if (pmi.is_checkable())
					pmi.set_checked(!pmi.is_checked());

				if (!pmi.func_clicked().is_null())
					pmi.func_clicked().invoke();
			}
		}
	}
	else if (e.type == CL_InputEvent::pointer_moved)
	{
		int previous_item_index = popup->get_selected_item_index();
		popup->select_item_at(e.mouse_pos);
		int new_item_index = popup->get_selected_item_index();
		if (previous_item_index != new_item_index)
		{
			if (popup_windows.back() != popup)
				close_last_popup_window();

			CL_PopupMenuItem pmi = popup->get_selected_item();

			if (!pmi.is_null() && pmi.has_submenu() && !pmi.is_disabled())
			{
				CL_Point pos = popup->get_submenu_screen_position();
				create_popup_window(pmi.get_submenu(), pos);
			}
		}
	}
}

void CL_MenuModalLoop::on_menubar_mouse_input(CL_InputEvent e)
{
	e.mouse_pos = menubar->screen_to_component_coords(e.mouse_pos);

	if (ignore_menubar_mouse_up && e.type == CL_InputEvent::released)
	{
		ignore_menubar_mouse_up = false;
	}
	else if (e.type == CL_InputEvent::released)
	{
		end();
	}
	else if (e.type == CL_InputEvent::pointer_moved)
	{
		int old_index = menubar->impl->get_selected_item_index();
		menubar->impl->select_item_at(e.mouse_pos);
		int new_index = menubar->impl->get_selected_item_index();
		if (new_index != old_index)
		{
			close_all_popup_windows();

			CL_PopupMenu menu = menubar->impl->get_selected_menu();
			create_popup_window(menu, menubar->impl->get_submenu_screen_pos());
		}
	}
}

void CL_MenuModalLoop::on_keyboard_input(CL_InputEvent e)
{
	CL_PopupMenuWindow *window = popup_windows.back();

	if (e.type == CL_InputEvent::pressed)
	{
		if (e.id == CL_KEY_LEFT && !popup)
		{
			CL_PopupMenuWindow *window = popup_windows.back();
			popup_windows.pop_back();
			delete window;
			
			if (popup_windows.empty())
			{
				menubar->impl->select_previous();
				create_popup_window(menubar->impl->get_selected_menu(), menubar->impl->get_submenu_screen_pos());
			}
			else
			{
				window = popup_windows.back();
				window->request_repaint();
			}
		}
		else if (e.id == CL_KEY_RIGHT && !popup)
		{
			CL_PopupMenuItem pmi = window->get_selected_item();
			if (!pmi.is_null() && pmi.has_submenu())
			{
				create_popup_window(pmi.get_submenu(), window->get_submenu_screen_position());
			}
			else
			{
				close_all_popup_windows();
				menubar->impl->select_next();
				create_popup_window(menubar->impl->get_selected_menu(), menubar->impl->get_submenu_screen_pos());
			}
		}
		else if (e.id == CL_KEY_DOWN)
		{
			window->select_next();
		}
		else if (e.id == CL_KEY_UP)
		{
			window->select_previous();
		}
	}
	else if (e.type == CL_InputEvent::released)
	{
		if (e.id == CL_KEY_ENTER || e.id == CL_KEY_NUMPAD_ENTER)
		{
			CL_PopupMenuItem pmi = window->get_selected_item();
			end();
			if (!pmi.func_clicked().is_null())
				pmi.func_clicked().invoke();
		}
		else if (e.id == CL_KEY_ESCAPE)
		{
			end();
		}
	}
}

CL_PopupMenuWindow *CL_MenuModalLoop::find_popup_at(const CL_Point &mouse_screen_pos)
{
	std::vector<CL_PopupMenuWindow*>::size_type i;
	for (i=0; i<popup_windows.size(); i++)
	{
		CL_Point pos = popup_windows[i]->screen_to_component_coords(mouse_screen_pos);
		CL_Rect r = popup_windows[i]->get_size();
		if (r.contains(pos))
		{
			return popup_windows[i];
		}
	}

	return 0;
}

bool CL_MenuModalLoop::is_above_menubar(const CL_Point &mouse_screen_pos)
{
	if (menubar)
	{
		CL_Point menu_pos  = menubar->screen_to_component_coords(mouse_screen_pos);
		CL_Rect r = menubar->get_geometry().get_size();
		if (r.contains(menu_pos))
			return true;
	}

	return false;
}
