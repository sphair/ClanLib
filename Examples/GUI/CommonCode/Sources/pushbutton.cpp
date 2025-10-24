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
#include "pushbutton.h"

PushButton::PushButton(CL_GUIManager &manager, CL_ResourceManager &application_resources) : 
	CL_Window(&manager, CL_GUITopLevelDescription("PushButton", CL_Rect(256 + 16, 256 + 16, CL_Size(256, 180)), false))
{
	CL_GraphicContext gc = get_gc();
	test_image = CL_Image(gc, "tux", &application_resources);

	set_draggable(true);

	CL_Rect client_area = get_client_area();

	pushbutton1 = new CL_PushButton(this);
	pushbutton1->set_geometry(CL_Rect(client_area.left + 11, client_area.top + 10, CL_Size(128, 40)));
	pushbutton1->set_text("Push Button");
	pushbutton1->set_flat(false);
	pushbutton1->func_clicked().set(this, &PushButton::on_clicked, pushbutton1);

	int label_xpos = client_area.left + 31;
	int yoffset = client_area.top + 80;
	CL_Size label_size(50, 15);
	const int gap = 16;

	checkbox_flat = new CL_CheckBox(this);
	checkbox_flat->set_geometry(CL_Rect(client_area.left + 11, yoffset, CL_Size(100, 15)));
	checkbox_flat->func_checked().set(this, &PushButton::on_checked_flat, checkbox_flat);
	checkbox_flat->func_unchecked().set(this, &PushButton::on_unchecked_flat, checkbox_flat);
	checkbox_flat->set_text("Flat");

	yoffset+=gap;
	checkbox_icon = new CL_CheckBox(this);
	checkbox_icon->set_geometry(CL_Rect(client_area.left + 11, yoffset, CL_Size(100, 15)));
	checkbox_icon->func_checked().set(this, &PushButton::on_checked_icon, checkbox_icon);
	checkbox_icon->func_unchecked().set(this, &PushButton::on_unchecked_icon, checkbox_icon);
	checkbox_icon->set_text("Icon");

	yoffset+=gap;
	checkbox_toggle = new CL_CheckBox(this);
	checkbox_toggle->set_geometry(CL_Rect(client_area.left + 11, yoffset, CL_Size(100, 15)));
	checkbox_toggle->func_checked().set(this, &PushButton::on_checked_toggle, checkbox_toggle);
	checkbox_toggle->func_unchecked().set(this, &PushButton::on_unchecked_toggle, checkbox_toggle);
	checkbox_toggle->set_text("Enable Toggle");

	yoffset+=gap;
	checkbox_disable = new CL_CheckBox(this);
	checkbox_disable->set_geometry(CL_Rect(client_area.left + 11, yoffset, CL_Size(100, 15)));
	checkbox_disable->func_checked().set(this, &PushButton::on_checked_disable, checkbox_disable);
	checkbox_disable->func_unchecked().set(this, &PushButton::on_unchecked_disable, checkbox_disable);
	checkbox_disable->set_text("Disable");

	int xoffset = client_area.left + 36;
	yoffset = client_area.top + 60;

	info_clicked = new Info(this);
	info_clicked->set(xoffset, yoffset, "Clicked");
}

PushButton::~PushButton()
{
}


void PushButton::on_clicked(CL_PushButton *pushbutton)
{
	info_clicked->activate();
}

void PushButton::on_checked_disable(CL_CheckBox *checkbox)
{
	pushbutton1->set_enabled(false);
}

void PushButton::on_unchecked_disable(CL_CheckBox *checkbox)
{
	pushbutton1->set_enabled(true);
}

void PushButton::on_checked_icon(CL_CheckBox *checkbox)
{
	pushbutton1->set_icon(test_image);
}

void PushButton::on_unchecked_icon(CL_CheckBox *checkbox)
{
	pushbutton1->set_icon(CL_Image());
}

void PushButton::on_checked_toggle(CL_CheckBox *checkbox)
{
	pushbutton1->set_toggle(true);
}

void PushButton::on_unchecked_toggle(CL_CheckBox *checkbox)
{
	pushbutton1->set_toggle(false);
}

void PushButton::on_checked_flat(CL_CheckBox *checkbox)
{
	pushbutton1->set_flat(false);
}

void PushButton::on_unchecked_flat(CL_CheckBox *checkbox)
{
	pushbutton1->set_flat(true);
}

