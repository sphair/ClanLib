
#pragma once

#include "property_item.h"

class PropertyItemPosition : public PropertyItemLineEdit
{
public:
	enum PositionType
	{
		type_x1,
		type_y1,
		type_x2,
		type_y2,
		type_width,
		type_height
	};

	PropertyItemPosition(PositionType type)
	: type(type)
	{
	}

	void selection_changed(const std::vector<HolderComponent *> &selection)
	{
	}

	void apply_changes(HolderComponent *holder)
	{
	}
/*
	void deactivate(CL_GUIComponent *component)
	{
		PropertyItemLineEdit::deactivate(component);

		std::vector<HolderComponent *> selection = get_selection();
		for (size_t i = 0; i < selection.size(); i++)
		{
			CL_GUIComponent *comp = selection[i]->get_first_child();
			CL_Rect g = comp->get_geometry();

		}
	}
*/
private:
	PositionType type;
};
