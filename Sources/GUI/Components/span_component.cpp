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
#include "API/GUI/Components/span_component.h"
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
// SpanComponent_Impl Class:

class SpanComponent_Impl
{
public:
	SpanComponent_Impl() : alignment(SpanComponent::align_left)
	{
	}

	void on_render(Canvas &canvas, const Rect &update_rect);

	SpanComponent *label;
	SpanLayout span;
	SpanComponent::Alignment alignment;
};

/////////////////////////////////////////////////////////////////////////////
// SpanComponent Construction:

SpanComponent::SpanComponent(GUIComponent *parent)
: GUIComponent(parent), impl(new SpanComponent_Impl)
{
	set_tag_name(CssStr::SpanComponent::type_name);
	impl->label = this;
	func_render().set(impl.get(), &SpanComponent_Impl::on_render);
}

SpanComponent::~SpanComponent()
{
}

/////////////////////////////////////////////////////////////////////////////
// SpanComponent Attributes:

SpanComponent *SpanComponent::get_named_item(GUIComponent *reference_component, const std::string &id)
{
	SpanComponent *object = NULL;
	if (reference_component)
		object = dynamic_cast<SpanComponent*>(reference_component->get_named_item(id));

	if (!object)
		throw Exception(string_format("Cannot find SpanComponent named item: %1", id));

	return object;
}

std::string SpanComponent::get_text() const
{
	return impl->span.get_combined_text();
}

float SpanComponent::get_preferred_content_width()
{
	Canvas canvas = get_canvas();
	return impl->span.find_preferred_size(canvas).width;
}

float SpanComponent::get_preferred_content_height(float width)
{
	Canvas canvas = get_canvas();
	impl->span.layout(canvas, width);
	return impl->span.get_size().height;
}

/////////////////////////////////////////////////////////////////////////////
// SpanComponent Operations:

void SpanComponent::set_text(const std::string &text)
{
	impl->span = SpanLayout();
	Colorf text_color = get_css_properties().color.color;
	Font font = get_font();
	impl->span.add_text(text, font, text_color);
	request_repaint();
}

void SpanComponent::set_span(const SpanLayout &layout)
{
	impl->span = layout;
	request_repaint();
}

SpanComponent::Alignment SpanComponent::get_alignment() const
{
	return impl->alignment;
}

void SpanComponent::set_alignment(SpanComponent::Alignment alignment)
{
	impl->alignment = alignment;
}

/////////////////////////////////////////////////////////////////////////////
// SpanComponent Events:

/////////////////////////////////////////////////////////////////////////////
// SpanComponent Implementation:

void SpanComponent_Impl::on_render(Canvas &canvas, const Rect &update_rect)
{
	Rect rect = label->get_geometry();
	Rect content_rect = label->get_content_box();
	switch (alignment)
	{
	case SpanComponent::align_left: span.set_align(span_left); break;
	case SpanComponent::align_center: span.set_align(span_center); break;
	case SpanComponent::align_right: span.set_align(span_right); break;
	case SpanComponent::align_justify: span.set_align(span_justify); break;
	default: break;
	}

	span.layout(canvas, content_rect.get_width());
	span.set_position(Point(content_rect.left, content_rect.top));
	span.draw_layout(canvas);
	span.set_component_geometry();
}

}
