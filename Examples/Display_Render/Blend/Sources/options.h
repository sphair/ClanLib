/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

/*
class Options : public clan::GUIComponent
{
public:
	Options(clan::GUIManager &gui, clan::Rect gui_position);
	virtual ~Options();

	clan::BlendFunc blendfunc[4];	// For src, dest, src_alpha, dest_alpha
	clan::BlendEquation blendequation[2];	// For color, alpha
	clan::LogicOp logic_operation;
	bool logic_operation_enabled;

	clan::Colorf primary_color;
	clan::Colorf blend_color;

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
	clan::Label *create_slider_label(clan::Slider *slider);
	void on_equation_selected(int value, clan::ComboBox *combo);
	void on_blend_selected(int value, clan::ComboBox *combo);
	void on_logic_selected(int value, clan::ComboBox *combo);
	clan::CheckBox *create_checkbox(int xpos, int ypos, const char *name, bool state);
	void on_render(clan::Canvas &canvas, const clan::Rect &update_rect);
	clan::Slider *create_slider(int xpos, int ypos);
	float get_value(clan::Slider *slider);
	clan::ComboBox *create_equation_combo_box(int xpos, int ypos, clan::PopupMenu &menu, int selected_item);
	clan::ComboBox *create_logic_combo_box(int xpos, int ypos, clan::PopupMenu &menu, int selected_item);
	clan::ComboBox *create_blend_combo_box(int xpos, int ypos, clan::PopupMenu &menu, int selected_item);
	void make_equation_menu(clan::PopupMenu &menu);
	void make_blend_menu(clan::PopupMenu &menu);
	void make_logic_menu(clan::PopupMenu &menu);
	clan::Label *create_combobox_label(clan::ComboBox *combo, const char *text);
private:
	clan::Slider *slider_numballs;
	clan::Slider *slider_primary_red;
	clan::Slider *slider_primary_green;
	clan::Slider *slider_primary_blue;
	clan::Slider *slider_primary_alpha;
	clan::Slider *slider_blend_red;
	clan::Slider *slider_blend_green;
	clan::Slider *slider_blend_blue;
	clan::Slider *slider_blend_alpha;
	clan::Label *label_numballs;
	clan::Label *label_primary_red;
	clan::Label *label_primary_green;
	clan::Label *label_primary_blue;
	clan::Label *label_primary_alpha;
	clan::Label *label_blend_red;
	clan::Label *label_blend_green;
	clan::Label *label_blend_blue;
	clan::Label *label_blend_alpha;
	clan::CheckBox *checkbox_moveballs;
	clan::CheckBox *checkbox_premult_alpha;
	clan::CheckBox *checkbox_blending;

	clan::Label *label_src;
	clan::Label *label_dest;
	clan::Label *label_src_alpha;
	clan::Label *label_dest_alpha;
	clan::Label *label_color;
	clan::Label *label_alpha;
	clan::Label *label_logic;

	clan::ComboBox *combo_logic;
	clan::PopupMenu combo_logic_menu;

	clan::ComboBox *combo_src_blend;
	clan::ComboBox *combo_dest_blend;
	clan::ComboBox *combo_src_alpha_blend;
	clan::ComboBox *combo_dest_alpha_blend;
	clan::PopupMenu combo_src_blend_menu;
	clan::PopupMenu combo_dest_blend_menu;
	clan::PopupMenu combo_src_alpha_blend_menu;
	clan::PopupMenu combo_dest_alpha_blend_menu;

	clan::ComboBox *combo_color_equation;
	clan::ComboBox *combo_alpha_equation;
	clan::PopupMenu combo_color_equation_menu;
	clan::PopupMenu combo_alpha_equation_menu;

};

*/
