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

#include "precomp.h"
#include "API/Core/System/clanstring.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Core/Resources/resourcetype_raw.h"
#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/inputsource_provider.h"
#include "API/GUI/component_style.h"
#include "component_manager_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ComponentManager_Generic construction:

CL_ComponentManager_Generic::CL_ComponentManager_Generic(
	const std::string &res_id,
	CL_ResourceManager *resources,
	CL_Component *parent,
	CL_StyleManager *style)
: style_manager(style), parent(parent)
{
	if (style_manager == 0) style_manager = parent->get_style_manager();

	CL_Resource resource = resources->get_resource(res_id);
	CL_DomElement element = resource.get_element();
	if (element.has_attribute("file"))
	{
		document = CL_DomDocument(resources->get_resource_provider()->open_source(element.get_attribute("file")), true);
		element = document.get_document_element();
	}
	else
	{
		document = element.get_owner_document();
	}

	parse_components(element, parent, true);
}

CL_ComponentManager_Generic::CL_ComponentManager_Generic(
	const std::string &filename,
	CL_Component *parent,
	CL_StyleManager *style,
	CL_InputSourceProvider *input_provider,
	bool delete_provider)
: style_manager(style), parent(parent)
{
	if (style_manager == 0) style_manager = parent->get_style_manager();

	if (input_provider == 0)
	{
		input_provider = CL_InputSourceProvider::create_file_provider(".");
		delete_provider = true;
	}

	document = CL_DomDocument(input_provider->open_source(filename), true);
	if (delete_provider) delete input_provider;

	parse_components(document.get_document_element(), parent, true);
}

CL_ComponentManager_Generic::CL_ComponentManager_Generic(
	const CL_DomDocument &document,
	const CL_DomElement &element,
	CL_Component *parent,
	CL_StyleManager *style)
: style_manager(style), parent(parent), document(document)
{
	if (style_manager == 0) style_manager = parent->get_style_manager();
	parse_components(element, parent, true);
}

CL_ComponentManager_Generic::~CL_ComponentManager_Generic()
{
	std::vector<CL_Component *>::iterator it;
	
	for (it = root_components.begin (); it != root_components.end (); ++it)
		delete *it;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ComponentManager_Generic attributes:

CL_Component *CL_ComponentManager_Generic::get_component(const std::string &component_id) const
{
	std::map<std::string, CL_Component *>::const_iterator it = component_map.find(component_id);
	if (it != component_map.end())
	{
		return (*it).second;
	}

	throw CL_Error(std::string("Unable to locate component ") + component_id);
}

/////////////////////////////////////////////////////////////////////////////
// CL_ComponentManager_Generic operations:

/////////////////////////////////////////////////////////////////////////////
// CL_ComponentManager_Generic implementation:

void CL_ComponentManager_Generic::parse_components(const CL_DomElement &componentsElement, CL_Component *parent, bool root_level)
{
	for (CL_DomNode node = componentsElement.get_first_child(); !node.is_null(); node = node.get_next_sibling())
	{
		if (!node.is_element()) continue;

		CL_DomElement element = node.to_element();
		std::string component_type = element.get_tag_name();
		std::string component_name = element.get_attribute("name");

		CL_Component *component = style_manager->create_component(component_type, parent->get_client_area());
		component->sig_set_options()(element);

		// Does this belong here? -- mbn
		if (element.has_attribute("width") == false && element.has_attribute("height") == false) component->find_preferred_size();

				if (!component_name.empty()) 
		{
			component_map[component_name] = component;
		}

		if (root_level)
		{
			root_components.push_back(component);
		}
		else
		{
			/*
				remove all childs at destruction
			*/
			component->get_parent ()->delete_child (component);
		}

		// Parse any child components, if any.
		if (element.named_item("components").is_element())
		{
			parse_components(element.named_item("components").to_element(), component, false);
		}
	}
}
