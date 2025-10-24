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
#include "API/Core/Text/string_format.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_theme_part_property.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/gui_message_pointer.h"
#include "API/GUI/gui_message_focus_change.h"
#include "API/GUI/Components/push_button.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "API/Display/2D/image.h"
#include "API/Display/2D/span_layout.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_metrics.h"
#include "../gui_css_strings.h"

/////////////////////////////////////////////////////////////////////////////
// CL_PushButton_Impl Class:

class CL_PushButton_Impl
{
public:
	CL_PushButton_Impl() : toggle_mode(false), flat_mode(false), icon_position(CL_PushButton::icon_left) {}
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_process_message(CL_GUIMessage &msg);
	void on_style_changed();
	void on_enablemode_changed();
	void create_parts();
	void update_default_state(bool button_focused); // take the state from the message as the focused component hasn't been updated yet at this stage. 

	CL_PushButton *button;
	CL_Callback_v0 func_clicked;
	CL_String text;
	CL_Image icon;
	CL_PushButton::IconPosition icon_position;
	bool toggle_mode;
	bool flat_mode;
	CL_GUIThemePart part;
	CL_GUIThemePart part_focus;
};

/////////////////////////////////////////////////////////////////////////////
// CL_PushButton Construction:

CL_PushButton::CL_PushButton(CL_GUIComponent *parent)
: CL_GUIComponent(parent), impl(new CL_PushButton_Impl)
{
	set_type_name(CssStr::PushButton::type_name);
	set_blocks_default_action(true);
	set_focus_policy(focus_local);
	set_double_click_enabled(false);

	func_process_message().set(impl.get(), &CL_PushButton_Impl::on_process_message);
	func_render().set(impl.get(), &CL_PushButton_Impl::on_render);
	func_style_changed().set(impl.get(), &CL_PushButton_Impl::on_style_changed);
	func_enablemode_changed().set(impl.get(), &CL_PushButton_Impl::on_enablemode_changed);

	impl->button = this;
	impl->create_parts();
}

CL_PushButton::~CL_PushButton()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_PushButton Attributes:

CL_PushButton *CL_PushButton::get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id)
{
	CL_PushButton *object = NULL;
	if (reference_component)
		object = dynamic_cast<CL_PushButton*>(reference_component->get_named_item(id));

	if (!object)
		throw CL_Exception(cl_format("Cannot find CL_PushButton named item: %1", id));

	return object;
}

bool CL_PushButton::is_pushed() const
{
	return impl->part.get_state(CssStr::pressed);
}

bool CL_PushButton::is_toggle() const
{
	return impl->toggle_mode;
}

bool CL_PushButton::is_flat() const
{
	return impl->flat_mode;
}

CL_Image CL_PushButton::get_icon() const
{
	return impl->icon;
}

CL_PushButton::IconPosition CL_PushButton::get_icon_position() const
{
	return impl->icon_position;
}

const CL_String &CL_PushButton::get_text() const
{
	return impl->text;
}

CL_Size CL_PushButton::get_preferred_size() const
{
	return impl->part.get_preferred_size();
}

/////////////////////////////////////////////////////////////////////////////
// CL_PushButton Operations:

void CL_PushButton::set_pushed(bool enable)
{
	if (impl->part.set_state(CssStr::pressed, enable))
		request_repaint();
}

void CL_PushButton::set_toggle(bool enable)
{
	if (impl->toggle_mode != enable)
	{
		impl->toggle_mode = enable;
		request_repaint();
	}
}

void CL_PushButton::set_flat(bool enable)
{
	if (impl->flat_mode != enable)
	{
		impl->flat_mode = enable;
		request_repaint();
	}
}

void CL_PushButton::set_icon(const CL_Image &icon)
{
	impl->icon = icon;
	request_repaint();
}

void CL_PushButton::set_icon_position(IconPosition pos)
{
	if (impl->icon_position != pos)
	{
		impl->icon_position = pos;
		request_repaint();
	}
}

void CL_PushButton::set_text(const CL_StringRef &text)
{
	impl->text = text;
	request_repaint();
}

