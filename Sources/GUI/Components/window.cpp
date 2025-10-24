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
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_message_close.h"
#include "API/GUI/gui_message_activation_change.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_theme_part_property.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_window_manager.h"
#include "API/GUI/Components/window.h"
#include "API/Display/2D/draw.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_metrics.h"
#include "API/CSSLayout/css_layout.h"
#include "API/CSSLayout/css_layout_element.h"
#include "../gui_component_impl.h"
#include "../gui_manager_impl.h"
#include "../gui_css_strings.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Window_Impl Class:

class CL_Window_Impl
{
public:
	CL_Window_Impl() : draw_decorations(false), draw_start(false), draggable(false)
	{
		prop_text_color = CL_GUIThemePartProperty(CssStr::text_color, "black");
	}

	void check_move_window(CL_GUIMessage &msg);

	void on_process_message(CL_GUIMessage &msg);
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_style_changed();
	void on_resized();

	void create_parts();

	CL_Rect get_client_area() const;

	CL_Window *window;

	CL_String title;

	CL_Font font;

	CL_Colorf text_color;

	bool draw_decorations;

	bool draw_start;
	CL_Point last_mouse_pos;

	bool draggable;

	CL_Rect part_buttonclose_rect;
	CL_Rect part_caption_rect;

	CL_GUIThemePart part_component;
	CL_GUIThemePart part_caption;
	CL_GUIThemePart part_frameleft;
	CL_GUIThemePart part_frameright;
	CL_GUIThemePart part_framebottom;
	CL_GUIThemePart part_buttonclose;
	CL_GUIThemePartProperty prop_text_color;

	CL_CSSLayout css_layout;
};

/////////////////////////////////////////////////////////////////////////////
// CL_Window Construction:

CL_Window::CL_Window(CL_GUIComponent *owner, const CL_GUITopLevelDescription &description)
: CL_GUIComponent(owner, description), impl(new CL_Window_Impl)
{
	impl->window = this;
	impl->title = description.get_title();

	if (owner->get_gui_manager().get_window_manager().get_window_manager_type() == CL_GUIWindowManager::cl_wm_type_system)
		impl->draw_decorations = false;
	else
		impl->draw_decorations = description.get_decorations();

	func_process_message().set(impl.get(), &CL_Window_Impl::on_process_message);
	func_render().set(impl.get(), &CL_Window_Impl::on_render);
	func_style_changed().set(impl.get(), &CL_Window_Impl::on_style_changed);
	func_resized().set(impl.get(), &CL_Window_Impl::on_resized);

	set_type_name(CssStr::Window::type_name);

	if(get_gui_manager().has_layout(this))
		set_layout(get_gui_manager().create_layout(this));

	impl->create_parts();
}

CL_Window::CL_Window(CL_GUIManager *manager, const CL_GUITopLevelDescription &description)
: CL_GUIComponent(manager, description), impl(new CL_Window_Impl)
{
	impl->window = this;
	impl->title = description.get_title();

	if (manager->get_window_manager().get_window_manager_type() == CL_GUIWindowManager::cl_wm_type_system)
		impl->draw_decorations = false;
	else
		impl->draw_decorations = description.get_decorations();

	func_process_message().set(impl.get(), &CL_Window_Impl::on_process_message);
	func_render().set(impl.get(), &CL_Window_Impl::on_render);
	func_style_changed().set(impl.get(), &CL_Window_Impl::on_style_changed);
	func_resized().set(impl.get(), &CL_Window_Impl::on_resized);

	set_type_name(CssStr::Window::type_name);

	if(get_gui_manager().has_layout(this))
		set_layout(get_gui_manager().create_layout(this));

	impl->create_parts();
}

CL_Window::~CL_Window()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Window Attributes:

CL_StringRef CL_Window::get_title() const 
{
	return impl->title;
}

bool CL_Window::get_draggable() const 
{
	return impl->draggable;
}

CL_Size CL_Window::get_preferred_size() const
{
	return impl->part_component.get_preferred_size();
}

CL_Rect CL_Window::get_client_area() const
{
	return impl->get_client_area();
}

bool CL_Window::is_minimized() const
{
	const CL_GUIComponent *root_component = get_top_level_component();

	std::vector<CL_GUITopLevelWindow>::size_type pos, size;
	size = CL_GUIComponent::impl->gui_manager->root_components.size();
	for (pos = 0; pos < size; pos++)
	{
		CL_GUITopLevelWindow *cur = CL_GUIComponent::impl->gui_manager->root_components[pos];
		if (cur->component == root_component)
			return CL_GUIComponent::impl->gui_manager->window_manager.is_minimized(cur);
	}

	return false;
}

