/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

#include "css_select_node2.h"

class CL_API_CSSLAYOUT CL_DomSelectNode : public CL_CSSSelectNode2
{
public:
	CL_DomSelectNode(const CL_DomElement &element);
	bool parent();
	bool prev_sibling();
	void push();
	void pop();

	CL_String name();
	CL_String lang();
	CL_String id();
	std::vector<CL_String> element_classes();
	std::vector<CL_String> pseudo_classes();
	CL_String get_attribute_value(const CL_String &name, bool &out_found);
	int child_index();

private:
	CL_DomElement dom_element;
	CL_DomElement pos;
	bool is_modified;
	std::vector<CL_DomElement> saved_elements;
};
