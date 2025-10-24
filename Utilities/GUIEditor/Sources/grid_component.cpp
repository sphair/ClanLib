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
#include "grid_component.h"
#include "custom_component.h"
#include "holder_component.h"
#include "component_type.h"
#include "main_window.h"
#include "selection.h"

/////////////////////////////////////////////////////////////////////////////
// GridComponent Construction:

GridComponent::GridComponent(CL_GUIComponent *parent, MainWindow *main_window)
: CL_GUIComponent(parent), main_window(main_window), selected_holder(0),
  boundary(0,0,320,200), resizing(false), moving_boundary(false),
  tab_page_count(0)
{
	set_type_name(cl_text("grid"));
	func_process_message().set(this, &GridComponent::on_process_message);
	func_render().set(this, &GridComponent::on_render);
	func_resized().set(this, &GridComponent::on_resized);

	if (get_holder_parent(this) == 0)
	{
		// The first grid component sets up filtering by default.
		get_gui_manager().func_filter_message().set(this, &GridComponent::on_filter_message);
	}

	CL_SpriteDescription desc;
/*
	desc.add_frame(CL_TargaProvider::load("images/manwalk_0001.tga"));
	desc.add_frame(CL_TargaProvider::load("images/manwalk_0002.tga"));
	desc.add_frame(CL_TargaProvider::load("images/manwalk_0003.tga"));
	desc.add_frame(CL_TargaProvider::load("images/manwalk_0004.tga"));
	desc.add_frame(CL_TargaProvider::load("images/manwalk_0005.tga"));
	desc.add_frame(CL_TargaProvider::load("images/manwalk_0006.tga"));
	desc.add_frame(CL_TargaProvider::load("images/manwalk_0007.tga"));
	desc.add_frame(CL_TargaProvider::load("images/manwalk_0008.tga"));
	desc.add_frame(CL_TargaProvider::load("images/manwalk_0009.tga"));
	desc.add_frame(CL_TargaProvider::load("images/manwalk_0010.tga"));
	desc.add_frame(CL_TargaProvider::load("images/manwalk_0011.tga"));
	desc.add_frame(CL_TargaProvider::load("images/manwalk_0012.tga"));
	desc.add_frame(CL_TargaProvider::load("images/manwalk_0013.tga"));
	desc.add_frame(CL_TargaProvider::load("images/manwalk_0014.tga"));
	desc.add_frame(CL_TargaProvider::load("images/manwalk_0015.tga"));
	desc.add_frame(CL_TargaProvider::load("images/manwalk_0016.tga"));
	desc.add_frame(CL_TargaProvider::load("images/manwalk_0017.tga"));
	desc.add_frame(CL_TargaProvider::load("images/manwalk_0018.tga"));
	desc.add_frame(CL_TargaProvider::load("images/manwalk_0019.tga"));
	desc.add_frame(CL_TargaProvider::load("images/manwalk_0020.tga"));
	desc.add_frame(CL_TargaProvider::load("images/manwalk_0021.tga"));
	desc.add_frame(CL_TargaProvider::load("images/manwalk_0022.tga"));
	desc.add_frame(CL_TargaProvider::load("images/manwalk_0023.tga"));
	desc.add_frame(CL_TargaProvider::load("images/manwalk_0024.tga"));
	desc.add_frame(CL_TargaProvider::load("images/manwalk_0025.tga"));
	desc.add_frame(CL_TargaProvider::load("images/manwalk_0026.tga"));
	desc.add_frame(CL_TargaProvider::load("images/manwalk_0027.tga"));
	desc.add_frame(CL_TargaProvider::load("images/manwalk_0028.tga"));
	desc.add_frame(CL_TargaProvider::load("images/manwalk_0029.tga"));
	for (unsigned int i=0; i<desc.get_frames().size(); i++)
		desc.set_frame_delay(i, 0.01);
*/
	desc.add_frame(CL_PNGProvider::load("gfx/pointer.png"));

//	default_cursor = CL_Cursor(get_display_window(), desc, CL_Point(7, 2));
//	set_cursor(default_cursor);
}

/////////////////////////////////////////////////////////////////////////////
// GridComponent Attributes:

