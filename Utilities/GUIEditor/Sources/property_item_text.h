
#pragma once

#include "property_item.h"

class PropertyItemText : public PropertyItemLineEdit
{
public:
	PropertyItemText()
	: PropertyItemLineEdit("text")
	{
	}

	void selection_changed(const std::vector<HolderComponent *> &selection)
	{
		if (!selection.empty())
			value = get_item_value(selection[0]);
	}

	void apply_changes(HolderComponent *holder)
	{
		CL_GUIComponent *comp = holder->get_component();

		CL_PushButton *button = dynamic_cast<CL_PushButton*>(comp);
		CL_LineEdit *lineedit = dynamic_cast<CL_LineEdit*>(comp);
		CL_CheckBox *checkbox = dynamic_cast<CL_CheckBox*>(comp);
		CL_RadioButton *radiobutton = dynamic_cast<CL_RadioButton*>(comp);
		CL_Label *label = dynamic_cast<CL_Label*>(comp);
		CL_Frame *frame = dynamic_cast<CL_Frame*>(comp);

		if (button)
			button->set_text(value);
		if (lineedit)
			lineedit->set_text(value);
		if (checkbox)
			checkbox->set_text(value);
		if (radiobutton)
			radiobutton->set_text(value);
		if (label)
			label->set_text(value);
		if (frame)
			frame->set_header_text(value);
	}

	CL_String get_item_value(HolderComponent *holder)
	{
		CL_GUIComponent *comp = holder->get_component();

		CL_PushButton *button = dynamic_cast<CL_PushButton*>(comp);
		CL_LineEdit *lineedit = dynamic_cast<CL_LineEdit*>(comp);
		CL_CheckBox *checkbox = dynamic_cast<CL_CheckBox*>(comp);
		CL_RadioButton *radiobutton = dynamic_cast<CL_RadioButton*>(comp);
		CL_Label *label = dynamic_cast<CL_Label*>(comp);
		CL_Frame *frame = dynamic_cast<CL_Frame*>(comp);

		if (button)
			return button->get_text();
		else if (lineedit)
			return lineedit->get_text();
		else if (checkbox)
			return checkbox->get_text();
		else if (radiobutton)
			return radiobutton->get_text();
		else if (label)
			return label->get_text();
		else if (frame)
			return frame->get_header_text();
		else
			return CL_String();
	}
};
