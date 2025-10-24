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
#include "property_component.h"
#include "holder_component.h"
#include "grid_component.h"
#include "custom_component.h"
#include "selection.h"
#include "main_window.h"

/////////////////////////////////////////////////////////////////////////////
// PropertyComponent Construction:

PropertyComponent::PropertyComponent(MainWindow *main_window)
: CL_GUIComponent(main_window), main_window(main_window), selected_component(0),
  container(0)
{
	func_process_message().set(this, &PropertyComponent::on_process_message);
	func_render().set(this, &PropertyComponent::on_render);
	func_resized().set(this, &PropertyComponent::on_resized);

	container = new CL_GUIComponent(this);
	slots.connect(main_window->get_selection()->sig_selection_changed(), this, &PropertyComponent::on_selection_changed);
}

/////////////////////////////////////////////////////////////////////////////
// PropertyComponent Attributes:

/////////////////////////////////////////////////////////////////////////////
// PropertyComponent Operations:

void PropertyComponent::clear()
{
	delete container;
	container = 0;
}

void PropertyComponent::on_selection_changed()
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
	callback_info = selected_holder->get_callback_info();
	selected_component = selected_holder->get_first_child();
	update_property_list(selected_component);
}

/////////////////////////////////////////////////////////////////////////////
// PropertyComponent Implementation:

void PropertyComponent::on_process_message(CL_GUIMessage &msg)
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

void PropertyComponent::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_Rect g = get_geometry().get_size();

//	CL_Draw::box(gc, g, CL_Colorf::gray);
}

void PropertyComponent::on_resized()
{
	if (container)
		container->set_geometry(get_geometry().get_size());
}

