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
#include "pushbutton.h"

PushButton::PushButton(clan::GUIManager &manager, clan::ResourceManager &application_resources) : 
	clan::Window(&manager, clan::GUITopLevelDescription("PushButton", clan::Rect(256 + 16, 256 + 16, clan::Size(256, 180)), false))
{
	func_close().set(this, &PushButton::on_close);
	clan::Canvas canvas = get_canvas();
	test_image = clan::Image::resource(canvas, "tux", application_resources);

	clan::Rect client_area = get_client_area();

	pushbutton1 = new clan::PushButton(this);
	pushbutton1->set_geometry(clan::Rect(client_area.left + 11, client_area.top + 10, clan::Size(128, 40)));
	pushbutton1->set_text("Push Button");
	pushbutton1->func_clicked().set(this, &PushButton::on_clicked, pushbutton1);

	int label_xpos = client_area.left + 31;
	int yoffset = client_area.top + 80;
	clan::Size label_size(50, 15);
	const int gap = 16;

	checkbox_icon = new clan::CheckBox(this);
	checkbox_icon->set_geometry(clan::Rect(client_area.left + 11, yoffset, clan::Size(100, 15)));
	checkbox_icon->func_checked().set(this, &PushButton::on_checked_icon, checkbox_icon);
	checkbox_icon->func_unchecked().set(this, &PushButton::on_unchecked_icon, checkbox_icon);
	checkbox_icon->set_text("Icon");

	yoffset+=gap;
	checkbox_toggle = new clan::CheckBox(this);
	checkbox_toggle->set_geometry(clan::Rect(client_area.left + 11, yoffset, clan::Size(100, 15)));
	checkbox_toggle->func_checked().set(this, &PushButton::on_checked_toggle, checkbox_toggle);
	checkbox_toggle->func_unchecked().set(this, &PushButton::on_unchecked_toggle, checkbox_toggle);
	checkbox_toggle->set_text("Enable Toggle");

	yoffset+=gap;
	checkbox_disable = new clan::CheckBox(this);
	checkbox_disable->set_geometry(clan::Rect(client_area.left + 11, yoffset, clan::Size(100, 15)));
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


void PushButton::on_clicked(clan::PushButton *pushbutton)
{
	info_clicked->activate();
}

void PushButton::on_checked_disable(clan::CheckBox *checkbox)
{
	pushbutton1->set_enabled(false);
}

void PushButton::on_unchecked_disable(clan::CheckBox *checkbox)
{
	pushbutton1->set_enabled(true);
}

void PushButton::on_checked_icon(clan::CheckBox *checkbox)
{
	pushbutton1->set_icon(test_image);
}

void PushButton::on_unchecked_icon(clan::CheckBox *checkbox)
{
	pushbutton1->set_icon(clan::Image());
}

void PushButton::on_checked_toggle(clan::CheckBox *checkbox)
{
	pushbutton1->set_toggle(true);
}

void PushButton::on_unchecked_toggle(clan::CheckBox *checkbox)
{
	pushbutton1->set_toggle(false);
}

bool PushButton::on_close()
{
	exit_with_code(0);
	return true;
}

