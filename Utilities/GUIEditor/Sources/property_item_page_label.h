
#pragma once

#include "property_item.h"

class PropertyItemPageLabel : public PropertyItemLineEdit
{
public:
	void selection_changed(const std::vector<HolderComponent *> &selection)
	{
	}

	void apply_changes(HolderComponent *holder)
	{
		CL_GUIComponent *comp = holder->get_first_child();

		CL_Tab *tab = dynamic_cast<CL_Tab*>(comp);
		if (tab)
		{
			int current_page = tab->get_current_page_index();
			CL_TabPage *tab_page = tab->get_page(current_page);
			tab_page->set_label(value);
		}
	}
};
