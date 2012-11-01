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
**    Magnus Norddahl
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_message_close.h"
#include "API/GUI/gui_message_activation_change.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_window_manager.h"
#include "API/GUI/Components/window.h"
#include "API/GUI/Components/label.h"
#include "API/Display/2D/image.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_metrics.h"
#include "../gui_component_impl.h"
#include "../gui_manager_impl.h"
#include "../gui_css_strings.h"
#include "API/Display/2D/canvas.h"

#ifdef INCLUDE_COMPONENTS

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Window_Impl Class:

class Window_Impl
{
public:
	Window_Impl() : draw_caption(false), drag_start(false), draggable(false)
	{
	}

	void check_move_window(std::shared_ptr<GUIMessage> &msg);
	void on_process_message(std::shared_ptr<GUIMessage> &msg);

	Rect get_client_area() const;

	Window *window;

	bool draw_caption;

	bool drag_start;
	Point last_mouse_pos;

	bool draggable;

	Label *part_caption;
	GUIComponent *part_client;
	GUIComponent *part_frameleft;
	GUIComponent *part_frameright;
	GUIComponent *part_framebottom;
	GUIComponent *part_buttonclose;
};

/////////////////////////////////////////////////////////////////////////////
// Window Construction:

Window::Window(GUIComponent *owner, const GUITopLevelDescription &description)
: GUIComponent(owner, description), impl(new Window_Impl)
{
	impl->window = this;

	if (owner->get_gui_manager().get_window_manager().get_window_manager_type() == GUIWindowManager::cl_wm_type_system)
		impl->draw_caption = false;
	else
		impl->draw_caption = description.has_caption();

	func_process_message().set(impl.get(), &Window_Impl::on_process_message);

	set_tag_name(CssStr::Window::type_name);

	impl->part_caption = new Label(this);
	impl->part_caption->set_tag_name(CssStr::Window::part_caption);
	impl->part_caption->set_text(description.get_title());
	impl->part_client = new GUIComponent(this);
	impl->part_frameleft = new GUIComponent(this);
	impl->part_frameleft->set_tag_name(CssStr::Window::part_frameleft);
	impl->part_frameright = new GUIComponent(this);
	impl->part_frameright->set_tag_name(CssStr::Window::part_frameright);
	impl->part_framebottom = new GUIComponent(this);
	impl->part_framebottom->set_tag_name(CssStr::Window::part_framebottom);
	impl->part_buttonclose = new GUIComponent(impl->part_caption);
	impl->part_buttonclose->set_tag_name(CssStr::Window::part_buttonclose);

	if (!impl->draw_caption)
		impl->part_caption->set_visible(false);
}

Window::Window(GUIManager *manager, const GUITopLevelDescription &description)
: GUIComponent(manager, description), impl(new Window_Impl)
{
	impl->window = this;

	if (manager->get_window_manager().get_window_manager_type() == GUIWindowManager::cl_wm_type_system)
		impl->draw_caption = false;
	else
		impl->draw_caption = description.has_caption();

	func_process_message().set(impl.get(), &Window_Impl::on_process_message);

	set_tag_name(CssStr::Window::type_name);

	impl->part_caption = new Label(this);
	impl->part_caption->set_tag_name(CssStr::Window::part_caption);
	impl->part_caption->set_text(description.get_title());
	impl->part_client = new GUIComponent(this);
	impl->part_frameleft = new GUIComponent(this);
	impl->part_frameleft->set_tag_name(CssStr::Window::part_frameleft);
	impl->part_frameright = new GUIComponent(this);
	impl->part_frameright->set_tag_name(CssStr::Window::part_frameright);
	impl->part_framebottom = new GUIComponent(this);
	impl->part_framebottom->set_tag_name(CssStr::Window::part_framebottom);
	impl->part_buttonclose = new GUIComponent(impl->part_caption);
	impl->part_buttonclose->set_tag_name(CssStr::Window::part_buttonclose);

	if (!impl->draw_caption)
		impl->part_caption->set_visible(false);
}

Window::~Window()
{
}

/////////////////////////////////////////////////////////////////////////////
// Window Attributes:

std::string Window::get_title() const 
{
	return impl->part_caption->get_text();
}

bool Window::get_draggable() const 
{
	return impl->draggable;
}

Rect Window::get_client_area() const
{
	return impl->get_client_area();
}

bool Window::is_minimized() const
{
	const GUIComponent *root_component = get_top_level_component();

	std::vector<GUITopLevelWindow>::size_type pos, size;
	size = GUIComponent::impl->gui_manager.lock()->root_components.size();
	for (pos = 0; pos < size; pos++)
	{
		GUITopLevelWindow *cur = GUIComponent::impl->gui_manager.lock()->root_components[pos];
		if (cur->component == root_component)
			return GUIComponent::impl->gui_manager.lock()->window_manager.is_minimized(cur);
	}

	return false;
}