CL_Rect GridComponent::get_boundary()
{
	return boundary;
}

/////////////////////////////////////////////////////////////////////////////
// GridComponent Operations:

void GridComponent::on_add_component(int id)
{
	// HACK: Don't try to create non-creatable components like the Select tool
	if(id == 1337)
		return;

	HolderComponent *holder = new HolderComponent(this);
	CL_GUIComponent *new_component = ComponentTypes::create_component(id, holder);

	if (new_component_count.find(id) == new_component_count.end())
		new_component_count[id] = 1;

	new_component->set_id_name(cl_format(cl_text("%1%2"), new_component->get_type_name(), new_component_count[id]++));

	CL_TempString type = new_component->get_type_name();

	if (type == cl_text("slider"))
	{
		CL_Slider *sl = (CL_Slider *) new_component;
		sl->set_ranges(0, 100, 10, 10);
	}
	else if (type == cl_text("checkbox"))
	{
		CL_CheckBox *checkbox = (CL_CheckBox *) new_component;
		checkbox->set_text("Checkbox");
	}
	else if (type == cl_text("frame"))
	{
		CL_Frame *frame = (CL_Frame *) new_component;
		frame->set_header_text("Frame");
	}
	else if (type == cl_text("scrollbar"))
	{
		CL_ScrollBar *sb = (CL_ScrollBar *) new_component;
		sb->set_ranges(0, 100, 2, 10);
	}
	else if (type == cl_text("tab"))
	{
		CL_Tab *tab = (CL_Tab *) new_component;
		CL_TabPage *page1 = tab->add_page(cl_text("Page 1"));
		page1->func_resized().set(this, &GridComponent::on_tab_page_resized, page1);
		page1->set_id_name(cl_format(cl_text("%1%2"), page1->get_type_name(), tab_page_count++));
	}

	holders.push_back(holder);

	holder->set_geometry(CL_Rect(8-4, 8-4, 8+80+4, 8+21+4));
	new_component->set_geometry(CL_RectPS(4,4, 80,21));
}

void GridComponent::load(const CL_StringRef &fullname)
{
	CL_DomDocument doc;
	CL_File file = CL_File(fullname, CL_File::open_existing);
	doc.load(file);

	CL_DomElement element_components = doc.get_document_element();

	load(element_components, this);
}

