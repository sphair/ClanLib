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
**    Kenneth Gangstoe
*/

#include "precomp.h"
#include "holder_component.h"
#include "grid_component.h"
#include "custom_component.h"

/////////////////////////////////////////////////////////////////////////////
// HolderComponent Construction:

HolderComponent::HolderComponent(CL_GUIComponent *parent)
: CL_GUIComponent(parent), parent_grid(0), anchor_tl(cl_anchor_top_left), anchor_br(cl_anchor_top_left)
{
	set_type_name("holder");
	func_render().set(this, &HolderComponent::on_render);
	func_resized().set(this, &HolderComponent::on_resized);

//	font = CL_Font(get_gc(), "Tahoma", -11);

	parent_grid = static_cast<GridComponent*>(parent);
}

/////////////////////////////////////////////////////////////////////////////
// HolderComponent Attributes:

CL_GUIComponent *HolderComponent::get_container()
{
	return this;
}

CL_ComponentAnchorPoint HolderComponent::get_anchor_tl()
{
	return anchor_tl;
}

CL_ComponentAnchorPoint HolderComponent::get_anchor_br()
{
	return anchor_br;
}

CL_String HolderComponent::get_position_equation_x() const
{
	return pos_equation_x;
}

CL_String HolderComponent::get_position_equation_y() const
{
	return pos_equation_y;
}

CL_String HolderComponent::get_position_equation_x2() const
{
	return pos_equation_x2;
}

CL_String HolderComponent::get_position_equation_y2() const
{
	return pos_equation_y2;
}

/////////////////////////////////////////////////////////////////////////////
// HolderComponent Operations:

void HolderComponent::set_anchor_tl(CL_ComponentAnchorPoint ap)
{
	anchor_tl = ap;
}

void HolderComponent::set_anchor_br(CL_ComponentAnchorPoint ap)
{
	anchor_br = ap;
}

