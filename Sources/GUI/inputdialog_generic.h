/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    Magnus Norddahl
**    (if your name is missing here, please add it)
*/

#ifndef header_inputdialog_generic
#define header_inputdialog_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/GUI/inputdialog.h"
#include "API/GUI/button.h"
#include "API/GUI/label.h"
#include "API/GUI/inputbox.h"
#include "API/GUI/checkbox.h"
#include "API/Display/display.h"

#include <vector>

class CL_Font;
class CL_InputBox;
class CL_CheckBox;

class CL_InputDialog_Generic
{
public:
	CL_InputDialog_Generic(
		CL_InputDialog *self,
		const std::string &title,
		const std::string &button1,
		const std::string &button2,
		const std::string &button3);

	~CL_InputDialog_Generic();

	CL_InputDialog *inputdialog;

	CL_Button *button[3];
	int button_count;

	std::vector<CL_Label*> labels;
	std::vector<CL_Component*> components;

	CL_Slot slot_button[3];
	
	CL_Button * get_button(int button);
	
	void on_button(int button);

	CL_InputBox * add_input_box(const std::string &label,
		const std::string &default_value,
		int component_width);

	CL_CheckBox * add_check_box(const std::string &text,
		bool checked,
		int component_width);

	int result_button;

private:
	enum
	{
		space_between_buttons = 10,
		space_between_title_and_inputbox = 10,
		space_between_inputboxes = 10,
		space_between_inputbox_and_buttons = 20,
		space_between_buttons_and_bottom = 10,
		space_between_text_and_left_edge = 10,
		space_between_text_and_right_edge = 10,
		space_between_label_and_input_box = 10
	};
	CL_Component *add_component(const std::string &label, CL_Component * component, int component_width);
};

inline CL_Component * CL_InputDialog_Generic::add_component(const std::string &label, CL_Component * component, int component_width)
{
	int width = inputdialog->get_client_area()->get_width() -
		space_between_text_and_left_edge - space_between_text_and_right_edge;
	int height = inputdialog->get_client_area()->get_height();

	// We need to know how many space get client_area in 
	// CL_Window, and then we can calculate offsets from edges
	CL_Rect const msgbox_rc(
		CL_Display::get_width()  / 2 - 400 / 2,
		CL_Display::get_height() / 2 - 400 / 2,
		CL_Display::get_width()  / 2 + 400 / 2,
		CL_Display::get_height() / 2 + 400 / 2);

	inputdialog->set_position(msgbox_rc);

	CL_Rect const client_area_rc =
		inputdialog->get_client_area()->get_position();

	// Now calculate edges offsets
	int const left_edge_offset   = client_area_rc.left;
	int const top_edge_offset    = client_area_rc.top;
	int const right_edge_offset  = msgbox_rc.right - client_area_rc.right - msgbox_rc.left;
	int const bottom_edge_offset = msgbox_rc.bottom - client_area_rc.bottom - msgbox_rc.top;

	// Calculating height of all input boxes, which are allready
	// in dialog

	if (button_count > 0)
		// If there some buttons allready, use the button pos to calculate height
		// of all inputboxes
		height = button[0]->get_position().top - space_between_inputbox_and_buttons;
	else
		// Otherwise calculate from client_area pos and constants
		height -= space_between_inputbox_and_buttons + space_between_buttons_and_bottom;

	// If there allready some input boxes, add space between last inputbox
	if (!components.empty())
		height += space_between_inputboxes;
	else
		height += space_between_title_and_inputbox;

	if (!label.empty())
		labels.push_back(new CL_Label(
			CL_Point(space_between_text_and_left_edge, height),
			label,
			inputdialog->get_client_area()));

	int new_input_box_offset = space_between_text_and_left_edge;
	if (!label.empty())
		new_input_box_offset += labels.back()->get_width() +
			space_between_label_and_input_box;

	// msvc workaround for loop
	{
		// Check all other input boxes positions.
		for(size_t pos = 0; pos < components.size(); ++pos)
		{
			if (components[pos]->get_position().left > new_input_box_offset)
				new_input_box_offset = components[pos]->get_position().left;
		}
	}
	
	// add new input box to InputDialog
	components.push_back(component);

	components.back()->set_position(new_input_box_offset, height);
	components.back()->set_width(component_width);

	// setting focus on first added input_box
	if (components.size() == 1)
		components.back()->set_focus();
	
	if (new_input_box_offset + component_width > width)
		width = new_input_box_offset + component_width;

	// msvc workaround for loop
	{
		// Reposition all input boxes
		for(size_t pos = 0; pos < components.size(); ++pos)
		{
			components[pos]->set_position(new_input_box_offset,
				components[pos]->get_position().top);
			if (width < components[pos]->get_position().right)
				width = components[pos]->get_position().right;
		}
	}

	height += components.back()->get_height() + space_between_inputbox_and_buttons;
	int const btn_pos_y = height;
	height += space_between_buttons_and_bottom;

	width += space_between_text_and_left_edge + space_between_text_and_right_edge;

	switch(button_count) {
	case 1:
		{
			button[0]->set_position(
				width / 2 - button[0]->get_width() / 2,
				btn_pos_y);

			// add buttons height to height
			height += button[0]->get_height();
		}
		break;
	case 2:
		{
			int const total_buttons_size =
				button[0]->get_width() +
				space_between_buttons  +
				button[1]->get_width();

			button[0]->set_position(
				width / 2 - total_buttons_size / 2,
				btn_pos_y);
			button[1]->set_position(
				width / 2 - total_buttons_size / 2 +
				button[0]->get_width() + space_between_buttons,
				btn_pos_y);

			// add buttons height to height
			height += button[0]->get_height();
		}
		break;
	case 3:
		{
			int const total_buttons_size =
				button[0]->get_width() + space_between_buttons +
				button[1]->get_width() + space_between_buttons +
				button[2]->get_width();

			button[0]->set_position(
				width / 2 - total_buttons_size / 2, btn_pos_y);
			button[1]->set_position(
				width / 2 - total_buttons_size / 2 +
				button[0]->get_width() + space_between_buttons,
				btn_pos_y);
			button[2]->set_position(
				width / 2 - total_buttons_size / 2 +
					button[0]->get_width() + space_between_buttons +
					button[1]->get_width() + space_between_buttons,
				btn_pos_y);

			// add buttons height to height
			height += button[0]->get_height();
		}
		break;
	}

	// Calc and set real dialog position
	width += left_edge_offset + right_edge_offset;

	height += top_edge_offset + bottom_edge_offset;

	inputdialog->set_position(CL_Rect(
		CL_Display::get_width()  / 2 - width / 2,
		CL_Display::get_height() / 2 - height / 2,
		CL_Display::get_width()  / 2 + width / 2,
		CL_Display::get_height() / 2 + height / 2));

	return components.back();
}

#endif