void GridComponent::load(CL_DomElement &element, CL_GUIComponent *parent)
{
	CL_DomElement e = element.get_first_child().to_element();

	while (e.is_element())
	{
		HolderComponent *holder = new HolderComponent(parent);
		
		CL_TempString tag = e.get_tag_name();
		CL_GUIComponent *new_comp = 0;

		if (tag == cl_text("button"))
		{
			CL_PushButton *co = new CL_PushButton(holder);
			co->set_text(e.get_attribute(cl_text("text")));
			new_comp = co;
		}
		else if (tag == cl_text("checkbox"))
		{
			CL_CheckBox *co = new CL_CheckBox(holder);
			co->set_text(e.get_attribute(cl_text("text")));
			new_comp = co;
		}
		else if (tag == cl_text("radiobutton"))
		{
			CL_RadioButton *co = new CL_RadioButton(holder);
			co->set_text(e.get_attribute(cl_text("text")));
			co->set_group_name(e.get_attribute("group"));
			new_comp = co;
		}
		else if (tag == cl_text("label"))
		{
			CL_Label *co = new CL_Label(holder);
			co->set_text(e.get_attribute(cl_text("text")));
			new_comp = co;
		}
		else if (tag == cl_text("statusbar"))
		{
			CL_StatusBar *co = new CL_StatusBar(holder);
			new_comp = co;
		}
		else if (tag == cl_text("lineedit"))
		{
			CL_LineEdit *co = new CL_LineEdit(holder);
			co->set_text(e.get_attribute(cl_text("text")));
			new_comp = co;
		}
		else if (tag == cl_text("imageview"))
		{
			CL_ImageView *co = new CL_ImageView(holder);
			new_comp = co;
		}
		else if (tag == cl_text("slider"))
		{
			CL_Slider *co = new CL_Slider(holder);
			co->set_min(CL_StringHelp::text_to_int(e.get_attribute(cl_text("min"))));
			co->set_max(CL_StringHelp::text_to_int(e.get_attribute(cl_text("max"))));
			co->set_tick_count(CL_StringHelp::text_to_int(e.get_attribute(cl_text("ticks"))));
			co->set_page_step(CL_StringHelp::text_to_int(e.get_attribute(cl_text("page_step"))));
			new_comp = co;
		}
		else if (tag == cl_text("listview"))
		{
			CL_ListView *co = new CL_ListView(holder);
			load_listview(e, co);
			new_comp = co;
		}
		else if (tag == cl_text("tab"))
		{
			CL_Tab *co = new CL_Tab(holder);
			new_comp = co;

			CL_DomElement tab_child = e.get_first_child().to_element();
			while (tab_child.is_element())
			{
				if (tab_child.get_tag_name() == cl_text("tabpage"))
				{
					CL_TempString label = tab_child.get_attribute(cl_text("label"), "Error: NO LABEL!");
					int id = CL_StringHelp::text_to_int(tab_child.get_attribute(cl_text("id"), "0"));
					CL_TabPage *tab_page = co->add_page(label, id);
					load(tab_child, tab_page);
				}

				tab_child = tab_child.get_next_sibling().to_element();
			}

		}
		else if (tag == cl_text("menubar"))
		{
			CL_MenuBar *co = new CL_MenuBar(holder);
			new_comp = co;
		}
		else if (tag == cl_text("frame"))
		{
			CL_Frame *co = new CL_Frame(holder);
			co->set_header_text(e.get_attribute(cl_text("text")));
			new_comp = co;

			CL_DomElement frame_child = e.get_first_child().to_element();
			load(e, co);
		}
		else if (tag == cl_text("spin"))
		{
			CL_Spin *co = new CL_Spin(holder);
			new_comp = co;
		}
		else if (tag == cl_text("combobox"))
		{
			CL_ComboBox *co = new CL_ComboBox(holder);
			new_comp = co;
		}
		else if (tag == cl_text("toolbar"))
		{
			CL_ToolBar *co = new CL_ToolBar(holder);
			new_comp = co;
		}
		else if (tag == cl_text("dialog"))
		{
			int w = CL_StringHelp::text_to_int(e.get_attribute(cl_text("width")));
			int h = CL_StringHelp::text_to_int(e.get_attribute(cl_text("height")));
			boundary.right = w;
			boundary.bottom = h;
		}
		else // unknow component... create CustomComponent.
		{
			CustomComponent *co = new CustomComponent(holder);
			co->set_type_name(tag);
			new_comp = co;
		}
		
		if (new_comp == 0)
		{
			delete holder;
		}
		else
		{
			int dist_tl_x = CL_StringHelp::text_to_int(e.get_attribute(cl_text("dist_tl_x")));
			int dist_tl_y = CL_StringHelp::text_to_int(e.get_attribute(cl_text("dist_tl_y")));
			int dist_rb_x = CL_StringHelp::text_to_int(e.get_attribute(cl_text("dist_br_x")));
			int dist_rb_y = CL_StringHelp::text_to_int(e.get_attribute(cl_text("dist_br_y")));
			CL_ComponentAnchorPoint ap_tl = (CL_ComponentAnchorPoint)CL_StringHelp::text_to_int(e.get_attribute(cl_text("anchor_tl")));
			CL_ComponentAnchorPoint ap_br = (CL_ComponentAnchorPoint)CL_StringHelp::text_to_int(e.get_attribute(cl_text("anchor_br")));

			holder->set_anchor_tl(ap_tl);
			holder->set_anchor_br(ap_br);

			holders.push_back(holder);
			new_comp->set_id_name(e.get_attribute(cl_text("id")));
			new_comp->set_class_name(e.get_attribute(cl_text("class")));

			CL_Rect holder_g = load_geometry(e);
			CL_Size comp_size = holder_g.get_size();

			holder_g.expand(CL_Rect(4,4,4,4));
			holder->set_geometry(holder_g);
			CL_Rect comp_g(CL_Point(4,4), CL_Size(comp_size.width, comp_size.height));
			new_comp->set_geometry(comp_g);
		}

		e = e.get_next_sibling().to_element();
	}
}

