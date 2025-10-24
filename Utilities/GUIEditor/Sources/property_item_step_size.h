
#pragma once

#include "property_item.h"

class PropertyItemStepSize : public PropertyItemLineEdit
{
public:
	void selection_changed(const std::vector<HolderComponent *> &selection)
	{
	}

	void apply_changes(HolderComponent *holder)
	{
		CL_GUIComponent *comp = holder->get_first_child();
		CL_Slider *slider = dynamic_cast<CL_Slider*>(comp);
		if (slider)
			slider->set_page_step(CL_StringHelp::text_to_int(value));
	}
};
