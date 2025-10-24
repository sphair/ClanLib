
#pragma once

#include "property_item.h"

class PropertyItemClassName : public PropertyItemLineEdit
{
public:
	PropertyItemClassName()
	: PropertyItemLineEdit(".class")
	{
	}

	void selection_changed(const std::vector<HolderComponent *> &selection)
	{
		if (!selection.empty())
		{
			CL_GUIComponent *comp = selection[0]->get_component();
			value = comp->get_class_name();
		}
	}

	void apply_changes(HolderComponent *holder)
	{
		CL_GUIComponent *comp = holder->get_component();
		comp->set_class_name(value);
	}
};
