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
#include "property_component_tab_order.h"
#include "holder_component.h"
#include "grid_component.h"
#include "custom_component.h"
#include "selection.h"
#include "main_window.h"

/////////////////////////////////////////////////////////////////////////////
// PropertyComponentTabOrder Construction:

PropertyComponentTabOrder::PropertyComponentTabOrder(MainWindow *main_window)
: CL_GUIComponent(main_window), main_window(main_window), selected_component(0),
  container(0)
{
	func_process_message().set(this, &PropertyComponentTabOrder::on_process_message);
	func_render().set(this, &PropertyComponentTabOrder::on_render);
	func_resized().set(this, &PropertyComponentTabOrder::on_resized);

	container = new CL_GUIComponent(this);
	slots.connect(main_window->get_selection()->sig_selection_changed(), this, &PropertyComponentTabOrder::on_selection_changed);
}

/////////////////////////////////////////////////////////////////////////////
// PropertyComponentTabOrder Attributes:

/////////////////////////////////////////////////////////////////////////////
// PropertyComponentTabOrder Operations:

void PropertyComponentTabOrder::clear()
{
	delete container;
	container = 0;
}

void PropertyComponentTabOrder::on_selection_changed()
{
	std::vector<HolderComponent*> selected_holders = main_window->get_selection()->get_selection();

	if (selected_holders.empty())
	{
		update_property_list(0);
		selected_component = 0;
		selected_holder = 0;
		return;
	}

	selected_holder = selected_holders.back();
	selected_component = selected_holder->get_first_child();
	update_property_list(selected_component);
}

/////////////////////////////////////////////////////////////////////////////
// PropertyComponentTabOrder Implementation:

void PropertyComponentTabOrder::on_process_message(CL_GUIMessage &msg)
{
/*	if (msg.is_type(CL_GUIMessage_Input::get_type_name()))
	{
		CL_GUIMessage_Input input_msg = msg;
		CL_InputEvent e = input_msg.get_event();

		if (e.type == CL_InputEvent::pressed && e.id == CL_MOUSE_LEFT)
		{
			if (boundary_outer.contains(e.mouse_pos) && !boundary_inner.is_inside(e.mouse_pos))
			{
				capture_mouse(true);
				moving_boundary = true;
			}
		}
	}
*/
}

void PropertyComponentTabOrder::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
}

void PropertyComponentTabOrder::on_resized()
{
	if (container)
		container->set_geometry(get_geometry().get_size());
}

void PropertyComponentTabOrder::update_property_list(CL_GUIComponent *comp)
{
	delete container;
	container = 0;

	if (comp == 0)
	{
		return;
	}

	container = new CL_GUIComponent(this);
	container->set_geometry(get_geometry().get_size());

	add_header(cl_text("Tab Order"));
	add_lineedit(cl_text("Index"), CL_StringHelp::int_to_text(comp->get_tab_order()));
	add_info(cl_text("Info Test"));
	add_check(cl_text("Tab Controller"), comp->get_type_name() == cl_text("grid"));
}

void PropertyComponentTabOrder::on_property_changed(CL_InputEvent event)
{
	CL_GUIComponent *comp = selected_component;
	if (comp == 0)
	{
		return;
	}

	comp->set_tab_order(get_property_int(cl_text("Index")));
	comp->set_tab_order_controller(get_property_bool(cl_text("Tab Controller")));
}

void PropertyComponentTabOrder::add_lineedit(const CL_StringRef &property_name, const CL_StringRef &value) 
{
	CL_Rect rect = get_child_rect(container);
	int top = rect.bottom-1;
	CL_Size s = get_geometry().get_size();

	CL_Label *label = new CL_Label(container);
	label->set_class_name(cl_text("properties"));
	label->set_text(property_name);
	CL_Rect label_rect = CL_RectPS(4,top, label->get_preferred_width(),label->get_preferred_height());
	label->set_geometry(label_rect);

	CL_LineEdit *le = new CL_LineEdit(container);
	le->set_class_name(cl_text("properties"));
	le->set_geometry(CL_Rect(label_rect.right,top, s.width-8, top+le->get_preferred_height()));
	le->set_text(value);
	le->func_after_edit_changed().set(this, &PropertyComponentTabOrder::on_property_changed);
	properties[property_name] = le;
}


CL_Rect PropertyComponentTabOrder::get_child_rect(CL_GUIComponent *parent) 
{
	CL_GUIComponent *child = parent->get_first_child();

	if (child == 0)
		return CL_Rect(0,0,0,0);
	
	CL_Rect bounding_rect = child->get_geometry();
	child = child->get_next_sibling();

	while (child)
	{
		bounding_rect.bounding_rect(child->get_geometry());
		child = child->get_next_sibling();
	}

	return bounding_rect;
}


