
#pragma once

#include "property_item.h"

class PropertyItemTickCount : public PropertyItemLineEdit
{
public:
	PropertyItemTickCount()
	: PropertyItemLineEdit("tick count")
	{
	}

	void selection_changed(const std::vector<HolderComponent *> &selection)
	{
		if (!selection.empty())
		{
			CL_GUIComponent *comp = selection[0]->get_component();
			CL_Slider *slider = dynamic_cast<CL_Slider*>(comp);
			if (slider)
				value = CL_StringHelp::int_to_text(slider->get_tick_count());
		}
	}

	void apply_changes(HolderComponent *holder)
	{
		CL_GUIComponent *comp = holder->get_component();
		CL_Slider *slider = dynamic_cast<CL_Slider*>(comp);
		if (slider)
			slider->set_tick_count(CL_StringHelp::text_to_int(value));
	}
};
