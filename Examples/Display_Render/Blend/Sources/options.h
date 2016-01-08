/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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

#include "../../../ThemeAero/Sources/theme.h"

class Options : public clan::TextureWindow
{
public:
	Options(clan::Canvas &canvas);
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
	std::shared_ptr<clan::ListBoxView> create_listbox(int xpos, int ypos, const std::string &title);

	void insert_equation(std::shared_ptr<clan::ListBoxView> &listbox);
	void insert_blend(std::shared_ptr<clan::ListBoxView> &listbox);

	void on_logic_selected();
	void on_equation_selected(std::shared_ptr<clan::ListBoxView> listbox, int offset);
	void on_blend_selected(std::shared_ptr<clan::ListBoxView> listbox, int offset);
	std::shared_ptr<clan::CheckBoxView> create_checkbox(int xpos, int ypos, const std::string &name, bool state);
	std::shared_ptr<clan::SliderView> create_slider(int xpos, int ypos);
	void set_value(std::shared_ptr<clan::SliderView> slider, float value, float max_value);
	float get_value(std::shared_ptr<clan::SliderView> slider);
	std::shared_ptr<clan::LabelView> create_slider_label(int xpos, int ypos);

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
private:
	std::shared_ptr<clan::SliderView> slider_numballs;
	std::shared_ptr<clan::SliderView> slider_primary_red;
	std::shared_ptr<clan::SliderView> slider_primary_green;
	std::shared_ptr<clan::SliderView> slider_primary_blue;
	std::shared_ptr<clan::SliderView> slider_primary_alpha;
	std::shared_ptr<clan::SliderView> slider_blend_red;
	std::shared_ptr<clan::SliderView> slider_blend_green;
	std::shared_ptr<clan::SliderView> slider_blend_blue;
	std::shared_ptr<clan::SliderView> slider_blend_alpha;
	std::shared_ptr<clan::LabelView> label_numballs;
	std::shared_ptr<clan::LabelView> label_primary_red;
	std::shared_ptr<clan::LabelView> label_primary_green;
	std::shared_ptr<clan::LabelView> label_primary_blue;
	std::shared_ptr<clan::LabelView> label_primary_alpha;
	std::shared_ptr<clan::LabelView> label_blend_red;
	std::shared_ptr<clan::LabelView> label_blend_green;
	std::shared_ptr<clan::LabelView> label_blend_blue;
	std::shared_ptr<clan::LabelView> label_blend_alpha;
	std::shared_ptr<clan::CheckBoxView> checkbox_moveballs;
	std::shared_ptr<clan::CheckBoxView> checkbox_premult_alpha;
	std::shared_ptr<clan::CheckBoxView> checkbox_blending;

	std::shared_ptr<clan::ListBoxView> listbox_logic;

};


