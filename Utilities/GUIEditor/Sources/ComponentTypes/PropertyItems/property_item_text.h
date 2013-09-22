/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
*/

#pragma once

#include "property_item.h"

class PropertyItemText : public PropertyItemLineEdit
{
public:
	PropertyItemText()
	: PropertyItemLineEdit("text")
	{
	}

	void selection_changed(const std::vector<GridObject *> &selection)
	{
		if (!selection.empty())
			value = get_item_value(selection[0]);
	}

	void apply_changes(GridObject *object)
	{
		clan::GUIComponent *comp = object->get_component();

		clan::PushButton *button = dynamic_cast<clan::PushButton*>(comp);
		clan::LineEdit *lineedit = dynamic_cast<clan::LineEdit*>(comp);
		clan::TextEdit *textedit = dynamic_cast<clan::TextEdit*>(comp);
		clan::CheckBox *checkbox = dynamic_cast<clan::CheckBox*>(comp);
		clan::RadioButton *radiobutton = dynamic_cast<clan::RadioButton*>(comp);
		clan::Label *label = dynamic_cast<clan::Label*>(comp);
		clan::Frame *frame = dynamic_cast<clan::Frame*>(comp);

		if (button)
			button->set_text(value);
		if (lineedit)
			lineedit->set_text(value);
		if (textedit)
			textedit->set_text(value);
		if (checkbox)
			checkbox->set_text(value);
		if (radiobutton)
			radiobutton->set_text(value);
		if (label)
			label->set_text(value);
		if (frame)
			frame->set_header_text(value);
	}

	std::string get_item_value(GridObject *object)
	{
		clan::GUIComponent *comp = object->get_component();

		clan::PushButton *button = dynamic_cast<clan::PushButton*>(comp);
		clan::LineEdit *lineedit = dynamic_cast<clan::LineEdit*>(comp);
		clan::TextEdit *textedit = dynamic_cast<clan::TextEdit*>(comp);
		clan::CheckBox *checkbox = dynamic_cast<clan::CheckBox*>(comp);
		clan::RadioButton *radiobutton = dynamic_cast<clan::RadioButton*>(comp);
		clan::Label *label = dynamic_cast<clan::Label*>(comp);
		clan::Frame *frame = dynamic_cast<clan::Frame*>(comp);

		if (button)
			return button->get_text();
		else if (lineedit)
			return lineedit->get_text();
        else if (textedit)
			return textedit->get_text();
		else if (checkbox)
			return checkbox->get_text();
		else if (radiobutton)
			return radiobutton->get_text();
		else if (label)
			return label->get_text();
		else if (frame)
			return frame->get_header_text();
		else
			return std::string();
	}
};
