/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#ifndef header_component_manager_generic
#define header_component_manager_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include <string>
#include <stack>
#include <vector>
#include <stdio.h>

#include "API/GUI/component_manager.h"
#include "API/GUI/stylemanager.h"
#include "API/GUI/component.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/XML/dom_document.h"

class CL_InputSource;

class CL_ComponentManager_Generic
{
//! Construction:
public:
	CL_ComponentManager_Generic(
		const std::string &resource_id,
		CL_ResourceManager *resources,
		CL_Component *parent,
		CL_StyleManager *style);

	CL_ComponentManager_Generic(
		const std::string &filename,
		CL_Component *parent,
		CL_StyleManager *style,
		CL_InputSourceProvider *input_provider,
		bool delete_provider);

	CL_ComponentManager_Generic(
		const CL_DomDocument &document,
		const CL_DomElement &element,
		CL_Component *parent,
		CL_StyleManager *style);
		
	~CL_ComponentManager_Generic();

//! Attributes:
public:
	CL_Component *get_component(const std::string &component_id) const;

	CL_StyleManager *get_style_manager() const { return style_manager; }

//! Operations:
public:

//! Implementation:
private:
	void parse_components(const CL_DomElement &componentsElement, CL_Component *parent, bool root_level);

	CL_StyleManager *style_manager;
	
	CL_Component *parent;

	CL_DomDocument document;

	std::map<std::string, CL_Component *> component_map;

	std::vector<CL_Component *> root_components;
};

#endif
