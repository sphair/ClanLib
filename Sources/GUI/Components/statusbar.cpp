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
**    Magnus Norddahl
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/Components/statusbar.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "API/Display/Font/font.h"
#include "API/Display/2D/image.h"
#include "API/Core/Text/string_format.h"
#include "../gui_css_strings.h"
#include "API/CSSLayout/css_box_properties.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// StatusBar_Impl Class:

class StatusBar_Part
{
public:
	StatusBar_Part(int id, int width, GUIComponent *component)
	: id(id), width(width), component(component), visible(true)
	{
	}

	int id;
	int width;
	Image icon;
	std::string text;
	GUIComponent *component;
	bool visible;
	Rect position;
	Callback_v0 func_double_clicked;
};

class StatusBar_Impl
{
public:
	StatusBar_Impl() : show_size_grip(true)
	{
	}

	bool on_input_doubleclick(const InputEvent &input_event);
	void on_resized();
	void on_render(Canvas &canvas, const Rect &update_rect);
	void position_parts();
	void create_parts();
	unsigned int find_part(int id) const;
	void throw_if_part_exists(int id) const;

	std::string status_text;
	StatusBar *statusbar;
	
	GUIThemePart part_status_text;
	GUIThemePart part_status_part;
	GUIThemePart part_size_grip;
	std::vector<StatusBar_Part> statusbar_parts;
	bool show_size_grip;
};

/////////////////////////////////////////////////////////////////////////////
// StatusBar Construction:

StatusBar::StatusBar(GUIComponent *parent)
: GUIComponent(parent, CssStr::StatusBar::type_name), impl(new StatusBar_Impl)
{
	impl->statusbar = this;

	func_resized().set(impl.get(), &StatusBar_Impl::on_resized);
	func_render().set(impl.get(), &StatusBar_Impl::on_render);
	func_input_doubleclick().set(impl.get(), &StatusBar_Impl::on_input_doubleclick);

	impl->create_parts();
}

StatusBar::~StatusBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// StatusBar Attributes:

StatusBar *StatusBar::get_named_item(GUIComponent *reference_component, const std::string &id)
{
	StatusBar *object = NULL;
	if (reference_component)
		object = dynamic_cast<StatusBar*>(reference_component->get_named_item(id));

	if (!object)
		throw Exception(string_format("Cannot find StatusBar named item: %1", id));

	return object;
}

Callback_v0 &StatusBar::func_part_double_clicked(int id)
{
	unsigned int index = impl->find_part(id);
	return impl->statusbar_parts[index].func_double_clicked;
}

/////////////////////////////////////////////////////////////////////////////
// StatusBar Operations:

void StatusBar::set_status_text(const std::string &text)
{
	impl->status_text = text;
	request_repaint();
}

void StatusBar::set_part_text(int id, const std::string &text)
{
	unsigned int index = impl->find_part(id);
	impl->statusbar_parts[index].icon = Image();
	impl->statusbar_parts[index].text = text;
	request_repaint();
}

void StatusBar::set_part_text(int id, const Image &icon, const std::string &text)
{
	unsigned int index = impl->find_part(id);
	impl->statusbar_parts[index].icon = icon;
	impl->statusbar_parts[index].text = text;
	request_repaint();
}

void StatusBar::add_part(int id, int width, GUIComponent *component)
{
	impl->throw_if_part_exists(id);
	StatusBar_Part status_part(id, width, component);
	impl->statusbar_parts.push_back(status_part);
	impl->position_parts();
	request_repaint();
	if (component)
		component->set_visible(true, false);
}

void StatusBar::show_part(int id, bool show)
{
	unsigned int index = impl->find_part(id);
	impl->statusbar_parts[index].visible = true;
	if (impl->statusbar_parts[index].component)
		impl->statusbar_parts[index].component->set_visible(show, false);
	impl->position_parts();
	request_repaint();
}

void StatusBar::remove_part(int id)
{
	unsigned int index = impl->find_part(id);
	if (impl->statusbar_parts[index].component)
		impl->statusbar_parts[index].component->set_visible(false);
	impl->statusbar_parts.erase(impl->statusbar_parts.begin() + index);
	impl->position_parts();
	request_repaint();
}

void StatusBar::show_size_grip(bool show)
{
	impl->show_size_grip = show;
	impl->position_parts();
	request_repaint();
}

