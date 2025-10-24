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
**    Magnus Norddahl
*/

#include "precomp.h"
#include "css_browser.h"
#include "css_view.h"

CSSBrowser::CSSBrowser(CL_GUIManager *gui)
: CL_Window(gui, get_window_description()), label(0), edit(0), view(0)
{
	view = new CSSView(this);
	func_close().set(this, &CSSBrowser::on_close);
	func_resized().set(this, &CSSBrowser::on_resized);
	label = new CL_Label(this);
	edit = new CL_LineEdit(this);
	label->set_text("Address:");
	on_resized();
}

CL_GUITopLevelDescription CSSBrowser::get_window_description()
{
	CL_GUITopLevelDescription desc;
	desc.set_size(CL_Size(1280, 865), false);
	desc.set_title("CSS Browser");
	desc.set_allow_resize(true);
	return desc;
}

bool CSSBrowser::on_close()
{
	exit_with_code(0);
	return true;
}

void CSSBrowser::on_resized()
{
	CL_Rect client_box = get_client_area();
	int height = 38;
	CL_Rect toolbar_box(client_box.left, client_box.top, client_box.right, client_box.top + height);
	toolbar_box.shrink(11,11,11,5);
	CL_Rect label_box(toolbar_box.left, toolbar_box.top, toolbar_box.left+50, toolbar_box.bottom);
	CL_Rect edit_box(label_box.right, label_box.top, toolbar_box.right, label_box.bottom);
	CL_Rect view_box(client_box.left, client_box.top + height, client_box.right, client_box.bottom);
	label->set_geometry(label_box);
	edit->set_geometry(edit_box);
	view->set_geometry(view_box);
}
