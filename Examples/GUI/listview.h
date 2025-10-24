/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

class GUI;

class ListView : public CL_Window
{
public:
	ListView(GUI *gui);

private:
	void on_selection_changed(CL_ListViewSelection selection, CL_ListView *listview);
	bool on_item_edited(CL_ListViewItem item, CL_String &string, CL_ListView *listview);
	void on_key_pressed(CL_InputEvent input_event, CL_ListView *listview);
	void on_key_released(CL_InputEvent input_event, CL_ListView *listview);
	void on_mouse_right_up(CL_Point point, CL_ListView *listview);

private:
	GUI *gui;

	CL_ListView *listview1;

	CL_ToolBar *toolbar;

	CL_Sprite sprite_mycomputer;
	CL_Sprite sprite_folder;
	CL_Sprite sprite_overlay;

	Info *info_selection_changed;
	Info *info_item_edited;
	Info *info_key_pressed;
	Info *info_key_released;
	Info *info_mouse_right_up;
};
