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
**    Mark Page
*/

#include "precomp.h"
#include "tabpage.h"
#include "GUI.h"

TabPage::TabPage(GUI *gui) :
	CL_Window(&gui->get_gui_manager(), CL_GUITopLevelDescription("Tab Page", CL_Rect(32 + 256*3, 256*1 + 180*2 + 32, CL_Size(256, 180)), false)),
	gui(gui)
{
	set_draggable(true);

	CL_Rect client_area = get_client_area();

	tab = new CL_Tab(this);
	tab->set_geometry(CL_Rect(client_area.left + 16, client_area.top + 10, CL_Size(220, 110)));

	// tab 1
	CL_TabPage *tab_page_1 = tab->add_page("foo");

	label_p1 = new CL_Label(tab_page_1);
	label_p1->set_geometry(CL_Rect(CL_Point(10,10),CL_Size(130,20)));
	label_p1->set_text("tab page 1");

	btn_p1 = new CL_PushButton(tab_page_1);
	btn_p1->set_geometry(CL_Rect(CL_Point(10,30),CL_Size(130,20)));
	btn_p1->set_text("Button Foo");

	// tab 2
	CL_TabPage *tab_page_2 = tab->add_page("bar");

	label_p2 = new CL_Label(tab_page_2);
	label_p2->set_geometry(CL_Rect(CL_Point(10,10),CL_Size(130,20)));
	label_p2->set_text("tab page 2");

	btn_p2 = new CL_PushButton(tab_page_2);
	btn_p2->set_geometry(CL_Rect(CL_Point(10,30),CL_Size(80,26)));
	btn_p2->set_text("Button Bar");

	// tab 3
	CL_TabPage *tab_page_3 = tab->add_page("p3");
}
