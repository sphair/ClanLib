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

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Window_Impl Class:

class Window_Impl
{
public:
	Window_Impl() : draw_caption(false), draw_start(false), draggable(false)
	{
		prop_text_color = GUIThemePartProperty(CssStr::text_color, "black");
	}

	void check_move_window(GUIMessage &msg);

	void on_process_message(GUIMessage &msg);
	void on_render(Canvas &canvas, const Rect &update_rect);
	void on_style_changed();
	void on_resized();

	void create_parts();

	Rect get_client_area() const;

	Window *window;

	std::string title;

	Font font;

	Colorf text_color;

	bool draw_caption;

	bool draw_start;
	Point last_mouse_pos;

	bool draggable;

	Rect part_buttonclose_rect;
	Rect part_caption_rect;

	GUIThemePart part_component;
	GUIThemePart part_caption;
	GUIThemePart part_frameleft;
	GUIThemePart part_frameright;
	GUIThemePart part_framebottom;
	GUIThemePart part_buttonclose;
	GUIThemePartProperty prop_text_color;
};

/////////////////////////////////////////////////////////////////////////////
// Window Construction:

Window::Window(GUIComponent *owner, const GUITopLevelDescription &description)
: GUIComponent(owner, description), impl(new Window_Impl)
{
	impl->window = this;
	impl->title = description.get_title();

	if (owner->get_gui_manager().get_window_manager().get_window_manager_type() == GUIWindowManager::cl_wm_type_system)
		impl->draw_caption = false;
	else
		impl->draw_caption = description.has_caption();

	func_process_message().set(impl.get(), &Window_Impl::on_process_message);
	func_render().set(impl.get(), &Window_Impl::on_render);
	func_style_changed().set(impl.get(), &Window_Impl::on_style_changed);
	func_resized().set(impl.get(), &Window_Impl::on_resized);

	set_tag_name(CssStr::Window::type_name);

	impl->create_parts();
}

