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
**    Magnus Norddahl
*/
#include "GameIDE/precomp.h"
#include "view_frame.h"
#include "workspace.h"
#include "dockable_component.h"

namespace clan
{

ViewFrame::ViewFrame(GUIComponent *parent, DockableComponent *dockable)
: GUIComponent(parent, "view-frame"), dockable(dockable)
{
	part_header = GUIThemePart(this, "header");
	func_resized().set(this, &ViewFrame::on_resized);
	func_render().set(this, &ViewFrame::on_render);
	func_input_doubleclick().set(this, &ViewFrame::on_input_doubleclick);
	dockable->set_parent_component(this);
	dockable->set_visible(true, false);
	set_visible(true, false);
	on_resized();
	request_repaint();
}

void ViewFrame::on_render(Canvas &canvas, const Rect &update_box)
{
	part_header.render_box(canvas, header_box);
	part_header.render_text(canvas, dockable->get_title(), part_header.get_content_box(header_box));
}

void ViewFrame::on_resized()
{
	Rect client_box = get_content_box();
	int header_height = part_header.get_css_height();
	scene_box = Rect(client_box.left, client_box.top, client_box.right, client_box.bottom - header_height);
	header_box = Rect(client_box.left, client_box.bottom - header_height, client_box.right, client_box.bottom);
	dockable->set_geometry(scene_box);
}

bool ViewFrame::on_input_doubleclick(const InputEvent &input_event)
{
	if (input_event.id == mouse_left)
	{
		if(header_box.contains(input_event.mouse_pos))
		{
			dockable->show_as_window();
		}
		return true;
	}
	else
	{
		return false;
	}
}


}

