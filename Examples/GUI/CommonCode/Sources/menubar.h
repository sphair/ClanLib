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
**    Mark Page
*/

#pragma once

#include "info.h"

class MenuBar : public CL_Window
{
public:
	MenuBar(CL_GUIManager &manager, CL_ResourceManager &application_resources);

private:
	void on_item_selected();

	void on_clicked(CL_PushButton *pushbutton);
	void on_checked_status_size_grip(CL_CheckBox *checkbox);
	void on_unchecked_status_size_grip(CL_CheckBox *checkbox);
	void on_checked_status_text(CL_CheckBox *checkbox);
	void on_unchecked_status_text(CL_CheckBox *checkbox);
	void on_checked_show_clantest(CL_CheckBox *checkbox);
	void on_unchecked_show_clantest(CL_CheckBox *checkbox);
	void on_checked_remove_clantest(CL_CheckBox *checkbox);
	void on_unchecked_remove_clantest(CL_CheckBox *checkbox);
	void on_part_double_clicked_0(CL_StatusBar *statusbar);
	void on_part_double_clicked_1(CL_StatusBar *statusbar);
	void on_part_double_clicked_2(CL_StatusBar *statusbar);

private:
	CL_MenuBar *menubar1;

	CL_PixelBuffer tux_image;

	CL_PopupMenu menu_file;
	CL_PopupMenu menu_edit;
	CL_PopupMenuItem item_submenu;
	CL_PopupMenu menu_submenu;

	Info *info_item_selected;

	CL_StatusBar *statusbar1;

	CL_Label *label_status;
	CL_CheckBox *checkbox_status_size_grip;
	CL_CheckBox *checkbox_status_text;
	CL_CheckBox *checkbox_status_show_clantest;
	CL_CheckBox *checkbox_status_remove_clantest;

	CL_PushButton *pushbutton1;

	Info *info_clicked;
	Info *info_part_clicked;

	CL_GUIComponent *component1;

	CL_Image tux_head;

};
