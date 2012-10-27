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
#include "API/Core/Text/string_format.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_message_pointer.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/Components/label.h"
#include "API/Core/Text/string_help.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "API/Display/Font/font.h"
#include "API/Display/2D/span_layout.h"
#include "API/Display/2D/canvas.h"
#include "../gui_css_strings.h"

#ifdef INCLUDE_COMPONENTS

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Label_Impl Class:

class Label_Impl
{
public:
	Label_Impl() : alignment(Label::align_left)
	{
	}

	void on_process_message(GUIMessage &msg);
	void on_render(Canvas &canvas, const Rect &update_rect);
	void on_style_changed();
	void create_parts();

	Label *label;
	SpanLayout span;
	GUIThemePart part_component;
	Label::Alignment alignment;
};

/////////////////////////////////////////////////////////////////////////////
// Label Construction:

Label::Label(GUIComponent *parent)
: GUIComponent(parent), impl(new Label_Impl)
{
	set_tag_name(CssStr::Label::type_name);
	impl->label = this;
	func_process_message().set(impl.get(), &Label_Impl::on_process_message);
	func_render().set(impl.get(), &Label_Impl::on_render);
	func_style_changed().set(impl.get(), &Label_Impl::on_style_changed);
	// todo: disabled

	impl->create_parts();
}

Label::~Label()
{
}

/////////////////////////////////////////////////////////////////////////////
// Label Attributes:

Label *Label::get_named_item(GUIComponent *reference_component, const std::string &id)
{
	Label *object = NULL;
	if (reference_component)
		object = dynamic_cast<Label*>(reference_component->get_named_item(id));

	if (!object)
		throw Exception(string_format("Cannot find Label named item: %1", id));

	return object;
}

std::string Label::get_text() const
{
	return impl->span.get_combined_text();
}

Size Label::get_preferred_size() const
{
	return impl->part_component.get_preferred_size();
}

/////////////////////////////////////////////////////////////////////////////
// Label Operations:

void Label::set_text(const std::string &text)
{
	impl->span = SpanLayout();
	GUIThemePartProperty prop_text_color(CssStr::text_color, "black");
	Colorf text_color = impl->part_component.get_property(prop_text_color);
	Font font = impl->part_component.get_font();
	impl->span.add_text(text, font, text_color);
	request_repaint();
}

void Label::set_span(const SpanLayout &layout)
{
	impl->span = layout;
	request_repaint();
}

Label::Alignment Label::get_alignment() const
{
	return impl->alignment;
}

void Label::set_alignment(Label::Alignment alignment)
{
	impl->alignment = alignment;
}

/////////////////////////////////////////////////////////////////////////////
// Label Events:

/////////////////////////////////////////////////////////////////////////////
// Label Implementation:

void Label_Impl::on_process_message(GUIMessage &msg)
{
	if (msg.is_type(GUIMessage_Pointer::get_type_name()))
	{
		GUIMessage_Pointer pointer = msg;
		if (pointer.get_pointer_type() == GUIMessage_Pointer::pointer_enter)
		{
			part_component.set_state(CssStr::hot, true);
			label->request_repaint();
		}
		else
		{
			part_component.set_state(CssStr::hot, false);
			part_component.set_state(CssStr::pressed, false);
			label->request_repaint();
		}
	}
}

void Label_Impl::on_render(Canvas &canvas, const Rect &update_rect)
{
	Rect rect = label->get_geometry();
	part_component.render_box(canvas, rect.get_size(), update_rect);
	Rect content_rect = part_component.get_content_box(rect.get_size());
	switch (alignment)
	{
	case Label::align_left: span.set_align(span_left); break;
	case Label::align_center: span.set_align(span_center); break;
	case Label::align_right: span.set_align(span_right); break;
	case Label::align_justify: span.set_align(span_justify); break;
	default: break;
	}

	span.layout(canvas, content_rect.get_width());
	span.set_position(Point(content_rect.left, content_rect.top));
	span.draw_layout(canvas);
	span.set_component_geometry();
}

void Label_Impl::create_parts()
{
	part_component = GUIThemePart(label);
	part_component.set_state(CssStr::hot, false);
	part_component.set_state(CssStr::normal, true);
	part_component.set_state(CssStr::disabled, false);
}

void Label_Impl::on_style_changed()
{
	create_parts();
}

}

#endif