CL_DomElement HolderComponent::to_element(CL_DomDocument &doc)
{
	CL_GUIComponent *comp = get_first_child();

	CL_StringRef type = comp->get_type_name();

	CL_DomElement e = doc.create_element(comp->get_type_name());
	e.set_attribute("class", comp->get_class_name());
	e.set_attribute("id", comp->get_id_name());

	if (!pos_equation_x.empty())
		e.set_attribute("eq-x", pos_equation_x);
	if (!pos_equation_y.empty())
		e.set_attribute("eq-y", pos_equation_y);
	if (!pos_equation_x2.empty())
		e.set_attribute("eq-x2", pos_equation_x2);
	if (!pos_equation_y2.empty())
		e.set_attribute("eq-y2", pos_equation_y2);

	if (type == "button")
	{
		CL_PushButton *co = dynamic_cast<CL_PushButton*>(comp);
		e.set_attribute("text", co->get_text());
	}
	else if (comp->get_type_name() == "checkbox")
	{
		CL_CheckBox *co = dynamic_cast<CL_CheckBox*>(comp);
		e.set_attribute("text", co->get_text());
	}
	else if (comp->get_type_name() == "radiobutton")
	{
		CL_RadioButton *co = dynamic_cast<CL_RadioButton*>(comp);
		e.set_attribute("text", co->get_text());
		e.set_attribute("group", co->get_group_name());
	}
	else if (comp->get_type_name() == "label")
	{
		CL_Label *co = dynamic_cast<CL_Label*>(comp);
		e.set_attribute("text", co->get_text());
	}
	else if (comp->get_type_name() == "lineedit")
	{
		CL_LineEdit *co = dynamic_cast<CL_LineEdit*>(comp);
		e.set_attribute("text", co->get_text());
	}
	else if (comp->get_type_name() == "menubar")
	{
		// CL_MenuBar *co = dynamic_cast<CL_MenuBar*>(comp);
	}
	else if (comp->get_type_name() == "statusbar")
	{
		// CL_StatusBar *co = dynamic_cast<CL_StatusBar*>(comp);
	}
	else if (comp->get_type_name() == "toolbar")
	{
		// CL_ToolBar *co = dynamic_cast<CL_ToolBar*>(comp);
	}
	else if (comp->get_type_name() == "imageview")
	{
		// CL_ImageView *co = dynamic_cast<CL_ImageView*>(comp);
	}
	else if (comp->get_type_name() == "listview")
	{
		CL_ListView *co = dynamic_cast<CL_ListView*>(comp);
		save_listview(e, co);
	}
	else if (comp->get_type_name() == "slider")
	{
		CL_Slider *co = dynamic_cast<CL_Slider*>(comp);
		e.set_attribute("min", CL_StringHelp::int_to_text(co->get_min()));
		e.set_attribute("max", CL_StringHelp::int_to_text(co->get_max()));
		e.set_attribute("ticks", CL_StringHelp::int_to_text(co->get_tick_count()));
		e.set_attribute("page_step", CL_StringHelp::int_to_text(co->get_page_step()));
	}
	else if (comp->get_type_name() == "tab")
	{
		CL_Tab *co = dynamic_cast<CL_Tab*>(comp);

		CL_GUIComponent *child = co->get_first_child();
		while (child != 0)
		{
			if (child->get_type_name() == "tabpage")
			{
				CL_TabPage *tab_page = dynamic_cast<CL_TabPage*>(child);
				CL_DomElement tabpage_element = doc.create_element("tabpage");
				tabpage_element.set_attribute("label", tab_page->get_label());

				CL_GUIComponent *tabpage_child = child->get_first_child();
				while (tabpage_child != 0)
				{
					if (tabpage_child->get_type_name() == "holder")
					{
						HolderComponent *holder_comp = dynamic_cast<HolderComponent*>(tabpage_child);
						CL_DomElement tabpage_child_element = holder_comp->to_element(doc);
						tabpage_element.append_child(tabpage_child_element);
					}

					tabpage_child = tabpage_child->get_next_sibling();
				}

				e.append_child(tabpage_element);
			}

			child = child->get_next_sibling();
		}
		
	}
	else if (comp->get_type_name() == "frame")
	{
		CL_Frame *co = dynamic_cast<CL_Frame*>(comp);
		e.set_attribute("text", co->get_header_text());

		CL_GUIComponent *child = co->get_first_child();
		while (child != 0)
		{
			if (child->get_type_name() == "holder")
			{
				HolderComponent *holder_comp = dynamic_cast<HolderComponent*>(child);
				CL_DomElement frame_child_element = holder_comp->to_element(doc);
				e.append_child(frame_child_element);
			}

			child = child->get_next_sibling();
		}

	}
	else if (comp->get_type_name() == "spin")
	{
		// CL_Spin *co = dynamic_cast<CL_Spin*>(comp);
	}
	else if (comp->get_type_name() == "combobox")
	{
		// CL_ComboBox *co = dynamic_cast<CL_ComboBox*>(comp);
	}
	else // custom component
	{
		// CustomComponent *co = dynamic_cast<CustomComponent*>(comp);
	}

	save_anchors(e,comp);
	save_geometry(e,comp); // write geom="..." attribute

	return e;
}

CallbackInfo HolderComponent::get_callback_info()
{
	return callback_info; 
}

void HolderComponent::set_position_equations(const CL_String &str_x, const CL_String &str_y)
{
	pos_equation_x = str_x;
	pos_equation_y = str_y;
}

void HolderComponent::set_position_equations2( const CL_String &str_x, const CL_String &str_y )
{
	pos_equation_x2 = str_x;
	pos_equation_y2 = str_y;
}

/////////////////////////////////////////////////////////////////////////////
// HolderComponent Implementation:

CL_Rect HolderComponent::get_grabber_w() const
{
	CL_Rect box = get_size();
	return CL_Rect(CL_Point(box.left-7, (box.top+box.bottom)/2-3), CL_Size(6, 6));
}

CL_Rect HolderComponent::get_grabber_nw() const
{
	CL_Rect box = get_size();
	return CL_Rect(CL_Point(box.left-7, box.top-7), CL_Size(6, 6));
}

