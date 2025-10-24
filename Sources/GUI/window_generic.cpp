/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "precomp.h"
#include "window_generic.h"
#include "API/Core/XML/dom_element.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_Window_Generic::CL_Window_Generic(
	CL_Window *window,
	const std::string &title,
	int titlebar_buttons)
	: title(title), titlebar_buttons(titlebar_buttons), window(window)
{
	CL_Component *client_area = new CL_Component(window);
	window->set_client_area(client_area);

	slot_set_options = window->sig_set_options().connect(this, &CL_Window_Generic::on_set_options);
	slot_close = sig_close_button_clicked.connect(this, &CL_Window_Generic::on_close);
}

CL_Window_Generic::~CL_Window_Generic()
{
	delete window->get_client_area();
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_Window_Generic::on_set_options(const CL_DomElement &options)
{
	if (options.has_attribute("title"))
		title = options.get_attribute("title");
}

void CL_Window_Generic::on_close()
{
	if (window->is_modal())
		window->quit();
	else
		window->close();
}
