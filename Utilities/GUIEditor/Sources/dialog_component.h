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

class DialogDocument;
class DialogContainer;
class DialogProperty;

class DialogComponent
{
public:
	CL_StringRef get_type() const;
	CL_StringRef get_name() const;
	int get_id() const { return id; }

	std::vector<DialogProperty> get_properties() const;
	CL_StringRef get_property(const CL_StringRef &name, const CL_StringRef &default_value = CL_StringRef()) const;
	void set_property(const CL_StringRef &name, const CL_StringRef &value);

	const std::vector<DialogContainer *> &get_containers() const { return containers; }
	std::vector<DialogContainer *> &get_containers() { return containers; }
	DialogContainer *create_container();

	DialogDocument *get_document();
	DialogContainer *get_owner() { return owner; }

	void destroy_component();

private:
	DialogComponent(DialogContainer *container, CL_DomElement element, int id);

	DialogContainer *owner;
	std::vector<DialogContainer *> containers;
	CL_DomElement element;
	int id;

	friend class DialogDocument;
	friend class DialogContainer;
};

class DialogProperty
{
public:
	CL_String name;
	CL_String value;
};
