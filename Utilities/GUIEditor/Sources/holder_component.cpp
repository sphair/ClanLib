/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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

#include "precomp.h"
#include "holder_component.h"
#include "grid_component.h"
#include "custom_component.h"
#include "callback_info.h"

class HolderComponent_Impl
{
public:
	HolderComponent_Impl() : selected(false), delete_me(false), component(0), moving(false), resizing(false), parent_grid(0), anchor_tl(cl_anchor_top_left), anchor_br(cl_anchor_top_left)
	{
	}

	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);

	void on_resized();

	bool selected;
	bool delete_me;

	HolderComponent *component;

	void move_up();
	void move_down();
	void move_left();
	void move_right();

	void on_mouse_down_left(const CL_GUIMessage &msg, CL_InputEvent &event);
	void on_mouse_up_left(const CL_GUIMessage &msg, CL_InputEvent &event);
	void on_mouse_move(const CL_GUIMessage &msg, CL_InputEvent &event);

	void save_geometry(CL_DomElement &e, CL_GUIComponent *comp);
	void save_anchors(CL_DomElement &e, CL_GUIComponent *comp);
	void save_listview(CL_DomElement &e, CL_ListView *lv);

	CL_Point get_dist(CL_ComponentAnchorPoint ap, CL_Point p, CL_Rect boundary);
	CL_Rect convert_coordinates(CL_GUIComponent *from, CL_GUIComponent *to);
	CL_GUIComponent *get_toplevel_component();
	CL_Rect get_resize_rect();
	CL_GUIComponent *get_tab_or_frame_parent(CL_GUIComponent *comp);
	bool moving;
	bool resizing;
	CL_Point move_start_tl;
	CL_Point grab_pos;
	CL_Size original_size;
	CL_Font font;

	CL_ComponentAnchorPoint anchor_tl;
	CL_ComponentAnchorPoint anchor_br;
	GridComponent *parent_grid;

	CL_Point tmp_new_pos;

	CallbackInfo callback_info;
};

/////////////////////////////////////////////////////////////////////////////
// HolderComponent Construction:

HolderComponent::HolderComponent(CL_GUIComponent *parent)
: CL_GUIComponent(parent), impl(new HolderComponent_Impl())
{
	set_type_name(cl_text("holder"));
	func_process_message().set(this, &HolderComponent::on_process_message);
	func_render().set(impl.get(), &HolderComponent_Impl::on_render);
	func_resized().set(impl.get(), &HolderComponent_Impl::on_resized);

	impl->component = this;
	impl->font = CL_Font(get_gc(), "Tahoma", -11);

	impl->parent_grid = (GridComponent*)parent;
}

/////////////////////////////////////////////////////////////////////////////
// HolderComponent Attributes:

bool HolderComponent::marked_for_delete()
{
	return impl->delete_me;
}

CL_ComponentAnchorPoint HolderComponent::get_anchor_tl()
{
	return impl->anchor_tl;
}

CL_ComponentAnchorPoint HolderComponent::get_anchor_br()
{
	return impl->anchor_br;
}

/////////////////////////////////////////////////////////////////////////////
// HolderComponent Operations:

void HolderComponent::set_selected(bool value)
{
	impl->selected = value;
	invalidate_rect();
}

void HolderComponent::set_anchor_tl(CL_ComponentAnchorPoint ap)
{
	impl->anchor_tl = ap;
}

void HolderComponent::set_anchor_br(CL_ComponentAnchorPoint ap)
{
	impl->anchor_br = ap;
}

