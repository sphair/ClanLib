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

#pragma once

#include "API/Core/Signals/callback_1.h"
#include "API/Core/Signals/slot.h"

class CL_GUIManager;
class CL_GUIComponent;
class CL_GUIMessage;
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
	CL_MenuModalLoop(CL_GUIManager manager);
	~CL_MenuModalLoop();

	void start(CL_GUIComponent *owner, CL_PopupMenu menu, const CL_Point &pos);
	void start(CL_GUIComponent *owner, CL_MenuBar *menubar);
	void end();

private:
	void on_filter_message(CL_GUIMessage &message);
	void on_filter_input_message(CL_GUIMessage_Input &message);

	void on_popup_mouse_input(CL_PopupMenuWindow *popup, CL_InputEvent e);
	void on_menubar_mouse_input(CL_InputEvent e);
	void on_keyboard_input(CL_InputEvent e);

	void create_popup_window(CL_PopupMenu menu, const CL_Point &pos);
	void close_all_popup_windows();
	void close_last_popup_window();

	CL_PopupMenuWindow *find_popup_at(const CL_Point &pos);
	bool is_above_menubar(const CL_Point &pos);
	CL_GUIComponent *owner;
	CL_MenuBar *menubar;
	std::vector<CL_PopupMenuWindow *> popup_windows;
	bool running;
	bool ignore_menubar_mouse_up;
	CL_Slot slot_filter;
};

