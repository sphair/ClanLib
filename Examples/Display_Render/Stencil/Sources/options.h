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

	bool logic_operation_enabled;
	clan::CompareFunction compare_function;
	int compare_reference;

	int num_balls;
	bool is_moveballs_set;
	bool is_circle_set;
	clan::StencilOp stencil_pass;
	clan::StencilOp stencil_fail;

private:
	void checkbox_moveballs_changed();
	void checkbox_circle_changed();
	void update_all_slider_text();
	void slider_numballs_changed();
	void slider_compare_reference_changed();
	clan::Label *create_slider_label(clan::Slider *slider);
	void on_passfail_selected(int value, clan::ComboBox *combo);
	void on_compare_selected(int value, clan::ComboBox *combo);
	clan::CheckBox *create_checkbox(int xpos, int ypos, const char *name, bool state);
	void on_render(clan::Canvas &canvas, const clan::Rect &update_rect);
	clan::Slider *create_slider(int xpos, int ypos);
	float get_value(clan::Slider *slider);
	clan::ComboBox *create_compare_combo_box(int xpos, int ypos, clan::PopupMenu &menu, int selected_item);
	clan::ComboBox *create_passfail_combo_box(int xpos, int ypos, clan::PopupMenu &menu, int selected_item);
	void make_compare_menu(clan::PopupMenu &menu);
	void make_passfail_menu(clan::PopupMenu &menu);
	clan::Label *create_combobox_label(clan::ComboBox *combo, const char *text);
private:
	clan::Slider *slider_numballs;
	clan::Label *label_numballs;

	clan::Slider *slider_compare_reference;
	clan::Label *label_compare_reference;

	clan::CheckBox *checkbox_moveballs;
	clan::CheckBox *checkbox_circle;

	clan::Label *label_comparefunc;
	clan::ComboBox *combo_comparefunc;
	clan::PopupMenu combo_compare_menu;

	clan::Label *label_pass;
	clan::ComboBox *combo_pass;
	clan::PopupMenu combo_pass_menu;

	clan::Label *label_fail;
	clan::ComboBox *combo_fail;
	clan::PopupMenu combo_fail_menu;

};

*/
