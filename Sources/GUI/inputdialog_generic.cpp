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

#include "precomp.h"
#include "inputdialog_generic.h"
#include "API/GUI/stylemanager.h"
#include "API/Display/display.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/Resources/resourcetype_integer.h"
#include "API/Core/Resources/resourcetype_boolean.h"

CL_InputDialog_Generic::CL_InputDialog_Generic(
	CL_InputDialog *self,
	const std::string &title,
	const std::string &button1,
	const std::string &button2,
	const std::string &button3)
:
	inputdialog(self),
	result_button(-1)
{
	CL_ResourceManager * resources = inputdialog->get_style_manager()->get_resources();

	bool const is_fixed_size_buttons = CL_Boolean("InputDialog/fixed_size_buttons",
		resources, false);
	int min_button_width = CL_Integer("InputDialog/min_button_width",
		resources, 80);
	int min_button_height = CL_Integer("InputDialog/min_button_height",
		resources, 20);

	int width = CL_Integer("InputDialog/min_width", resources, 150);

	// Calculating height of text
	int height = 0;

	inputdialog->set_title(title);

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
	int const bottom_edge_offset = msgbox_rc.bottom - client_area_rc.bottom - msgbox_rc.top; 	int num_buttons = 0;
	if (!button1.empty())
		num_buttons = 1;
	if (!button2.empty())
		num_buttons = 2;
	if (!button3.empty())
		num_buttons = 3;

	height += space_between_inputbox_and_buttons;
	int const btn_pos_y = height;
	height += space_between_buttons_and_bottom;

	switch(num_buttons) {
	case 1:
		{
			if (is_fixed_size_buttons)
			{
				button[0] = new CL_Button(
					CL_Point(width / 2, btn_pos_y),
					button1,
					inputdialog->get_client_area());
				if (button[0]->get_width() > min_button_width)
					min_button_width = button[0]->get_width();
				if (button[0]->get_height() > min_button_height)
					min_button_height = button[0]->get_height();

				button[0]->set_width(min_button_width);
				button[0]->set_height(min_button_height);
			}
			else
			{
				button[0] = new CL_Button(
					CL_Point(width / 2, btn_pos_y),
					button1,
					inputdialog->get_client_area());
			}

			if (width < button[0]->get_width())
				width = button[0]->get_width();

			width += space_between_text_and_left_edge + space_between_text_and_right_edge;
			
			button[0]->set_position(
				width / 2 - button[0]->get_width() / 2,
				btn_pos_y);

			slot_button[0] = button[0]->sig_clicked().connect(
				this, &CL_InputDialog_Generic::on_button, 0);

			// add buttons height to height
			height += button[0]->get_height();
		}
		break;
	case 2:
		{
			if (is_fixed_size_buttons)
			{
				button[0] = new CL_Button(
					CL_Point(width / 2, btn_pos_y),
					button1,
					inputdialog->get_client_area());
				button[1] = new CL_Button(
					CL_Point(width / 2, btn_pos_y),
					button2,
					inputdialog->get_client_area());
				
				// calculate min width and height of buttons,
				// if some button is bigger than min size
				for(int i = 0; i < 2; ++i)
				{
					if (button[i]->get_width() > min_button_width)
						min_button_width = button[i]->get_width();
					if (button[i]->get_height() > min_button_height)
						min_button_height = button[i]->get_height();
				}
				// set buttons size to max width and height.
				// and use 'j' for msvc workaround :(
				for(int j = 0; j < 2; ++j)
				{
					button[j]->set_width(min_button_width);
					button[j]->set_height(min_button_height);
				}
			}
			else
			{
				button[0] = new CL_Button(
					CL_Point(width / 2, btn_pos_y),
					button1,
					inputdialog->get_client_area());
				button[1] = new CL_Button(
					CL_Point(width / 2, btn_pos_y),
					button2,
					inputdialog->get_client_area());
			}

			int const total_buttons_size =
				button[0]->get_width() +
				space_between_buttons  +
				button[1]->get_width();

			if (width < total_buttons_size)
				width = total_buttons_size;

			width += space_between_text_and_left_edge + space_between_text_and_right_edge;

			button[0]->set_position(
				width / 2 - total_buttons_size / 2,
				btn_pos_y);
			button[1]->set_position(
				width / 2 - total_buttons_size / 2 +
				button[0]->get_width() + space_between_buttons,
				btn_pos_y);

			slot_button[0] = button[0]->sig_clicked().connect(
				this, &CL_InputDialog_Generic::on_button, 0);
			slot_button[1] = button[1]->sig_clicked().connect(
				this, &CL_InputDialog_Generic::on_button, 1);

			// add buttons height to height
			height += button[0]->get_height();
		}
		break;
	case 3:
		{
			if (is_fixed_size_buttons)
			{
				button[0] = new CL_Button(
					CL_Point(width / 2, btn_pos_y),
					button1,
					inputdialog->get_client_area());
				button[1] = new CL_Button(
					CL_Point(width / 2, btn_pos_y),
					button2,
					inputdialog->get_client_area());
				button[2] = new CL_Button(
					CL_Point(width / 2, btn_pos_y),
					button3,
					inputdialog->get_client_area());
				
				// calculate min width and height of buttons,
				// if some button is bigger than min size
				for(int i = 0; i < 3; ++i)
				{
					if (button[i]->get_width() > min_button_width)
						min_button_width = button[i]->get_width();
					if (button[i]->get_height() > min_button_height)
						min_button_height = button[i]->get_height();
				}
				// set buttons size to max width and height
				for(int j = 0; j < 3; ++j)
				{
					button[j]->set_width(min_button_width);
					button[j]->set_height(min_button_height);
				}
			}
			else
			{
				button[0] = new CL_Button(
					CL_Point(width / 2, btn_pos_y),
					button1,
					inputdialog->get_client_area());
				button[1] = new CL_Button(
					CL_Point(width / 2, btn_pos_y),
					button2,
					inputdialog->get_client_area());
				button[2] = new CL_Button(
					CL_Point(width / 2, btn_pos_y),
					button3,
					inputdialog->get_client_area());
			}

			int const total_buttons_size =
				button[0]->get_width() + space_between_buttons +
				button[1]->get_width() + space_between_buttons +
				button[2]->get_width();

			if (width < total_buttons_size)
				width = total_buttons_size;
			
			width += space_between_text_and_left_edge + space_between_text_and_right_edge;

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

			slot_button[0] = button[0]->sig_clicked().connect(
				this, &CL_InputDialog_Generic::on_button, 0);
			slot_button[1] = button[1]->sig_clicked().connect(
				this, &CL_InputDialog_Generic::on_button, 1);
			slot_button[2] = button[2]->sig_clicked().connect(
				this, &CL_InputDialog_Generic::on_button, 2);

			// add buttons height to height
			height += button[0]->get_height();
		}
		break;
	}
	button_count = num_buttons;

	// Calc and set real dialog position
	width += left_edge_offset + right_edge_offset;

	height += top_edge_offset + bottom_edge_offset;

	inputdialog->set_position(CL_Rect(
		CL_Display::get_width()  / 2 - width / 2,
		CL_Display::get_height() / 2 - height / 2,
		CL_Display::get_width()  / 2 + width / 2,
		CL_Display::get_height() / 2 + height / 2));
}

CL_InputDialog_Generic::~CL_InputDialog_Generic()
{
	for(int i = 0; i < button_count; ++i)
		delete button[i];
	for(size_t j = 0; j < labels.size(); ++j)
		delete labels[j];
	for(size_t k = 0; k < components.size(); ++k)
		delete components[k];
}

void CL_InputDialog_Generic::on_button(int button)
{
	result_button = button;
	inputdialog->quit();
}

CL_Button * CL_InputDialog_Generic::get_button(int button)
{
	if (button >= button_count) return NULL; //invalid button

	return this->button[button];
}

CL_InputBox * CL_InputDialog_Generic::add_input_box(const std::string &label,
	const std::string &default_value,
	int inputbox_width)
{
	return (CL_InputBox *) add_component(label, new CL_InputBox(default_value, inputdialog->get_client_area()), inputbox_width);
}

CL_CheckBox * CL_InputDialog_Generic::add_check_box(const std::string &text,
	bool checked,
	int checkbox_width)
{
	CL_CheckBox * checkbox = new CL_CheckBox(text, inputdialog->get_client_area());
	checkbox->set_checked(checked);
	return (CL_CheckBox *) add_component("", checkbox, checkbox_width);
}
