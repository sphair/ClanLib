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

#include "precomp.h"
#include "grid_object.h"
#include "grid_component.h"
#include "ComponentTypes/component_types.h"
#include "ComponentTypes/component_type.h"

GridObject::GridObject(GridComponent *grid_component, clan::GUIComponent *parent, int id, const clan::Vec2i &pos)
: clan::GUIComponent(parent), grid_component(grid_component), anchor_tl(clan::anchor_top_left), anchor_br(clan::anchor_top_left), component_type(0)
{
	set_tag_name("object");
	func_render().set(this, &GridObject::on_render);
	func_resized().set(this, &GridObject::on_resized);

	component_type = ComponentTypes::find_component(id);
	clan::GUIComponent *new_component = component_type->create_component(get_container());
	set_geometry(clan::Rect(pos, new_component->get_size()));
}

clan::GUIComponent *GridObject::get_container()
{
	return this;
}

clan::ComponentAnchorPoint GridObject::get_anchor_tl()
{
	return anchor_tl;
}

clan::ComponentAnchorPoint GridObject::get_anchor_br()
{
	return anchor_br;
}

std::string GridObject::get_position_equation_x() const
{
	return pos_equation_x;
}

std::string GridObject::get_position_equation_y() const
{
	return pos_equation_y;
}

std::string GridObject::get_position_equation_x2() const
{
	return pos_equation_x2;
}

std::string GridObject::get_position_equation_y2() const
{
	return pos_equation_y2;
}

void GridObject::set_anchor_tl(clan::ComponentAnchorPoint ap)
{
	anchor_tl = ap;
}

void GridObject::set_anchor_br(clan::ComponentAnchorPoint ap)
{
	anchor_br = ap;
}

