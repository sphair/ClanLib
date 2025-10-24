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
#include "grid_component.h"
#include "custom_component.h"
#include "holder_component.h"
#include "component_types.h"
#include "main_window.h"
#include "selection.h"
#include "snapline.h"

/////////////////////////////////////////////////////////////////////////////
// GridComponent Construction:

GridComponent::GridComponent(CL_GUIComponent *parent, MainWindow *main_window)
: CL_GUIComponent(parent), main_window(main_window), component_container(0),
  component_overlay(0), boundary(320,200)
{
	set_type_name("grid");
	func_input_pressed().set(this, &GridComponent::on_input_pressed);
	func_input_released().set(this, &GridComponent::on_input_released);
	func_input_doubleclick().set(this, &GridComponent::on_input_doubleclick);
	func_input_pointer_moved().set(this, &GridComponent::on_input_pointer_moved);
	func_render().set(this, &GridComponent::on_render);
	func_resized().set(this, &GridComponent::on_resized);
	part_windowframe = CL_GUIThemePart(this, "window-frame");

	edit_state.set_grid_component(this);

	component_container = new CL_GUIComponent(this);
	component_overlay = new CL_GUIComponent(this);
	component_overlay->func_render().set(this, &GridComponent::on_render_overlay);
}

/////////////////////////////////////////////////////////////////////////////
// GridComponent Attributes:

CL_Size GridComponent::get_dialog_size()
{
	return boundary;
}

const std::vector<HolderComponent*> &GridComponent::get_holders() const
{
	return holders;
}

/////////////////////////////////////////////////////////////////////////////
// GridComponent Operations:

HolderComponent *GridComponent::on_add_component(int id, const CL_Vec2i &pos)
{
	HolderComponent *holder = new HolderComponent(component_container);

	CL_GUIComponent *new_component = ComponentTypes::create_component(id, holder->get_container());

	holder->set_geometry(CL_Rect(pos, new_component->get_size()));
	holders.push_back(holder);

	return holder;
}

void GridComponent::load(const CL_StringRef &fullname)
{
	CL_DomDocument doc;
	CL_File file = CL_File(fullname, CL_File::open_existing, CL_File::access_read);
	doc.load(file);

	CL_DomElement element_components = doc.get_document_element();

	load(element_components, component_container);

	request_repaint();
}

