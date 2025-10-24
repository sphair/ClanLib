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
#include "API/Core/Text/string_format.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_message_pointer.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/gui_theme_part_property.h"
#include "API/GUI/Components/label.h"
#include "API/Core/Text/string_help.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "API/Display/Font/font.h"
#include "API/Display/2D/span_layout.h"
#include "../gui_css_strings.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Label_Impl Class:

class CL_Label_Impl
{
public:
	CL_Label_Impl() : alignment(CL_Label::align_left)
	{
	}

	void on_process_message(CL_GUIMessage &msg);
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_style_changed();
	void create_parts();

	CL_Label *label;
	CL_SpanLayout span;
	CL_GUIThemePart part_component;
	CL_Label::Alignment alignment;
};

/////////////////////////////////////////////////////////////////////////////
// CL_Label Construction:

CL_Label::CL_Label(CL_GUIComponent *parent)
: CL_GUIComponent(parent), impl(new CL_Label_Impl)
{
	set_type_name(CssStr::Label::type_name);
	impl->label = this;
	func_process_message().set(impl.get(), &CL_Label_Impl::on_process_message);
	func_render().set(impl.get(), &CL_Label_Impl::on_render);
	func_style_changed().set(impl.get(), &CL_Label_Impl::on_style_changed);
	// todo: disabled

	impl->create_parts();
}

CL_Label::~CL_Label()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Label Attributes:

CL_Label *CL_Label::get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id)
{
	CL_Label *object = NULL;
	if (reference_component)
		object = dynamic_cast<CL_Label*>(reference_component->get_named_item(id));

	if (!object)
		throw CL_Exception(cl_format("Cannot find CL_Label named item: %1", id));

	return object;
}

CL_String CL_Label::get_text() const
{
	return impl->span.get_combined_text();
}

CL_Size CL_Label::get_preferred_size() const
{
	return impl->part_component.get_preferred_size();
}

/////////////////////////////////////////////////////////////////////////////
// CL_Label Operations:

void CL_Label::set_text(const CL_StringRef &text)
{
	impl->span = CL_SpanLayout();
	CL_GUIThemePartProperty prop_text_color(CssStr::text_color, "black");
	CL_Colorf text_color = impl->part_component.get_property(prop_text_color);
	CL_Font font = impl->part_component.get_font();
	impl->span.add_text(text, font, text_color);
	request_repaint();
}

void CL_Label::set_span(const CL_SpanLayout &layout)
{
	impl->span = layout;
	request_repaint();
}

CL_Label::Alignment CL_Label::get_alignment() const
{
	return impl->alignment;
}

void CL_Label::set_alignment(CL_Label::Alignment alignment)
{
	impl->alignment = alignment;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Label Events:

/////////////////////////////////////////////////////////////////////////////
// CL_Label Implementation:

void CL_Label_Impl::on_process_message(CL_GUIMessage &msg)
{
	if (msg.is_type(CL_GUIMessage_Pointer::get_type_name()))
	{
		CL_GUIMessage_Pointer pointer = msg;
		if (pointer.get_pointer_type() == CL_GUIMessage_Pointer::pointer_enter)
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

void CL_Label_Impl::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_Rect rect = label->get_geometry();
	part_component.render_box(gc, rect.get_size(), update_rect);
	CL_Rect content_rect = part_component.get_content_box(rect.get_size());
	switch (alignment)
	{
	case CL_Label::align_left: span.set_align(cl_left); break;
	case CL_Label::align_center: span.set_align(cl_center); break;
	case CL_Label::align_right: span.set_align(cl_right); break;
	case CL_Label::align_justify: span.set_align(cl_justify); break;
	default: break;
	}

	span.layout(gc, content_rect.get_width());
	span.set_position(CL_Point(content_rect.left, content_rect.top));
	span.draw_layout(gc);
	span.set_component_geometry();
}

void CL_Label_Impl::create_parts()
{
	part_component = CL_GUIThemePart(label);
	part_component.set_state(CssStr::hot, false);
	part_component.set_state(CssStr::normal, true);
	part_component.set_state(CssStr::disabled, false);
}

void CL_Label_Impl::on_style_changed()
{
	create_parts();
}
