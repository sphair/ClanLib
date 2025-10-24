/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
#include "manager.h"
#include "GUI.h"

Manager::Manager(GUI *gui) : 
	CL_Window(&gui->get_gui_manager(), CL_GUITopLevelDescription("Window Manager", CL_Rect(256*3 + 32, 256 + 180 + 32, CL_Size(256, 180)), false)),
	gui(gui)
{
	set_draggable(true);

	CL_Rect client_area = get_client_area();

	groupbox = new CL_GroupBox(this);
	groupbox->set_geometry(CL_Rect(client_area.left + 5, client_area.top + 5, client_area.right - 5, client_area.bottom - 5));

	button_system = new CL_PushButton(this);
	button_system->set_geometry(CL_Rect(client_area.left + 11, client_area.top + 10, CL_Size(128, 20)));
	button_system->set_text("System");
	button_system->func_clicked().set(this, &Manager::on_clicked_system, button_system);

	if (gui->get_manager() == manager_system)
		button_system->set_enabled(false);

	button_texture = new CL_PushButton(this);
	button_texture->set_geometry(CL_Rect(client_area.left + 11, client_area.top + 40, CL_Size(128, 20)));
	button_texture->set_text("Texture");
	button_texture->func_clicked().set(this, &Manager::on_clicked_texture, button_texture);

	if (gui->get_manager() == manager_texture)
		button_texture->set_enabled(false);

	checkbox_constant_repaint = new CL_CheckBox(this);
	checkbox_constant_repaint->set_geometry(CL_Rect(client_area.left + 11, client_area.top + 100, CL_Size(128, 40)));
	checkbox_constant_repaint->set_text("Constant repaint windows");
	checkbox_constant_repaint->func_state_changed().set(this, &Manager::on_statechanged_constant_repaint);

	int xoffset = client_area.left + 150;
	int yoffset = client_area.top + 4;
	int ygap = 24;

	radiobutton_aero = new CL_RadioButton(this);
	radiobutton_aero->set_geometry(CL_Rect(xoffset, yoffset , CL_Size(64, 24)));
	radiobutton_aero->set_text("Aero");
	radiobutton_aero->set_selected(gui->get_theme() == GUI::theme_aero);
	radiobutton_aero->set_group_name("Theme");
	radiobutton_aero->func_selected().set(this, &Manager::on_theme_selected, radiobutton_aero);
	yoffset += ygap;

	radiobutton_aero_packed = new CL_RadioButton(this);
	radiobutton_aero_packed->set_geometry(CL_Rect(xoffset, yoffset , CL_Size(64, 24)));
	radiobutton_aero_packed->set_text("Aero Packed");
	radiobutton_aero_packed->set_selected(gui->get_theme() == GUI::theme_aero_packed);
	radiobutton_aero_packed->set_group_name("Theme");
	radiobutton_aero_packed->func_selected().set(this, &Manager::on_theme_selected, radiobutton_aero_packed);
	yoffset += ygap;

	radiobutton_luna = new CL_RadioButton(this);
	radiobutton_luna->set_geometry(CL_Rect(xoffset, yoffset , CL_Size(64, 24)));
	radiobutton_luna->set_text("Luna");
	radiobutton_luna->set_selected(gui->get_theme() == GUI::theme_luna);
	radiobutton_luna->set_group_name("Theme");
	radiobutton_luna->func_selected().set(this, &Manager::on_theme_selected, radiobutton_luna);
	yoffset += ygap;

	radiobutton_luna_packed = new CL_RadioButton(this);
	radiobutton_luna_packed->set_geometry(CL_Rect(xoffset, yoffset , CL_Size(64, 24)));
	radiobutton_luna_packed->set_text("Luna Packed");
	radiobutton_luna_packed->set_selected(gui->get_theme() == GUI::theme_luna_packed);
	radiobutton_luna_packed->set_group_name("Theme");
	radiobutton_luna_packed->func_selected().set(this, &Manager::on_theme_selected, radiobutton_luna_packed);
	yoffset += ygap;
}

void Manager::run(CL_GraphicContext &gc)
{
}

void Manager::on_clicked_system(CL_PushButton *button)
{
	gui->set_manager(manager_system);
}

void Manager::on_clicked_texture(CL_PushButton *button)
{
	gui->set_manager(manager_texture);
}

void Manager::on_statechanged_constant_repaint()
{
	gui->set_constant_repaint(checkbox_constant_repaint->is_checked());
}

void Manager::on_theme_selected(CL_RadioButton *radiobutton)
{
	if (radiobutton == radiobutton_aero)
		gui->set_theme(GUI::theme_aero);

	if (radiobutton == radiobutton_aero_packed)
		gui->set_theme(GUI::theme_aero_packed);

	if (radiobutton == radiobutton_luna)
		gui->set_theme(GUI::theme_luna);

	if (radiobutton == radiobutton_luna_packed)
		gui->set_theme(GUI::theme_luna_packed);
}
