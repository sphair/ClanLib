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
**    Mark Page
*/

#include "precomp.h"
#include "tabpage.h"

TabPage::TabPage(clan::GUIManager &manager) :
	clan::Window(&manager, clan::GUITopLevelDescription("Tab Page", clan::Rect(32 + 256*3, 256*1 + 180*2 + 32, clan::Size(256, 180)), false))
{

	func_close().set(this, &TabPage::on_close);

	clan::Rect client_area = get_client_area();

	tab = new clan::Tab(this);
	tab->set_geometry(clan::Rect(client_area.left + 16, client_area.top + 10, clan::Size(220, 110)));

	// tab 1
	clan::TabPage *tab_page_1 = tab->add_page("foo");

	label_p1 = new clan::Label(tab_page_1);
	label_p1->set_geometry(clan::Rect(clan::Point(10,10),clan::Size(130,20)));
	label_p1->set_text("tab page 1");

	btn_p1 = new clan::PushButton(tab_page_1);
	btn_p1->set_geometry(clan::Rect(clan::Point(10,30),clan::Size(130,20)));
	btn_p1->set_text("Button Foo");

	// tab 2
	clan::TabPage *tab_page_2 = tab->add_page("bar");

	label_p2 = new clan::Label(tab_page_2);
	label_p2->set_geometry(clan::Rect(clan::Point(10,10),clan::Size(130,20)));
	label_p2->set_text("tab page 2");

	btn_p2 = new clan::PushButton(tab_page_2);
	btn_p2->set_geometry(clan::Rect(clan::Point(10,30),clan::Size(80,26)));
	btn_p2->set_text("Button Bar");

	// tab 3
	clan::TabPage *tab_page_3 = tab->add_page("p3");
}
bool TabPage::on_close()
{
	exit_with_code(0);
	return true;
}