void HolderComponent::on_process_message(CL_GUIMessage &msg)
{
	if (msg.is_type(CL_GUIMessage_Input::get_type_name()))
	{
		CL_GUIMessage_Input input_msg = msg;
		CL_InputEvent e = input_msg.get_event();

		if (e.device.get_type() == CL_InputDevice::keyboard)
		{
			if (e.type == CL_InputEvent::pressed) 
			{
				if (e.id == CL_KEY_RETURN)
				{
				}
				if (e.id == CL_KEY_DELETE)
				{
					msg.set_consumed();
					impl->delete_me = true;
				}
				else if (e.id == CL_KEY_ESCAPE)
				{
				}
				else if (e.id == CL_KEY_UP)
				{
					msg.set_consumed();
					impl->move_up();
				}
				else if (e.id == CL_KEY_DOWN)
				{
					msg.set_consumed();
					impl->move_down();
				}
				else if (e.id == CL_KEY_RIGHT)
				{
					msg.set_consumed();
					impl->move_right();
				}
				else if (e.id == CL_KEY_LEFT)
				{
					msg.set_consumed();
					impl->move_left();
				}
			}
		}
		else if (e.device.get_type() == CL_InputDevice::pointer)
		{
			if (e.type == CL_InputEvent::pressed && e.id == CL_MOUSE_LEFT)
			{
				impl->on_mouse_down_left(msg, e);
				msg.set_consumed();
			}
			if (e.type == CL_InputEvent::released && e.id == CL_MOUSE_LEFT)
			{
				msg.set_consumed();
				impl->on_mouse_up_left(msg, e);
			}
			if (e.type == CL_InputEvent::pointer_moved)
			{
				msg.set_consumed();
				impl->on_mouse_move(msg, e);
			}
		}
	}
	else if (msg.is_type(CL_GUIMessage_Pointer::get_type_name()))
	{
		CL_GUIMessage_Pointer ptr_msg(msg);
		
		if (ptr_msg.get_pointer_type() == CL_GUIMessage_Pointer::pointer_enter)
		{
			msg.set_consumed();
//			set_cursor(cl_cursor_size_all);
		}
	}
}