void PropertyComponent::update_property_list(CL_GUIComponent *comp)
{
	delete container;
	container = 0;

	if (comp == 0)
	{
		return;
	}

	container = new CL_GUIComponent(this);
	container->set_geometry(get_geometry().get_size());

	add_header(cl_text("Properties"));

	CL_TempString type = comp->get_type_name();

	if (type != cl_text("grid"))
	{
		CL_Size comp_size = comp->get_geometry().get_size();
		add_field(cl_text("Width"), CL_StringHelp::int_to_text(comp_size.width));
		add_field(cl_text("Height"), CL_StringHelp::int_to_text(comp_size.height));
	}

	if (type == cl_text("button"))
	{
		CL_PushButton *co = dynamic_cast<CL_PushButton*>(comp);
		add_field(cl_text("Text"), co->get_text());
	}
	else if (type == cl_text("lineedit"))
	{
		CL_LineEdit *co = dynamic_cast<CL_LineEdit*>(comp);
		add_field(cl_text("Text"), co->get_text());
	}
	else if (type == cl_text("checkbox"))
	{
		CL_CheckBox *co = dynamic_cast<CL_CheckBox*>(comp);
		add_field(cl_text("Text"), co->get_text());
	}
	else if (type == cl_text("radiobutton"))
	{
		CL_RadioButton *co = dynamic_cast<CL_RadioButton*>(comp);
		add_field(cl_text("Text"), co->get_text());
		add_field(cl_text("Group Name"), co->get_group_name());
	}
	else if (type == cl_text("label"))
	{
		CL_Label *co = dynamic_cast<CL_Label*>(comp);
		add_field(cl_text("Text"), co->get_text());
	}
	else if (type == cl_text("statusbar"))
	{
		CL_StatusBar *co = dynamic_cast<CL_StatusBar*>(comp);
	}
	else if (type == cl_text("frame"))
	{
		CL_Frame *co = dynamic_cast<CL_Frame*>(comp);
		add_field(cl_text("Text"), co->get_header_text());
	}
	else if (type == cl_text("slider"))
	{
		CL_Slider *co = dynamic_cast<CL_Slider*>(comp);
		add_field(cl_text("Min"), CL_StringHelp::int_to_text(co->get_min()));
		add_field(cl_text("Max"), CL_StringHelp::int_to_text(co->get_max()));
		add_field(cl_text("Tick Count"), CL_StringHelp::int_to_text(co->get_tick_count()));
		add_field(cl_text("Step Size"), CL_StringHelp::int_to_text(co->get_page_step()));
	}
	else if (type == cl_text("grid"))
	{
		GridComponent *co = dynamic_cast<GridComponent*>(comp);
		CL_Size s = co->get_boundary().get_size();
		add_field(cl_text("Width"), CL_StringHelp::int_to_text(s.width));
		add_field(cl_text("Height"), CL_StringHelp::int_to_text(s.height));
	}
	else if (type == cl_text("menubar"))
	{
	}
	else if (type == cl_text("spin"))
	{
	}
	else if (type == cl_text("statusbar"))
	{
	}
	else if (type == cl_text("combobox"))
	{
	}
	else if (type == cl_text("listview"))
	{
	}
	else if (type == cl_text("tab"))
	{
		CL_Tab *tab = dynamic_cast<CL_Tab*>(comp);
		int current_page = tab->get_current_page_index();
		CL_TabPage *tab_page = tab->get_page(current_page);
		add_field(cl_text("Page label"), tab_page->get_label());
	}
	else
	{
		add_field(cl_text("Type"), comp->get_type_name());
	}

	add_field(cl_text("#id"), comp->get_id_name());
	add_field(cl_text(".class"), comp->get_class_name());

	// Layout
	if (type != cl_text("grid"))
	{
		add_anchor_radios();
	}

	// Other non-lineedit controls go here...
	if (type == cl_text("tab"))
	{
		CL_Rect child_rect = get_child_rect(container);

		CL_PushButton *add_tab = new CL_PushButton(container);
		add_tab->set_geometry(CL_RectPS(child_rect.left, child_rect.bottom+10, 80,24));
		add_tab->set_text(cl_text("Add tab"));

		CL_PushButton *del_tab = new CL_PushButton(container);
		del_tab->set_geometry(CL_RectPS(child_rect.left, child_rect.bottom+10+32, 80,24));
		del_tab->set_text(cl_text("Delete tab"));

		CL_Tab *tab = dynamic_cast<CL_Tab*>(comp);
		add_tab->func_clicked().set(this, &PropertyComponent::on_add_tab, tab);
		del_tab->func_clicked().set(this, &PropertyComponent::on_delete_tab, tab);
	}

	// Callback functions:
	add_header(type + cl_text(" Callbacks:"));
	std::vector<CallbackData> callbacks = callback_info.get_callbacks(type);
	std::vector<CallbackData>::iterator it_cb;
	for (it_cb = callbacks.begin(); it_cb != callbacks.end(); ++it_cb)
	{
		CallbackData &cb = (*it_cb);
		add_field(cb.get_callback_name(), cb.get_handler_function_name());
	}

	add_header(cl_text("Component Callbacks:"));
	callbacks = callback_info.get_callbacks(cl_text("gui_component"));
	for (it_cb = callbacks.begin(); it_cb != callbacks.end(); ++it_cb)
	{
		CallbackData &cb = (*it_cb);
		add_field(cb.get_callback_name(), cb.get_handler_function_name());
	}

}

