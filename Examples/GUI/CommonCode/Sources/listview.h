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
**    Mark Page
*/

#pragma once

#include "info.h"

class ListView : public clan::Window
{
public:
	ListView(clan::GUIManager &manager);

private:
	bool on_close();
	void on_selection_changed(clan::ListViewSelection selection, clan::ListView *listview);
	bool on_item_edited(clan::ListViewItem item, std::string &string, clan::ListView *listview);
	void on_key_pressed(clan::InputEvent &input_event, clan::ListView *listview);
	void on_key_released(clan::InputEvent &input_event, clan::ListView *listview);
	void on_mouse_right_up(clan::Point point, clan::ListView *listview);

private:
	clan::ListView *listview1;

	clan::ToolBar *toolbar;

	clan::Sprite sprite_mycomputer;
	clan::Sprite sprite_folder;
	clan::Sprite sprite_overlay;

	Info *info_selection_changed;
	Info *info_item_edited;
	Info *info_key_pressed;
	Info *info_key_released;
	Info *info_mouse_right_up;
};
