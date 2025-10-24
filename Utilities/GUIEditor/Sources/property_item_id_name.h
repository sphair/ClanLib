
#pragma once

#include "property_item.h"

class PropertyItemIdName : public PropertyItemLineEdit
{
public:
	void selection_changed(const std::vector<HolderComponent *> &selection)
	{
	}

	void apply_changes(HolderComponent *holder)
	{
		CL_GUIComponent *comp = holder->get_first_child();
		comp->set_id_name(value);
	}
};
