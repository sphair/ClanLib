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
**    Mark Page
*/

#include "precomp.h"
#include "checkbox.h"
#include "GUI.h"

CheckBox::CheckBox(GUI *gui) : 
	CL_Window(&gui->get_gui_manager(), CL_GUITopLevelDescription("CheckBox", CL_Rect(8, 256 + 16, CL_Size(256, 180)), false)),
	gui(gui)
{
	set_draggable(true);

	CL_Rect client_area = get_client_area();

	checkbox1 = new CL_CheckBox(this);
	checkbox1->set_geometry(CL_Rect(client_area.left + 11, client_area.top + 10, CL_Size(100, 15)));
	checkbox1->func_checked().set(this, &CheckBox::on_checked, checkbox1);
	checkbox1->func_unchecked().set(this, &CheckBox::on_unchecked, checkbox1);
	checkbox1->func_indeterminated().set(this, &CheckBox::on_indeterminated, checkbox1);
	checkbox1->func_state_changed().set(this, &CheckBox::on_state_changed, checkbox1);
	checkbox1->set_text("Checkbox");

	int xoffset = client_area.left + 36;
	int yoffset = client_area.top + 40;
	const int gap = 16;
	CL_Size label_size(50, 15);

	info_checked = new Info(gui, this);
	info_checked->set(xoffset, yoffset, "Checked");
	yoffset += gap;
	info_unchecked = new Info(gui, this);
	info_unchecked->set(xoffset, yoffset, "Unchecked");
	yoffset += gap;
	info_indeterminated = new Info(gui, this);
	info_indeterminated->set(xoffset, yoffset, "Indeterminated");
	yoffset += gap;
	info_state_changed = new Info(gui, this);
	info_state_changed->set(xoffset, yoffset, "State Changed");
	yoffset += gap;

	yoffset = client_area.top + 126 - gap;

	xoffset = client_area.left + 11;

	checkbox_3state = new CL_CheckBox(this);
	checkbox_3state->set_geometry(CL_Rect(xoffset, yoffset, CL_Size(100, 15)));
	checkbox_3state->func_checked().set(this, &CheckBox::on_checked_3state, checkbox_3state);
	checkbox_3state->func_unchecked().set(this, &CheckBox::on_unchecked_3state, checkbox_3state);
	checkbox_3state->set_text("3 State");

	yoffset += gap;

	checkbox_disable = new CL_CheckBox(this);
	checkbox_disable->set_geometry(CL_Rect(xoffset, yoffset, CL_Size(100, 15)));
	checkbox_disable->func_checked().set(this, &CheckBox::on_checked_disable, checkbox_disable);
	checkbox_disable->func_unchecked().set(this, &CheckBox::on_unchecked_disable, checkbox_disable);
	checkbox_disable->set_text("Disable");
}

void CheckBox::on_checked(CL_CheckBox *checkbox)
{
	info_checked->activate();
}

void CheckBox::on_unchecked(CL_CheckBox *checkbox)
{
	info_unchecked->activate();
}

void CheckBox::on_indeterminated(CL_CheckBox *checkbox)
{
	info_indeterminated->activate();
}

void CheckBox::on_state_changed(CL_CheckBox *checkbox)
{
	info_state_changed->activate();
}

void CheckBox::on_checked_disable(CL_CheckBox *checkbox)
{
	checkbox1->set_enabled(false);
}

void CheckBox::on_unchecked_disable(CL_CheckBox *checkbox)
{
	checkbox1->set_enabled(true);
}

void CheckBox::on_checked_3state(CL_CheckBox *checkbox)
{
	checkbox1->set_3state(true);
}

void CheckBox::on_unchecked_3state(CL_CheckBox *checkbox)
{
	checkbox1->set_3state(false);
}
