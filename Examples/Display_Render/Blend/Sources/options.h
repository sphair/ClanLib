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

	CL_BlendFunc blendfunc[4];	// For src, dest, src_alpha, dest_alpha
	CL_BlendEquation blendequation[2];	// For color, alpha
	CL_LogicOp logic_operation;
	bool logic_operation_enabled;

	CL_Colorf primary_color;
	CL_Colorf blend_color;

	int num_balls;
	bool is_moveballs_set;
	bool is_blending_set;
	bool is_premult_alpha_set;

private:
	void checkbox_moveballs_changed();
	void checkbox_blending_changed();
	void checkbox_premult_alpha_changed();
	void update_all_slider_text();
	void slider_numballs_changed();
	void slider_primary_red_changed();
	void slider_primary_green_changed();
	void slider_primary_blue_changed();
	void slider_primary_alpha_changed();
	void slider_blend_red_changed();
	void slider_blend_green_changed();
	void slider_blend_blue_changed();
	void slider_blend_alpha_changed();
	CL_Label *create_slider_label(CL_Slider *slider);
	void on_equation_selected(int value, CL_ComboBox *combo);
	void on_blend_selected(int value, CL_ComboBox *combo);
	void on_logic_selected(int value, CL_ComboBox *combo);
	CL_CheckBox *create_checkbox(int xpos, int ypos, const char *name, bool state);
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	CL_Slider *create_slider(int xpos, int ypos);
	float get_value(CL_Slider *slider);
	CL_ComboBox *create_equation_combo_box(int xpos, int ypos, CL_PopupMenu &menu, int selected_item);
	CL_ComboBox *create_logic_combo_box(int xpos, int ypos, CL_PopupMenu &menu, int selected_item);
	CL_ComboBox *create_blend_combo_box(int xpos, int ypos, CL_PopupMenu &menu, int selected_item);
	void make_equation_menu(CL_PopupMenu &menu);
	void make_blend_menu(CL_PopupMenu &menu);
	void make_logic_menu(CL_PopupMenu &menu);
	CL_Label *create_combobox_label(CL_ComboBox *combo, const char *text);
private:
	CL_Slider *slider_numballs;
	CL_Slider *slider_primary_red;
	CL_Slider *slider_primary_green;
	CL_Slider *slider_primary_blue;
	CL_Slider *slider_primary_alpha;
	CL_Slider *slider_blend_red;
	CL_Slider *slider_blend_green;
	CL_Slider *slider_blend_blue;
	CL_Slider *slider_blend_alpha;
	CL_Label *label_numballs;
	CL_Label *label_primary_red;
	CL_Label *label_primary_green;
	CL_Label *label_primary_blue;
	CL_Label *label_primary_alpha;
	CL_Label *label_blend_red;
	CL_Label *label_blend_green;
	CL_Label *label_blend_blue;
	CL_Label *label_blend_alpha;
	CL_CheckBox *checkbox_moveballs;
	CL_CheckBox *checkbox_premult_alpha;
	CL_CheckBox *checkbox_blending;

	CL_Label *label_src;
	CL_Label *label_dest;
	CL_Label *label_src_alpha;
	CL_Label *label_dest_alpha;
	CL_Label *label_color;
	CL_Label *label_alpha;
	CL_Label *label_logic;

	CL_ComboBox *combo_logic;
	CL_PopupMenu combo_logic_menu;

	CL_ComboBox *combo_src_blend;
	CL_ComboBox *combo_dest_blend;
	CL_ComboBox *combo_src_alpha_blend;
	CL_ComboBox *combo_dest_alpha_blend;
	CL_PopupMenu combo_src_blend_menu;
	CL_PopupMenu combo_dest_blend_menu;
	CL_PopupMenu combo_src_alpha_blend_menu;
	CL_PopupMenu combo_dest_alpha_blend_menu;

	CL_ComboBox *combo_color_equation;
	CL_ComboBox *combo_alpha_equation;
	CL_PopupMenu combo_color_equation_menu;
	CL_PopupMenu combo_alpha_equation_menu;

};

