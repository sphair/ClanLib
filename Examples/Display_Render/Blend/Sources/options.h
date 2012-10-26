/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

class Options : public GUIComponent
{
public:
	Options(GUIManager &gui, Rect gui_position);
	virtual ~Options();

	BlendFunc blendfunc[4];	// For src, dest, src_alpha, dest_alpha
	BlendEquation blendequation[2];	// For color, alpha
	LogicOp logic_operation;
	bool logic_operation_enabled;

	Colorf primary_color;
	Colorf blend_color;

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
	Label *create_slider_label(Slider *slider);
	void on_equation_selected(int value, ComboBox *combo);
	void on_blend_selected(int value, ComboBox *combo);
	void on_logic_selected(int value, ComboBox *combo);
	CheckBox *create_checkbox(int xpos, int ypos, const char *name, bool state);
	void on_render(GraphicContext &gc, const Rect &update_rect);
	Slider *create_slider(int xpos, int ypos);
	float get_value(Slider *slider);
	ComboBox *create_equation_combo_box(int xpos, int ypos, PopupMenu &menu, int selected_item);
	ComboBox *create_logic_combo_box(int xpos, int ypos, PopupMenu &menu, int selected_item);
	ComboBox *create_blend_combo_box(int xpos, int ypos, PopupMenu &menu, int selected_item);
	void make_equation_menu(PopupMenu &menu);
	void make_blend_menu(PopupMenu &menu);
	void make_logic_menu(PopupMenu &menu);
	Label *create_combobox_label(ComboBox *combo, const char *text);
private:
	Slider *slider_numballs;
	Slider *slider_primary_red;
	Slider *slider_primary_green;
	Slider *slider_primary_blue;
	Slider *slider_primary_alpha;
	Slider *slider_blend_red;
	Slider *slider_blend_green;
	Slider *slider_blend_blue;
	Slider *slider_blend_alpha;
	Label *label_numballs;
	Label *label_primary_red;
	Label *label_primary_green;
	Label *label_primary_blue;
	Label *label_primary_alpha;
	Label *label_blend_red;
	Label *label_blend_green;
	Label *label_blend_blue;
	Label *label_blend_alpha;
	CheckBox *checkbox_moveballs;
	CheckBox *checkbox_premult_alpha;
	CheckBox *checkbox_blending;

	Label *label_src;
	Label *label_dest;
	Label *label_src_alpha;
	Label *label_dest_alpha;
	Label *label_color;
	Label *label_alpha;
	Label *label_logic;

	ComboBox *combo_logic;
	PopupMenu combo_logic_menu;

	ComboBox *combo_src_blend;
	ComboBox *combo_dest_blend;
	ComboBox *combo_src_alpha_blend;
	ComboBox *combo_dest_alpha_blend;
	PopupMenu combo_src_blend_menu;
	PopupMenu combo_dest_blend_menu;
	PopupMenu combo_src_alpha_blend_menu;
	PopupMenu combo_dest_alpha_blend_menu;

	ComboBox *combo_color_equation;
	ComboBox *combo_alpha_equation;
	PopupMenu combo_color_equation_menu;
	PopupMenu combo_alpha_equation_menu;

};

