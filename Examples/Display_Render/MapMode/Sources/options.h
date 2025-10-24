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

class Options : public CL_GUIComponent
{
public:
	Options(CL_GUIManager &gui, CL_Rect gui_position);
	virtual ~Options();

	CL_MapMode current_mapmode;

	int num_balls;
	bool is_moveballs_set;
	int grid_angle;

private:
	void checkbox_moveballs_changed();
	void update_all_slider_text();
	void slider_numballs_changed();
	void slider_grid_angle_changed();
	CL_Label *create_slider_label(CL_Slider *slider);
	void on_mapmode_selected(int value, CL_ComboBox *combo);
	CL_CheckBox *create_checkbox(int xpos, int ypos, const char *name, bool state);
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	CL_Slider *create_slider(int xpos, int ypos);
	float get_value(CL_Slider *slider);
	CL_ComboBox *create_mapmode_combo_box(int xpos, int ypos, CL_PopupMenu &menu, int selected_item);
	void make_mapmode_menu(CL_PopupMenu &menu);
	CL_Label *create_combobox_label(CL_ComboBox *combo, const char *text);
private:
	CL_Slider *slider_numballs;
	CL_Label *label_numballs;
	CL_Slider *slider_grid_angle;
	CL_Label *label_grid_angle;

	CL_CheckBox *checkbox_moveballs;

	CL_Label *label_mapmode;
	CL_ComboBox *combo_mapmode;
	CL_PopupMenu combo_mapmode_menu;

};

