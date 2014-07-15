/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
**    Mark Page
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_message_close.h"
#include "API/GUI/gui_message_activation_change.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_message_pointer.h"
#include "API/GUI/gui_message_focus_change.h"
#include "API/GUI/gui_window_manager.h"
#include "API/GUI/Components/window.h"
#include "API/Display/2D/image.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_metrics.h"
#include "../gui_component_impl.h"
#include "../gui_manager_impl.h"
#include "../gui_css_strings.h"
namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Window_Impl Class:

class Window_Impl
{
public:
	Window_Impl() : drag_start(false), draggable(true), has_frame(true) {}

	void init(Window *this_component);
	void check_move_window(std::shared_ptr<GUIMessage> &msg);

	void on_process_message(std::shared_ptr<GUIMessage> &msg);
	void on_render(Canvas &canvas, const Rect &update_rect);

	Rect get_part_buttonclose_rect() const;

	void create_parts();

	Rect get_client_area() const;

	Window *window;

	std::string title;

	bool drag_start;
	Point last_mouse_pos;

	bool draggable;

	bool has_frame;	// Only set by the constructor, or bad things will happen

	GUIThemePart part_caption;
	GUIThemePart part_frameleft;
	GUIThemePart part_frameright;
	GUIThemePart part_framebottom;
	GUIThemePart part_buttonclose;
};

/////////////////////////////////////////////////////////////////////////////
// Window Construction:

Window::Window(GUIComponent *owner, const GUITopLevelDescription &description)
: GUIComponent(owner, description, CssStr::Window::type_name), impl(std::make_shared<Window_Impl>())
{
	impl->init(this);
	impl->title = description.get_title();
}

Window::Window(GUIComponent *parent)
: GUIComponent(parent, CssStr::Window::type_name), impl(std::make_shared<Window_Impl>())
{
	impl->init(this);
}

Window::Window(GUIManager *manager, const GUITopLevelDescription &description)
: GUIComponent(manager, description, CssStr::Window::type_name), impl(std::make_shared<Window_Impl>())
{
	if (manager->get_window_manager().get_window_manager_type() == GUIWindowManager::cl_wm_type_system)
		impl->has_frame = false;

	impl->init(this);
	impl->title = description.get_title();
}

void Window_Impl::init(Window *this_component)
{
	window = this_component;

	this_component->func_process_message() = bind_member(this, &Window_Impl::on_process_message);
	this_component->func_render() = bind_member(this, &Window_Impl::on_render);
	
	create_parts();
}

Window::~Window()
{
}

/////////////////////////////////////////////////////////////////////////////
// Window Attributes:

