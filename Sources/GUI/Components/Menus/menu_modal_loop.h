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
**    Harry Storbacka
*/

#pragma once

#include "API/Core/Signals/callbackcontainer.h"
#include "API/Core/Signals/callback.h"

namespace clan
{

class GUIManager;
class GUIComponent;
class GUIMessage;
class InputEvent;
class MenuBar;
class PopupMenu;
class PopupMenuWindow;
class Point;
class GUIMessage_Input;
class Rect;
class Size;
class AcceleratorTable;

class MenuModalLoop
{
public:
	MenuModalLoop(GUIManager manager);
	~MenuModalLoop();

	void start(GUIComponent *owner, PopupMenu menu, const Point &pos);
	void start(GUIComponent *owner, MenuBar *menubar);
	void end();

private:
	void on_filter_message(std::shared_ptr<GUIMessage> &message);
	void on_filter_input_message(std::shared_ptr<GUIMessage_Input> &message);

	void on_popup_mouse_input(PopupMenuWindow *popup, InputEvent e);
	void on_menubar_mouse_input(InputEvent e);
	void on_keyboard_input(InputEvent e);

	void create_popup_window(PopupMenu menu, const Point &pos);
	void close_all_popup_windows();
	void close_last_popup_window();

    CallbackContainer cc;
	PopupMenuWindow *find_popup_at(const Point &pos);
	bool is_above_menubar(const Point &pos);
	GUIComponent *owner;
	MenuBar *menubar;
	std::vector<PopupMenuWindow *> popup_windows;
	bool running;
	bool ignore_menubar_mouse_up;

	bool popup;
};

}
