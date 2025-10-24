
#pragma once

#include "property_item.h"

class PropertyItemPageLabel : public PropertyItemLineEdit
{
public:
	PropertyItemPageLabel()
	: PropertyItemLineEdit("page label")
	{
	}

	void selection_changed(const std::vector<HolderComponent *> &selection)
	{
		if (!selection.empty())
		{
			CL_GUIComponent *comp = selection[0]->get_component();
			CL_TabPage *tab_page = dynamic_cast<CL_TabPage*>(comp);
			if (tab_page)
				value = tab_page->get_label();
		}
	}

	void apply_changes(HolderComponent *holder)
	{
		CL_GUIComponent *comp = holder->get_component();

		CL_Tab *tab = dynamic_cast<CL_Tab*>(comp);
		if (tab)
		{
			int current_page = tab->get_current_page_index();
			CL_TabPage *tab_page = tab->get_page(current_page);
			//tab_page->set_label(value);
		}
	}
};
