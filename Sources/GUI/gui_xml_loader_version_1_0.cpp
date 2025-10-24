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
**    Harry Storbacka
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_layout_corners.h"
#include "API/Core/XML/dom_document.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/Text/string_help.h"

#include "API/GUI/Components/push_button.h"
#include "API/GUI/Components/checkbox.h"
#include "API/GUI/Components/radiobutton.h"
#include "API/GUI/Components/label.h"
#include "API/GUI/Components/listview.h"
#include "API/GUI/Components/listview_header.h"
#include "API/GUI/Components/progressbar.h"
#include "API/GUI/Components/tab.h"
#include "API/GUI/Components/tab_page.h"
#include "API/GUI/Components/slider.h"
#include "API/GUI/Components/menubar.h"
#include "API/GUI/Components/lineedit.h"
#include "API/GUI/Components/statusbar.h"
#include "API/GUI/Components/toolbar.h"
#include "API/GUI/Components/frame.h"
#include "API/GUI/Components/combobox.h"
#include "API/GUI/Components/spin.h"
#include "API/GUI/Components/scrollbar.h"
#include "API/GUI/Components/imageview.h"

#include "gui_xml_loader_version_1_0.h"
#include "gui_layout_provider_corners.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GUIXMLLoaderVersion_1_0 Construction:

CL_GUIXMLLoaderVersion_1_0::CL_GUIXMLLoaderVersion_1_0(CL_GUIComponent *component, CL_GUILayout &layout)
: create_custom_callback(0), dialog_width(200), dialog_height(140)
{
	this->component = component;
	this->layout = layout;
}


CL_GUIXMLLoaderVersion_1_0::~CL_GUIXMLLoaderVersion_1_0()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIXMLLoaderVersion_1_0 Attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_GUIXMLLoaderVersion_1_0 Operations:

void CL_GUIXMLLoaderVersion_1_0::load(CL_DomDocument &doc)
{
	CL_DomElement doc_element = doc.get_document_element();
	load(doc_element, component);

	layout.set_preferred_size(CL_Size(dialog_width, dialog_height));
	CL_Rect win_geom = component->get_window_geometry();
	component->set_geometry(CL_Rect(win_geom.left, win_geom.top, win_geom.left + dialog_width, win_geom.top + dialog_height)); // set client area to specified size.
}

