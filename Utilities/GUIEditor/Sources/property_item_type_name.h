
#pragma once

#include "property_item.h"

class PropertyItemTypeName : public PropertyItemLineEdit
{
public:
	PropertyItemTypeName()
	: PropertyItemLineEdit(".class")
	{
	}

	void selection_changed(const std::vector<HolderComponent *> &selection)
	{
		if (!selection.empty())
		{
			CL_GUIComponent *comp = selection[0]->get_component();
			value = comp->get_type_name();
		}
	}

	void apply_changes(HolderComponent *holder)
	{
		CL_GUIComponent *comp = holder->get_component();
		comp->set_type_name(value);
	}
};