Window::Window(GUIManager *manager, const GUITopLevelDescription &description)
: GUIComponent(manager, description), impl(new Window_Impl)
{
	impl->window = this;
	impl->title = description.get_title();

	if (manager->get_window_manager().get_window_manager_type() == GUIWindowManager::cl_wm_type_system)
		impl->draw_caption = false;
	else
		impl->draw_caption = description.has_caption();

	func_process_message().set(impl.get(), &Window_Impl::on_process_message);
	func_render().set(impl.get(), &Window_Impl::on_render);
	func_style_changed().set(impl.get(), &Window_Impl::on_style_changed);
	func_resized().set(impl.get(), &Window_Impl::on_resized);

	set_tag_name(CssStr::Window::type_name);

	impl->create_parts();
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

Size Window::get_preferred_size() const
{
	return impl->part_component.get_preferred_size();
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
	part_component = GUIThemePart(window);

	if (draw_caption)
	{
		part_caption = GUIThemePart(window, CssStr::Window::part_caption);
		part_frameleft = GUIThemePart(window, CssStr::Window::part_frameleft);
		part_frameright = GUIThemePart(window, CssStr::Window::part_frameright);
		part_framebottom = GUIThemePart(window, CssStr::Window::part_framebottom);
		part_buttonclose = GUIThemePart(window, CssStr::Window::part_buttonclose);

		part_buttonclose.set_state(CssStr::normal, true);

		Rect rect = window->get_size();

		Size part_buttonclose_size = part_buttonclose.get_preferred_size();
		int frameright_width = part_frameright.get_preferred_width();
		int caption_height = part_caption.get_preferred_height();

		part_buttonclose_rect = Rect(rect.right - part_buttonclose_size.width - frameright_width - 2, rect.top + caption_height - part_buttonclose_size.height - 3, rect.right - frameright_width - 2, rect.top + caption_height - 3);
	}

	font = part_component.get_font();
	text_color = part_component.get_property(prop_text_color);
}

void Window_Impl::on_style_changed()
{
	create_parts();
}

void Window_Impl::on_resized()
{
	create_parts();
}

void Window_Impl::on_process_message(GUIMessage &msg)
{
	if (draw_caption)
		check_move_window(msg);

	if (msg.is_type(GUIMessage_Input::get_type_name()))
	{
		GUIMessage_Input input_msg = msg;
		InputEvent e = input_msg.get_event();

		if (e.type == InputEvent::pressed && e.id == mouse_left)
		{
			if(part_buttonclose_rect.contains(e.mouse_pos))
				if(part_buttonclose.set_state(CssStr::pressed, true))
					window->request_repaint();
		}
		else if (e.type == InputEvent::released && e.id == mouse_left)
		{
			if(draw_caption)
			{
				if(part_buttonclose.set_state(CssStr::pressed, false))
				{
					window->request_repaint();
					if (!window->func_close().is_null() && window->func_close().invoke())
						msg.set_consumed();
				}
			}
		}
		else if (e.type == InputEvent::pointer_moved)
		{
			if(draw_caption)
			{
				bool inside = part_buttonclose_rect.contains(e.mouse_pos);

				if (part_buttonclose.get_state(CssStr::hot) != inside)
				{
					part_buttonclose.set_state(CssStr::hot, inside);
					window->request_repaint();
				}
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
	Rect rect = window->get_size();

	if (draw_caption)
	{
		int caption_height = part_caption.get_preferred_height();
		int frameleft_width = part_frameleft.get_preferred_width();
		int frameright_width = part_frameright.get_preferred_width();
		int framebottom_height = part_framebottom.get_preferred_height();

		return Rect(rect.left + frameleft_width, rect.top + caption_height, rect.right - frameright_width, rect.bottom - framebottom_height);
	}
	else
	{
		return rect;
	}
}

void Window_Impl::on_render(Canvas &canvas, const Rect &update_rect)
{
	Rect rect = window->get_size();

	if (draw_caption)
	{
		int caption_height = part_caption.get_preferred_height();
		int frameleft_width = part_frameleft.get_preferred_width();
		int frameright_width = part_frameright.get_preferred_width();
		int framebottom_height = part_framebottom.get_preferred_height();

		Rect content_rect = Rect(rect.left + frameleft_width, rect.top + caption_height, rect.right - frameright_width, rect.bottom - framebottom_height);
		part_component.render_box(canvas, content_rect, update_rect);

		Rect caption_rect = Rect(rect.left, rect.top, rect.right, rect.top + caption_height);
		part_caption.render_box(canvas, caption_rect, update_rect);

		Rect frameleft_rect = Rect(rect.left, rect.top + caption_height, rect.left + frameleft_width, rect.bottom - framebottom_height);
		part_frameleft.render_box(canvas, frameleft_rect, update_rect);

		Rect frameright_rect = Rect(rect.right - frameright_width, rect.top + caption_height, rect.right, rect.bottom - framebottom_height);
		part_frameright.render_box(canvas, frameright_rect, update_rect);

		Rect framebottom_rect = Rect(rect.left, rect.bottom - framebottom_height, rect.right, rect.bottom);
		part_framebottom.render_box(canvas, framebottom_rect, update_rect);

		part_buttonclose.render_box(canvas, part_buttonclose_rect, update_rect);

		Size text_size = font.get_text_size(canvas, title);
		font.draw_text(canvas,
			caption_rect.left + 10,
			caption_rect.top + caption_rect.get_height()/2 + text_size.height/2 - 2,
			title,
			text_color);
	}
	else
	{
		part_component.render_box(canvas, rect, update_rect);
	}
}

void Window_Impl::check_move_window(GUIMessage &msg)
{
	if (draggable == false)
	{
		draw_start = false;
		return;
	}

	if (msg.is_type(GUIMessage_Input::get_type_name()))
	{
		GUIMessage_Input input_msg = msg;
		InputEvent e = input_msg.get_event();

		if (e.type == InputEvent::pressed && e.id == mouse_left)
		{
			window->bring_to_front();
			Rect rect = window->get_size();
			int caption_height = part_caption.get_preferred_height();
			Rect caption_rect = Rect(rect.left, rect.top, rect.right, rect.top + caption_height);
			if (caption_rect.contains(e.mouse_pos))
			{
				draw_start = true;
				window->capture_mouse(true);
				last_mouse_pos = e.mouse_pos;
			}
		}
		else if (e.type == InputEvent::released && e.id == mouse_left)
		{
			if(draw_start)
			{
				draw_start = false;
				window->capture_mouse(false);
			}
		}
		else if (e.type == InputEvent::pointer_moved && draw_start == true)
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