clan::DomElement GridObject::to_element(clan::DomDocument &doc)
{
	clan::GUIComponent *comp = get_first_child();

	std::string type = comp->get_tag_name();

	clan::DomElement e = doc.create_element(comp->get_tag_name());
	std::string comp_class;
	comp->get_class(comp_class);

	e.set_attribute("class", comp_class);
	e.set_attribute("id", comp->get_id());
	e.set_attribute_bool("enabled", comp->is_enabled());

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
		clan::PushButton *co = dynamic_cast<clan::PushButton*>(comp);
		e.set_attribute("text", co->get_text());
	}
	else if (comp->get_tag_name() == "checkbox")
	{
		clan::CheckBox *co = dynamic_cast<clan::CheckBox*>(comp);
		e.set_attribute("text", co->get_text());
	}
	else if (comp->get_tag_name() == "radiobutton")
	{
		clan::RadioButton *co = dynamic_cast<clan::RadioButton*>(comp);
		e.set_attribute("text", co->get_text());
		e.set_attribute("group", co->get_group_name());
	}
	else if (comp->get_tag_name() == "label")
	{
		clan::Label *co = dynamic_cast<clan::Label*>(comp);
		e.set_attribute("text", co->get_text());
	}
	else if (comp->get_tag_name() == "lineedit")
	{
		clan::LineEdit *co = dynamic_cast<clan::LineEdit*>(comp);
		e.set_attribute("text", co->get_text());
	}
	else if (comp->get_tag_name() == "textedit")
	{
		clan::TextEdit *co = dynamic_cast<clan::TextEdit*>(comp);
		e.set_attribute("text", co->get_text());
	}
	else if (comp->get_tag_name() == "menubar")
	{
		// clan::MenuBar *co = dynamic_cast<clan::MenuBar*>(comp);
	}
	else if (comp->get_tag_name() == "statusbar")
	{
		// clan::StatusBar *co = dynamic_cast<clan::StatusBar*>(comp);
	}
	else if (comp->get_tag_name() == "toolbar")
	{
		// clan::ToolBar *co = dynamic_cast<clan::ToolBar*>(comp);
	}
	else if (comp->get_tag_name() == "imageview")
	{
		// clan::ImageView *co = dynamic_cast<clan::ImageView*>(comp);
	}
	else if (comp->get_tag_name() == "listview")
	{
		clan::ListView *co = dynamic_cast<clan::ListView*>(comp);
		save_listview(e, co);
	}
	else if (comp->get_tag_name() == "slider")
	{
		clan::Slider *co = dynamic_cast<clan::Slider*>(comp);
		e.set_attribute("min", clan::StringHelp::int_to_text(co->get_min()));
		e.set_attribute("max", clan::StringHelp::int_to_text(co->get_max()));
		e.set_attribute("ticks", clan::StringHelp::int_to_text(co->get_tick_count()));
		e.set_attribute("page_step", clan::StringHelp::int_to_text(co->get_page_step()));
	}
	else if (comp->get_tag_name() == "tab")
	{
		clan::Tab *co = dynamic_cast<clan::Tab*>(comp);

		clan::GUIComponent *child = co->get_first_child();
		while (child != 0)
		{
			if (child->get_tag_name() == "tabpage")
			{
				clan::TabPage *tab_page = dynamic_cast<clan::TabPage*>(child);
				clan::DomElement tabpage_element = doc.create_element("tabpage");
				tabpage_element.set_attribute("label", tab_page->get_label());

				clan::GUIComponent *tabpage_child = child->get_first_child();
				while (tabpage_child != 0)
				{
					if (tabpage_child->get_tag_name() == "object")
					{
						GridObject *object_comp = dynamic_cast<GridObject*>(tabpage_child);
						clan::DomElement tabpage_child_element = object_comp->to_element(doc);
						tabpage_element.append_child(tabpage_child_element);
					}

					tabpage_child = tabpage_child->get_next_sibling();
				}

				e.append_child(tabpage_element);
			}

			child = child->get_next_sibling();
		}
		
	}
	else if (comp->get_tag_name() == "frame")
	{
		clan::Frame *co = dynamic_cast<clan::Frame*>(comp);
		e.set_attribute("text", co->get_header_text());

		clan::GUIComponent *child = co->get_first_child();
		while (child != 0)
		{
			if (child->get_tag_name() == "object")
			{
				GridObject *object_comp = dynamic_cast<GridObject*>(child);
				clan::DomElement frame_child_element = object_comp->to_element(doc);
				e.append_child(frame_child_element);
			}

			child = child->get_next_sibling();
		}

	}
	else if (comp->get_tag_name() == "spin")
	{
		// clan::Spin *co = dynamic_cast<clan::Spin*>(comp);
	}
	else if (comp->get_tag_name() == "combobox")
	{
		// clan::ComboBox *co = dynamic_cast<clan::ComboBox*>(comp);
	}
	else // custom component
	{
		// CustomComponent *co = dynamic_cast<CustomComponent*>(comp);
	}

	save_anchors(e,comp);
	save_geometry(e,comp); // write geom="..." attribute

	return e;
}

void GridObject::set_position_equations(const std::string &str_x, const std::string &str_y)
{
	pos_equation_x = str_x;
	pos_equation_y = str_y;
}

void GridObject::set_position_equations2( const std::string &str_x, const std::string &str_y )
{
	pos_equation_x2 = str_x;
	pos_equation_y2 = str_y;
}

clan::Rect GridObject::get_grabber_w() const
{
	clan::Rect box = get_size();
	return clan::Rect(clan::Point(box.left-7, (box.top+box.bottom)/2-3), clan::Size(6, 6));
}

clan::Rect GridObject::get_grabber_nw() const
{
	clan::Rect box = get_size();
	return clan::Rect(clan::Point(box.left-7, box.top-7), clan::Size(6, 6));
}

clan::Rect GridObject::get_grabber_n() const
{
	clan::Rect box = get_size();
	return clan::Rect(clan::Point((box.left+box.right)/2-3, box.top-7), clan::Size(6, 6));
}