void GridComponent::save(const CL_StringRef &fullname)
{
	CL_DomDocument doc;

	CL_DomElement element_gui = doc.create_element(cl_text("gui")); 
	element_gui.set_attribute(cl_text("xmlns"), cl_text("http://clanlib.org/xmlns/gui-1.0"));
	
	doc.append_child(element_gui);

	CL_GUIComponent *comp = get_first_child();
	while (comp != 0)
	{
		if (comp->get_type_name() == cl_text("holder"))
		{
			HolderComponent *holder = dynamic_cast<HolderComponent*>(comp);
			if (holder)
			{
				CL_DomElement element = holder->to_element(doc);
				element_gui.append_child(element);
			}
		}
		
		comp = comp->get_next_sibling();
	}

	CL_DomElement element = to_element(doc); // save grid (window) settings
	element_gui.append_child(element);

	CL_File file;
	file.open(fullname, CL_File::create_always);
	doc.save(file);
}

void GridComponent::remove_holder(HolderComponent *holder)
{
	std::vector<HolderComponent*>::iterator it;
	for (it = holders.begin(); it != holders.end(); ++it)
	{
		if ((*it) == holder)
		{
			it = holders.erase(it);
			if (selected_holder = holder)
				selected_holder = 0;
			break;
		}
	}
}

void GridComponent::set_boundary_size(const CL_Size &size)
{
	boundary.right = size.width;
	boundary.bottom = size.height;
}

CL_DomElement GridComponent::to_element(CL_DomDocument &doc)
{
	CL_DomElement de = doc.create_element("dialog");
	de.set_attribute(cl_text("width"), CL_StringHelp::int_to_text(boundary.get_width()));
	de.set_attribute(cl_text("height"), CL_StringHelp::int_to_text(boundary.get_height()));
	return de;
}

/////////////////////////////////////////////////////////////////////////////
// GridComponent Implementation:

void GridComponent::on_process_message(CL_GUIMessage &msg)
{
	if (msg.is_type(CL_GUIMessage_Input::get_type_name()))
	{
		CL_GUIMessage_Input input_msg = msg;
		CL_InputEvent e = input_msg.get_event();

		bool cursor_set = false;

		if (e.type == CL_InputEvent::pressed || e.type == CL_InputEvent::pointer_moved)
		{
			CL_Rect boundary_outer = boundary;
			CL_Rect boundary_inner = boundary;
			boundary_outer.expand(CL_Rect(3,3,3,3)); 
			boundary_inner.shrink(CL_Rect(3,3,3,3)); 

			CL_Rect boundary_resize(boundary.right-8, boundary.bottom-8, boundary.right, boundary.bottom);

			if (boundary_outer.contains(e.mouse_pos) && !boundary_inner.contains(e.mouse_pos))
			{
				if (e.type == CL_InputEvent::pressed && e.id == CL_MOUSE_LEFT)
				{
					msg.set_consumed();
					capture_mouse(true);
					moving_boundary = true;
				}
				else
				{
					msg.set_consumed();
//					set_cursor(cl_cursor_size_nwse);
					cursor_set = true;
				}
			}
			else if (boundary_resize.contains(e.mouse_pos))
			{
				if (e.type == CL_InputEvent::pressed && e.id == CL_MOUSE_LEFT)
				{
					msg.set_consumed();
					capture_mouse(true);
					resizing = true;
				}
				else
				{
					msg.set_consumed();
//					set_cursor(cl_cursor_size_nwse);
					cursor_set = true;
				}
			}
			else
			{
				if (e.type == CL_InputEvent::pressed && e.id == CL_MOUSE_LEFT)
				{
					msg.set_consumed();
					main_window->get_selection()->clear();
				}
			}
		}
		if (e.type == CL_InputEvent::released && e.id == CL_MOUSE_LEFT)
		{
			msg.set_consumed();
			capture_mouse(false);
			resizing = false;
			moving_boundary = false;
		}
		else if (e.type == CL_InputEvent::pointer_moved)
		{
			if (resizing)
			{
				msg.set_consumed();
//				set_cursor(cl_cursor_size_nwse);
				cursor_set = true;
				boundary.right = (e.mouse_pos.x / 8) * 8;
				boundary.bottom = (e.mouse_pos.y / 8) * 8;
				if (boundary.right < 8) boundary.right = 8; 
				if (boundary.bottom < 8) boundary.bottom = 8; 
				invalidate_rect();
			}
		}

//		if (!cursor_set)
//			set_cursor(default_cursor);
	}
}