std::string Window::get_title() const 
{
	return impl->title;
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
	impl->title = str;
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

void Window_Impl::create_parts()
{
	if (has_frame)
	{
		part_caption = GUIThemePart(window, CssStr::Window::part_caption);
		part_frameleft = GUIThemePart(window, CssStr::Window::part_frameleft);
		part_frameright = GUIThemePart(window, CssStr::Window::part_frameright);
		part_framebottom = GUIThemePart(window, CssStr::Window::part_framebottom);
		part_buttonclose = GUIThemePart(window, CssStr::Window::part_buttonclose);

		part_buttonclose.set_pseudo_class(CssStr::normal, true);
	}

}

void Window_Impl::on_process_message(std::shared_ptr<GUIMessage> &msg)
{
	if (!window->is_enabled())
		return;
	if (!has_frame)		// Performed by the OS on system wm
		return;

	check_move_window(msg);
	Rect part_buttonclose_rect = get_part_buttonclose_rect();

	std::shared_ptr<GUIMessage_Input> input_msg = std::dynamic_pointer_cast<GUIMessage_Input>(msg);
	if (input_msg)
	{
		InputEvent &e = input_msg->input_event;

		if (e.type == InputEvent::pressed && e.id == mouse_left)
		{
			if(part_buttonclose_rect.contains(e.mouse_pos))
				if(part_buttonclose.set_pseudo_class(CssStr::pressed, true))
					window->request_repaint();
		}
		else if (e.type == InputEvent::released && e.id == mouse_left)
		{
			if(part_buttonclose.set_pseudo_class(CssStr::pressed, false))
			{
				window->request_repaint();
				if (window->func_close() && window->func_close()())
					msg->consumed = true;
			}
		}
		else if (e.type == InputEvent::pointer_moved)
		{
			bool inside = part_buttonclose_rect.contains(e.mouse_pos);

			if (part_buttonclose.get_pseudo_class(CssStr::hover) != inside)
			{
				part_buttonclose.set_pseudo_class(CssStr::hover, inside);
				window->request_repaint();
			}
		}
	}

}

Rect Window_Impl::get_client_area() const
{
	Rect rect = window->get_size();
	if (!has_frame)
		return rect;

	int caption_height = part_caption.get_css_height();
	int frameleft_width = part_frameleft.get_css_width();
	int frameright_width = part_frameright.get_css_width();
	int framebottom_height = part_framebottom.get_css_height();

	return Rect(rect.left + frameleft_width, rect.top + caption_height, rect.right - frameright_width, rect.bottom - framebottom_height);
}

Rect Window_Impl::get_part_buttonclose_rect() const
{
	Rect rect = window->get_size();

	Size part_buttonclose_size = part_buttonclose.get_css_size();
	int frameright_width = part_frameright.get_css_width();
	int caption_height = part_caption.get_css_height();

	return Rect(rect.right - part_buttonclose_size.width - frameright_width - 2, rect.top + caption_height - part_buttonclose_size.height - 3, rect.right - frameright_width - 2, rect.top + caption_height - 3);

}

void Window_Impl::on_render(Canvas &canvas, const Rect &update_rect)
{
	if (!has_frame)
		return;

	Rect rect = window->get_size();

	int caption_height = part_caption.get_css_height();
	int frameleft_width = part_frameleft.get_css_width();
	int frameright_width = part_frameright.get_css_width();
	int framebottom_height = part_framebottom.get_css_height();

	Rect caption_rect = Rect(rect.left, rect.top, rect.right, rect.top + caption_height);
	part_caption.render_box(canvas, caption_rect);

	Rect frameleft_rect = Rect(rect.left, rect.top + caption_height, rect.left + frameleft_width, rect.bottom - framebottom_height);
	part_frameleft.render_box(canvas, frameleft_rect);

	Rect frameright_rect = Rect(rect.right - frameright_width, rect.top + caption_height, rect.right, rect.bottom - framebottom_height);
	part_frameright.render_box(canvas, frameright_rect);

	Rect framebottom_rect = Rect(rect.left, rect.bottom - framebottom_height, rect.right, rect.bottom);
	part_framebottom.render_box(canvas, framebottom_rect);

	Rect part_buttonclose_rect = get_part_buttonclose_rect();
	part_buttonclose.render_box(canvas, part_buttonclose_rect);

	Size text_size = window->get_render_text_size(canvas, title);
	window->render_text(canvas, title,
		caption_rect.left + 10,
		caption_rect.top + caption_rect.get_height()/2 + text_size.height/2 - 2);
}

void Window_Impl::check_move_window(std::shared_ptr<GUIMessage> &msg)
{
	if (draggable == false)
	{
		drag_start = false;
		return;
	}

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
			window->bring_to_front();
			Rect rect = window->get_size();
			int caption_height = part_caption.get_css_height();
			Rect caption_rect = Rect(rect.left, rect.top, rect.right, rect.top + caption_height);
			if (caption_rect.contains(e.mouse_pos))
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
			if (window->get_parent_component())
			{
				Rect geometry = window->get_geometry();
				geometry.translate(e.mouse_pos.x - last_mouse_pos.x, e.mouse_pos.y - last_mouse_pos.y);
				window->set_geometry(geometry);
			}
			else
			{
				Rect geometry = window->get_window_geometry();
				geometry.translate(e.mouse_pos.x - last_mouse_pos.x, e.mouse_pos.y - last_mouse_pos.y);
				window->set_window_geometry(geometry);
			}
		}
	}
}

}