clan::Rect GridObject::get_grabber_ne() const
{
	clan::Rect box = get_size();
	return clan::Rect(clan::Point(box.right, box.top-7), clan::Size(6, 6));
}

clan::Rect GridObject::get_grabber_e() const
{
	clan::Rect box = get_size();
	return clan::Rect(clan::Point(box.right, (box.top+box.bottom)/2-3), clan::Size(6, 6));
}

clan::Rect GridObject::get_grabber_se() const
{
	clan::Rect box = get_size();
	return clan::Rect(clan::Point(box.right, box.bottom), clan::Size(6, 6));
}

clan::Rect GridObject::get_grabber_s() const
{
	clan::Rect box = get_size();
	return clan::Rect(clan::Point((box.left+box.right)/2-3, box.bottom), clan::Size(6, 6));
}

clan::Rect GridObject::get_grabber_sw() const
{
	clan::Rect box = get_size();
	return clan::Rect(clan::Point(box.left-7, box.bottom), clan::Size(6, 6));
}

void GridObject::on_render(clan::Canvas &canvas, const clan::Rect &update_rect)
{
	// Do a fill rect for otherwise transparent components:
	std::string type = get_first_child()->get_tag_name();
	if (type == "toolbar" || type == "menubar" /*|| type == "label"*/)
	{
		clan::Rect child_geom = get_first_child()->get_geometry();
		get_canvas().fill_rect(child_geom, clan::Colorf(0.35f, 0.498f, 0.603f, 0.2f));
	}

	/*  DON'T DELETE THIS COMMENTED CODE!
	clan::Point tl = get_geometry().get_top_left();
	clan::Point tl_win = component_to_window_coords(tl);
	canvas.set_font(font);
	canvas.draw_text(20,24, clan::string_format("%1, %2", tl.x, tl.y), clan::Colorf::red);
	canvas.draw_text(60,24, clan::string_format("%1, %2", tl_win.x, tl_win.y), clan::Colorf::blue);
	*/
 }

void GridObject::on_resized()
{
	clan::GUIComponent *child = get_first_child();
	if (child)
		child->set_geometry(get_size());
}

void GridObject::save_geometry(clan::DomElement &e, clan::GUIComponent *comp)
{
	clan::Rect object_g = comp->get_parent_component()->get_geometry();
	clan::Rect g = get_geometry();

	std::string left = clan::StringHelp::int_to_text(g.left);
	std::string right = clan::StringHelp::int_to_text(g.right);
	std::string top = clan::StringHelp::int_to_text(g.top);
	std::string bottom = clan::StringHelp::int_to_text(g.bottom);

	e.set_attribute("geom", left + "," + top + "," + right + "," + bottom);
}

void GridObject::save_anchors(clan::DomElement &e, clan::GUIComponent *comp)
{
	clan::Rect boundary = grid_component->get_dialog_size();

	clan::GUIComponent *tab_or_frame_parent = get_tab_or_frame_parent(comp);
	if (tab_or_frame_parent)
	{
		boundary = tab_or_frame_parent->get_geometry().get_size();
	}

	clan::Rect g = get_geometry();

	clan::Point tl = g.get_top_left();
	clan::Point br = g.get_bottom_right();

	clan::Point dist_tl = get_dist(anchor_tl, tl, boundary);
	clan::Point dist_br = get_dist(anchor_br, br, boundary);

	e.set_attribute("anchor_tl", clan::StringHelp::int_to_text(anchor_tl));
	e.set_attribute("anchor_br", clan::StringHelp::int_to_text(anchor_br));
	e.set_attribute("dist_tl_x", clan::StringHelp::int_to_text(dist_tl.x));
	e.set_attribute("dist_tl_y", clan::StringHelp::int_to_text(dist_tl.y));
	e.set_attribute("dist_br_x", clan::StringHelp::int_to_text(dist_br.x));
	e.set_attribute("dist_br_y", clan::StringHelp::int_to_text(dist_br.y));
}

