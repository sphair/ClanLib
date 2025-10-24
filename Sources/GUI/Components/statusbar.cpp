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
**    Magnus Norddahl
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_theme_part_property.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/Components/statusbar.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "API/Display/Font/font.h"
#include "API/Display/2D/image.h"
#include "API/Core/Text/string_format.h"
#include "../gui_css_strings.h"

/////////////////////////////////////////////////////////////////////////////
// CL_StatusBar_Impl Class:

class CL_StatusBar_Part
{
public:
	CL_StatusBar_Part(int id, int width, CL_GUIComponent *component)
	: id(id), width(width), component(component), visible(true)
	{
	}

	int id;
	int width;
	CL_Image icon;
	CL_String text;
	CL_GUIComponent *component;
	bool visible;
	CL_Rect position;
	CL_Callback_v0 func_double_clicked;
};

class CL_StatusBar_Impl
{
public:
	CL_StatusBar_Impl() : show_size_grip(true)
	{
		prop_text_color = CL_GUIThemePartProperty(CssStr::text_color, "black");
	}

	void on_process_message(CL_GUIMessage &msg);
	void on_resized();
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void position_parts();
	void create_parts();
	void on_style_changed();
	unsigned int find_part(int id) const;
	void throw_if_part_exists(int id) const;

	CL_String status_text;
	CL_StatusBar *statusbar;
	CL_Font font;
	CL_Colorf text_color;
	CL_GUIThemePart part_component;
	CL_GUIThemePart part_status_text;
	CL_GUIThemePart part_status_part;
	CL_GUIThemePart part_size_grip;
	CL_GUIThemePartProperty prop_text_color;
	std::vector<CL_StatusBar_Part> statusbar_parts;
	bool show_size_grip;
};

/////////////////////////////////////////////////////////////////////////////
// CL_StatusBar Construction:

CL_StatusBar::CL_StatusBar(CL_GUIComponent *parent)
: CL_GUIComponent(parent), impl(new CL_StatusBar_Impl)
{
	set_type_name("statusbar");
	impl->statusbar = this;

	func_process_message().set(impl.get(), &CL_StatusBar_Impl::on_process_message);
	func_resized().set(impl.get(), &CL_StatusBar_Impl::on_resized);
	func_render().set(impl.get(), &CL_StatusBar_Impl::on_render);
	func_style_changed().set(impl.get(), &CL_StatusBar_Impl::on_style_changed);

	impl->create_parts();
}

CL_StatusBar::~CL_StatusBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_StatusBar Attributes:

CL_StatusBar *CL_StatusBar::get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id)
{
	CL_StatusBar *object = NULL;
	if (reference_component)
		object = dynamic_cast<CL_StatusBar*>(reference_component->get_named_item(id));

	if (!object)
		throw CL_Exception(cl_format("Cannot find CL_StatusBar named item: %1", id));

	return object;
}

CL_Size CL_StatusBar::get_preferred_size() const
{
	return impl->part_component.get_preferred_size();
}

CL_Callback_v0 &CL_StatusBar::func_part_double_clicked(int id)
{
	unsigned int index = impl->find_part(id);
	return impl->statusbar_parts[index].func_double_clicked;
}

/////////////////////////////////////////////////////////////////////////////
// CL_StatusBar Operations:

void CL_StatusBar::set_status_text(const CL_StringRef &text)
{
	impl->status_text = text;
	request_repaint();
}

void CL_StatusBar::set_part_text(int id, const CL_StringRef &text)
{
	unsigned int index = impl->find_part(id);
	impl->statusbar_parts[index].icon = CL_Image();
	impl->statusbar_parts[index].text = text;
	request_repaint();
}

void CL_StatusBar::set_part_text(int id, const CL_Image &icon, const CL_StringRef &text)
{
	unsigned int index = impl->find_part(id);
	impl->statusbar_parts[index].icon = icon;
	impl->statusbar_parts[index].text = text;
	request_repaint();
}

void CL_StatusBar::add_part(int id, int width, CL_GUIComponent *component)
{
	impl->throw_if_part_exists(id);
	CL_StatusBar_Part status_part(id, width, component);
	impl->statusbar_parts.push_back(status_part);
	impl->position_parts();
	request_repaint();
	if (component)
		component->set_visible(true, false);
}

void CL_StatusBar::show_part(int id, bool show)
{
	unsigned int index = impl->find_part(id);
	impl->statusbar_parts[index].visible = true;
	if (impl->statusbar_parts[index].component)
		impl->statusbar_parts[index].component->set_visible(show, false);
	impl->position_parts();
	request_repaint();
}

void CL_StatusBar::remove_part(int id)
{
	unsigned int index = impl->find_part(id);
	if (impl->statusbar_parts[index].component)
		impl->statusbar_parts[index].component->set_visible(false);
	impl->statusbar_parts.erase(impl->statusbar_parts.begin() + index);
	impl->position_parts();
	request_repaint();
}

