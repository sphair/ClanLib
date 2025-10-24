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

#pragma once

#include "API/Core/Signals/callback_2.h"

class CL_GUIComponent;
class CL_InputEvent;
class CL_MenuBar;
class CL_PopupMenu;
class CL_PopupMenuWindow;
class CL_Point;
class CL_GUIMessage_Input;
class CL_Rect;
class CL_Size;
class CL_AcceleratorTable;

class CL_MenuModalLoop
{
public:
	CL_MenuModalLoop(CL_GUIComponent *owner, CL_PopupMenu menu, const CL_Point &pos);

	CL_MenuModalLoop(CL_GUIComponent *owner, CL_MenuBar *menubar, int show_index);

	~CL_MenuModalLoop();

	/// Used for modal windows
	void exec();

	/// Start the menu loop for non modal windows
	///
	/// This object will destroy itself when required
	void start();

private:
	bool is_mouse_click_through(CL_GUIMessage_Input message_input);

	void process_message(CL_GUIMessage_Input message_input);

	void process_menubar_mouse_message(CL_GUIMessage_Input message_input);

	void menubar_on_mouse_pressed(CL_InputEvent input_event);

	void menubar_on_mouse_moved(CL_InputEvent input_event);

	void menubar_on_mouse_released(CL_InputEvent input_event);

	void process_popup_mouse_message(CL_GUIMessage_Input message_input, CL_PopupMenuWindow *popup);

	CL_Rect get_menubar_screen_rect();

	CL_Rect get_popup_window_screen_rect(CL_PopupMenuWindow *popup_window);

	void on_key_pressed(CL_InputEvent &input_event);

	void on_key_released(CL_InputEvent &input_event);

	void on_menubar_key_pressed(CL_InputEvent &input_event);

	void on_menubar_key_released(CL_InputEvent &input_event);

	void on_popupmenu_key_pressed(CL_InputEvent &input_event, CL_PopupMenuWindow *popup);

	void on_popupmenu_key_released(CL_InputEvent &input_event, CL_PopupMenuWindow *popup);

	int exec_handler(CL_AcceleratorTable &accel_table, bool loop_until_complete);

	CL_GUIComponent *owner;

	CL_MenuBar *menubar;

	CL_PopupMenuWindow *popup_window;

	int menubar_shown_menu_index;

	bool destroy_self_flag;

	CL_Callback_2<int, CL_AcceleratorTable &, bool> old_func_exec_handler;

};


