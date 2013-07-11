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
**    Kenneth Gangstoe
*/

#include "precomp.h"
#include "grid_object.h"
#include "grid_component.h"
#include "ComponentTypes/component_types.h"
#include "ComponentTypes/component_type.h"

GridObject::GridObject(GridComponent *grid_component, GUIComponent *parent, int id, const Vec2i &pos)
: GUIComponent(parent), grid_component(grid_component), anchor_tl(cl_anchor_top_left), anchor_br(cl_anchor_top_left), component_type(0)
{
	set_tag_name("object");
	func_render().set(this, &GridObject::on_render);
	func_resized().set(this, &GridObject::on_resized);

	component_type = ComponentTypes::find_component(id);
	GUIComponent *new_component = component_type->create_component(get_container());
	set_geometry(Rect(pos, new_component->get_size()));
}

GUIComponent *GridObject::get_container()
{
	return this;
}

ComponentAnchorPoint GridObject::get_anchor_tl()
{
	return anchor_tl;
}

ComponentAnchorPoint GridObject::get_anchor_br()
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

void GridObject::set_anchor_tl(ComponentAnchorPoint ap)
{
	anchor_tl = ap;
}

void GridObject::set_anchor_br(ComponentAnchorPoint ap)
{
	anchor_br = ap;
}

