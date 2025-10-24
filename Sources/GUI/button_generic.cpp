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
#include "button_generic.h"
#include "API/Core/XML/dom_element.h"
#include "API/Display/keys.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_Button_Generic::CL_Button_Generic(
	CL_Button *self,
	const std::string &text,
	bool toggle_mode)
:
	text(text),
	toggle_mode(toggle_mode),
	toggled(false),
	visually_down(false),
	pressed_by(NOT_PRESSED),
	button(self)
{
	slots.connect(button->sig_set_options(), this, &CL_Button_Generic::on_set_options);
	slots.connect(button->sig_input_down(), this, &CL_Button_Generic::on_input_down);
	slots.connect(button->sig_input_up(), this, &CL_Button_Generic::on_input_up);
	slots.connect(button->sig_mouse_move(), this, &CL_Button_Generic::on_mouse_move);
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_Button_Generic::on_set_options(const CL_DomElement &options)
{
	if (options.has_attribute("text")) text = options.get_attribute("text");
	if (options.has_attribute("toggled")) toggled = CL_String::to_bool(options.get_attribute("toggled"));
	if (options.has_attribute("togglemode")) toggle_mode = CL_String::to_bool(options.get_attribute("togglemode"));

	visually_down = toggled;
}

void CL_Button_Generic::on_mouse_move(const CL_InputEvent &key)
{
	if (pressed_by != PRESSED_BY_MOUSE)
		return;
	if(!button->is_mouse_captured())
		return;

	if(button->has_mouse_over())
		visually_down = !toggled;
	else
		visually_down = toggled;
}

void CL_Button_Generic::on_input_down(const CL_InputEvent &key)
{
	if (pressed_by != NOT_PRESSED)
		return;

	if(button->is_mouse_captured())
		return;

	switch(key.id)
	{
	case CL_MOUSE_LEFT:
		pressed_by = PRESSED_BY_MOUSE;
		break;
	case CL_KEY_SPACE:
	case CL_KEY_ENTER:
		pressed_by = PRESSED_BY_KEYBOARD;
		break;
	default:
		return;
	}

	sig_pressed();
	button->capture_mouse();

	visually_down = !visually_down;
}

void CL_Button_Generic::on_input_up(const CL_InputEvent &key)
{
	if (pressed_by == NOT_PRESSED)
		return;
	
	if (button->is_mouse_captured() == false)
		return;

	switch(pressed_by)
	{
	case PRESSED_BY_MOUSE:
		{
			if (key.id != CL_MOUSE_LEFT)
				return;

			button->release_mouse();
			sig_released();
			bool mouse_over = button->has_mouse_over();
			if (mouse_over)
			{
				if (toggle_mode)
				{
					toggled = !toggled;
					sig_toggled(toggled);
				}

				sig_clicked();
			}
		}
		break;
	case PRESSED_BY_KEYBOARD:
		{
			if (key.id != CL_KEY_SPACE && key.id != CL_KEY_ENTER)
				return;
			button->release_mouse();
			if (toggle_mode)
			{
				toggled = !toggled;
				sig_toggled(toggled);
			}
			sig_clicked();
		}
		break;
	default:
		return;
	}

	pressed_by = NOT_PRESSED;

	visually_down = toggled;
}