void PropertyComponent::on_property_changed(CL_InputEvent event)
{
	CL_GUIComponent *comp = selected_component;
	if (comp == 0)
	{
		return;
	}

	if (comp->get_type_name() == cl_text("button"))
	{
		CL_PushButton *co = dynamic_cast<CL_PushButton*>(comp);
		co->set_text(properties[cl_text("Text")]->get_text());
	}
	else if (comp->get_type_name() == cl_text("radiobutton"))
	{
		CL_RadioButton *co = dynamic_cast<CL_RadioButton*>(comp);
		co->set_text(properties[cl_text("Text")]->get_text());
		co->set_group_name(properties["Group Name"]->get_text());
	}
	else if (comp->get_type_name() == cl_text("checkbox"))
	{
		CL_CheckBox *co = dynamic_cast<CL_CheckBox*>(comp);
		co->set_text(properties[cl_text("Text")]->get_text());
	}
	else if (comp->get_type_name() == cl_text("label"))
	{
		CL_Label *co = dynamic_cast<CL_Label*>(comp);
		co->set_text(properties[cl_text("Text")]->get_text());
	}
	else if (comp->get_type_name() == cl_text("frame"))
	{
		CL_Frame *co = dynamic_cast<CL_Frame*>(comp);
		co->set_header_text(properties[cl_text("Text")]->get_text());
	}
	else if (comp->get_type_name() == cl_text("statusbar"))
	{
		CL_StatusBar *co = dynamic_cast<CL_StatusBar*>(comp);
	}
	else if (comp->get_type_name() == cl_text("menubar"))
	{
		CL_MenuBar *co = dynamic_cast<CL_MenuBar*>(comp);
	}
	else if (comp->get_type_name() == cl_text("spin"))
	{
		CL_Spin *co = dynamic_cast<CL_Spin*>(comp);
	}
	else if (comp->get_type_name() == cl_text("combobox"))
	{
		CL_ComboBox *co = dynamic_cast<CL_ComboBox*>(comp);
	}
	else if (comp->get_type_name() == cl_text("lineedit"))
	{
		CL_LineEdit *co = dynamic_cast<CL_LineEdit*>(comp);
		co->set_text(properties[cl_text("Text")]->get_text());
	}
	else if (comp->get_type_name() == cl_text("slider"))
	{
		CL_Slider *co = dynamic_cast<CL_Slider*>(comp);
		co->set_min(CL_StringHelp::text_to_int(properties[cl_text("Min")]->get_text()));
		co->set_max(CL_StringHelp::text_to_int(properties[cl_text("Max")]->get_text()));
		co->set_tick_count(CL_StringHelp::text_to_int(properties[cl_text("Tick Count")]->get_text()));
		co->set_page_step(CL_StringHelp::text_to_int(properties[cl_text("Step Size")]->get_text()));
	}
	else if (comp->get_type_name() == cl_text("tab"))
	{
		CL_Tab *co = dynamic_cast<CL_Tab*>(comp);
		int index = co->get_current_page_index();
		CL_String new_label = properties[cl_text("Page label")]->get_text();
		co->set_label(index, new_label);
	}
	else if (comp->get_type_name() == cl_text("grid"))
	{
		GridComponent *co = dynamic_cast<GridComponent*>(comp);
		CL_Size s;
		s.width = CL_StringHelp::text_to_int(properties[cl_text("Width")]->get_text());
		s.height = CL_StringHelp::text_to_int(properties[cl_text("Height")]->get_text());
		co->set_boundary_size(s);
	}
	else // custom component
	{
		CustomComponent *co = dynamic_cast<CustomComponent*>(comp);
		if (co != 0)
		{
			co->set_type_name(properties[cl_text("Type")]->get_text());
		}
	}

	if (comp->get_type_name() != cl_text("grid"))
	{
		CL_Rect g = comp->get_geometry();

		int new_width = CL_StringHelp::text_to_int(properties[cl_text("Width")]->get_text());
		int new_height = CL_StringHelp::text_to_int(properties[cl_text("Height")]->get_text());

		// set holder size first to avoid scaling of held component caused by
		HolderComponent *holder = dynamic_cast<HolderComponent*>(comp->get_parent_component());
		if (holder)
		{
			CL_Rect hg = holder->get_geometry();
			holder->set_geometry(CL_RectPS(hg.left, hg.top, new_width+8, new_height+8));
		}

		comp->set_geometry(CL_RectPS(g.left, g.top, new_width, new_height));
	}

	comp->set_id_name(properties[cl_text("#id")]->get_text());
	comp->set_class_name(properties[cl_text(".class")]->get_text());

	// Callbacks
	std::vector<CallbackData> callbacks = callback_info.get_callbacks(comp->get_type_name());
	std::vector<CallbackData>::iterator it_cb;
	for (it_cb = callbacks.begin(); it_cb != callbacks.end(); ++it_cb)
	{
		CallbackData &cb = (*it_cb);
		cb.set_handler_function_name(properties[cb.get_callback_name()]->get_text());
	}

	callbacks = callback_info.get_callbacks(cl_text("gui_component"));
	for (it_cb = callbacks.begin(); it_cb != callbacks.end(); ++it_cb)
	{
		CallbackData &cb = (*it_cb);
		cb.set_handler_function_name(properties[cb.get_callback_name()]->get_text());
	}
}

