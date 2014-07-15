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
**    Mark Page
*/

#include "GUI/precomp.h"
#include "API/Core/Text/string_format.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_message_pointer.h"
#include "API/GUI/gui_message_focus_change.h"
#include "API/GUI/Components/dragbox.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "../gui_css_strings.h"
#include "API/Display/2D/canvas.h"
#include "API/GUI/gui_theme_part.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// DragBox_Impl Class:

class DragBox_Impl
{
public:
	DragBox_Impl() : drag_start(false) {}

	void init(DragBox *this_component);

	void on_process_message(std::shared_ptr<GUIMessage> &msg);
	DragBox *component;
	bool drag_start;
	Point last_mouse_pos;

};

/////////////////////////////////////////////////////////////////////////////
// DragBox Construction:

DragBox::DragBox(GUIComponent *parent)
: GUIComponent(parent, CssStr::DragBox::type_name), impl(std::make_shared<DragBox_Impl>())
{
	impl->init(this);
}

DragBox::DragBox(GUIManager *manager, const GUITopLevelDescription &description)
	: GUIComponent(manager, description, CssStr::DragBox::type_name), impl(std::make_shared<DragBox_Impl>())
{
	impl->init(this);
}

void DragBox_Impl::init(DragBox *this_component)
{
	component = this_component;

	this_component->set_blocks_default_action(true);
	this_component->set_focus_policy(GUIComponent::focus_local);
	this_component->set_double_click_enabled(false);

	this_component->func_process_message() = bind_member(this, &DragBox_Impl::on_process_message);

	this_component->set_pseudo_class(CssStr::disabled, !this_component->is_enabled());
}

DragBox::~DragBox()
{
}

/////////////////////////////////////////////////////////////////////////////
// DragBox Attributes:

DragBox *DragBox::get_named_item(GUIComponent *reference_component, const std::string &id)
{
	DragBox *object = NULL;
	if (reference_component)
		object = dynamic_cast<DragBox*>(reference_component->get_named_item(id));

	if (!object)
		throw Exception(string_format("Cannot find DragBox named item: %1", id));

	return object;
}

/////////////////////////////////////////////////////////////////////////////
// DragBox Operations:

/////////////////////////////////////////////////////////////////////////////
// DragBox Events:

/////////////////////////////////////////////////////////////////////////////
// DragBox Implementation:

void DragBox_Impl::on_process_message(std::shared_ptr<GUIMessage> &msg)
{
	if (!component->is_enabled())
		return;

	std::shared_ptr<GUIMessage_Pointer> pointer = std::dynamic_pointer_cast<GUIMessage_Pointer>(msg);
	if (pointer)
	{
		msg->consumed = true;
	}

	std::shared_ptr<GUIMessage_Input> input_msg = std::dynamic_pointer_cast<GUIMessage_Input>(msg);
	if (input_msg)
	{
		InputEvent &e = input_msg->input_event;

		if (e.type == InputEvent::pressed && e.id == mouse_left)
		{
			Rect rect = component->get_content_box();
			if (rect.contains(e.mouse_pos))
			{
				drag_start = true;
				component->capture_mouse(true);
				last_mouse_pos = e.mouse_pos;
			}
		}
		else if (e.type == InputEvent::released && e.id == mouse_left)
		{
			if(drag_start)
			{
				drag_start = false;
				component->capture_mouse(false);
			}
		}
		else if (e.type == InputEvent::pointer_moved && drag_start == true)
		{
			if (component->get_parent_component())
			{
				Rect geometry = component->get_geometry();
				geometry.translate(e.mouse_pos.x - last_mouse_pos.x, e.mouse_pos.y - last_mouse_pos.y);
				component->set_geometry(geometry);
			}
			else
			{
				Rect geometry = component->get_window_geometry();
				geometry.translate(e.mouse_pos.x - last_mouse_pos.x, e.mouse_pos.y - last_mouse_pos.y);
				component->set_window_geometry(geometry);
			}
		}
	}

}

}