void PropertyComponentTabOrder::add_header(const CL_StringRef &header)
{
	CL_Label *label_properties_header = new CL_Label(container);
	label_properties_header->set_class_name("properties_header");
	label_properties_header->set_text(header);

	CL_Rect child_rect = get_child_rect(container);
	int y_offset = 10;
	label_properties_header->set_geometry(CL_Rect(4,y_offset+child_rect.bottom,container->get_geometry().get_width()-8, y_offset+child_rect.bottom+label_properties_header->get_preferred_height()));
}

void PropertyComponentTabOrder::add_info(const CL_StringRef &text)
{
	CL_Label *info = new CL_Label(container);
	info->set_class_name("properties_info");
	info->set_text(text);

	CL_Rect child_rect = get_child_rect(container);
	int y_offset = 10;
	info->set_geometry(CL_Rect(4,y_offset+child_rect.bottom,container->get_geometry().get_width()-8, y_offset+child_rect.bottom+info->get_preferred_height()));
}

void PropertyComponentTabOrder::add_check( const CL_StringRef &property_name, bool initial_value )
{
	CL_Rect rect = get_child_rect(container);
	int top = rect.bottom-1;
	CL_Size s = get_geometry().get_size();

	CL_Label *label = new CL_Label(container);
	label->set_class_name(cl_text("properties"));
	label->set_text(property_name);
	CL_Rect label_rect = CL_RectPS(4,top, label->get_preferred_width(),label->get_preferred_height());
	label->set_geometry(label_rect);

	CL_CheckBox *check = new CL_CheckBox(container);
	check->set_class_name(cl_text("properties"));
	check->set_geometry(CL_Rect(label_rect.right,top, s.width-8, top+check->get_preferred_height()));
	check->set_checked(initial_value);
	check->func_state_changed().set(this, &PropertyComponentTabOrder::on_property_changed, CL_InputEvent());
	properties[property_name] = check;
}

CL_TempString PropertyComponentTabOrder::get_property_str(const CL_StringRef &property_name)
{
	std::map<CL_String, CL_GUIComponent*>::iterator it;
	it = properties.find(property_name);
	if (it == properties.end())
		throw CL_Exception(cl_text("PropertyComponentTabOrder: No such str property: ") + property_name);

	if ((*it).second->get_type_name() == cl_text("lineedit"))
		return ((CL_LineEdit*)((*it).second))->get_text();
	else if ((*it).second->get_type_name() == cl_text("checkbox"))
		return ((CL_CheckBox*)((*it).second))->is_checked() ? cl_text("true") : cl_text("false");

	return CL_TempString();
}

int PropertyComponentTabOrder::get_property_int(const CL_StringRef &property_name)
{
	std::map<CL_String, CL_GUIComponent*>::iterator it;
	it = properties.find(property_name);
	if (it == properties.end())
		throw CL_Exception(cl_text("PropertyComponentTabOrder: No such int property: ") + property_name);

	if ((*it).second->get_type_name() == cl_text("lineedit"))
		return CL_StringHelp::text_to_int(((CL_LineEdit*)((*it).second))->get_text());
	if ((*it).second->get_type_name() == cl_text("checkbox"))
		return ((CL_CheckBox*)((*it).second))->is_checked() ? 1 : 0;

	return -31337;
}

bool PropertyComponentTabOrder::get_property_bool(const CL_StringRef &property_name)
{
	std::map<CL_String, CL_GUIComponent*>::iterator it;
	it = properties.find(property_name);
	if (it == properties.end())
		throw CL_Exception(cl_text("PropertyComponentTabOrder: No such bool property: ") + property_name);

	if ((*it).second->get_type_name() == cl_text("lineedit"))
		return CL_StringHelp::text_to_bool(((CL_LineEdit*)((*it).second))->get_text());
	if ((*it).second->get_type_name() == cl_text("checkbox"))
		return ((CL_CheckBox*)((*it).second))->is_checked() ? true : false;

	return false;
}

float PropertyComponentTabOrder::get_property_float(const CL_StringRef &property_name)
{
	std::map<CL_String, CL_GUIComponent*>::iterator it;
	it = properties.find(property_name);
	if (it == properties.end())
		throw CL_Exception(cl_text("PropertyComponentTabOrder: No such float property: ") + property_name);

	if ((*it).second->get_type_name() == cl_text("lineedit"))
		return CL_StringHelp::text_to_float(((CL_LineEdit*)((*it).second))->get_text());

	return 0.0f;
}