void GridComponent::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_Rect g = get_geometry().get_size();

	bool tab_parent = (get_parent_component()->get_type_name() == cl_text("tabpage"));

	if (tab_parent)
	{
		CL_Draw::fill(gc, g, CL_Colorf::whitesmoke);
	}
	else
	{
		CL_Draw::fill(gc, g, CL_Colorf::darkgray);
		CL_Draw::fill(gc, boundary, CL_Colorf::lightgrey/*CL_Colorf("E0DFE3")*/);
	}

	render_grid(gc, update_rect);

	if (!tab_parent)
	{
		CL_Draw::line(gc, (float)boundary.left, (float)boundary.bottom, (float)boundary.right, (float)boundary.bottom, CL_Colorf::black);
		CL_Draw::line(gc, (float)boundary.right, (float)boundary.top, (float)boundary.right, (float)boundary.bottom, CL_Colorf::black);
		CL_Rect boundary_resize(boundary.right-5, boundary.bottom-5, boundary.right, boundary.bottom);
		CL_Draw::fill(gc, boundary_resize, CL_Colorf::darkslategray);
	}
}

void GridComponent::render_grid(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	int grid_width = boundary.get_width() / 8;
	int grid_height = boundary.get_height() / 8;
	if (grid_rect != boundary)
	{
		grid_points.resize(grid_width * grid_height, CL_Vec2i(0,0));
		for (int y = 0; y < grid_height; y++)
		{
			for (int x = 0; x < grid_width; x++)
			{
				grid_points[x+y*grid_width] = CL_Vec2i(boundary.left + x * 8, boundary.top + y * 8);
			}
		}
		grid_rect = boundary;

	}
	CL_PrimitivesArray grid_array(gc);
	grid_array.set_attributes(0, &grid_points[0]);
	grid_array.set_attribute(1, CL_Colorf::gray);
	gc.set_program_object(cl_program_color_only);
	gc.draw_primitives(cl_points, (int) grid_points.size(), grid_array);
	gc.reset_program_object();
}

void GridComponent::on_resized()
{
}

CL_Rect GridComponent::load_geometry(CL_DomElement &e)
{
	CL_TempString str = e.get_attribute(cl_text("geom"));
	std::vector<CL_TempString> split = CL_StringHelp::split_text(str, cl_text(","));

	CL_Rect r;
	r.left = CL_StringHelp::text_to_int(split[0]);
	r.top = CL_StringHelp::text_to_int(split[1]);
	r.right = CL_StringHelp::text_to_int(split[2]);
	r.bottom = CL_StringHelp::text_to_int(split[3]);

	return r;
}

void GridComponent::load_listview(CL_DomElement &e, CL_ListView *lv)
{
}

CL_GUIComponent *GridComponent::get_holder_parent(CL_GUIComponent *comp)
{
	CL_GUIComponent *parent = comp->get_parent_component();

	while (parent != 0)
	{
		if (parent->get_type_name() == cl_text("holder"))
		{
			return parent;
		}
		parent = parent->get_parent_component();
	}

	return 0;
}

void GridComponent::on_tab_page_resized(CL_TabPage *page)
{
}

