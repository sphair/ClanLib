/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
*/


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