/////////////////////////////////////////////////////////////////////////////
// CL_PushButton Events:

CL_Callback_v0 &CL_PushButton::func_clicked()
{
	return impl->func_clicked;
}

/////////////////////////////////////////////////////////////////////////////
// CL_PushButton Implementation:

void CL_PushButton_Impl::create_parts()
{
	part = CL_GUIThemePart(button);
	part.set_state(CssStr::normal, true);
	part.set_state(CssStr::hot, false);
	part.set_state(CssStr::pressed, false);
	part.set_state(CssStr::defaulted, button->is_default());
	part.set_state(CssStr::disabled, !button->is_enabled());
	part.set_state(CssStr::flat, button->is_flat());

	part_focus = CL_GUIThemePart(button, "focus");
}

void CL_PushButton_Impl::on_style_changed()
{
	create_parts();
	update_default_state(false);
}

void CL_PushButton_Impl::on_enablemode_changed()
{
	create_parts();
	button->request_repaint();
}

void CL_PushButton_Impl::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_Rect rect = button->get_size();
	part.render_box(gc, rect, update_rect);

	if (button->has_focus())
	{
		part_focus.render_box(gc, rect, update_rect);
	}

	CL_Rect content_rect = part.get_content_box(rect);

	if (!icon.is_null())
	{
		CL_Font font = part.get_font();

		CL_GUIThemePartProperty prop_text_color(CssStr::text_color, "black");
		CL_GUIThemePartProperty prop_align(CssStr::align, CssStr::left);
		CL_GUIThemePartProperty prop_vertical_align(CssStr::vertical_align, CssStr::center);

		CL_String h_align = part.get_property(prop_align); // left|center|right|justify
		CL_String v_align = part.get_property(prop_vertical_align); // top|center|bottom

		CL_Size icon_size = icon.get_size();
		CL_Point center = content_rect.get_center();

		CL_FontMetrics metrics = font.get_font_metrics();
		CL_Colorf text_color = part.get_property(prop_text_color);

		CL_SpanLayout layout;
		if (icon_position == CL_PushButton::icon_top)
		{
			layout.add_image(icon, 0);
			if (!text.empty())
				layout.add_text("\n", font, text_color);
		}
		else if (icon_position == CL_PushButton::icon_left)
		{
			layout.add_image(icon, metrics.get_height() / 2 - metrics.get_ascent() + icon_size.height / 2);
			if (!text.empty())
				layout.add_text(" ", font, text_color);
		}

		layout.add_text(text, font, text_color);

		if (icon_position == CL_PushButton::icon_bottom)
		{
			if (!text.empty())
				layout.add_text("\n", font, text_color);
			layout.add_image(icon, 0);
		}
		else if (icon_position == CL_PushButton::icon_right)
		{
			if (!text.empty())
				layout.add_text(" ", font, text_color);
			layout.add_image(icon, metrics.get_height() / 2 - metrics.get_ascent() + icon_size.height / 2);
		}

		if (h_align == CssStr::left)
			layout.set_align(cl_left);
		else if (h_align == CssStr::center)
			layout.set_align(cl_center);
		else if (h_align == CssStr::right)
			layout.set_align(cl_right);
		else if (h_align == CssStr::justify)
			layout.set_align(cl_justify);

		layout.layout(gc, content_rect.get_width()); // To do: also add support for CL_SpanLayout::layout(gc, width, height) so we can clip vertically

		if (v_align == CssStr::top)
		{
			layout.set_position(CL_Point(content_rect.left, content_rect.top));
		}
		else if (v_align == CssStr::center)
		{
			layout.set_position(CL_Point(content_rect.left, part.get_vertical_text_align(gc, font, content_rect).baseline - layout.get_first_baseline_offset() - (layout.get_last_baseline_offset()-layout.get_first_baseline_offset()) / 2));
		}
		else if (v_align == CssStr::bottom)
		{
			layout.set_position(CL_Point(content_rect.left, content_rect.bottom));
		}

		layout.draw_layout_ellipsis(gc, content_rect);
	}
	else
	{
		part.render_text(gc, text, content_rect, update_rect);
	}
}

