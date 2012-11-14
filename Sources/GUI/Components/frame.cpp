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
#include "API/GUI/gui_component_description.h"
#include "API/GUI/Components/frame.h"
#include "API/GUI/Components/label.h"
#include "API/GUI/Components/imageview.h"
#include "API/Display/Font/font.h"
#include "API/Display/2D/sprite.h"
#include "API/Display/2D/image.h"
#include "../gui_css_strings.h"
#include "API/Display/2D/canvas.h"

namespace clan
{

class Frame_Impl
{
public:
	Frame *frame;

	GUIComponent *part_header;
	GUIComponent *part_header_left;
	ImageView *part_header_icon;
	Label *part_header_caption;
	GUIComponent *part_header_right;
	GUIComponent *part_body;
};

Frame::Frame(GUIComponent *parent)
: GUIComponent(parent), impl(new Frame_Impl)
{
	set_tag_name(CssStr::Frame::type_name);
	impl->frame = this;

	impl->part_header = new GUIComponent(this);
	impl->part_header_left = new GUIComponent(impl->part_header);
	impl->part_header_icon = new ImageView(impl->part_header);
	impl->part_header_caption = new Label(impl->part_header);
	impl->part_header_right = new GUIComponent(impl->part_header);
	impl->part_body = new GUIComponent(this);

	impl->part_header->set_tag_name(CssStr::Frame::part_header);
	impl->part_header_left->set_tag_name(CssStr::Frame::part_header_left);
	impl->part_header_icon->set_tag_name(CssStr::Frame::part_header_icon);
	impl->part_header_caption->set_tag_name(CssStr::Frame::part_header_caption);
	impl->part_header_right->set_tag_name(CssStr::Frame::part_header_right);
	impl->part_body->set_tag_name(CssStr::Frame::part_body);
}

Frame::~Frame()
{
}

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
	return impl->part_header_icon->get_sprite();
}

std::string Frame::get_header_text() const
{
	return impl->part_header_caption->get_text();
}

void Frame::set_header_icon(const Sprite &icon)
{
	impl->part_header_icon->set_sprite(icon);
}

void Frame::set_header_text(const std::string &text)
{
	impl->part_header_caption->set_text(text);
}

}