DomElement GridObject::to_element(DomDocument &doc)
{
	GUIComponent *comp = get_first_child();

	std::string type = comp->get_tag_name();

	DomElement e = doc.create_element(comp->get_tag_name());
	std::string class_name;
	comp->get_class(class_name);
	e.set_attribute("class", class_name);
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
		PushButton *co = dynamic_cast<PushButton*>(comp);
		e.set_attribute("text", co->get_text());
	}
	else if (comp->get_tag_name() == "checkbox")
	{
		CheckBox *co = dynamic_cast<CheckBox*>(comp);
		e.set_attribute("text", co->get_text());
	}
	else if (comp->get_tag_name() == "radiobutton")
	{
		RadioButton *co = dynamic_cast<RadioButton*>(comp);
		e.set_attribute("text", co->get_text());
		e.set_attribute("group", co->get_group_name());
	}
	else if (comp->get_tag_name() == "label")
	{
		Label *co = dynamic_cast<Label*>(comp);
		e.set_attribute("text", co->get_text());
	}
	else if (comp->get_tag_name() == "lineedit")
	{
		LineEdit *co = dynamic_cast<LineEdit*>(comp);
		e.set_attribute("text", co->get_text());
	}
	else if (comp->get_tag_name() == "textedit")
	{
		TextEdit *co = dynamic_cast<TextEdit*>(comp);
		e.set_attribute("text", co->get_text());
	}
	else if (comp->get_tag_name() == "menubar")
	{
		// MenuBar *co = dynamic_cast<MenuBar*>(comp);
	}
	else if (comp->get_tag_name() == "statusbar")
	{
		// StatusBar *co = dynamic_cast<StatusBar*>(comp);
	}
	else if (comp->get_tag_name() == "toolbar")
	{
		// ToolBar *co = dynamic_cast<ToolBar*>(comp);
	}
	else if (comp->get_tag_name() == "imageview")
	{
		// ImageView *co = dynamic_cast<ImageView*>(comp);
	}
	else if (comp->get_tag_name() == "listview")
	{
		ListView *co = dynamic_cast<ListView*>(comp);
		save_listview(e, co);
	}
	else if (comp->get_tag_name() == "slider")
	{
		Slider *co = dynamic_cast<Slider*>(comp);
		e.set_attribute("min", StringHelp::int_to_text(co->get_min()));
		e.set_attribute("max", StringHelp::int_to_text(co->get_max()));
		e.set_attribute("ticks", StringHelp::int_to_text(co->get_tick_count()));
		e.set_attribute("page_step", StringHelp::int_to_text(co->get_page_step()));
	}
	else if (comp->get_tag_name() == "tab")
	{
		Tab *co = dynamic_cast<Tab*>(comp);

		GUIComponent *child = co->get_first_child();
		while (child != 0)
		{
			if (child->get_tag_name() == "tabpage")
			{
				TabPage *tab_page = dynamic_cast<TabPage*>(child);
				DomElement tabpage_element = doc.create_element("tabpage");
				tabpage_element.set_attribute("label", tab_page->get_label());

				GUIComponent *tabpage_child = child->get_first_child();
				while (tabpage_child != 0)
				{
					if (tabpage_child->get_tag_name() == "object")
					{
						GridObject *object_comp = dynamic_cast<GridObject*>(tabpage_child);
						DomElement tabpage_child_element = object_comp->to_element(doc);
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
		Frame *co = dynamic_cast<Frame*>(comp);
		e.set_attribute("text", co->get_header_text());

		GUIComponent *child = co->get_first_child();
		while (child != 0)
		{
			if (child->get_tag_name() == "object")
			{
				GridObject *object_comp = dynamic_cast<GridObject*>(child);
				DomElement frame_child_element = object_comp->to_element(doc);
				e.append_child(frame_child_element);
			}

			child = child->get_next_sibling();
		}

	}
	else if (comp->get_tag_name() == "spin")
	{
		// Spin *co = dynamic_cast<Spin*>(comp);
	}
	else if (comp->get_tag_name() == "combobox")
	{
		// ComboBox *co = dynamic_cast<ComboBox*>(comp);
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

Rect GridObject::get_grabber_w() const
{
	Rect box = get_size();
	return Rect(Point(box.left-7, (box.top+box.bottom)/2-3), Size(6, 6));
}

Rect GridObject::get_grabber_nw() const
{
	Rect box = get_size();
	return Rect(Point(box.left-7, box.top-7), Size(6, 6));
}

Rect GridObject::get_grabber_n() const
{
	Rect box = get_size();
	return Rect(Point((box.left+box.right)/2-3, box.top-7), Size(6, 6));
}

Rect GridObject::get_grabber_ne() const
{
	Rect box = get_size();
	return Rect(Point(box.right, box.top-7), Size(6, 6));
}

Rect GridObject::get_grabber_e() const
{
	Rect box = get_size();
	return Rect(Point(box.right, (box.top+box.bottom)/2-3), Size(6, 6));
}

Rect GridObject::get_grabber_se() const
{
	Rect box = get_size();
	return Rect(Point(box.right, box.bottom), Size(6, 6));
}

Rect GridObject::get_grabber_s() const
{
	Rect box = get_size();
	return Rect(Point((box.left+box.right)/2-3, box.bottom), Size(6, 6));
}

Rect GridObject::get_grabber_sw() const
{
	Rect box = get_size();
	return Rect(Point(box.left-7, box.bottom), Size(6, 6));
}

void GridObject::on_render(Canvas &canvas, const Rect &update_rect)
{
	// Do a fill rect for otherwise transparent components:
	std::string type = get_first_child()->get_tag_name();
	if (type == "toolbar" || type == "menubar" /*|| type == "label"*/)
	{
		Rect child_geom = get_first_child()->get_geometry();
		canvas.fill_rect(child_geom, Colorf(0.35f, 0.498f, 0.603f, 0.2f));
	}

	/*  DON'T DELETE THIS COMMENTED CODE!
	Point tl = get_geometry().get_top_left();
	Point tl_win = component_to_window_coords(tl);
	canvas.set_font(font);
	canvas.draw_text(20,24, string_format("%1, %2", tl.x, tl.y), Colorf::red);
	canvas.draw_text(60,24, string_format("%1, %2", tl_win.x, tl_win.y), Colorf::blue);
	*/
 }

void GridObject::on_resized()
{
	GUIComponent *child = get_first_child();
	if (child)
		child->set_geometry(get_size());
}

void GridObject::save_geometry(DomElement &e, GUIComponent *comp)
{
	Rect object_g = comp->get_parent_component()->get_geometry();
	Rect g = get_geometry();

	std::string left = StringHelp::int_to_text(g.left);
	std::string right = StringHelp::int_to_text(g.right);
	std::string top = StringHelp::int_to_text(g.top);
	std::string bottom = StringHelp::int_to_text(g.bottom);

	e.set_attribute("geom", left + "," + top + "," + right + "," + bottom);
}

void GridObject::save_anchors(DomElement &e, GUIComponent *comp)
{
	Rect boundary = grid_component->get_dialog_size();

	GUIComponent *tab_or_frame_parent = get_tab_or_frame_parent(comp);
	if (tab_or_frame_parent)
	{
		boundary = tab_or_frame_parent->get_geometry().get_size();
	}

	Rect g = get_geometry();

	Point tl = g.get_top_left();
	Point br = g.get_bottom_right();

	Point dist_tl = get_dist(anchor_tl, tl, boundary);
	Point dist_br = get_dist(anchor_br, br, boundary);

	e.set_attribute("anchor_tl", StringHelp::int_to_text(anchor_tl));
	e.set_attribute("anchor_br", StringHelp::int_to_text(anchor_br));
	e.set_attribute("dist_tl_x", StringHelp::int_to_text(dist_tl.x));
	e.set_attribute("dist_tl_y", StringHelp::int_to_text(dist_tl.y));
	e.set_attribute("dist_br_x", StringHelp::int_to_text(dist_br.x));
	e.set_attribute("dist_br_y", StringHelp::int_to_text(dist_br.y));
}

Point GridObject::get_dist(ComponentAnchorPoint ap, Point p, Rect boundary)
{
	int bw = boundary.get_width(), bh = boundary.get_height(); 
	if (ap == cl_anchor_top_left)
	{
		return Point(p.x, p.y);
	}
	else if (ap == cl_anchor_top_right)
	{
		return Point(bw-p.x, p.y);
	}
	else if (ap == cl_anchor_bottom_left)
	{
		return Point(p.x, bh-p.y);
	}
	else if (ap == cl_anchor_bottom_right)
	{
		return Point(bw-p.x, bh-p.y);
	}

	return Point(0,0);
}

void GridObject::save_listview(DomElement &e, ListView *lv)
{
	ListViewHeader *header = lv->get_header();
	
	DomDocument doc = e.get_owner_document(); 
	DomElement e_header = doc.create_element("listview_header");
	e.append_child(e_header);

	ListViewColumnHeader col = header->get_first_column();

	while (!col.is_null())
	{
		DomElement e_col = doc.create_element("listview_column");
		e_col.set_attribute("col_id", col.get_column_id());
		e_col.set_attribute("caption", col.get_caption());
		e_col.set_attribute("width", StringHelp::int_to_text(col.get_width()));

		col = col.get_next_sibling();

		e_header.append_child(e_col);
	}
}

Rect GridObject::convert_coordinates(GUIComponent *move_comp, GUIComponent *new_parent)
{
	Point from_tl = move_comp->component_to_window_coords(Point(0,0));
	Point to_tl = new_parent->component_to_window_coords(Point(0,0));
	Point translated = from_tl - to_tl;
	return Rect(translated, move_comp->get_geometry().get_size());
}

GUIComponent *GridObject::get_toplevel_component()
{
	GUIComponent *test = get_parent_component();
	while (true)
	{
		if (test->get_parent_component() == 0)
			break;
		test = test->get_parent_component();
	}

	return test;
}

GUIComponent *GridObject::get_tab_or_frame_parent(GUIComponent *comp)
{
	GUIComponent *test = comp->get_parent_component();
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
	Size size = get_size();

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

GridObject *GridObject::find_object_at(GUIComponent *container, const Point &pos)
{
	GUIComponent *child = container->get_component_at(pos);
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