void PropertyComponent::add_field(const CL_StringRef &property_name, const CL_StringRef &value) 
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
	le->func_after_edit_changed().set(this, &PropertyComponent::on_property_changed);
	properties[property_name] = le;
}

void PropertyComponent::add_anchor_radios() 
{
	CL_Rect g = get_geometry().get_size();
	CL_Rect rect = get_child_rect(container);
	int top = rect.bottom + 4;

	CL_Frame *frame = new CL_Frame(container);
	frame->set_geometry(CL_Rect(5,top, g.right-8, top+100));
	frame->set_header_text(cl_text("Layout:"));

	int left = 12;

	CL_Label *label_tl = new CL_Label(container);
	label_tl->set_text(cl_text("Top Left:"));
	label_tl->set_geometry(CL_RectPS(left+10,top+16, 80,23));

	CL_Label *label_br = new CL_Label(container);
	label_br->set_text(cl_text("Bottom Right:"));
	label_br->set_geometry(CL_RectPS(left+100,top+16, 80,23));

	CL_RadioButton *tl = new CL_RadioButton(container);
	CL_RadioButton *tr = new CL_RadioButton(container);
	CL_RadioButton *bl = new CL_RadioButton(container);
	CL_RadioButton *br = new CL_RadioButton(container);
	CL_RadioButton *scale = new CL_RadioButton(container);
	tl->set_id_name(cl_text("tl")); tr->set_id_name(cl_text("tr")); bl->set_id_name(cl_text("bl")); br->set_id_name(cl_text("br"));
	scale->set_id_name(cl_text("%"));

	CL_Size s = get_geometry().get_size();
	top = rect.bottom+4+32;
	tl->set_geometry(CL_RectPS(        10+left, top,    20, 23));
 	tr->set_geometry(CL_RectPS(     10+50+left, top,    20, 23));
	bl->set_geometry(CL_RectPS(        10+left, top+32, 20, 23));
	br->set_geometry(CL_RectPS(     10+50+left, top+32, 20, 23));
	scale->set_geometry(CL_RectPS(  10+25+left, top+16, 20, 23));

	tl->set_group_name(cl_text("TL"));
	tr->set_group_name(cl_text("TL"));
	bl->set_group_name(cl_text("TL"));
	br->set_group_name(cl_text("TL"));
	scale->set_group_name(cl_text("TL"));

	CL_RadioButton *br_tl = new CL_RadioButton(container);
	CL_RadioButton *br_tr = new CL_RadioButton(container);
	CL_RadioButton *br_bl = new CL_RadioButton(container);
	CL_RadioButton *br_br = new CL_RadioButton(container);
	CL_RadioButton *br_scale = new CL_RadioButton(container);
	br_tl->set_id_name(cl_text("tl")); br_tr->set_id_name(cl_text("tr")); br_bl->set_id_name(cl_text("bl")); br_br->set_id_name(cl_text("br"));
	br_scale->set_id_name(cl_text("%"));

	left += 100;
	br_tl->set_geometry(CL_RectPS(        left, top,    20, 23));
	br_tr->set_geometry(CL_RectPS(     left+50, top,    20, 23));
	br_bl->set_geometry(CL_RectPS(        left, top+32, 20, 23));
	br_br->set_geometry(CL_RectPS(     left+50, top+32, 20, 23));
	br_scale->set_geometry(CL_RectPS(  left+25, top+16, 20, 23));

	br_tl->set_group_name(cl_text("BR"));
	br_tr->set_group_name(cl_text("BR"));
	br_bl->set_group_name(cl_text("BR"));
	br_br->set_group_name(cl_text("BR"));
	br_scale->set_group_name(cl_text("BR"));

	scale->func_group_selection_changed().set(this, &PropertyComponent::on_anchoring_changed);
	br_scale->func_group_selection_changed().set(this, &PropertyComponent::on_anchoring_changed);

	CL_ComponentAnchorPoint cap_tl = selected_holder->get_anchor_tl();
	CL_ComponentAnchorPoint cap_br = selected_holder->get_anchor_br();

	switch (cap_tl)
	{
	case cl_anchor_top_left: tl->set_selected(true); break;
	case cl_anchor_top_right: tr->set_selected(true); break;
	case cl_anchor_bottom_left: bl->set_selected(true); break;
	case cl_anchor_bottom_right: br->set_selected(true); break;
	case cl_anchor_relative: scale->set_selected(true); break;
	default:break;
	}

	switch (cap_br)
	{
	case cl_anchor_top_left: br_tl->set_selected(true); break;
	case cl_anchor_top_right: br_tr->set_selected(true); break;
	case cl_anchor_bottom_left: br_bl->set_selected(true); break;
	case cl_anchor_bottom_right: br_br->set_selected(true); break;
	case cl_anchor_relative: br_scale->set_selected(true); break;
	default:break;
	}
}

