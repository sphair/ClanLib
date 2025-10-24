
#pragma once

#include "property_item.h"

class PropertyItemStepSize : public PropertyItemLineEdit
{
public:
	PropertyItemStepSize()
	: PropertyItemLineEdit("step size")
	{
	}

	void selection_changed(const std::vector<HolderComponent *> &selection)
	{
		if (!selection.empty())
		{
			CL_GUIComponent *comp = selection[0]->get_component();
			CL_Slider *slider = dynamic_cast<CL_Slider*>(comp);
			if (slider)
				value = CL_StringHelp::int_to_text(slider->get_page_step());
		}
	}

	void apply_changes(HolderComponent *holder)
	{
		CL_GUIComponent *comp = holder->get_component();
		CL_Slider *slider = dynamic_cast<CL_Slider*>(comp);
		if (slider)
			slider->set_page_step(CL_StringHelp::text_to_int(value));
	}
};
