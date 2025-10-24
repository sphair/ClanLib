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

/////////////////////////////////////////////////////////////////////////////
// CL_GUIThemeLayoutManager_Impl Class:

class CL_GUIThemeLayoutManager_Impl
{
public:
	CL_DomDocument layout;
	CL_CSSDocument2 css;

	bool has_layout(const CL_String &target_component_type);

	CL_CSSLayout create_layout(const CL_String &target_component_type);
	CL_CSSLayout create_layout(CL_String component_type, CL_DomElement &dom_element);
	void create_layout_parts(CL_DomNode dom_node, CL_CSSLayoutElement &css_element);
};

bool CL_GUIThemeLayoutManager_Impl::has_layout(const CL_String &target_component_type)
{
	CL_DomElement dom_element = layout.get_document_element().get_first_child_element().to_element();

	while (dom_element.is_element())
	{
		if(target_component_type == dom_element.get_tag_name())
			return true;

		dom_element = dom_element.get_next_sibling_element();
	}

	return false;
}

CL_CSSLayout CL_GUIThemeLayoutManager_Impl::create_layout(const CL_String &target_component_type)
{
	CL_DomElement dom_element = layout.get_document_element().get_first_child_element().to_element();

	while (dom_element.is_element())
	{
		CL_String component_type = dom_element.get_tag_name();

		if(target_component_type == component_type)
		{
			return create_layout(component_type, dom_element);
		}

		dom_element = dom_element.get_next_sibling_element();
	}

	throw CL_Exception("Unable to create layout for component type " + target_component_type);
}

CL_CSSLayout CL_GUIThemeLayoutManager_Impl::create_layout(CL_String component_type, CL_DomElement &dom_element)
{
	CL_CSSLayout css_layout;
	CL_CSSLayoutElement css_element = css_layout.create_element(component_type);
	css_layout.set_root_element(css_element);

	create_layout_parts(dom_element.get_first_child(), css_element);

	return css_layout;
}

void CL_GUIThemeLayoutManager_Impl::create_layout_parts(CL_DomNode dom_node, CL_CSSLayoutElement &css_element)
{
	while (!dom_node.is_null())
	{
		if (dom_node.is_element())
		{
			CL_DomElement dom_element = dom_node.to_element();
			CL_String part_type = dom_element.get_tag_name();
			CL_CSSLayoutElement part_css_element = css_element.create_element(part_type);
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
// CL_GUIThemeLayoutManager Construction:

CL_GUIThemeLayoutManager::CL_GUIThemeLayoutManager()
: impl(CL_SharedPtr<CL_GUIThemeLayoutManager_Impl>(new CL_GUIThemeLayoutManager_Impl()))
{
}

CL_GUIThemeLayoutManager::CL_GUIThemeLayoutManager(const CL_StringRef &xml_fullname, const CL_StringRef &css_fullname)
: impl(CL_SharedPtr<CL_GUIThemeLayoutManager_Impl>(new CL_GUIThemeLayoutManager_Impl()))
{
	CL_String xml_path = CL_PathHelp::get_fullpath(xml_fullname, CL_PathHelp::path_type_file);
	CL_String xml_filename = CL_PathHelp::get_filename(xml_fullname, CL_PathHelp::path_type_file);
	CL_VirtualFileSystem xml_vfs(xml_path);
	CL_VirtualDirectory xml_dir = xml_vfs.get_root_directory();
	CL_IODevice xml_file;
	xml_file = xml_dir.open_file_read(xml_filename);
	impl->layout.load(xml_file); 

	impl->css.add_sheet(css_fullname);
}

CL_GUIThemeLayoutManager::CL_GUIThemeLayoutManager(const CL_StringRef &xml_fullname, const CL_StringRef &css_fullname, const CL_VirtualDirectory &dir)
: impl(CL_SharedPtr<CL_GUIThemeLayoutManager_Impl>(new CL_GUIThemeLayoutManager_Impl()))
{
	CL_IODevice file;
	file = dir.open_file_read(xml_fullname);
	impl->layout.load(file); 

	throw CL_Exception("CSS loading not implemented");
}	

CL_GUIThemeLayoutManager::CL_GUIThemeLayoutManager(CL_IODevice &xml_file, CL_IODevice &css_file)
: impl(CL_SharedPtr<CL_GUIThemeLayoutManager_Impl>(new CL_GUIThemeLayoutManager_Impl()))
{
	impl->layout.load(xml_file); 

	throw CL_Exception("CSS loading not implemented");
}

CL_GUIThemeLayoutManager::CL_GUIThemeLayoutManager(CL_DomDocument &doc, CL_CSSDocument2 &css)
: impl(CL_SharedPtr<CL_GUIThemeLayoutManager_Impl>(new CL_GUIThemeLayoutManager_Impl()))
{
	impl->layout = doc;
	impl->css = css;
}

CL_GUIThemeLayoutManager::~CL_GUIThemeLayoutManager()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUILayoutXMLLoader Attributes:

bool CL_GUIThemeLayoutManager::has_layout(const CL_String &component_type)
{
	return impl->has_layout(component_type);
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUILayoutXMLLoader Operations:

CL_CSSLayout CL_GUIThemeLayoutManager::create_layout(const CL_String &component_type)
{
	return impl->create_layout(component_type);
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIThemeLayoutManager Implementation:
