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
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_theme_part_property.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/Components/frame.h"
#include "API/Display/Font/font.h"
#include "API/Display/2D/sprite.h"
#include "../gui_css_strings.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Frame_Impl Class:

class CL_Frame_Impl
{
public:
	void on_process_message(CL_GUIMessage &msg);
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_style_changed();

	void create_parts();

	CL_Frame *frame;

	CL_Font font;

	CL_Colorf text_color;

	CL_Sprite header_icon;
	CL_String header_text;

	CL_GUIThemePart part_component;
	CL_GUIThemePart part_header;
	CL_GUIThemePart part_header_left;
	CL_GUIThemePart part_header_caption;
	CL_GUIThemePart part_header_right;
	CL_GUIThemePart part_body;
	CL_GUIThemePartProperty prop_text_color;
};

/////////////////////////////////////////////////////////////////////////////
// CL_Frame Construction:

CL_Frame::CL_Frame(CL_GUIComponent *parent)
: CL_GUIComponent(parent), impl(new CL_Frame_Impl)
{
	set_type_name(CssStr::Frame::type_name);
	impl->frame = this;
	impl->prop_text_color = CL_GUIThemePartProperty(CssStr::text_color, "blue");
	func_process_message().set(impl.get(), &CL_Frame_Impl::on_process_message);
	func_render().set(impl.get(), &CL_Frame_Impl::on_render);
	func_style_changed().set(impl.get(), &CL_Frame_Impl::on_style_changed);

	impl->create_parts();
}

CL_Frame::~CL_Frame()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Frame Attributes:

CL_Frame *CL_Frame::get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id)
{
	CL_Frame *object = NULL;
	if (reference_component)
		object = dynamic_cast<CL_Frame*>(reference_component->get_named_item(id));

	if (!object)
		throw CL_Exception(cl_format("Cannot find CL_Frame named item: %1", id));

	return object;
}

CL_Sprite CL_Frame::get_header_icon() const
{
	return impl->header_icon;
}

CL_StringRef CL_Frame::get_header_text() const
{
	return impl->header_text;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Frame Operations:

void CL_Frame::set_header_icon(const CL_Sprite &icon)
{
	impl->header_icon = icon;
}

void CL_Frame::set_header_text(const CL_StringRef &text)
{
	impl->header_text = text;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Frame Implementation:

void CL_Frame_Impl::on_process_message(CL_GUIMessage &msg)
{
}

void CL_Frame_Impl::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_Rect rect(CL_Point(0,0), frame->get_geometry().get_size());
	CL_Rect content = part_component.get_content_box(rect);
	CL_Rect rect_header(content.left, content.top, content.right, content.top + part_header.get_preferred_height());
	CL_Rect rect_body(content.left, rect_header.bottom, content.right, content.bottom);
	CL_Rect rect_header_content = part_header.get_content_box(rect_header);

	int content_caption_width = 0;
	if (!header_icon.is_null())
		content_caption_width = header_icon.get_width() + 4;
	if (!header_text.empty())
	{
		content_caption_width += font.get_text_size(gc, header_text).width;
	}

	int caption_width = part_header_caption.get_render_box(CL_Rect(0, 0, content_caption_width, rect_header_content.get_height())).get_width();

	CL_Rect rect_header_left(rect_header_content.left, rect_header_content.top, rect_header_content.left + part_header_left.get_preferred_width(), rect_header_content.bottom);
	CL_Rect rect_header_caption(rect_header_left.right, rect_header_content.top, rect_header_left.right + caption_width, rect_header_content.bottom);
	CL_Rect rect_header_right(rect_header_caption.right, rect_header_content.top, rect_header_content.right, rect_header_content.bottom);

	part_component.render_box(gc, rect, update_rect);
	part_header.render_box(gc, rect_header, update_rect);
	part_body.render_box(gc, rect_body, update_rect);
	part_header_left.render_box(gc, rect_header_left, update_rect);
	part_header_caption.render_box(gc, rect_header_caption, update_rect);
	part_header_right.render_box(gc, rect_header_right, update_rect);

	CL_Rect rect_caption_content = part_header_caption.get_content_box(rect_header_caption);
	int xpos = rect_caption_content.left;
	if (!header_icon.is_null())
	{
		header_icon.draw(gc, (float)xpos, (float)rect_caption_content.bottom - 6 - header_icon.get_height());
		xpos += header_icon.get_width() + 4;
	}
	if (!header_text.empty())
	{
		font.draw_text(gc, xpos, rect_caption_content.bottom - 6, header_text, text_color);
	}
}

void CL_Frame_Impl::create_parts()
{
	part_component = CL_GUIThemePart(frame);
	part_header = CL_GUIThemePart(frame, CssStr::Frame::part_header);
	part_header_left = CL_GUIThemePart(frame, CssStr::Frame::part_header_left);
	part_header_caption = CL_GUIThemePart(frame, CssStr::Frame::part_header_caption);
	part_header_right = CL_GUIThemePart(frame, CssStr::Frame::part_header_right);
	part_body = CL_GUIThemePart(frame, CssStr::Frame::part_body);
	text_color = part_component.get_property(prop_text_color);

	font = part_component.get_font();
}

void CL_Frame_Impl::on_style_changed()
{
	create_parts();
}
