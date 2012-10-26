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
**    Harry Storbacka
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_layout_corners.h"
#include "API/Core/XML/dom_document.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"

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
#include "API/GUI/Components/textedit.h"
#include "API/GUI/Components/statusbar.h"
#include "API/GUI/Components/toolbar.h"
#include "API/GUI/Components/frame.h"
#include "API/GUI/Components/combobox.h"
#include "API/GUI/Components/spin.h"
#include "API/GUI/Components/scrollbar.h"
#include "API/GUI/Components/imageview.h"
#include "API/GUI/Components/window.h"

#include "gui_xml_loader_version_1_0.h"
#include "Layout/gui_layout_provider_corners.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GUIXMLLoaderVersion_1_0 Construction:

GUIXMLLoaderVersion_1_0::GUIXMLLoaderVersion_1_0(GUIComponent *component, GUILayout &layout)
: create_custom_callback(0), dialog_width(200), dialog_height(140)
{
	this->component = component;
	this->layout = layout;
}


GUIXMLLoaderVersion_1_0::~GUIXMLLoaderVersion_1_0()
{
}

/////////////////////////////////////////////////////////////////////////////
// GUIXMLLoaderVersion_1_0 Attributes:


/////////////////////////////////////////////////////////////////////////////
// GUIXMLLoaderVersion_1_0 Operations:

void GUIXMLLoaderVersion_1_0::load(DomDocument &doc)
{
	DomElement doc_element = doc.get_document_element();
	load(doc_element, component);

	if (dialog_width != 0 || dialog_height != 0)
	{
		if (!layout.is_null())
			layout.set_preferred_size(Size(dialog_width, dialog_height));
		Rect win_geom = component->get_window_geometry();
		component->set_geometry(Rect(win_geom.left, win_geom.top, win_geom.left + dialog_width, win_geom.top + dialog_height)); // set client area to specified size.
	}

	if (dialog_title != "")
	{
		Window *win = dynamic_cast<Window*>(component);
		if (win)
			win->set_title(dialog_title);
	}
}

void GUIXMLLoaderVersion_1_0::set_create_custom_callback(Callback_2<GUIComponent*, GUIComponent*, std::string> *callback)
{
	create_custom_callback = callback;
}

/////////////////////////////////////////////////////////////////////////////
// GUIXMLLoaderVersion_1_0 Implementation:

void GUIXMLLoaderVersion_1_0::load(DomElement &element, GUIComponent *parent)
{
	DomElement e = element.get_first_child().to_element();

	dialog_width = 0;
	dialog_height = 0;

	while (e.is_element())
	{
		std::string tag = e.get_tag_name();
		GUIComponent *new_comp = 0;

		if (tag == "button")
		{
			PushButton *co = new PushButton(parent);
			if (e.has_attribute("text"))
				co->set_text(e.get_attribute("text"));
			new_comp = co;
		}
		else if (tag == "checkbox")
		{
			CheckBox *co = new CheckBox(parent);
			if (e.has_attribute("text"))
				co->set_text(e.get_attribute("text"));
			new_comp = co;
		}
		else if (tag == "radiobutton")
		{
			RadioButton *co = new RadioButton(parent);
			if (e.has_attribute("text"))
				co->set_text(e.get_attribute("text"));
			if (e.has_attribute("group"))
				co->set_group_name(e.get_attribute("group"));
			new_comp = co;
		}
		else if (tag == "label")
		{
			Label *co = new Label(parent);
			if (e.has_attribute("text"))
				co->set_text(e.get_attribute("text"));
			new_comp = co;
		}
		else if (tag == "toolbar")
		{
			ToolBar *co = new ToolBar(parent);
			new_comp = co;
		}
		else if (tag == "progressbar")
		{
			ProgressBar *co = new ProgressBar(parent);
			new_comp = co;
		}
		else if (tag == "lineedit")
		{
			LineEdit *co = new LineEdit(parent);
			if (e.has_attribute("text"))
				co->set_text(e.get_attribute("text"));
			if (e.has_attribute("password_mode"))
				co->set_password_mode(e.get_attribute_bool("password_mode", false));
			new_comp = co;
		}
		else if (tag == "textedit")
		{
			TextEdit *co = new TextEdit(parent);
			if (e.has_attribute("text"))
				co->set_text(e.get_attribute("text"));
			new_comp = co;
		}
		else if (tag == "slider")
		{
			Slider *co = new Slider(parent);
			co->set_min(StringHelp::text_to_int(e.get_attribute("min")));
			co->set_max(StringHelp::text_to_int(e.get_attribute("max")));
			co->set_tick_count(StringHelp::text_to_int(e.get_attribute("ticks")));
			co->set_page_step(StringHelp::text_to_int(e.get_attribute("page_step")));
			new_comp = co;
		}
		else if (tag == "listview")
		{
			ListView *co = new ListView(parent);
			ListViewHeader *header = co->get_header();

			std::vector<DomNode> columns_nodes = e.select_nodes("listview_header/listview_column");
			for(size_t i = 0; i < columns_nodes.size(); ++i)
			{
				DomElement column_element = columns_nodes[i].to_element();
				std::string id = column_element.get_attribute("col_id");
				std::string caption = column_element.get_attribute("caption");
				int width = column_element.get_attribute_int("width");

				ListViewColumnHeader column = header->create_column(id, caption);
				column.set_width(width);
				header->append(column);
			}

			new_comp = co;
		}
		else if (tag == "tab")
		{
			Tab *co = new Tab(parent);
			new_comp = co;

			DomElement tab_child = e.get_first_child().to_element();
			while (tab_child.is_element())
			{
				if (tab_child.get_tag_name() == "tabpage")
				{
					std::string label = tab_child.get_attribute("label", "Error: NO LABEL!");
					int id = StringHelp::text_to_int(tab_child.get_attribute("id", "0"));
					TabPage *tab_page = co->add_page(label, id);
					GUILayoutCorners tabpage_layout;
					tab_page->set_layout(tabpage_layout);
					load(tab_child, tab_page);
				}

				tab_child = tab_child.get_next_sibling().to_element();
			}
		}
		else if (tag == "statusbar")
		{
			StatusBar *co = new StatusBar(parent);
			new_comp = co;
		}
		else if (tag == "menubar")
		{
			MenuBar *co = new MenuBar(parent);
			new_comp = co;
		}
		else if (tag == "combobox")
		{
			ComboBox *co = new ComboBox(parent);
			new_comp = co;
		}
		else if (tag == "scrollbar")
		{
			ScrollBar *co = new ScrollBar(parent);
			new_comp = co;
		}
		else if (tag == "spin")
		{
			Spin *co = new Spin(parent);
			new_comp = co;
		}
		else if (tag == "imageview")
		{
			ImageView *co = new ImageView(parent);
			new_comp = co;
		}
		else if (tag == "frame")
		{
			Frame *co = new Frame(parent);
			if (e.has_attribute("text"))
				co->set_header_text(e.get_attribute("text"));
			new_comp = co;
			GUILayoutCorners layout_corners;
			co->set_layout(layout_corners);
			load(e, co);
		}
		else if (tag == "dialog")
		{
			dialog_width = StringHelp::text_to_int(e.get_attribute("width"));
			dialog_height = StringHelp::text_to_int(e.get_attribute("height"));
			if (e.has_attribute("title"))
				dialog_title = e.get_attribute("title");
		}
		else // unknown tag... try create a custom_component
		{
			GUIComponent *co = 0;
			if (create_custom_callback && !create_custom_callback->is_null())
			{
				co = create_custom_callback->invoke(parent, tag);
			}
			new_comp = co;
		}

		if (new_comp)
		{
			if (!e.has_attribute("id"))
				throw Exception(string_format("GUIXMLLoaderVersion_1_0 need the field id for the component \"%1\"", tag));
			new_comp->set_id(e.get_attribute("id"));

			new_comp->set_class(e.get_attribute("class"));
			new_comp->set_enabled(e.get_attribute_bool("enabled", true));

			//Position
			int dist_tl_x = StringHelp::text_to_int(e.get_attribute("dist_tl_x"));
			int dist_tl_y = StringHelp::text_to_int(e.get_attribute("dist_tl_y"));
			int dist_rb_x = StringHelp::text_to_int(e.get_attribute("dist_br_x"));
			int dist_rb_y = StringHelp::text_to_int(e.get_attribute("dist_br_y"));
			//Anchor
			ComponentAnchorPoint ap_tl = (ComponentAnchorPoint)StringHelp::text_to_int(e.get_attribute("anchor_tl"));
			ComponentAnchorPoint ap_br = (ComponentAnchorPoint)StringHelp::text_to_int(e.get_attribute("anchor_br"));

			Rect g;
			if (e.has_attribute("geom")) //Load geometry from the old geom attribute
			{
				std::string str = e.get_attribute("geom");
				std::vector<std::string> split = StringHelp::split_text(str, ",");

				g.left = StringHelp::text_to_int(split[0]);
				g.top = StringHelp::text_to_int(split[1]);
				g.right = StringHelp::text_to_int(split[2]);
				g.bottom = StringHelp::text_to_int(split[3]);
			}
			else //Use the fields dist_*
			{
				Point tl = GUILayoutProvider_Corners::get_point(parent->get_geometry(), ap_tl, dist_tl_x, dist_tl_y);
				Point rb = GUILayoutProvider_Corners::get_point(parent->get_geometry(), ap_br, dist_tl_x, dist_tl_y);
				g.left= tl.x;
				g.top = tl.y;
				g.right = rb.x;
				g.bottom = rb.y;
			}
			new_comp->set_geometry(g);

			GUILayout parent_layout = parent->get_layout();
			if (!parent_layout.is_null())
			{
				parent_layout.get_provider();
				GUILayoutProvider_Corners *corner_provider_layout = dynamic_cast<GUILayoutProvider_Corners*>(parent_layout.get_provider());

				if (corner_provider_layout)
					corner_provider_layout->add_component(new_comp, ap_tl, dist_tl_x, dist_tl_y, ap_br, dist_rb_x, dist_rb_y);
			}
		}

		e = e.get_next_sibling().to_element();
	}

	GUILayout parent_layout = parent->get_layout();
	if (!parent_layout.is_null())
	{
		parent_layout.set_geometry(parent->get_size());
	}
}

}