/////////////////////////////////////////////////////////////////////////////
// StatusBar Implementation:

bool StatusBar_Impl::on_input_doubleclick(const InputEvent &input_event)
{
	if (input_event.id == mouse_left)
	{
		for (unsigned int index = 0; index < statusbar_parts.size(); index++)
		{
			StatusBar_Part &statusbar_part = statusbar_parts[index];
			if (statusbar_part.position.contains(input_event.mouse_pos))
			{
				if (!statusbar_part.func_double_clicked.is_null())
					statusbar_part.func_double_clicked.invoke();
				return true;
			}
		}
	}
	return false;
}

void StatusBar_Impl::on_resized()
{
	position_parts();
	statusbar->request_repaint();
}

void StatusBar_Impl::on_render(Canvas &canvas, const Rect &update_rect)
{
	Rect rect(Point(0,0), statusbar->get_geometry().get_size());
	Rect content = statusbar->get_content_box();

	Rect rect_status_text = content;

	if (show_size_grip)
	{
		int preferred_width = part_size_grip.get_preferred_width();
		Rect rect_sizegrip(content.right - preferred_width, content.top, content.right, content.bottom);
		part_size_grip.render_box(canvas, rect_sizegrip, update_rect);
		rect_status_text.right = rect_sizegrip.left;
	}

	if (!statusbar_parts.empty())
		rect_status_text.right = statusbar_parts[0].position.left;

	part_status_text.render_box(canvas, rect_status_text, update_rect);
	Rect status_text_content = part_status_text.get_content_box(rect_status_text);

	statusbar->get_font().draw_text(canvas, status_text_content.left + 4, content.bottom - 6, status_text, statusbar->get_css_properties().color.color);

	for (unsigned int index = 0; index < statusbar_parts.size(); index++)
	{
		StatusBar_Part &statusbar_part = statusbar_parts[index];
		part_status_part.render_box(canvas, statusbar_part.position, update_rect);
		Rect part_content = part_status_part.get_content_box(statusbar_part.position);
		int icon_width = 0;
		if (!statusbar_part.icon.is_null())
		{
			statusbar_part.icon.draw(canvas, (float)part_content.left + 4, (float)part_content.bottom - 6 - statusbar_part.icon.get_height());
			icon_width = statusbar_part.icon.get_width() + 4;
		}
		if (!statusbar_part.text.empty())
		{
			statusbar->get_font().draw_text(canvas, part_content.left + 4 + icon_width, part_content.bottom - 6, statusbar_part.text, statusbar->get_css_properties().color.color);
		}
	}
}

void StatusBar_Impl::position_parts()
{
	Rect rect(Point(0,0), statusbar->get_geometry().get_size());
	Rect content = statusbar->get_content_box();

	int xpos = content.right;
	if (show_size_grip)
	{
		int preferred_width = part_size_grip.get_preferred_width();
		Rect rect_sizegrip(content.right - preferred_width, content.top, content.right, content.bottom);
		xpos = rect_sizegrip.left;
	}

	for (unsigned int index = statusbar_parts.size(); index > 0; index--)
	{
		StatusBar_Part &statusbar_part = statusbar_parts[index-1];
		int left = xpos - statusbar_part.width;
		int right = xpos;
		Rect new_position(left, content.top, right, content.bottom);
		if (statusbar_part.component && statusbar_part.position != new_position)
			statusbar_part.component->set_geometry(part_status_part.get_content_box(new_position));

		statusbar_part.position = new_position;
		xpos = left;
	}
}

unsigned int StatusBar_Impl::find_part(int id) const
{
	for (unsigned int index = 0; index < statusbar_parts.size(); index++)
	{
		if (statusbar_parts[index].id == id)
			return index;
	}
	throw Exception(string_format("No status bar part with id %1", id));
}

void StatusBar_Impl::throw_if_part_exists(int id) const
{
	for (unsigned int index = 0; index < statusbar_parts.size(); index++)
	{
		if (statusbar_parts[index].id == id)
			throw Exception(string_format("Status bar part with id %1 already exists", id));
	}
}

void StatusBar_Impl::create_parts()
{
	part_status_text = GUIThemePart(statusbar, "statustext");
	part_status_part = GUIThemePart(statusbar, "statuspart");
	part_size_grip = GUIThemePart(statusbar, "sizegrip");
}

}