clan::Point GridObject::get_dist(clan::ComponentAnchorPoint ap, clan::Point p, clan::Rect boundary)
{
	int bw = boundary.get_width(), bh = boundary.get_height(); 
	if (ap == clan::anchor_top_left)
	{
		return clan::Point(p.x, p.y);
	}
	else if (ap == clan::anchor_top_right)
	{
		return clan::Point(bw-p.x, p.y);
	}
	else if (ap == clan::anchor_bottom_left)
	{
		return clan::Point(p.x, bh-p.y);
	}
	else if (ap == clan::anchor_bottom_right)
	{
		return clan::Point(bw-p.x, bh-p.y);
	}

	return clan::Point(0,0);
}

void GridObject::save_listview(clan::DomElement &e, clan::ListView *lv)
{
	clan::ListViewHeader *header = lv->get_header();
	
	clan::DomDocument doc = e.get_owner_document(); 
	clan::DomElement e_header = doc.create_element("listview_header");
	e.append_child(e_header);

	clan::ListViewColumnHeader col = header->get_first_column();

	while (!col.is_null())
	{
		clan::DomElement e_col = doc.create_element("listview_column");
		e_col.set_attribute("col_id", col.get_column_id());
		e_col.set_attribute("caption", col.get_caption());
		e_col.set_attribute("width", clan::StringHelp::int_to_text(col.get_width()));

		col = col.get_next_sibling();

		e_header.append_child(e_col);
	}
}

clan::Rect GridObject::convert_coordinates(clan::GUIComponent *move_comp, clan::GUIComponent *new_parent)
{
	clan::Point from_tl = move_comp->component_to_window_coords(clan::Point(0,0));
	clan::Point to_tl = new_parent->component_to_window_coords(clan::Point(0,0));
	clan::Point translated = from_tl - to_tl;
	return clan::Rect(translated, move_comp->get_geometry().get_size());
}

clan::GUIComponent *GridObject::get_toplevel_component()
{
	clan::GUIComponent *test = get_parent_component();
	while (true)
	{
		if (test->get_parent_component() == 0)
			break;
		test = test->get_parent_component();
	}

	return test;
}

clan::GUIComponent *GridObject::get_tab_or_frame_parent(clan::GUIComponent *comp)
{
	clan::GUIComponent *test = comp->get_parent_component();
	while (test != 0)
	{
		if (test->get_tag_name() == "tabpage" || test->get_tag_name() == "frame")
			return test;
		test = test->get_parent_component();
	}

	return 0;
}

std::vector<SnapLine> GridObject::get_snaplines() const
{
	clan::Size size = get_size();

	std::vector<SnapLine> snaplines;

	// Edges
	snaplines.push_back(SnapLine(SnapLine::type_top, 0, SnapLine::priority_medium));
	snaplines.push_back(SnapLine(SnapLine::type_bottom, size.height, SnapLine::priority_medium));
	snaplines.push_back(SnapLine(SnapLine::type_left, 0, SnapLine::priority_medium));
	snaplines.push_back(SnapLine(SnapLine::type_right, size.width, SnapLine::priority_medium));

	// Margins
	snaplines.push_back(SnapLine(SnapLine::type_left, size.width + 5, SnapLine::priority_low));
	snaplines.push_back(SnapLine(SnapLine::type_top, size.height + 5, SnapLine::priority_low));
	snaplines.push_back(SnapLine(SnapLine::type_right, -5, SnapLine::priority_low));
	snaplines.push_back(SnapLine(SnapLine::type_bottom, -5, SnapLine::priority_low));

	return snaplines;
}

GridObject *GridObject::find_object_at(clan::GUIComponent *container, const clan::Point &pos)
{
	clan::GUIComponent *child = container->get_component_at(pos);
	if (child && child != container)
	{
		while (child && !dynamic_cast<GridObject*>(child))
			child = child->get_parent_component();
		return child ? dynamic_cast<GridObject*>(child) : 0;
	}
	else
	{
		return 0;
	}
}
