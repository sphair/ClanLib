/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

	bool logic_operation_enabled;
	clan::CompareFunction compare_function;
	int compare_reference;

	int num_balls;
	bool is_moveballs_set;
	bool is_circle_set;
	clan::StencilOp stencil_pass;
	clan::StencilOp stencil_fail;

private:
	void insert_compare(std::shared_ptr<clan::ListBoxView> &listbox);
	void insert_passfail(std::shared_ptr<clan::ListBoxView> &listbox);
	void on_equation_compare(std::shared_ptr<clan::ListBoxView> listbox);
	void on_equation_passfail(std::shared_ptr<clan::ListBoxView> listbox, bool is_pass);
	std::shared_ptr<clan::ListBoxView> create_listbox(int xpos, int ypos, const std::string &title);
	void checkbox_moveballs_changed();
	void checkbox_circle_changed();
	void update_all_slider_text();
	void slider_numballs_changed();
	void slider_compare_reference_changed();
	std::shared_ptr<clan::LabelView> create_slider_label(int xpos, int ypos);
	std::shared_ptr<clan::CheckBoxView> create_checkbox(int xpos, int ypos, const std::string &name, bool state);
	std::shared_ptr<clan::SliderView> create_slider(int xpos, int ypos);
	float get_value(std::shared_ptr<clan::SliderView> slider);
private:
	std::shared_ptr<clan::SliderView> slider_numballs;
	std::shared_ptr<clan::LabelView> label_numballs;

	std::shared_ptr<clan::SliderView> slider_compare_functionreference;
	std::shared_ptr<clan::LabelView> label_compare_reference;

	std::shared_ptr<clan::CheckBoxView> checkbox_moveballs;
	std::shared_ptr<clan::CheckBoxView> checkbox_circle;

	std::shared_ptr<clan::LabelView> label_comparefunc;
	std::shared_ptr<clan::LabelView> label_pass;
	std::shared_ptr<clan::LabelView> label_fail;
	std::shared_ptr<clan::ListBoxView> listbox1;
	std::shared_ptr<clan::ListBoxView> listbox2;
	std::shared_ptr<clan::ListBoxView> listbox3;

};