CL_Rect PropertyComponent::get_child_rect(CL_GUIComponent *parent) 
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

void PropertyComponent::on_anchoring_changed(CL_RadioButton *rb)
{
	CL_String id = rb->get_id_name();

	if (rb->get_group_name() == cl_text("TL"))
	{
		if (id == cl_text("tl"))
		{
			selected_holder->set_anchor_tl(cl_anchor_top_left);
		}
		else if (id == cl_text("tr"))
		{
			selected_holder->set_anchor_tl(cl_anchor_top_right);
		}
		else if (id == cl_text("bl"))
		{
			selected_holder->set_anchor_tl(cl_anchor_bottom_left);
		}
		else if (id == cl_text("br"))
		{
			selected_holder->set_anchor_tl(cl_anchor_bottom_right);
		}
		else if (id == cl_text("%"))
		{
			selected_holder->set_anchor_tl(cl_anchor_relative);
		}
	}
	else
	{
		if (id == cl_text("tl"))
		{
			selected_holder->set_anchor_br(cl_anchor_top_left);
		}
		else if (id == cl_text("tr"))
		{
			selected_holder->set_anchor_br(cl_anchor_top_right);
		}
		else if (id == cl_text("bl"))
		{
			selected_holder->set_anchor_br(cl_anchor_bottom_left);
		}
		else if (id == cl_text("br"))
		{
			selected_holder->set_anchor_br(cl_anchor_bottom_right);
		}
		else if (id == cl_text("%"))
		{
			selected_holder->set_anchor_br(cl_anchor_relative);
		}
	}
}

void PropertyComponent::on_add_tab(CL_Tab *tab)
{
	int num_pages = tab->get_page_count();
	tab->add_page(cl_format(cl_text("Page %1"), num_pages+1));
}

void PropertyComponent::on_delete_tab(CL_Tab *tab)
{
}

void PropertyComponent::add_header(const CL_StringRef &header)
{
	CL_Label *label_properties_header = new CL_Label(container);
	label_properties_header->set_class_name("properties_header");
	label_properties_header->set_text(header);

	CL_Rect child_rect = get_child_rect(container);
	int y_offset = 10;
	label_properties_header->set_geometry(CL_Rect(4,y_offset+child_rect.bottom,container->get_geometry().get_width()-8, y_offset+child_rect.bottom+label_properties_header->get_preferred_height()));
}



