/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
**    Kenneth Gangstoe
*/

#include "GUI/precomp.h"
#include "gui_theme_layout_manager.h"
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/IOData/virtual_directory.h"
#include "API/Core/XML/dom_document.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/XML/dom_text.h"
#include "API/CSSLayout/css_layout.h"
#include "API/CSSLayout/css_layout_element.h"
#include "API/CSSLayout/css_layout_text.h"
#include "API/CSSLayout/css_document2.h"
#include "API/CSSLayout/css_property_list2.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GUIThemeLayoutManager_Impl Class:

class GUIThemeLayoutManager_Impl
{
public:
	DomDocument layout;
	CSSDocument css;

	bool has_layout(const std::string &target_component_type);

	CSSLayout create_layout(const std::string &target_component_type);
	CSSLayout create_layout(std::string component_type, DomElement &dom_element);
	void create_layout_parts(DomNode dom_node, CSSLayoutElement &css_element);
};

bool GUIThemeLayoutManager_Impl::has_layout(const std::string &target_component_type)
{
	DomElement dom_element = layout.get_document_element().get_first_child_element().to_element();

	while (dom_element.is_element())
	{
		if(target_component_type == dom_element.get_tag_name())
			return true;

		dom_element = dom_element.get_next_sibling_element();
	}

	return false;
}

CSSLayout GUIThemeLayoutManager_Impl::create_layout(const std::string &target_component_type)
{
	DomElement dom_element = layout.get_document_element().get_first_child_element().to_element();

	while (dom_element.is_element())
	{
		std::string component_type = dom_element.get_tag_name();

		if(target_component_type == component_type)
		{
			return create_layout(component_type, dom_element);
		}

		dom_element = dom_element.get_next_sibling_element();
	}

	throw Exception("Unable to create layout for component type " + target_component_type);
}

CSSLayout GUIThemeLayoutManager_Impl::create_layout(std::string component_type, DomElement &dom_element)
{
	CSSLayout css_layout;
	CSSLayoutElement css_element = css_layout.create_element(component_type);
	css_element.apply_properties(css.select(dom_element));
	css_element.apply_properties(dom_element.get_attribute("style"));
	css_layout.set_root_element(css_element);

	create_layout_parts(dom_element.get_first_child(), css_element);

	return css_layout;
}

void GUIThemeLayoutManager_Impl::create_layout_parts(DomNode dom_node, CSSLayoutElement &css_element)
{
	while (!dom_node.is_null())
	{
		if (dom_node.is_element())
		{
			DomElement dom_element = dom_node.to_element();
			std::string part_type = dom_element.get_tag_name();
			CSSLayoutElement part_css_element = css_element.create_element(part_type);
			part_css_element.apply_properties(css.select(dom_element));
			part_css_element.apply_properties(dom_element.get_attribute("style"));
			create_layout_parts(dom_element.get_first_child(), part_css_element);
		}
		else if (dom_node.is_text())
		{
			css_element.create_text(dom_node.to_text().get_node_value());
		}
		dom_node = dom_node.get_next_sibling();
	}
}

/////////////////////////////////////////////////////////////////////////////
// GUIThemeLayoutManager Construction:

GUIThemeLayoutManager::GUIThemeLayoutManager()
: impl(std::shared_ptr<GUIThemeLayoutManager_Impl>(new GUIThemeLayoutManager_Impl()))
{
}

GUIThemeLayoutManager::GUIThemeLayoutManager(const std::string &xml_fullname, const std::string &css_fullname)
: impl(std::shared_ptr<GUIThemeLayoutManager_Impl>(new GUIThemeLayoutManager_Impl()))
{
	std::string xml_path = PathHelp::get_fullpath(xml_fullname, PathHelp::path_type_file);
	std::string xml_filename = PathHelp::get_filename(xml_fullname, PathHelp::path_type_file);
	VirtualFileSystem xml_vfs(xml_path);
	VirtualDirectory xml_dir = xml_vfs.get_root_directory();
	IODevice xml_file;
	xml_file = xml_dir.open_file_read(xml_filename);
	impl->layout.load(xml_file); 

	impl->css.add_sheet(css_fullname);
}

GUIThemeLayoutManager::GUIThemeLayoutManager(const std::string &xml_fullname, const std::string &css_fullname, const VirtualDirectory &dir)
: impl(std::shared_ptr<GUIThemeLayoutManager_Impl>(new GUIThemeLayoutManager_Impl()))
{
	IODevice file;
	file = dir.open_file_read(xml_fullname);
	impl->layout.load(file); 

	throw Exception("CSS loading not implemented");
}	

GUIThemeLayoutManager::GUIThemeLayoutManager(IODevice &xml_file, IODevice &css_file)
: impl(std::shared_ptr<GUIThemeLayoutManager_Impl>(new GUIThemeLayoutManager_Impl()))
{
	impl->layout.load(xml_file); 

	throw Exception("CSS loading not implemented");
}

GUIThemeLayoutManager::GUIThemeLayoutManager(DomDocument &doc, CSSDocument &css)
: impl(std::shared_ptr<GUIThemeLayoutManager_Impl>(new GUIThemeLayoutManager_Impl()))
{
	impl->layout = doc;
	impl->css = css;
}

GUIThemeLayoutManager::~GUIThemeLayoutManager()
{
}

/////////////////////////////////////////////////////////////////////////////
// GUILayoutXMLLoader Attributes:

bool GUIThemeLayoutManager::has_layout(const std::string &component_type)
{
	return impl->has_layout(component_type);
}

/////////////////////////////////////////////////////////////////////////////
// GUILayoutXMLLoader Operations:

CSSLayout GUIThemeLayoutManager::create_layout(const std::string &component_type)
{
	return impl->create_layout(component_type);
}

/////////////////////////////////////////////////////////////////////////////
// GUIThemeLayoutManager Implementation:

}
