
#pragma once

#include "property_item.h"

class PropertyItemGroupName : public PropertyItemLineEdit
{
public:
	void selection_changed(const std::vector<HolderComponent *> &selection)
	{
	}

	void apply_changes(HolderComponent *holder)
	{
		CL_GUIComponent *comp = holder->get_first_child();

		CL_RadioButton *radiobutton = dynamic_cast<CL_RadioButton*>(comp);

		if (radiobutton)
			radiobutton->set_group_name(value);
	}
};