bool Window::is_maximized() const
{
	const GUIComponent *root_component = get_top_level_component();

	std::vector<GUITopLevelWindow>::size_type pos, size;
	size = GUIComponent::impl->gui_manager.lock()->root_components.size();
	for (pos = 0; pos < size; pos++)
	{
		GUITopLevelWindow *cur = GUIComponent::impl->gui_manager.lock()->root_components[pos];
		if (cur->component == root_component)
			return GUIComponent::impl->gui_manager.lock()->window_manager.is_maximized(cur);
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////
// Window Operations:

void Window::set_title(const std::string &str)
{
	impl->part_caption->set_text(str);
}

void Window::set_draggable(bool enable)
{
	impl->draggable = enable;
}

void Window::bring_to_front()
{
	const GUIComponent *root_component = get_top_level_component();

	std::vector<GUITopLevelWindow>::size_type pos, size;
	size = GUIComponent::impl->gui_manager.lock()->root_components.size();
	for (pos = 0; pos < size; pos++)
	{
		GUITopLevelWindow *cur = GUIComponent::impl->gui_manager.lock()->root_components[pos];
		if (cur->component == root_component)
			return GUIComponent::impl->gui_manager.lock()->window_manager.bring_to_front(cur);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Window Implementation:

void Window_Impl::on_process_message(std::shared_ptr<GUIMessage> &msg)
{
	if (draw_caption)
		check_move_window(msg);

	if (msg.is_type(GUIMessage_Input::get_type_name()))
	{
		GUIMessage_Input input_msg = msg;
		InputEvent e = input_msg.get_event();

		if (e.type == InputEvent::pressed && e.id == mouse_left)
		{
			if (part_buttonclose->get_geometry().contains(e.mouse_pos))
				part_buttonclose->set_pseudo_class(CssStr::pressed, true);
		}
		else if (e.type == InputEvent::released && e.id == mouse_left)
		{
			if(draw_caption)
			{
				part_buttonclose->set_pseudo_class(CssStr::pressed, false);
				if (part_buttonclose->get_geometry().contains(e.mouse_pos))
				{
					if (!window->func_close().is_null() && window->func_close().invoke())
						msg.set_consumed();
				}
			}
		}
		else if (e.type == InputEvent::pointer_moved)
		{
			if(draw_caption)
			{
				part_buttonclose->set_pseudo_class(CssStr::hot, part_buttonclose->get_geometry().contains(e.mouse_pos));
			}
		}
	}
	else if (msg.is_type(GUIMessage_Close::get_type_name()))
	{
		if (!window->func_close().is_null() && window->func_close().invoke())
			msg.set_consumed();
	}
	else if (msg.is_type(GUIMessage_ActivationChange::get_type_name()))
	{
		GUIMessage_ActivationChange ac(msg);
		if (ac.get_activation_type() == GUIMessage_ActivationChange::activation_gained)
		{
			window->GUIComponent::impl->activated = true;
			if (!window->func_activated().is_null() && window->func_activated().invoke())
				msg.set_consumed();
		}
		else if (ac.get_activation_type() == GUIMessage_ActivationChange::activation_lost)
		{
			window->GUIComponent::impl->activated = false;
			if (!window->func_deactivated().is_null() && window->func_deactivated().invoke())
				msg.set_consumed();
		}
	}
}

Rect Window_Impl::get_client_area() const
{
	return part_client->get_geometry();
}

void Window_Impl::check_move_window(std::shared_ptr<GUIMessage> &msg)
{
	if (draggable == false)
	{
		drag_start = false;
		return;
	}

	if (msg.is_type(GUIMessage_Input::get_type_name()))
	{
		GUIMessage_Input input_msg = msg;
		InputEvent e = input_msg.get_event();

		if (e.type == InputEvent::pressed && e.id == mouse_left)
		{
			window->bring_to_front();
			if (part_caption->get_geometry().contains(e.mouse_pos))
			{
				drag_start = true;
				window->capture_mouse(true);
				last_mouse_pos = e.mouse_pos;
			}
		}
		else if (e.type == InputEvent::released && e.id == mouse_left)
		{
			if(drag_start)
			{
				drag_start = false;
				window->capture_mouse(false);
			}
		}
		else if (e.type == InputEvent::pointer_moved && drag_start == true)
		{
			const GUIComponent *root_component = window->get_top_level_component();

			std::vector<GUITopLevelWindow>::size_type pos, size;
			size = window->GUIComponent::impl->gui_manager.lock()->root_components.size();

			for (pos = 0; pos < size; pos++)
			{
				GUITopLevelWindow *cur = window->GUIComponent::impl->gui_manager.lock()->root_components[pos];
				if (cur->component == root_component)
				{
					Rect geometry = window->get_window_geometry();
					geometry.translate(e.mouse_pos.x - last_mouse_pos.x, e.mouse_pos.y - last_mouse_pos.y);
					//last_mouse_pos = e.mouse_pos;
					window->set_window_geometry(geometry);
				}
			}
		}
	}
}

}

#endif