void CL_StatusBar::show_size_grip(bool show)
{
	impl->show_size_grip = show;
	impl->position_parts();
	request_repaint();
}

/////////////////////////////////////////////////////////////////////////////
// CL_StatusBar Implementation:

void CL_StatusBar_Impl::on_process_message(CL_GUIMessage &msg)
{
	if (msg.is_type(CL_GUIMessage_Input::get_type_name()))
	{
		CL_GUIMessage_Input input_msg = msg;
		CL_InputEvent e = input_msg.get_event();

		// to do: fix it to use double clicks instead!
		if (e.type == CL_InputEvent::released && e.id == CL_MOUSE_LEFT)
		{
			for (unsigned int index = 0; index < statusbar_parts.size(); index++)
			{
				CL_StatusBar_Part &statusbar_part = statusbar_parts[index];
				if (statusbar_part.position.contains(e.mouse_pos))
				{
					if (!statusbar_part.func_double_clicked.is_null())
						statusbar_part.func_double_clicked.invoke();
					return;
				}
			}
		}
	}
}

void CL_StatusBar_Impl::on_resized()
{
	position_parts();
	statusbar->request_repaint();
}

void CL_StatusBar_Impl::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_Rect rect(CL_Point(0,0), statusbar->get_geometry().get_size());
	part_component.render_box(gc, rect, update_rect);
	CL_Rect content = part_component.get_content_box(rect);

	CL_Rect rect_status_text = content;

	if (show_size_grip)
	{
		int preferred_width = part_size_grip.get_preferred_width();
		CL_Rect rect_sizegrip(content.right - preferred_width, content.top, content.right, content.bottom);
		part_size_grip.render_box(gc, rect_sizegrip, update_rect);
		rect_status_text.right = rect_sizegrip.left;
	}

	if (!statusbar_parts.empty())
		rect_status_text.right = statusbar_parts[0].position.left;

	part_status_text.render_box(gc, rect_status_text, update_rect);
	CL_Rect status_text_content = part_status_text.get_content_box(rect_status_text);

	font.draw_text(gc, status_text_content.left + 4, content.bottom - 6, status_text, text_color);

	for (unsigned int index = 0; index < statusbar_parts.size(); index++)
	{
		CL_StatusBar_Part &statusbar_part = statusbar_parts[index];
		part_status_part.render_box(gc, statusbar_part.position, update_rect);
		CL_Rect part_content = part_status_part.get_content_box(statusbar_part.position);
		int icon_width = 0;
		if (!statusbar_part.icon.is_null())
		{
			statusbar_part.icon.draw(gc, (float)part_content.left + 4, (float)part_content.bottom - 6 - statusbar_part.icon.get_height());
			icon_width = statusbar_part.icon.get_width() + 4;
		}
		if (!statusbar_part.text.empty())
		{
			font.draw_text(gc, part_content.left + 4 + icon_width, part_content.bottom - 6, statusbar_part.text, text_color);
		}
	}
}

void CL_StatusBar_Impl::position_parts()
{
	CL_Rect rect(CL_Point(0,0), statusbar->get_geometry().get_size());
	CL_Rect content = part_component.get_content_box(rect);

	int xpos = content.right;
	if (show_size_grip)
	{
		int preferred_width = part_size_grip.get_preferred_width();
		CL_Rect rect_sizegrip(content.right - preferred_width, content.top, content.right, content.bottom);
		xpos = rect_sizegrip.left;
	}

	for (unsigned int index = statusbar_parts.size(); index > 0; index--)
	{
		CL_StatusBar_Part &statusbar_part = statusbar_parts[index-1];
		int left = xpos - statusbar_part.width;
		int right = xpos;
		CL_Rect new_position(left, content.top, right, content.bottom);
		if (statusbar_part.component && statusbar_part.position != new_position)
			statusbar_part.component->set_geometry(part_status_part.get_content_box(new_position));

		statusbar_part.position = new_position;
		xpos = left;
	}
}

unsigned int CL_StatusBar_Impl::find_part(int id) const
{
	for (unsigned int index = 0; index < statusbar_parts.size(); index++)
	{
		if (statusbar_parts[index].id == id)
			return index;
	}
	throw CL_Exception(cl_format("No status bar part with id %1", id));
}

void CL_StatusBar_Impl::throw_if_part_exists(int id) const
{
	for (unsigned int index = 0; index < statusbar_parts.size(); index++)
	{
		if (statusbar_parts[index].id == id)
			throw CL_Exception(cl_format("Status bar part with id %1 already exists", id));
	}
}

void CL_StatusBar_Impl::create_parts()
{
	part_component = CL_GUIThemePart(statusbar);
	part_status_text = CL_GUIThemePart(statusbar, "statustext");
	part_status_part = CL_GUIThemePart(statusbar, "statuspart");
	part_size_grip = CL_GUIThemePart(statusbar, "sizegrip");
	font = part_component.get_font();
	text_color = part_component.get_property(prop_text_color);
}

void CL_StatusBar_Impl::on_style_changed()
{
	create_parts();
}
