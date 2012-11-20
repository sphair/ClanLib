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
#include "API/GUI/Components/label.h"
#include "API/Core/Text/string_help.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/2D/span_layout.h"
#include "API/Display/2D/canvas.h"
#include "API/CSSLayout/css_box_properties.h"
#include "../gui_css_strings.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Label_Impl Class:

class Label_Impl
{
public:
	Label_Impl() : alignment(Label::align_left), is_user_color(false)
	{
	}

	void on_render(Canvas &canvas, const Rect &update_rect);
	void on_default_properties(CSSBoxProperties &properties);

	Label *label;
	std::string text;
	Label::Alignment alignment;

	Colorf user_color;
	bool is_user_color;

};

/////////////////////////////////////////////////////////////////////////////
// Label Construction:

Label::Label(GUIComponent *parent)
: GUIComponent(parent, CssStr::Label::type_name), impl(new Label_Impl)
{
	impl->label = this;
	func_render().set(impl.get(), &Label_Impl::on_render);
	func_default_properties().set(impl.get(), &Label_Impl::on_default_properties);
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
	return impl->text;
}

float Label::get_preferred_content_width()
{
	Canvas canvas = get_canvas();

	return get_font().get_text_size(canvas, impl->text).width;
}

float Label::get_preferred_content_height(float width)
{
	Canvas canvas = get_canvas();
	return get_font().get_text_size(canvas, impl->text).height;
}

/////////////////////////////////////////////////////////////////////////////
// Label Operations:

void Label::set_text(const std::string &text)
{
	impl->text = text;
	request_repaint();
}

void Label::set_text_color(const Colorf color)
{
	impl->user_color = color;
	impl->is_user_color = true;
	impl->on_default_properties(get_css_properties());
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

void Label_Impl::on_render(Canvas &canvas, const Rect &update_rect)
{
	Rectf content_box = label->get_content_box();
	
	//FIXME: Use alignment
	//switch (label->get_css_properties().text_alignment)
	//{
	//case Label::align_left: span.set_align(span_left); break;
	//case Label::align_center: span.set_align(span_center); break;
	//case Label::align_right: span.set_align(span_right); break;
	//case Label::align_justify: span.set_align(span_justify); break;
	//default: break;
	//}

	FontMetrics metrics = label->get_font().get_font_metrics();
	label->get_font().draw_text_ellipsis(canvas, content_box.left, content_box.top + metrics.get_ascent(), content_box, text, label->get_css_properties().color.color);
}

void Label_Impl::on_default_properties(CSSBoxProperties &properties)
{
	if (is_user_color)
	{
		properties.color.color = Colorf::red;
		properties.color.type = CSSBoxColor::type_color;
	}
}	

}
