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
**    Mark Page
*/

#include "precomp.h"

#include "gui.h"
#include "window_1.h"

Window1::Window1(GUI &gui, Rect gui_position) :
	Window(&gui.get_gui_manager(), GUITopLevelDescription("Window 1", gui_position, false))
{
	Rect client_area = get_client_area();

	ScrollBar *scrollbar_vertical = new ScrollBar(this);
	scrollbar_vertical->set_geometry(Rect(client_area.left + 1, client_area.top + 10, Size(17, 200)));
	scrollbar_vertical->set_vertical();
	scrollbar_vertical->set_min(0);
	scrollbar_vertical->set_max(100);
	scrollbar_vertical->set_line_step(1);
	scrollbar_vertical->set_page_step(40);
	scrollbar_vertical->set_position(50);

	ScrollBar *scrollbar_horizontal = new ScrollBar(this);
	scrollbar_horizontal->set_geometry(Rect(client_area.left + 26, client_area.top + 10, Size(200, 17)));
	scrollbar_horizontal->set_horizontal();
	scrollbar_horizontal->set_min(0);
	scrollbar_horizontal->set_max(100);
	scrollbar_horizontal->set_line_step(1);
	scrollbar_horizontal->set_page_step(40);
	scrollbar_horizontal->set_position(50);

	PushButton *pushbutton1 = new PushButton(this);
	pushbutton1->set_geometry(Rect(client_area.left + 70, client_area.top + 100, Size(128, 40)));
	pushbutton1->set_text("Push Button");
}

Window1::~Window1()
{
}
