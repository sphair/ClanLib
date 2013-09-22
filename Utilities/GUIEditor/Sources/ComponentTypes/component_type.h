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

class GridObject;

class ComponentType
{
public:
	ComponentType(const std::string &xmlname, const std::string &name, int id, const std::string &resource_icon)
	: xmlname(xmlname), name(name), id(id), resource_icon(resource_icon)
	{
	}

	virtual clan::GUIComponent *create_component(clan::GUIComponent *parent) const = 0;
	virtual void on_show_context_menu(clan::PopupMenu &menu, GridObject *grid_object) { }

	std::string xmlname;
	std::string name;
	int id;
	std::string resource_icon;
};
