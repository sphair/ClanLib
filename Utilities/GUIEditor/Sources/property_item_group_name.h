
#pragma once

#include "property_item.h"

class PropertyItemGroupName : public PropertyItemLineEdit
{
public:
	PropertyItemGroupName()
	: PropertyItemLineEdit("group name")
	{
	}

	void selection_changed(const std::vector<HolderComponent *> &selection)
	{
		if (!selection.empty())
		{
			CL_GUIComponent *comp = selection[0]->get_component();
			CL_RadioButton *radiobutton = dynamic_cast<CL_RadioButton*>(comp);
			if (radiobutton)
				value = radiobutton->get_group_name();
		}
	}

	void apply_changes(HolderComponent *holder)
	{
		CL_GUIComponent *comp = holder->get_component();

		CL_RadioButton *radiobutton = dynamic_cast<CL_RadioButton*>(comp);

		if (radiobutton)
			radiobutton->set_group_name(value);
	}
};
