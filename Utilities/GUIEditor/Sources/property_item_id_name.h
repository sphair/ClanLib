
#pragma once

#include "property_item.h"

class PropertyItemIdName : public PropertyItemLineEdit
{
public:
	PropertyItemIdName()
	: PropertyItemLineEdit("#id")
	{
	}

	void selection_changed(const std::vector<HolderComponent *> &selection)
	{
		if (!selection.empty())
		{
			CL_GUIComponent *comp = selection[0]->get_component();
			value = comp->get_id_name();
		}
	}

	void apply_changes(HolderComponent *holder)
	{
		CL_GUIComponent *comp = holder->get_component();
		comp->set_id_name(value);
	}
};