CL_DomElement HolderComponent::to_element(CL_DomDocument &doc)
{
	CL_GUIComponent *comp = get_first_child();

	CL_StringRef type = comp->get_type_name();

	CL_DomElement e = doc.create_element(comp->get_type_name());
	e.set_attribute(cl_text("class"), comp->get_class_name());
	e.set_attribute(cl_text("id"), comp->get_id_name());

	if (type == cl_text("button"))
	{
		CL_PushButton *co = dynamic_cast<CL_PushButton*>(comp);
		e.set_attribute(cl_text("text"), co->get_text());
	}
	else if (comp->get_type_name() == cl_text("checkbox"))
	{
		CL_CheckBox *co = dynamic_cast<CL_CheckBox*>(comp);
		e.set_attribute(cl_text("text"), co->get_text());
	}
	else if (comp->get_type_name() == cl_text("radiobutton"))
	{
		CL_RadioButton *co = dynamic_cast<CL_RadioButton*>(comp);
		e.set_attribute(cl_text("text"), co->get_text());
		e.set_attribute(cl_text("group"), co->get_group_name());
	}
	else if (comp->get_type_name() == cl_text("label"))
	{
		CL_Label *co = dynamic_cast<CL_Label*>(comp);
		e.set_attribute(cl_text("text"), co->get_text());
	}
	else if (comp->get_type_name() == cl_text("lineedit"))
	{
		CL_LineEdit *co = dynamic_cast<CL_LineEdit*>(comp);
		e.set_attribute(cl_text("text"), co->get_text());
	}
	else if (comp->get_type_name() == cl_text("menubar"))
	{
		// CL_MenuBar *co = dynamic_cast<CL_MenuBar*>(comp);
	}
	else if (comp->get_type_name() == cl_text("statusbar"))
	{
		// CL_StatusBar *co = dynamic_cast<CL_StatusBar*>(comp);
	}
	else if (comp->get_type_name() == cl_text("toolbar"))
	{
		// CL_ToolBar *co = dynamic_cast<CL_ToolBar*>(comp);
	}
	else if (comp->get_type_name() == cl_text("imageview"))
	{
		// CL_ImageView *co = dynamic_cast<CL_ImageView*>(comp);
	}
	else if (comp->get_type_name() == cl_text("listview"))
	{
		CL_ListView *co = dynamic_cast<CL_ListView*>(comp);
		impl->save_listview(e, co);
	}
	else if (comp->get_type_name() == cl_text("slider"))
	{
		CL_Slider *co = dynamic_cast<CL_Slider*>(comp);
		e.set_attribute(cl_text("min"), CL_StringHelp::int_to_text(co->get_min()));
		e.set_attribute(cl_text("max"), CL_StringHelp::int_to_text(co->get_max()));
		e.set_attribute(cl_text("ticks"), CL_StringHelp::int_to_text(co->get_tick_count()));
		e.set_attribute(cl_text("page_step"), CL_StringHelp::int_to_text(co->get_page_step()));
	}
	else if (comp->get_type_name() == cl_text("tab"))
	{
		CL_Tab *co = dynamic_cast<CL_Tab*>(comp);

		CL_GUIComponent *child = co->get_first_child();
		while (child != 0)
		{
			if (child->get_type_name() == cl_text("tabpage"))
			{
				CL_TabPage *tab_page = dynamic_cast<CL_TabPage*>(child);
				CL_DomElement tabpage_element = doc.create_element(cl_text("tabpage"));
				tabpage_element.set_attribute(cl_text("label"), tab_page->get_label());

				CL_GUIComponent *tabpage_child = child->get_first_child();
				while (tabpage_child != 0)
				{
					if (tabpage_child->get_type_name() == cl_text("holder"))
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
	else if (comp->get_type_name() == cl_text("frame"))
	{
		CL_Frame *co = dynamic_cast<CL_Frame*>(comp);
		e.set_attribute(cl_text("text"), co->get_header_text());

		CL_GUIComponent *child = co->get_first_child();
		while (child != 0)
		{
			if (child->get_type_name() == cl_text("holder"))
			{
				HolderComponent *holder_comp = dynamic_cast<HolderComponent*>(child);
				CL_DomElement frame_child_element = holder_comp->to_element(doc);
				e.append_child(frame_child_element);
			}

			child = child->get_next_sibling();
		}

	}
	else if (comp->get_type_name() == cl_text("spin"))
	{
		// CL_Spin *co = dynamic_cast<CL_Spin*>(comp);
	}
	else if (comp->get_type_name() == cl_text("combobox"))
	{
		// CL_ComboBox *co = dynamic_cast<CL_ComboBox*>(comp);
	}
	else // custom component
	{
		// CustomComponent *co = dynamic_cast<CustomComponent*>(comp);
	}

	impl->save_anchors(e,comp);
	impl->save_geometry(e,comp); // write geom="..." attribute

	return e;
}

CallbackInfo HolderComponent::get_callback_info()
{
	return impl->callback_info; 
}

/////////////////////////////////////////////////////////////////////////////
// HolderComponent Implementation:

void HolderComponent_Impl::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_Rect geom = component->get_geometry().get_size();

	CL_Rect child_geom = component->get_first_child()->get_geometry();
	child_geom.expand(CL_Rect(2,2,1,1)); 

	if (selected)
	{
		CL_Draw::box(gc, child_geom, CL_Colorf::blue);

		CL_Rect fill_rect_br1(geom.right-4, geom.bottom-4, geom.right-1, geom.bottom-1);
		CL_Draw::fill(gc, fill_rect_br1, CL_Colorf::white);

		CL_Rect fill_rect_br2(geom.right-5, geom.bottom-5, geom.right-1, geom.bottom-1);
		CL_Draw::box(gc, fill_rect_br2, CL_Colorf::blue);
	}

	// Do a fillrect for otherwise transparent components:
	CL_TempString type = component->get_first_child()->get_type_name();
	if (type == cl_text("toolbar") || type == cl_text("menubar") || type == cl_text("label"))
	{
		CL_Rect child_geom = component->get_first_child()->get_geometry();
		CL_Draw::fill(component->get_gc(), child_geom, CL_Colorf(0.35f, 0.498f, 0.603f, 0.2f));
	}

	/*  DON'T DELETE THIS COMMENTED CODE!
	CL_Point tl = component->get_geometry().get_top_left();
	CL_Point tl_win = component->component_to_window_coords(tl);
	gc.set_font(font);
	gc.draw_text(20,24, cl_format("%1, %2", tl.x, tl.y), CL_Colorf::red);
	gc.draw_text(60,24, cl_format("%1, %2", tl_win.x, tl_win.y), CL_Colorf::blue);
	//gc.draw_text(120,24, cl_format("%1, %2", tmp_new_pos.x, tmp_new_pos.y), CL_Colorf::orange);

	if (moving)
		gc.draw_text(120,24, "moving", CL_Colorf::orange);
	*/
 }

void HolderComponent_Impl::on_resized()
{
}

CL_Rect HolderComponent_Impl::get_resize_rect()
{
	CL_Rect g = component->get_geometry();
	CL_Point br = component->component_to_window_coords(CL_Rect(g.get_size()).get_bottom_right());
	return CL_Rect(br.x-4, br.y-4, br.x+2, br.y+2);
}

void HolderComponent_Impl::on_mouse_down_left(const CL_GUIMessage &msg, CL_InputEvent &event)
{
	component->capture_mouse(true);

	CL_Rect g = component->get_geometry();
	move_start_tl = g.get_top_left();
	original_size = g.get_size();

	CL_Rect resize_rect = get_resize_rect();

	if (resize_rect.contains(event.mouse_pos))
	{
		resizing = true;
		grab_pos = event.mouse_pos;
//		component->set_cursor(cl_cursor_size_nwse);
	}
	else
	{
		moving = true;
		grab_pos = event.mouse_pos;
//		component->set_cursor(cl_cursor_size_all);
	}
}

void HolderComponent_Impl::on_mouse_up_left(const CL_GUIMessage &msg, CL_InputEvent &event)
{
	moving = false;
	resizing = false;
	component->capture_mouse(false);
	component->invalidate_rect();
}

void HolderComponent_Impl::on_mouse_move(const CL_GUIMessage &msg, CL_InputEvent &event)
{
	CL_Point mouse_pos = event.mouse_pos;
	CL_Point move_offset;
	move_offset.x = ((mouse_pos.x - grab_pos.x) / 8) * 8;
	move_offset.y = ((mouse_pos.y - grab_pos.y) / 8) * 8;

/*	if (get_resize_rect().contains(mouse_pos))
		component->set_cursor(cl_cursor_size_nwse);
	else
		component->set_cursor(cl_cursor_size_all);*/

	if (moving)
	{
		CL_Point new_pos = move_start_tl + move_offset;
		component->set_geometry(CL_Rect(new_pos, original_size));

		CL_Point new_pos_window_coords = component->component_to_window_coords(CL_Point(-1,-1)); // point must be outside so get_component_at wont return the component currently being moved.
		tmp_new_pos = new_pos_window_coords;
		CL_GUIComponent *new_parent = get_toplevel_component()->get_component_at(new_pos_window_coords);

		if (new_parent == 0)
			return;

		if (new_parent->get_type_name()== cl_text("holder"))
			new_parent = new_parent->get_first_child();

		if (new_parent != 0 &&
			((new_parent->get_type_name() == cl_text("frame")) ||
			 (new_parent->get_type_name() == cl_text("tabpage")) ||
			 (new_parent->get_type_name() == cl_text("grid"))))
		{
			if (new_parent != component->get_parent_component() &&
				new_parent != component && 
				new_parent != component->get_first_child())
			{
				if (GridComponent::is_grid_or_child_of_grid(new_parent))
				{
					// re-parent and set new coordinates
					CL_Rect new_rect_translated = convert_coordinates(component, new_parent);
					move_start_tl = new_rect_translated.get_top_left();
					move_offset = CL_Point(0,0);
					grab_pos = event.mouse_pos;
					component->set_parent_component(new_parent);
					component->set_geometry(new_rect_translated);
				}
			}
		}
//		component->set_cursor(cl_cursor_size_all);
	}
	else if (resizing)
	{
		CL_Rect old_g = component->get_geometry();
		CL_Point old_pos(old_g.left, old_g.top);

		CL_Size new_size;
		new_size.width = original_size.width + move_offset.x;
		new_size.height = original_size.height + move_offset.y;

		component->set_geometry(CL_Rect(old_pos, new_size));
		component->get_first_child()->set_geometry(CL_Rect(CL_Point(4,4), new_size - CL_Size(8,8)));
//		component->set_cursor(cl_cursor_size_nwse);
	}
}

void HolderComponent_Impl::move_left()
{
}

void HolderComponent_Impl::move_right()
{
}

void HolderComponent_Impl::move_up()
{
}

void HolderComponent_Impl::move_down()
{
}

void HolderComponent_Impl::save_geometry(CL_DomElement &e, CL_GUIComponent *comp)
{
	CL_Rect holder_g = comp->get_parent_component()->get_geometry();
	CL_Rect g = component->get_geometry();
	g.shrink(CL_Rect(4,4,4,4));

	CL_TempString left = CL_StringHelp::int_to_text(g.left);
	CL_TempString right = CL_StringHelp::int_to_text(g.right);
	CL_TempString top = CL_StringHelp::int_to_text(g.top);
	CL_TempString bottom = CL_StringHelp::int_to_text(g.bottom);

	e.set_attribute(cl_text("geom"), left + cl_text(",") + top + cl_text(",") + right + cl_text(",") + bottom);
}

void HolderComponent_Impl::save_anchors(CL_DomElement &e, CL_GUIComponent *comp)
{
	CL_Rect boundary = parent_grid->get_boundary();

	CL_GUIComponent *tab_or_frame_parent = get_tab_or_frame_parent(comp);
	if (tab_or_frame_parent)
	{
		boundary = tab_or_frame_parent->get_geometry().get_size();
	}

	CL_Rect g = component->get_geometry();
	g.shrink(CL_Rect(4,4,4,4));

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

CL_Point HolderComponent_Impl::get_dist(CL_ComponentAnchorPoint ap, CL_Point p, CL_Rect boundary)
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

void HolderComponent_Impl::save_listview(CL_DomElement &e, CL_ListView *lv)
{
	CL_ListViewHeader *header = lv->get_header();
	
	CL_DomDocument doc = e.get_owner_document(); 
	CL_DomElement e_header = doc.create_element(cl_text("listview_header"));
	e.append_child(e_header);

	CL_ListViewColumnHeader col = header->get_first_column();

	while (!col.is_null())
	{
		CL_DomElement e_col = doc.create_element(cl_text("listview_column"));
		e_col.set_attribute(cl_text("col_id"), col.get_column_id());
		e_col.set_attribute(cl_text("caption"), col.get_caption());
		e_col.set_attribute(cl_text("width"), CL_StringHelp::int_to_text(col.get_width()));

		col = col.get_next_sibling();
	}
}

CL_Rect HolderComponent_Impl::convert_coordinates(CL_GUIComponent *move_comp, CL_GUIComponent *new_parent)
{
	CL_Point from_tl = move_comp->component_to_window_coords(CL_Point(0,0));
	CL_Point to_tl = new_parent->component_to_window_coords(CL_Point(0,0));
	CL_Point translated = from_tl - to_tl;
	return CL_Rect(translated, move_comp->get_geometry().get_size());
}


CL_GUIComponent *HolderComponent_Impl::get_toplevel_component()
{
	CL_GUIComponent *test = component->get_parent_component();
	while (true)
	{
		if (test->get_parent_component() == 0)
			break;
		test = test->get_parent_component();
	}

	return test;
}


CL_GUIComponent *HolderComponent_Impl::get_tab_or_frame_parent(CL_GUIComponent *comp)
{
	CL_GUIComponent *test = comp->get_parent_component();
	while (test != 0)
	{
		if (test->get_type_name() == cl_text("tabpage") || test->get_type_name() == cl_text("frame"))
			return test;
		test = test->get_parent_component();
	}

	return 0;
}
