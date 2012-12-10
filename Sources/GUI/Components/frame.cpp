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
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/Components/frame.h"
#include "API/Display/Font/font.h"
#include "API/Display/2D/sprite.h"
#include "../gui_css_strings.h"
#include "API/Display/2D/canvas.h"
#include "API/CSSLayout/css_box_properties.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Frame_Impl Class:

class Frame_Impl
{
public:
	void on_process_message(std::shared_ptr<GUIMessage> &msg);
	void on_render(Canvas &canvas, const Rect &update_rect);

	void create_parts();

	Frame *frame;

	Sprite header_icon;
	std::string header_text;
	
	GUIThemePart part_header;
	GUIThemePart part_header_left;
	GUIThemePart part_header_caption;
	GUIThemePart part_header_right;
	GUIThemePart part_body;
};

/////////////////////////////////////////////////////////////////////////////
// Frame Construction:

Frame::Frame(GUIComponent *parent)
: GUIComponent(parent, CssStr::Frame::type_name), impl(new Frame_Impl)
{
	impl->frame = this;
	func_process_message().set(impl.get(), &Frame_Impl::on_process_message);
	func_render().set(impl.get(), &Frame_Impl::on_render);

	impl->create_parts();
}

Frame::~Frame()
{
}

/////////////////////////////////////////////////////////////////////////////
// Frame Attributes:

Frame *Frame::get_named_item(GUIComponent *reference_component, const std::string &id)
{
	Frame *object = NULL;
	if (reference_component)
		object = dynamic_cast<Frame*>(reference_component->get_named_item(id));

	if (!object)
		throw Exception(string_format("Cannot find Frame named item: %1", id));

	return object;
}

Sprite Frame::get_header_icon() const
{
	return impl->header_icon;
}

std::string Frame::get_header_text() const
{
	return impl->header_text;
}

/////////////////////////////////////////////////////////////////////////////
// Frame Operations:

void Frame::set_header_icon(const Sprite &icon)
{
	impl->header_icon = icon;
}

void Frame::set_header_text(const std::string &text)
{
	impl->header_text = text;
}

/////////////////////////////////////////////////////////////////////////////
// Frame Implementation:

void Frame_Impl::on_process_message(std::shared_ptr<GUIMessage> &msg)
{
}

void Frame_Impl::on_render(Canvas &canvas, const Rect &update_rect)
{
	Rect rect(Point(0,0), frame->get_geometry().get_size());
	Rect content = frame->get_content_box();
	Rect rect_header(content.left, content.top, content.right, content.top + part_header.get_preferred_height());
	Rect rect_body(content.left, rect_header.bottom, content.right, content.bottom);
	Rect rect_header_content = part_header.get_content_box(rect_header);

	int content_caption_width = 0;
	if (!header_icon.is_null())
		content_caption_width = header_icon.get_width() + 4;
	if (!header_text.empty())
	{
		content_caption_width += frame->get_text_size(canvas, header_text).width;
	}

	int caption_width = part_header_caption.get_render_box(Rect(0, 0, content_caption_width, rect_header_content.get_height())).get_width();

	Rect rect_header_left(rect_header_content.left, rect_header_content.top, rect_header_content.left + part_header_left.get_preferred_width(), rect_header_content.bottom);
	Rect rect_header_caption(rect_header_left.right, rect_header_content.top, rect_header_left.right + caption_width, rect_header_content.bottom);
	Rect rect_header_right(rect_header_caption.right, rect_header_content.top, rect_header_content.right, rect_header_content.bottom);

	part_header.render_box(canvas, rect_header, update_rect);
	part_body.render_box(canvas, rect_body, update_rect);
	part_header_left.render_box(canvas, rect_header_left, update_rect);
	part_header_caption.render_box(canvas, rect_header_caption, update_rect);
	part_header_right.render_box(canvas, rect_header_right, update_rect);

	Rect rect_caption_content = part_header_caption.get_content_box(rect_header_caption);
	int xpos = rect_caption_content.left;
	if (!header_icon.is_null())
	{
		header_icon.draw(canvas, (float)xpos, (float)rect_caption_content.bottom - 6 - header_icon.get_height());
		xpos += header_icon.get_width() + 4;
	}
	if (!header_text.empty())
	{
		frame->get_font().draw_text(canvas, xpos, rect_caption_content.bottom - 6, header_text, frame->get_css_properties().color.color);
	}
}

void Frame_Impl::create_parts()
{
	part_header = GUIThemePart(frame, CssStr::Frame::part_header);
	part_header_left = GUIThemePart(frame, CssStr::Frame::part_header_left);
	part_header_caption = GUIThemePart(frame, CssStr::Frame::part_header_caption);
	part_header_right = GUIThemePart(frame, CssStr::Frame::part_header_right);
	part_body = GUIThemePart(frame, CssStr::Frame::part_body);
}

}
