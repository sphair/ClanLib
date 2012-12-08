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
#include "API/GUI/gui_component_description.h"
#include "API/GUI/Components/label_span.h"
#include "API/Core/Text/string_help.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "API/Display/Font/font.h"
#include "API/Display/2D/span_layout.h"
#include "API/Display/2D/canvas.h"
#include "../gui_css_strings.h"
#include "API/CSSLayout/css_box_properties.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// LabelSpan_Impl Class:

class LabelSpan_Impl
{
public:
	LabelSpan_Impl() : alignment(LabelSpan::align_left)
	{
	}

	void on_process_message(std::shared_ptr<GUIMessage> &msg);
	void on_render(Canvas &canvas, const Rect &update_rect);
	void create_parts();

	LabelSpan *label;
	SpanLayout span;
	LabelSpan::Alignment alignment;
};

/////////////////////////////////////////////////////////////////////////////
// LabelSpan Construction:

LabelSpan::LabelSpan(GUIComponent *parent)
: GUIComponent(parent, CssStr::LabelSpan::type_name), impl(new LabelSpan_Impl)
{
	impl->label = this;
	func_process_message().set(impl.get(), &LabelSpan_Impl::on_process_message);
	func_render().set(impl.get(), &LabelSpan_Impl::on_render);
	// todo: disabled

	impl->create_parts();
}

LabelSpan::~LabelSpan()
{
}

/////////////////////////////////////////////////////////////////////////////
// LabelSpan Attributes:

LabelSpan *LabelSpan::get_named_item(GUIComponent *reference_component, const std::string &id)
{
	LabelSpan *object = NULL;
	if (reference_component)
		object = dynamic_cast<LabelSpan*>(reference_component->get_named_item(id));

	if (!object)
		throw Exception(string_format("Cannot find LabelSpan named item: %1", id));

	return object;
}

std::string LabelSpan::get_text() const
{
	return impl->span.get_combined_text();
}

/////////////////////////////////////////////////////////////////////////////
// LabelSpan Operations:

void LabelSpan::set_text(const std::string &text)
{
	impl->span = SpanLayout();
	Colorf text_color = get_property(CssStr::text_color, "black");
	Font font = get_font();
	impl->span.add_text(text, font, text_color);
	request_repaint();
}

void LabelSpan::set_span(const SpanLayout &layout)
{
	impl->span = layout;
	request_repaint();
}

LabelSpan::Alignment LabelSpan::get_alignment() const
{
	return impl->alignment;
}

void LabelSpan::set_alignment(LabelSpan::Alignment alignment)
{
	impl->alignment = alignment;
}

/////////////////////////////////////////////////////////////////////////////
// LabelSpan Events:

/////////////////////////////////////////////////////////////////////////////
// LabelSpan Implementation:

void LabelSpan_Impl::on_process_message(std::shared_ptr<GUIMessage> &msg)
{
	std::shared_ptr<GUIMessage_Pointer> pointer = std::dynamic_pointer_cast<GUIMessage_Pointer>(msg);
	if (pointer)
	{
		if (pointer->pointer_type == GUIMessage_Pointer::pointer_enter)
		{
			label->set_pseudo_class(CssStr::hot, true);
			label->request_repaint();
		}
		else
		{
			label->set_pseudo_class(CssStr::hot, false);
			label->set_pseudo_class(CssStr::pressed, false);
			label->request_repaint();
		}
	}
}

void LabelSpan_Impl::on_render(Canvas &canvas, const Rect &update_rect)
{
	Rect rect = label->get_geometry();
	Rect content_rect = label->get_content_box();
	switch (alignment)
	{
	case LabelSpan::align_left: span.set_align(span_left); break;
	case LabelSpan::align_center: span.set_align(span_center); break;
	case LabelSpan::align_right: span.set_align(span_right); break;
	case LabelSpan::align_justify: span.set_align(span_justify); break;
	default: break;
	}

	span.layout(canvas, content_rect.get_width());
	span.set_position(Point(content_rect.left, content_rect.top));
	span.draw_layout(canvas);
	span.set_component_geometry();
}

void LabelSpan_Impl::create_parts()
{
	label->set_pseudo_class(CssStr::hot, false);
	label->set_pseudo_class(CssStr::normal, true);
	label->set_pseudo_class(CssStr::disabled, false);
}

}