void GridComponent::load(CL_DomElement &element, CL_GUIComponent *parent)
{
	CL_DomElement e = element.get_first_child().to_element();

	while (e.is_element())
	{
		HolderComponent *holder = new HolderComponent(parent);
		
		CL_String tag = e.get_tag_name();
		CL_GUIComponent *new_comp = 0;

		if (tag == "button")
		{
			CL_PushButton *co = new CL_PushButton(holder->get_container());
			co->set_text(e.get_attribute("text"));
			new_comp = co;
		}
		else if (tag == "checkbox")
		{
			CL_CheckBox *co = new CL_CheckBox(holder->get_container());
			co->set_text(e.get_attribute("text"));
			new_comp = co;
		}
		else if (tag == "radiobutton")
		{
			CL_RadioButton *co = new CL_RadioButton(holder->get_container());
			co->set_text(e.get_attribute("text"));
			co->set_group_name(e.get_attribute("group"));
			new_comp = co;
		}
		else if (tag == "label")
		{
			CL_Label *co = new CL_Label(holder->get_container());
			co->set_text(e.get_attribute("text"));
			new_comp = co;
		}
		else if (tag == "statusbar")
		{
			CL_StatusBar *co = new CL_StatusBar(holder->get_container());
			new_comp = co;
		}
		else if (tag == "lineedit")
		{
			CL_LineEdit *co = new CL_LineEdit(holder->get_container());
			co->set_text(e.get_attribute("text"));
			new_comp = co;
		}
		else if (tag == "imageview")
		{
			CL_ImageView *co = new CL_ImageView(holder->get_container());
			new_comp = co;
		}
		else if (tag == "slider")
		{
			CL_Slider *co = new CL_Slider(holder->get_container());
			co->set_min(CL_StringHelp::text_to_int(e.get_attribute("min")));
			co->set_max(CL_StringHelp::text_to_int(e.get_attribute("max")));
			co->set_tick_count(CL_StringHelp::text_to_int(e.get_attribute("ticks")));
			co->set_page_step(CL_StringHelp::text_to_int(e.get_attribute("page_step")));
			new_comp = co;
		}
		else if (tag == "listview")
		{
			CL_ListView *co = new CL_ListView(holder->get_container());
//			load_listview(e, co);
			new_comp = co;
		}
		else if (tag == "tab")
		{
			CL_Tab *co = new CL_Tab(holder->get_container());
			new_comp = co;

			CL_DomElement tab_child = e.get_first_child().to_element();
			while (tab_child.is_element())
			{
				if (tab_child.get_tag_name() == "tabpage")
				{
					CL_String label = tab_child.get_attribute("label", "Error: NO LABEL!");
					int id = CL_StringHelp::text_to_int(tab_child.get_attribute("id", "0"));
					CL_TabPage *tab_page = co->add_page(label, id);
					load(tab_child, tab_page);
				}

				tab_child = tab_child.get_next_sibling().to_element();
			}

		}
		else if (tag == "menubar")
		{
			CL_MenuBar *co = new CL_MenuBar(holder->get_container());
			new_comp = co;
		}
		else if (tag == "frame")
		{
			CL_Frame *co = new CL_Frame(holder->get_container());
			co->set_header_text(e.get_attribute("text"));
			new_comp = co;

			CL_DomElement frame_child = e.get_first_child().to_element();
			load(e, co);
		}
		else if (tag == "spin")
		{
			CL_Spin *co = new CL_Spin(holder->get_container());
			new_comp = co;
		}
		else if (tag == "combobox")
		{
			CL_ComboBox *co = new CL_ComboBox(holder->get_container());
			new_comp = co;
		}
		else if (tag == "toolbar")
		{
			CL_ToolBar *co = new CL_ToolBar(holder->get_container());
			new_comp = co;
		}
		else if (tag == "dialog")
		{
			int w = CL_StringHelp::text_to_int(e.get_attribute("width"));
			int h = CL_StringHelp::text_to_int(e.get_attribute("height"));
			boundary.width = w;
			boundary.height = h;
		}
		else // unknown component... create CustomComponent.
		{
			CustomComponent *co = new CustomComponent(holder->get_container());
			co->set_type_name(tag);
			new_comp = co;
		}
		
		if (new_comp == 0)
		{
			delete holder;
		}
		else
		{
			int dist_tl_x = CL_StringHelp::text_to_int(e.get_attribute("dist_tl_x"));
			int dist_tl_y = CL_StringHelp::text_to_int(e.get_attribute("dist_tl_y"));
			int dist_rb_x = CL_StringHelp::text_to_int(e.get_attribute("dist_br_x"));
			int dist_rb_y = CL_StringHelp::text_to_int(e.get_attribute("dist_br_y"));
			CL_String pos_equation_x = e.get_attribute("eq-x", "");
			CL_String pos_equation_y = e.get_attribute("eq-y", "");
			CL_String pos_equation_x2 = e.get_attribute("eq-x2", "");
			CL_String pos_equation_y2 = e.get_attribute("eq-y2", "");
			holder->set_position_equations(pos_equation_x, pos_equation_y);
			holder->set_position_equations2(pos_equation_x2, pos_equation_y2);
			CL_ComponentAnchorPoint ap_tl = (CL_ComponentAnchorPoint)CL_StringHelp::text_to_int(e.get_attribute("anchor_tl"));
			CL_ComponentAnchorPoint ap_br = (CL_ComponentAnchorPoint)CL_StringHelp::text_to_int(e.get_attribute("anchor_br"));

			holder->set_anchor_tl(ap_tl);
			holder->set_anchor_br(ap_br);

			holders.push_back(holder);
			new_comp->set_id_name(e.get_attribute("id"));
			new_comp->set_class_name(e.get_attribute("class"));

			CL_Rect holder_g = load_geometry(e);
			CL_Size comp_size = holder_g.get_size();

			holder->set_geometry(holder_g);
			new_comp->set_geometry(comp_size);
		}

		e = e.get_next_sibling().to_element();
	}
}

