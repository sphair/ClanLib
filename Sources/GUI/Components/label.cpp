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
#include "API/Display/2D/span_layout.h"
#include "API/Display/2D/canvas.h"
#include "API/CSSLayout/ComputedValues/css_computed_box.h"
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
	void on_apply_properties(CSSComputedBox &properties);

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
	//func_apply_properties().set(impl.get(), &Label_Impl::on_apply_properties);
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

const std::string &Label::get_text() const
{
	return impl->text;
}

float Label::get_preferred_content_width()
{
	Canvas canvas = get_canvas();
	return get_render_text_box(canvas, impl->text).get_width();
}

float Label::get_preferred_content_height(float width)
{
	Canvas canvas = get_canvas();
	return get_render_text_box(canvas, impl->text).get_height();
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
	//impl->on_apply_properties(get_css_properties()); // FIXME: this is a hack that only works for simple non-inherited values
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
	label->render_text(canvas, text, 0, label->get_vertical_text_align(canvas).baseline);
}

void Label_Impl::on_apply_properties(CSSComputedBox &properties)
{
	if (is_user_color)
	{
		properties.color.color = user_color;
		properties.color.type = CSSValueColor::type_color;
	}
}	

}
