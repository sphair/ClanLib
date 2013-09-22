/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
**    Harry Storbacka
**    Kenneth Gangstoe
*/

#pragma once

#include "component_type.h"

class TabComponentType : public ComponentType
{
public:
	TabComponentType(const std::string &xmlname, const std::string &name, int id, const std::string &icon)	
	: ComponentType(xmlname, name, id, icon) {}

	virtual clan::GUIComponent *create_component(clan::GUIComponent *parent) const
	{
		static int tab_page_count = 0;

		clan::Tab *component = new clan::Tab(parent);
		component->set_geometry(clan::Rect(0, 0, 75, 23));

		clan::TabPage *page1 = component->add_page("Page 1");
		page1->set_id(clan::string_format("%1%2", page1->get_tag_name(), tab_page_count++));

		return component;
	}
};