void GridComponent::save(const CL_StringRef &fullname)
{
	CL_DomDocument doc;

	CL_DomElement element_gui = doc.create_element("gui"); 
	element_gui.set_attribute("xmlns", "http://clanlib.org/xmlns/gui-1.0");
	
	doc.append_child(element_gui);

	CL_GUIComponent *comp = component_container->get_first_child();
	while (comp != 0)
	{
		if (comp->get_type_name() == "holder")
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
	file.open(fullname, CL_File::create_always, CL_File::access_write);
	doc.save(file);
}

void GridComponent::remove_holder(HolderComponent *holder)
{
	std::vector<HolderComponent*>::iterator it;
	for (it = holders.begin(); it != holders.end(); ++it)
	{
		if ((*it) == holder)
		{
			main_window->get_selection()->remove_holder(holder);
			it = holders.erase(it);
			break;
		}
	}
}

void GridComponent::set_boundary_size(const CL_Size &size)
{
	boundary.width = size.width;
	boundary.height = size.height;
	on_resized();
	request_repaint();
}

CL_DomElement GridComponent::to_element(CL_DomDocument &doc)
{
	CL_DomElement de = doc.create_element("dialog");
	de.set_attribute("width", CL_StringHelp::int_to_text(boundary.width));
	de.set_attribute("height", CL_StringHelp::int_to_text(boundary.height));
	return de;
}

/////////////////////////////////////////////////////////////////////////////
// GridComponent Implementation:

CL_Rect GridComponent::get_boundary_grabber_se() const
{
	return CL_Rect(CL_Point(boundary.width, boundary.height), CL_Size(8, 8));
}

CL_Rect GridComponent::get_boundary_grabber_s() const
{
	return CL_Rect(CL_Point(0, boundary.height), CL_Size(get_boundary_grabber_se().left, 8));
}

CL_Rect GridComponent::get_boundary_grabber_e() const
{
	return CL_Rect(CL_Point(boundary.width, 0), CL_Size(8, get_boundary_grabber_se().top));
}

HolderComponent *GridComponent::find_holder_at(const CL_Point &pos)
{
	return HolderComponent::find_holder_at(component_container, pos);
}

bool GridComponent::deliver_input_to_tab(const CL_InputEvent &e)
{
	CL_GUIComponent *child = component_container->get_component_at(e.mouse_pos);
	if (child && child->get_type_name() == "tabheader")
	{
		CL_InputEvent e_child = e;
		e_child.mouse_pos = child->window_to_component_coords(component_to_window_coords(e.mouse_pos));
		CL_GUIMessage_Input msg;
		msg.set_event(e_child);
		msg.set_target(child);
		get_gui_manager().dispatch_message(msg);
		return true;
	}
	else
	{
		return false;
	}
}

bool GridComponent::on_input_pressed(const CL_InputEvent &e)
{
	if (e.id == CL_MOUSE_LEFT && deliver_input_to_tab(e))
		return true;
	return edit_state.on_input_pressed(offset_event(e));
}

bool GridComponent::on_input_released(const CL_InputEvent &e)
{
	if (e.id == CL_MOUSE_LEFT && deliver_input_to_tab(e))
		return true;
	return edit_state.on_input_released(offset_event(e));
}

bool GridComponent::on_input_doubleclick(const CL_InputEvent &e)
{
	if (e.id == CL_MOUSE_LEFT && deliver_input_to_tab(e))
		return true;
	return edit_state.on_input_doubleclick(offset_event(e));
}

bool GridComponent::on_input_pointer_moved(const CL_InputEvent &e)
{
	return edit_state.on_input_pointer_moved(offset_event(e));
}

CL_InputEvent GridComponent::offset_event(CL_InputEvent e)
{
	e.mouse_pos.x -= component_container->get_geometry().left;
	e.mouse_pos.y -= component_container->get_geometry().top;
	return e;
}

void GridComponent::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	set_cliprect(gc, get_size());
	CL_Rect g = get_geometry().get_size();

	bool tab_parent = (get_parent_component()->get_type_name() == "tabpage");

	if (tab_parent)
	{
		CL_Draw::fill(gc, g, CL_Colorf::whitesmoke);
	}
	else
	{
		//CL_Draw::fill(gc, g, CL_Colorf::darkgray);
		CL_Draw::fill(gc, g, CL_Colorf(199/255.0f, 209/255.0f, 224/255.0f));
		CL_Draw::fill(gc, boundary, CL_Colorf::lightgrey/*CL_Colorf("E0DFE3")*/);
	}
/*
	if (!tab_parent)
	{
		CL_Draw::line(gc, (float)boundary.left, (float)boundary.bottom, (float)boundary.right, (float)boundary.bottom, CL_Colorf::black);
		CL_Draw::line(gc, (float)boundary.right, (float)boundary.top, (float)boundary.right, (float)boundary.bottom, CL_Colorf::black);
		// CL_Draw::fill(gc, get_boundary_grabber_se(), CL_Colorf::darkslategray);
	}
*/
	CL_Rect framebox = part_windowframe.get_render_box(boundary);
	framebox.translate(-framebox.left, -framebox.top);
	part_windowframe.render_box(gc, framebox, update_rect);

	reset_cliprect(gc);
}

