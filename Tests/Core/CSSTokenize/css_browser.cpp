/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
: CL_Window(gui, get_window_description()), view(0)
{
	view = new CSSView(this);
	func_close().set(this, &CSSBrowser::on_close);
	func_resized().set(this, &CSSBrowser::on_resized);
	on_resized();
}

CL_GUITopLevelDescription CSSBrowser::get_window_description()
{
	CL_GUITopLevelDescription desc;
	desc.set_size(CL_Size(1000, 700), false);
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
	view->set_geometry(get_client_area());
}
