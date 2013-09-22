/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
*/

#include "precomp.h"
#include "view_border.h"

ViewBorder::ViewBorder(clan::GUIComponent *parent)
: clan::GUIComponent(parent)
{
	set_tag_name("viewborder");
	func_resized().set(this, &ViewBorder::on_resized);
	func_render().set(this, &ViewBorder::on_render);
}

void ViewBorder::on_resized()
{
	clan::Rect content_area = get_content_box();
	clan::GUIComponent *child = get_first_child();
	while (child)
	{
		child->set_geometry(content_area);
		child = child->get_next_sibling();
	}
}

void ViewBorder::on_render(clan::Canvas &canvas, const clan::Rect &update_rect)
{
}