void GridComponent::on_render_overlay(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	set_cliprect(gc, get_size());
	std::vector<HolderComponent *> selection = main_window->get_selection()->get_selection();
	for (size_t i = 0; i < selection.size(); i++)
	{
		CL_Rect grabbers[8] =
		{
			selection[i]->get_grabber_e(),
			selection[i]->get_grabber_se(),
			selection[i]->get_grabber_s(),
			selection[i]->get_grabber_sw(),
			selection[i]->get_grabber_w(),
			selection[i]->get_grabber_nw(),
			selection[i]->get_grabber_n(),
			selection[i]->get_grabber_ne()
		};

		for (int j=0; j<8; j++)
			grabbers[j] = window_to_component_coords(selection[i]->component_to_window_coords(grabbers[j]));

		CL_Rect pos = window_to_component_coords(selection[i]->component_to_window_coords(selection[i]->get_size()));
		pos.expand(4,4,3,3);
		CL_Draw::box(gc, pos, CL_Colorf(100.0f/255.0f, 100.0f/255.0f, 100.0f/255.0f, 0.25f));

		for (int j=0; j<8; j++)
		{
			CL_Draw::fill(gc, grabbers[j], CL_Colorf::white);
			CL_Draw::box(gc, grabbers[j], CL_Colorf::black);
		}
	}

	if (netselect_box.get_size() != CL_Size(0,0))
	{
		CL_Rect box = netselect_box;
		box.translate(component_container->get_geometry().left, component_container->get_geometry().top);

		CL_Colorf c = CL_Colorf::blue;
		c.set_alpha(0.1f);
		CL_Draw::fill(gc, box, c);
		CL_Draw::box(gc, box, CL_Colorf::blue);
	}

	reset_cliprect(gc);
}

void GridComponent::on_resized()
{
	CL_Rect framebox = part_windowframe.get_render_box(boundary);
	framebox.translate(-framebox.left, -framebox.top);
	component_container->set_geometry(part_windowframe.get_content_box(framebox));
	component_overlay->set_geometry(get_size());
}

CL_Rect GridComponent::load_geometry(CL_DomElement &e)
{
	CL_String str = e.get_attribute("geom");
	std::vector<CL_String> split = CL_StringHelp::split_text(str, ",");

	CL_Rect r;
	r.left = CL_StringHelp::text_to_int(split[0]);
	r.top = CL_StringHelp::text_to_int(split[1]);
	r.right = CL_StringHelp::text_to_int(split[2]);
	r.bottom = CL_StringHelp::text_to_int(split[3]);

	return r;
}

CL_Rect GridComponent::holder_to_grid_coords(HolderComponent *holder, const CL_Rect &rect)
{
	return component_container->window_to_component_coords(holder->component_to_window_coords(rect));
}

CL_Point GridComponent::holder_to_grid_coords(HolderComponent *holder, const CL_Point &point)
{
	return component_container->window_to_component_coords(holder->component_to_window_coords(point));
}

CL_Rect GridComponent::grid_to_holder_coords(HolderComponent *holder, const CL_Rect &rect)
{
	return component_container->component_to_window_coords(holder->window_to_component_coords(rect));
}

CL_Point GridComponent::grid_to_holder_coords(HolderComponent *holder, const CL_Point &point)
{
	return component_container->component_to_window_coords(holder->window_to_component_coords(point));
}

std::vector<SnapLine> GridComponent::get_snaplines() const
{
	CL_Size size = get_size();
	std::vector<SnapLine> snaplines;
	int margin = 11;
	snaplines.push_back(SnapLine(SnapLine::Top, margin, SnapLine::Low));
	snaplines.push_back(SnapLine(SnapLine::Bottom, boundary.height-margin, SnapLine::Low));
	snaplines.push_back(SnapLine(SnapLine::Left, margin, SnapLine::Low));
	snaplines.push_back(SnapLine(SnapLine::Right, boundary.width-margin, SnapLine::Low));
	return snaplines;
}

CL_Vec2i GridComponent::snap(HolderComponent *holder, const std::vector<SnapLine> &source_snaplines, const CL_Rect &source_rect)
{
	std::vector<SnapLine::SnapLineTarget> targets;

	// Add all other components
	std::vector<HolderComponent*> holders = get_holders();
	for (size_t holder_index = 0; holder_index < holders.size(); holder_index++)
	{
		HolderComponent *target_holder = holders[holder_index];

		if(target_holder != holder)
			targets.push_back(SnapLine::SnapLineTarget(target_holder->get_geometry(), target_holder->get_snaplines()));
	}

	// Add GridComponent itself
	targets.push_back(SnapLine::SnapLineTarget(get_dialog_size(), get_snaplines()));

	return SnapLine::snap(source_rect, source_snaplines, targets);
}

void GridComponent::set_netselect_box(CL_Rect new_netselect_box)
{
	if (netselect_box != new_netselect_box)
	{
		netselect_box = new_netselect_box;
		request_repaint();
	}
}

void GridComponent::select_objects(const CL_Rect &box)
{
	for (size_t i = 0; i < holders.size(); i++)
	{
		CL_Rect holder_box = holders[i]->get_geometry();
		holder_box.overlap(box);
		if (holder_box.right > holder_box.left && holder_box.bottom > holder_box.top)
		{
			main_window->get_selection()->add_holder(holders[i]);
		}
	}
	main_window->get_selection()->sig_selection_changed().invoke();
	request_repaint();
}