void CL_GUIXMLLoaderVersion_1_0::set_create_custom_callback(CL_Callback_2<CL_GUIComponent*, CL_GUIComponent*, CL_String> *callback)
{
	create_custom_callback = callback;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIXMLLoaderVersion_1_0 Implementation:


void CL_GUIXMLLoaderVersion_1_0::load(CL_DomElement &element, CL_GUIComponent *parent)
{
	CL_DomElement e = element.get_first_child().to_element();

	dialog_width = 0;
	dialog_height = 0;

	while (e.is_element())
	{
		CL_String tag = e.get_tag_name();
		CL_GUIComponent *new_comp = 0;

		if (tag == "button")
		{
			CL_PushButton *co = new CL_PushButton(parent);
			if (e.has_attribute("text"))
				co->set_text(e.get_attribute("text"));
			new_comp = co;
		}
		else if (tag == "checkbox")
		{
			CL_CheckBox *co = new CL_CheckBox(parent);
			if (e.has_attribute("text"))
				co->set_text(e.get_attribute("text"));
			new_comp = co;
		}
		else if (tag == "radiobutton")
		{
			CL_RadioButton *co = new CL_RadioButton(parent);
			if (e.has_attribute("text"))
				co->set_text(e.get_attribute("text"));
			if (e.has_attribute("group"))
				co->set_group_name(e.get_attribute("group"));
			new_comp = co;
		}
		else if (tag == "label")
		{
			CL_Label *co = new CL_Label(parent);
			if (e.has_attribute("text"))
				co->set_text(e.get_attribute("text"));
			new_comp = co;
		}
		else if (tag == "toolbar")
		{
			CL_ToolBar *co = new CL_ToolBar(parent);
			new_comp = co;
		}
		else if (tag == "progressbar")
		{
			CL_ProgressBar *co = new CL_ProgressBar(parent);
			new_comp = co;
		}
		else if (tag == "lineedit")
		{
			CL_LineEdit *co = new CL_LineEdit(parent);
			if (e.has_attribute("text"))
				co->set_text(e.get_attribute("text"));
			new_comp = co;
		}
		else if (tag == "slider")
		{
			CL_Slider *co = new CL_Slider(parent);
			co->set_min(CL_StringHelp::text_to_int(e.get_attribute("min")));
			co->set_max(CL_StringHelp::text_to_int(e.get_attribute("max")));
			co->set_tick_count(CL_StringHelp::text_to_int(e.get_attribute("ticks")));
			co->set_page_step(CL_StringHelp::text_to_int(e.get_attribute("page_step")));
			new_comp = co;
		}
		else if (tag == "listview")
		{
			CL_ListView *co = new CL_ListView(parent);
			CL_ListViewHeader *header = co->get_header();

			std::vector<CL_DomNode> columns_nodes = e.select_nodes("listview_header/listview_column");
			for(size_t i = 0; i < columns_nodes.size(); ++i)
			{
				CL_DomElement column_element = columns_nodes[i].to_element();
				CL_String id = column_element.get_attribute("col_id");
				CL_String caption = column_element.get_attribute("caption");
				int width = column_element.get_attribute_int("width");

				CL_ListViewColumnHeader column = header->create_column(id, caption);
				column.set_width(width);
				header->append(column);
			}

			new_comp = co;
		}
		else if (tag == "tab")
		{
			CL_Tab *co = new CL_Tab(parent);
			new_comp = co;

			CL_DomElement tab_child = e.get_first_child().to_element();
			while (tab_child.is_element())
			{
				if (tab_child.get_tag_name() == "tabpage")
				{
					CL_String label = tab_child.get_attribute("label", "Error: NO LABEL!");
					int id = CL_StringHelp::text_to_int(tab_child.get_attribute("id", "0"));
					CL_TabPage *tab_page = co->add_page(label, id);
					CL_GUILayoutCorners tabpage_layout;
					tab_page->set_layout(tabpage_layout);
					load(tab_child, tab_page);
				}

				tab_child = tab_child.get_next_sibling().to_element();
			}
		}
		else if (tag == "statusbar")
		{
			CL_StatusBar *co = new CL_StatusBar(parent);
			new_comp = co;
		}
		else if (tag == "menubar")
		{
			CL_MenuBar *co = new CL_MenuBar(parent);
			new_comp = co;
		}
		else if (tag == "combobox")
		{
			CL_ComboBox *co = new CL_ComboBox(parent);
			new_comp = co;
		}
		else if (tag == "scrollbar")
		{
			CL_ScrollBar *co = new CL_ScrollBar(parent);
			new_comp = co;
		}
		else if (tag == "spin")
		{
			CL_Spin *co = new CL_Spin(parent);
			new_comp = co;
		}
		else if (tag == "imageview")
		{
			CL_ImageView *co = new CL_ImageView(parent);
			new_comp = co;
		}
		else if (tag == "frame")
		{
			CL_Frame *co = new CL_Frame(parent);
			if (e.has_attribute("text"))
				co->set_header_text(e.get_attribute("text"));
			new_comp = co;
			CL_GUILayoutCorners layout_corners;
			co->set_layout(layout_corners);
			load(e, co);
		}
		else if (tag == "dialog")
		{
			dialog_width = CL_StringHelp::text_to_int(e.get_attribute("width"));
			dialog_height = CL_StringHelp::text_to_int(e.get_attribute("height"));
		}
		else // unknown tag... try create a custom_component
		{
			CL_GUIComponent *co = 0;
			if (create_custom_callback && !create_custom_callback->is_null())
			{
				co = create_custom_callback->invoke(parent, tag);
			}
			new_comp = co;
		}

		if (new_comp)
		{
			new_comp->set_id_name(e.get_attribute("id"));
			new_comp->set_class_name(e.get_attribute("class"));
			new_comp->set_enabled(e.get_attribute_bool("enabled", true));

			CL_String str = e.get_attribute("geom");
			std::vector<CL_String> split = CL_StringHelp::split_text(str, ",");

			CL_Rect g;
			g.left = CL_StringHelp::text_to_int(split[0]);
			g.top = CL_StringHelp::text_to_int(split[1]);
			g.right = CL_StringHelp::text_to_int(split[2]);
			g.bottom = CL_StringHelp::text_to_int(split[3]);
			new_comp->set_geometry(g);

			CL_GUILayout parent_layout = parent->get_layout();
			if (!parent_layout.is_null())
			{
				parent_layout.get_provider();
				CL_GUILayoutProvider_Corners *corner_provider_layout = dynamic_cast<CL_GUILayoutProvider_Corners*>(parent_layout.get_provider());
				if (corner_provider_layout)
				{
					int dist_tl_x = CL_StringHelp::text_to_int(e.get_attribute("dist_tl_x"));
					int dist_tl_y = CL_StringHelp::text_to_int(e.get_attribute("dist_tl_y"));
					int dist_rb_x = CL_StringHelp::text_to_int(e.get_attribute("dist_br_x"));
					int dist_rb_y = CL_StringHelp::text_to_int(e.get_attribute("dist_br_y"));
					CL_ComponentAnchorPoint ap_tl = (CL_ComponentAnchorPoint)CL_StringHelp::text_to_int(e.get_attribute("anchor_tl"));
					CL_ComponentAnchorPoint ap_br = (CL_ComponentAnchorPoint)CL_StringHelp::text_to_int(e.get_attribute("anchor_br"));

					corner_provider_layout->add_component(new_comp, ap_tl, dist_tl_x, dist_tl_y, ap_br, dist_rb_x, dist_rb_y);
				}
			}
		}

		e = e.get_next_sibling().to_element();
	}

	CL_GUILayout parent_layout = parent->get_layout();
	if (!parent_layout.is_null())
	{
		parent_layout.set_geometry(parent->get_size());
	}
}
