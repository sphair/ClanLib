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
#include "checkbox.h"

CheckBox::CheckBox(clan::GUIManager &manager) : 
	clan::Window(&manager, clan::GUITopLevelDescription("CheckBox", clan::Rect(8, 256 + 16, clan::Size(256, 180)), false))
{
	func_close().set(this, &CheckBox::on_close);
	clan::Rect client_area = get_client_area();

	checkbox1 = new clan::CheckBox(this);
	checkbox1->set_geometry(clan::Rect(client_area.left + 11, client_area.top + 10, clan::Size(100, 15)));
	checkbox1->func_checked().set(this, &CheckBox::on_checked, checkbox1);
	checkbox1->func_unchecked().set(this, &CheckBox::on_unchecked, checkbox1);
	checkbox1->func_indeterminated().set(this, &CheckBox::on_indeterminated, checkbox1);
	checkbox1->func_state_changed().set(this, &CheckBox::on_state_changed, checkbox1);
	checkbox1->set_text("Checkbox");

	int xoffset = client_area.left + 36;
	int yoffset = client_area.top + 40;
	const int gap = 16;
	clan::Size label_size(50, 15);

	info_checked = new Info(this);
	info_checked->set(xoffset, yoffset, "Checked");
	yoffset += gap;
	info_unchecked = new Info(this);
	info_unchecked->set(xoffset, yoffset, "Unchecked");
	yoffset += gap;
	info_indeterminated = new Info(this);
	info_indeterminated->set(xoffset, yoffset, "Indeterminated");
	yoffset += gap;
	info_state_changed = new Info(this);
	info_state_changed->set(xoffset, yoffset, "State Changed");
	yoffset += gap;

	yoffset = client_area.top + 126 - gap;

	xoffset = client_area.left + 11;

	checkbox_3state = new clan::CheckBox(this);
	checkbox_3state->set_geometry(clan::Rect(xoffset, yoffset, clan::Size(100, 15)));
	checkbox_3state->func_checked().set(this, &CheckBox::on_checked_3state, checkbox_3state);
	checkbox_3state->func_unchecked().set(this, &CheckBox::on_unchecked_3state, checkbox_3state);
	checkbox_3state->set_text("3 State");

	yoffset += gap;

	checkbox_disable = new clan::CheckBox(this);
	checkbox_disable->set_geometry(clan::Rect(xoffset, yoffset, clan::Size(100, 15)));
	checkbox_disable->func_checked().set(this, &CheckBox::on_checked_disable, checkbox_disable);
	checkbox_disable->func_unchecked().set(this, &CheckBox::on_unchecked_disable, checkbox_disable);
	checkbox_disable->set_text("Disable");
}

void CheckBox::on_checked(clan::CheckBox *checkbox)
{
	info_checked->activate();
}

void CheckBox::on_unchecked(clan::CheckBox *checkbox)
{
	info_unchecked->activate();
}

void CheckBox::on_indeterminated(clan::CheckBox *checkbox)
{
	info_indeterminated->activate();
}

void CheckBox::on_state_changed(clan::CheckBox *checkbox)
{
	info_state_changed->activate();
}

void CheckBox::on_checked_disable(clan::CheckBox *checkbox)
{
	checkbox1->set_enabled(false);
}

void CheckBox::on_unchecked_disable(clan::CheckBox *checkbox)
{
	checkbox1->set_enabled(true);
}

void CheckBox::on_checked_3state(clan::CheckBox *checkbox)
{
	checkbox1->set_3state(true);
}

void CheckBox::on_unchecked_3state(clan::CheckBox *checkbox)
{
	checkbox1->set_3state(false);
}
bool CheckBox::on_close()
{
	exit_with_code(0);
	return true;
}