bool CL_Window::is_maximized() const
{
	const CL_GUIComponent *root_component = get_top_level_component();

	std::vector<CL_GUITopLevelWindow>::size_type pos, size;
	size = CL_GUIComponent::impl->gui_manager->root_components.size();
	for (pos = 0; pos < size; pos++)
	{
		CL_GUITopLevelWindow *cur = CL_GUIComponent::impl->gui_manager->root_components[pos];
		if (cur->component == root_component)
			return CL_GUIComponent::impl->gui_manager->window_manager.is_maximized(cur);
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Window Operations:

void CL_Window::set_title(const CL_StringRef &str)
{
	impl->title = str;
}

void CL_Window::set_draggable(bool enable)
{
	impl->draggable = enable;
}

void CL_Window::bring_to_front()
{
	const CL_GUIComponent *root_component = get_top_level_component();

	std::vector<CL_GUITopLevelWindow>::size_type pos, size;
	size = CL_GUIComponent::impl->gui_manager->root_components.size();
	for (pos = 0; pos < size; pos++)
	{
		CL_GUITopLevelWindow *cur = CL_GUIComponent::impl->gui_manager->root_components[pos];
		if (cur->component == root_component)
			return CL_GUIComponent::impl->gui_manager->window_manager.bring_to_front(cur);
	}
}

void CL_Window::set_layout(CL_CSSLayout layout)
{
	impl->css_layout = layout;
	impl->create_parts();
}

/////////////////////////////////////////////////////////////////////////////
// CL_Window Implementation:

void CL_Window_Impl::create_parts()
{
	part_component = CL_GUIThemePart(window);

	if (draw_decorations)
	{
		part_caption = CL_GUIThemePart(window, CssStr::Window::part_caption);
		part_frameleft = CL_GUIThemePart(window, CssStr::Window::part_frameleft);
		part_frameright = CL_GUIThemePart(window, CssStr::Window::part_frameright);
		part_framebottom = CL_GUIThemePart(window, CssStr::Window::part_framebottom);
		part_buttonclose = CL_GUIThemePart(window, CssStr::Window::part_buttonclose);

		part_buttonclose.set_state(CssStr::normal, true);

		CL_Rect rect = window->get_size();

		CL_Size part_buttonclose_size = part_buttonclose.get_preferred_size();
		int frameright_width = part_frameright.get_preferred_width();
		int caption_height = part_caption.get_preferred_height();

		part_buttonclose_rect = CL_Rect(rect.right - part_buttonclose_size.width - frameright_width - 2, rect.top + caption_height - part_buttonclose_size.height - 3, rect.right - frameright_width - 2, rect.top + caption_height - 3);
	}

	font = part_component.get_font();
	text_color = part_component.get_property(prop_text_color);

	if (!css_layout.is_null())
	{
		CL_GraphicContext gc = window->get_gc();
		css_layout.layout(gc, window->get_size());

		part_buttonclose_rect = css_layout.find_element("close").get_content_box();
		part_caption_rect = css_layout.find_element("caption").get_content_box();
	}
}

void CL_Window_Impl::on_style_changed()
{
	create_parts();
}

void CL_Window_Impl::on_resized()
{
	create_parts();
}

void CL_Window_Impl::on_process_message(CL_GUIMessage &msg)
{
	if (draw_decorations)
		check_move_window(msg);

	if (msg.is_type(CL_GUIMessage_Input::get_type_name()))
	{
		CL_GUIMessage_Input input_msg = msg;
		CL_InputEvent e = input_msg.get_event();

		if (e.type == CL_InputEvent::pressed && e.id == CL_MOUSE_LEFT)
		{
			if(part_buttonclose_rect.contains(e.mouse_pos))
				if(part_buttonclose.set_state(CssStr::pressed, true))
					window->request_repaint();
		}
		else if (e.type == CL_InputEvent::released && e.id == CL_MOUSE_LEFT)
		{
			if(draw_decorations)
			{
				if(part_buttonclose.set_state(CssStr::pressed, false))
				{
					window->request_repaint();
					if (!window->func_close().is_null() && window->func_close().invoke())
						msg.set_consumed();
				}
			}
		}
		else if (e.type == CL_InputEvent::pointer_moved)
		{
			if(draw_decorations)
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
	else if (msg.is_type(CL_GUIMessage_Close::get_type_name()))
	{
		if (!window->func_close().is_null() && window->func_close().invoke())
			msg.set_consumed();
	}
	else if (msg.is_type(CL_GUIMessage_ActivationChange::get_type_name()))
	{
		CL_GUIMessage_ActivationChange ac(msg);
		if (ac.get_activation_type() == CL_GUIMessage_ActivationChange::activation_gained)
		{
			window->CL_GUIComponent::impl->activated = true;
			if (!window->func_activated().is_null() && window->func_activated().invoke())
				msg.set_consumed();
		}
		else if (ac.get_activation_type() == CL_GUIMessage_ActivationChange::activation_lost)
		{
			window->CL_GUIComponent::impl->activated = false;
			if (!window->func_deactivated().is_null() && window->func_deactivated().invoke())
				msg.set_consumed();
		}
	}
}

CL_Rect CL_Window_Impl::get_client_area() const
{
	CL_Rect rect = window->get_size();

	if (draw_decorations)
	{
		int caption_height = part_caption.get_preferred_height();
		int frameleft_width = part_frameleft.get_preferred_width();
		int frameright_width = part_frameright.get_preferred_width();
		int framebottom_height = part_framebottom.get_preferred_height();

		return CL_Rect(rect.left + frameleft_width, rect.top + caption_height, rect.right - frameright_width, rect.bottom - framebottom_height);
	}
	else
	{
		return rect;
	}
}

void CL_Window_Impl::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_Rect rect = window->get_size();

	if (draw_decorations)
	{
		int caption_height = part_caption.get_preferred_height();
		int frameleft_width = part_frameleft.get_preferred_width();
		int frameright_width = part_frameright.get_preferred_width();
		int framebottom_height = part_framebottom.get_preferred_height();

		CL_Rect content_rect = CL_Rect(rect.left + frameleft_width, rect.top + caption_height, rect.right - frameright_width, rect.bottom - framebottom_height);
		part_component.render_box(gc, content_rect, update_rect);

		CL_Rect caption_rect = CL_Rect(rect.left, rect.top, rect.right, rect.top + caption_height);
		part_caption.render_box(gc, caption_rect, update_rect);

		CL_Rect frameleft_rect = CL_Rect(rect.left, rect.top + caption_height, rect.left + frameleft_width, rect.bottom - framebottom_height);
		part_frameleft.render_box(gc, frameleft_rect, update_rect);

		CL_Rect frameright_rect = CL_Rect(rect.right - frameright_width, rect.top + caption_height, rect.right, rect.bottom - framebottom_height);
		part_frameright.render_box(gc, frameright_rect, update_rect);

		CL_Rect framebottom_rect = CL_Rect(rect.left, rect.bottom - framebottom_height, rect.right, rect.bottom);
		part_framebottom.render_box(gc, framebottom_rect, update_rect);

		part_buttonclose.render_box(gc, part_buttonclose_rect, update_rect);

		CL_Size text_size = font.get_text_size(gc, title);
		font.draw_text(gc,
			caption_rect.left + 10,
			caption_rect.top + caption_rect.get_height()/2 + text_size.height/2 - 2,
			title,
			text_color);
	}
	else
	{
		part_component.render_box(gc, rect, update_rect);
	}

	if (!css_layout.is_null())
		css_layout.render(gc);
}

void CL_Window_Impl::check_move_window(CL_GUIMessage &msg)
{
	if (draggable == false)
	{
		draw_start = false;
		return;
	}

	if (msg.is_type(CL_GUIMessage_Input::get_type_name()))
	{
		CL_GUIMessage_Input input_msg = msg;
		CL_InputEvent e = input_msg.get_event();

		if (e.type == CL_InputEvent::pressed && e.id == CL_MOUSE_LEFT)
		{
			window->bring_to_front();
			CL_Rect rect = window->get_size();
			int caption_height = part_caption.get_preferred_height();
			CL_Rect caption_rect = CL_Rect(rect.left, rect.top, rect.right, rect.top + caption_height);
			if (caption_rect.contains(e.mouse_pos))
			{
				draw_start = true;
				window->capture_mouse(true);
				last_mouse_pos = e.mouse_pos;
			}
		}
		else if (e.type == CL_InputEvent::released && e.id == CL_MOUSE_LEFT)
		{
			if(draw_start)
			{
				draw_start = false;
				window->capture_mouse(false);
			}
		}
		else if (e.type == CL_InputEvent::pointer_moved && draw_start == true)
		{
			const CL_GUIComponent *root_component = window->get_top_level_component();

			std::vector<CL_GUITopLevelWindow>::size_type pos, size;
			size = window->CL_GUIComponent::impl->gui_manager->root_components.size();

			for (pos = 0; pos < size; pos++)
			{
				CL_GUITopLevelWindow *cur = window->CL_GUIComponent::impl->gui_manager->root_components[pos];
				if (cur->component == root_component)
				{
					CL_Rect geometry = window->get_window_geometry();
					geometry.translate(e.mouse_pos.x - last_mouse_pos.x, e.mouse_pos.y - last_mouse_pos.y);
					//last_mouse_pos = e.mouse_pos;
					window->set_window_geometry(geometry);
				}
			}
		}
	}
}