bool GridComponent::on_filter_message(CL_GUIMessage &msg)
{
	CL_GUIComponent *comp = msg.get_target();
	if (comp == 0)
		return true;

	if (comp->get_type_name() == cl_text("tabheader")) // Allow clicking on tab header to change tab page.
		return true;

	if (!GridComponent::is_grid_or_child_of_grid(comp)) // Clicked the grid or a holder (only immediate child type of grid).
		return true;

	CL_GUIComponent *parent = comp->get_parent_component();

	CL_GUIComponent *holder_parent = get_holder_parent(comp);

	if (holder_parent && (comp->get_type_name() != cl_text("holder")))
	{
		if (msg.is_type(CL_GUIMessage_Input::get_type_name()))
		{
			CL_GUIMessage_Input input_msg = msg;
			CL_InputEvent e = input_msg.get_event();

			//: Convert to window coordinates.
			e.mouse_pos = comp->component_to_window_coords(e.mouse_pos);
			input_msg.set_event(e);

			if (e.type == CL_InputEvent::pressed && e.id == CL_MOUSE_LEFT)
			{
				on_filter_mouse_left_down(input_msg, e);
			}
			else if (e.type == CL_InputEvent::released && e.id == CL_MOUSE_LEFT)
			{
				on_filter_mouse_left_up(input_msg, e);
			}
			else if (e.type == CL_InputEvent::pointer_moved)
			{
				HolderComponent *holder = dynamic_cast<HolderComponent*>(holder_parent);
				holder->on_process_message(input_msg);
			}
			msg.set_consumed();
		}
		else if (msg.is_type(CL_GUIMessage_Pointer::get_type_name()))
		{
			HolderComponent *holder = dynamic_cast<HolderComponent*>(holder_parent);
			holder->on_process_message(msg);
			msg.set_consumed();
		}

		return false;
	}
	else if (comp->get_type_name() == cl_text("holder"))
	{
		if (msg.is_type(CL_GUIMessage_Input::get_type_name()))
		{
			CL_GUIMessage_Input input_msg = msg;
			CL_InputEvent e = input_msg.get_event();

			//: Convert to window coordinates.
			e.mouse_pos = comp->component_to_window_coords(e.mouse_pos);
			input_msg.set_event(e);

			HolderComponent *holder = dynamic_cast<HolderComponent*>(comp);
			holder->on_process_message(input_msg);

			if (e.type == CL_InputEvent::pressed && e.id == CL_KEY_DELETE)
			{
				if (holder->marked_for_delete())
				{
					if (selected_holder == holder)
						selected_holder = 0;

					std::vector<HolderComponent*>::iterator it;
					for (it = holders.begin(); it!=holders.end(); ++it)
					{
						if ((*it) == holder)
						{
							it = holders.erase(it);
							break;
						}
					}
					main_window->get_selection()->remove_holder(holder);
					delete holder;
					invalidate_rect();
					return false;
				}
			}
			else if (e.type == CL_InputEvent::pointer_moved)
			{
			}

			msg.set_consumed();
		}

		return false;
	}

	return true;
}

void GridComponent::on_filter_mouse_left_down(CL_GUIMessage &msg, CL_InputEvent &e)
{
	CL_GUIComponent *comp = msg.get_target();
	CL_GUIComponent *holder_parent = get_holder_parent(comp);

	if (main_window->get_editor_mode() == edit_mode_taborder)
	{
		on_mouse_left_tab_order(holder_parent);
		return;
	}

	if (!e.shift)
	{
		main_window->get_selection()->clear();
	}

	HolderComponent *holder = dynamic_cast<HolderComponent*>(holder_parent);
	holder->set_selected(true);
	holder->set_focus();
	main_window->get_selection()->add_holder(holder);

	holder->on_process_message(msg);

	invalidate_rect();
}

void GridComponent::on_filter_mouse_left_up(CL_GUIMessage &msg, CL_InputEvent &e)
{
/*	CL_GUIComponent *comp = msg.get_target();
	CL_GUIComponent *holder_parent = get_holder_parent(comp);

	HolderComponent *holder = dynamic_cast<HolderComponent*>(holder_parent);
*/
}

void GridComponent::on_mouse_left_tab_order(CL_GUIComponent *holder)
{
	if (holder == 0)
	{
		clear_tab_order_indexes();
	}

	CL_GUIComponent *comp = holder->get_first_child();

	if (comp == 0)
		return;

	
}

void GridComponent::clear_tab_order_indexes()
{
	// todo
}

bool GridComponent::is_grid_or_child_of_grid(CL_GUIComponent *comp)
{
	if (comp->get_type_name() == cl_text("grid"))
		return true;

	while (comp != 0)
	{
		if (comp->get_type_name() == cl_text("grid"))
			return true;

		comp = comp->get_parent_component();
	}

	return false;
}