CL_Rect HolderComponent::get_grabber_n() const
{
	CL_Rect box = get_size();
	return CL_Rect(CL_Point((box.left+box.right)/2-3, box.top-7), CL_Size(6, 6));
}

CL_Rect HolderComponent::get_grabber_ne() const
{
	CL_Rect box = get_size();
	return CL_Rect(CL_Point(box.right, box.top-7), CL_Size(6, 6));
}

CL_Rect HolderComponent::get_grabber_e() const
{
	CL_Rect box = get_size();
	return CL_Rect(CL_Point(box.right, (box.top+box.bottom)/2-3), CL_Size(6, 6));
}

CL_Rect HolderComponent::get_grabber_se() const
{
	CL_Rect box = get_size();
	return CL_Rect(CL_Point(box.right, box.bottom), CL_Size(6, 6));
}

CL_Rect HolderComponent::get_grabber_s() const
{
	CL_Rect box = get_size();
	return CL_Rect(CL_Point((box.left+box.right)/2-3, box.bottom), CL_Size(6, 6));
}

CL_Rect HolderComponent::get_grabber_sw() const
{
	CL_Rect box = get_size();
	return CL_Rect(CL_Point(box.left-7, box.bottom), CL_Size(6, 6));
}

void HolderComponent::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	// Do a fill rect for otherwise transparent components:
	CL_String type = get_first_child()->get_type_name();
	if (type == "toolbar" || type == "menubar" /*|| type == "label"*/)
	{
		CL_Rect child_geom = get_first_child()->get_geometry();
		CL_Draw::fill(get_gc(), child_geom, CL_Colorf(0.35f, 0.498f, 0.603f, 0.2f));
	}

	/*  DON'T DELETE THIS COMMENTED CODE!
	CL_Point tl = get_geometry().get_top_left();
	CL_Point tl_win = component_to_window_coords(tl);
	gc.set_font(font);
	gc.draw_text(20,24, cl_format("%1, %2", tl.x, tl.y), CL_Colorf::red);
	gc.draw_text(60,24, cl_format("%1, %2", tl_win.x, tl_win.y), CL_Colorf::blue);
	*/
 }

void HolderComponent::on_resized()
{
	CL_GUIComponent *child = get_first_child();
	if (child)
		child->set_geometry(get_size());
}

void HolderComponent::save_geometry(CL_DomElement &e, CL_GUIComponent *comp)
{
	CL_Rect holder_g = comp->get_parent_component()->get_geometry();
	CL_Rect g = get_geometry();

	CL_String left = CL_StringHelp::int_to_text(g.left);
	CL_String right = CL_StringHelp::int_to_text(g.right);
	CL_String top = CL_StringHelp::int_to_text(g.top);
	CL_String bottom = CL_StringHelp::int_to_text(g.bottom);

	e.set_attribute("geom", left + "," + top + "," + right + "," + bottom);
}

void HolderComponent::save_anchors(CL_DomElement &e, CL_GUIComponent *comp)
{
	CL_Rect boundary = parent_grid->get_dialog_size();

	CL_GUIComponent *tab_or_frame_parent = get_tab_or_frame_parent(comp);
	if (tab_or_frame_parent)
	{
		boundary = tab_or_frame_parent->get_geometry().get_size();
	}

	CL_Rect g = get_geometry();

	CL_Point tl = g.get_top_left();
	CL_Point br = g.get_bottom_right();

	CL_Point dist_tl = get_dist(anchor_tl, tl, boundary);
	CL_Point dist_br = get_dist(anchor_br, br, boundary);

	e.set_attribute("anchor_tl", CL_StringHelp::int_to_text(anchor_tl));
	e.set_attribute("anchor_br", CL_StringHelp::int_to_text(anchor_br));
	e.set_attribute("dist_tl_x", CL_StringHelp::int_to_text(dist_tl.x));
	e.set_attribute("dist_tl_y", CL_StringHelp::int_to_text(dist_tl.y));
	e.set_attribute("dist_br_x", CL_StringHelp::int_to_text(dist_br.x));
	e.set_attribute("dist_br_y", CL_StringHelp::int_to_text(dist_br.y));
}