void CL_PushButton_Impl::on_process_message(CL_GUIMessage &msg)
{
	if (!button->is_enabled())
		return;

	if (msg.is_type(CL_GUIMessage_Input::get_type_name()))
	{
		CL_GUIMessage_Input input_msg = msg;
		CL_InputEvent e = input_msg.get_event();

		if (toggle_mode)
		{
			if( e.type == CL_InputEvent::released &&
				(e.id == CL_MOUSE_LEFT || e.id == CL_KEY_RETURN || e.id == CL_KEY_SPACE || e.id == CL_KEY_NUMPAD_ENTER) )
			{
				part.set_state(CssStr::pressed, !part.get_state(CssStr::pressed));
				button->request_repaint();
				msg.set_consumed();
				
				if (!func_clicked.is_null())
					func_clicked.invoke();
			}
		}
		else
		{
			if (e.type == CL_InputEvent::pressed && 
				(e.id == CL_MOUSE_LEFT || e.id == CL_KEY_RETURN || e.id == CL_KEY_SPACE || e.id == CL_KEY_NUMPAD_ENTER))
			{
				part.set_state(CssStr::pressed, true);
				button->request_repaint();
				msg.set_consumed();
			}
			else if( e.type == CL_InputEvent::released &&
				(e.id == CL_MOUSE_LEFT || e.id == CL_KEY_RETURN || e.id == CL_KEY_SPACE || e.id == CL_KEY_NUMPAD_ENTER) &&
				part.get_state(CssStr::pressed) )
			{
				part.set_state(CssStr::pressed, false);
				button->request_repaint();
				msg.set_consumed();
				
				if (!func_clicked.is_null())
					func_clicked.invoke();
			}
		}
		if (e.type == CL_InputEvent::pressed && (e.id == CL_KEY_LEFT || e.id == CL_KEY_UP))
		{
			button->focus_previous();
			msg.set_consumed();
		}
		else if (e.type == CL_InputEvent::pressed && (e.id == CL_KEY_RIGHT || e.id == CL_KEY_DOWN))
		{
			button->focus_next();
			msg.set_consumed();
		}
	}
	else if (msg.is_type(CL_GUIMessage_Pointer::get_type_name()))
	{
		CL_GUIMessage_Pointer pointer = msg;
		if (pointer.get_pointer_type() == CL_GUIMessage_Pointer::pointer_enter)
		{
			part.set_state(CssStr::hot, true);
			button->request_repaint();
			msg.set_consumed();
		}
		else if (pointer.get_pointer_type() == CL_GUIMessage_Pointer::pointer_leave)
		{
			part.set_state(CssStr::hot, false);

			if (!toggle_mode)
				part.set_state(CssStr::pressed, false);
			button->request_repaint();
			msg.set_consumed();
		}
	}
	else if (msg.is_type(CL_GUIMessage_FocusChange::get_type_name()))
	{
		CL_GUIMessage_FocusChange focus_msg = msg;
		if (focus_msg.get_focus_type() == CL_GUIMessage_FocusChange::gained_focus)
		{
			part.set_state(CssStr::focused, true);
			if (!toggle_mode)
				update_default_state(true);
			button->request_repaint();
			msg.set_consumed();
		}
		else 
		{
			part.set_state(CssStr::focused, false);
			if (!toggle_mode)
				update_default_state(false);
			button->request_repaint();
			msg.set_consumed();
		}
	}
}

void CL_PushButton_Impl::update_default_state(bool focus_gained)
{
	bool is_default = false;

	if (focus_gained)
	{
		is_default = true;
	}
	else 
	{
		is_default = button->is_default();

		CL_GUIComponent *focus_comp = button->get_gui_manager().get_focused_component();
		if (focus_comp && (focus_comp != button))
		{
			if (focus_comp->get_blocks_default_action())
				is_default = false;
		}
	}

	part.set_state("defaulted", is_default); 
	button->request_repaint();
}
