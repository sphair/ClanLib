
#pragma once

#include "property_item.h"

class PropertyItemText : public PropertyItemLineEdit
{
public:
	void selection_changed(const std::vector<HolderComponent *> &selection)
	{
	}

	void apply_changes(HolderComponent *holder)
	{
		CL_GUIComponent *comp = holder->get_first_child();

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
};