CL_Point HolderComponent::get_dist(CL_ComponentAnchorPoint ap, CL_Point p, CL_Rect boundary)
{
	int bw = boundary.get_width(), bh = boundary.get_height(); 
	if (ap == cl_anchor_top_left)
	{
		return CL_Point(p.x, p.y);
	}
	else if (ap == cl_anchor_top_right)
	{
		return CL_Point(bw-p.x, p.y);
	}
	else if (ap == cl_anchor_bottom_left)
	{
		return CL_Point(p.x, bh-p.y);
	}
	else if (ap == cl_anchor_bottom_right)
	{
		return CL_Point(bw-p.x, bh-p.y);
	}

	return CL_Point(0,0);
}

void HolderComponent::save_listview(CL_DomElement &e, CL_ListView *lv)
{
	CL_ListViewHeader *header = lv->get_header();
	
	CL_DomDocument doc = e.get_owner_document(); 
	CL_DomElement e_header = doc.create_element("listview_header");
	e.append_child(e_header);

	CL_ListViewColumnHeader col = header->get_first_column();

	while (!col.is_null())
	{
		CL_DomElement e_col = doc.create_element("listview_column");
		e_col.set_attribute("col_id", col.get_column_id());
		e_col.set_attribute("caption", col.get_caption());
		e_col.set_attribute("width", CL_StringHelp::int_to_text(col.get_width()));

		col = col.get_next_sibling();
	}
}

CL_Rect HolderComponent::convert_coordinates(CL_GUIComponent *move_comp, CL_GUIComponent *new_parent)
{
	CL_Point from_tl = move_comp->component_to_window_coords(CL_Point(0,0));
	CL_Point to_tl = new_parent->component_to_window_coords(CL_Point(0,0));
	CL_Point translated = from_tl - to_tl;
	return CL_Rect(translated, move_comp->get_geometry().get_size());
}

CL_GUIComponent *HolderComponent::get_toplevel_component()
{
	CL_GUIComponent *test = get_parent_component();
	while (true)
	{
		if (test->get_parent_component() == 0)
			break;
		test = test->get_parent_component();
	}

	return test;
}

CL_GUIComponent *HolderComponent::get_tab_or_frame_parent(CL_GUIComponent *comp)
{
	CL_GUIComponent *test = comp->get_parent_component();
	while (test != 0)
	{
		if (test->get_type_name() == "tabpage" || test->get_type_name() == "frame")
			return test;
		test = test->get_parent_component();
	}

	return 0;
}

std::vector<SnapLine> HolderComponent::get_snaplines() const
{
	CL_Size size = get_size();

	std::vector<SnapLine> snaplines;

	// Edges
	snaplines.push_back(SnapLine(SnapLine::Top, 0, SnapLine::Medium));
	snaplines.push_back(SnapLine(SnapLine::Bottom, size.height, SnapLine::Medium));
	snaplines.push_back(SnapLine(SnapLine::Left, 0, SnapLine::Medium));
	snaplines.push_back(SnapLine(SnapLine::Right, size.width, SnapLine::Medium));

	// Margins
	snaplines.push_back(SnapLine(SnapLine::Left, size.width + 5, SnapLine::Low));
	snaplines.push_back(SnapLine(SnapLine::Top, size.height + 5, SnapLine::Low));
	snaplines.push_back(SnapLine(SnapLine::Right, -5, SnapLine::Low));
	snaplines.push_back(SnapLine(SnapLine::Bottom, -5, SnapLine::Low));

	return snaplines;
}

HolderComponent *HolderComponent::find_holder_at(CL_GUIComponent *container, const CL_Point &pos)
{
	CL_GUIComponent *child = container->get_component_at(pos);
	if (child && child != container)
	{
		while (child && !dynamic_cast<HolderComponent*>(child))
			child = child->get_parent_component();
		return child ? dynamic_cast<HolderComponent*>(child) : 0;
	}
	else
	{
		return 0;
	}
}
