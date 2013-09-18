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

class MenuBar : public clan::Window
{
public:
	MenuBar(clan::GUIManager &manager, clan::ResourceManager &application_resources);

private:
	bool on_close();
	void on_item_selected();

	void on_clicked(clan::PushButton *pushbutton);
	void on_checked_status_size_grip(clan::CheckBox *checkbox);
	void on_unchecked_status_size_grip(clan::CheckBox *checkbox);
	void on_checked_status_text(clan::CheckBox *checkbox);
	void on_unchecked_status_text(clan::CheckBox *checkbox);
	void on_checked_show_clantest(clan::CheckBox *checkbox);
	void on_unchecked_show_clantest(clan::CheckBox *checkbox);
	void on_checked_remove_clantest(clan::CheckBox *checkbox);
	void on_unchecked_remove_clantest(clan::CheckBox *checkbox);
	void on_part_double_clicked_0(clan::StatusBar *statusbar);
	void on_part_double_clicked_1(clan::StatusBar *statusbar);
	void on_part_double_clicked_2(clan::StatusBar *statusbar);

private:
	clan::MenuBar *menubar1;

	clan::PixelBuffer tux_image;

	clan::PopupMenu menu_file;
	clan::PopupMenu menu_edit;
	clan::PopupMenuItem item_submenu;
	clan::PopupMenu menu_submenu;

	Info *info_item_selected;

	clan::StatusBar *statusbar1;

	clan::Label *label_status;
	clan::CheckBox *checkbox_status_size_grip;
	clan::CheckBox *checkbox_status_text;
	clan::CheckBox *checkbox_status_show_clantest;
	clan::CheckBox *checkbox_status_remove_clantest;

	clan::PushButton *pushbutton1;

	Info *info_clicked;
	Info *info_part_clicked;

	clan::GUIComponent *component1;

	clan::Image tux_head;

};
