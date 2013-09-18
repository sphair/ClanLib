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
#include "radiobutton.h"

RadioButton::RadioButton(clan::GUIManager &manager) : 
	clan::Window(&manager, clan::GUITopLevelDescription("Radio Button", clan::Rect(256 + 256 + 24, 256 + 180 + 24, clan::Size(256, 180)), false))
{

	func_close().set(this, &RadioButton::on_close);

	clan::Rect client_area = get_client_area();

	radiobutton_g1b1 = new clan::RadioButton(this);
	radiobutton_g1b1->set_geometry(clan::Rect(client_area.left + 11, client_area.top + 0 , clan::Size(64, 32)));
	radiobutton_g1b1->set_text("Button 1");
	radiobutton_g1b1->set_selected(true);
	radiobutton_g1b1->set_group_name("Main Group");
	radiobutton_g1b1->func_selected().set(this, &RadioButton::on_selected, radiobutton_g1b1);
	radiobutton_g1b1->func_unselected().set(this, &RadioButton::on_unselected, radiobutton_g1b1);
	radiobutton_g1b1->func_group_selection_changed().set(this, &RadioButton::on_group_selection_changed, radiobutton_g1b1);

	radiobutton_g1b2 = new clan::RadioButton(this);
	radiobutton_g1b2->set_geometry(clan::Rect(client_area.left + 91, client_area.top + 0, clan::Size(64, 32)));
	radiobutton_g1b2->set_text("Button 2");
	radiobutton_g1b2->set_group_name("Main Group");
	radiobutton_g1b2->func_selected().set(this, &RadioButton::on_selected, radiobutton_g1b2);
	radiobutton_g1b2->func_unselected().set(this, &RadioButton::on_unselected, radiobutton_g1b2);
	radiobutton_g1b2->func_group_selection_changed().set(this, &RadioButton::on_group_selection_changed, radiobutton_g1b2);

	radiobutton_g1b3 = new clan::RadioButton(this);
	radiobutton_g1b3->set_geometry(clan::Rect(client_area.left + 176, client_area.top + 0, clan::Size(64, 32)));
	radiobutton_g1b3->set_text("Button 3");
	radiobutton_g1b3->set_group_name("Main Group");
	radiobutton_g1b3->func_selected().set(this, &RadioButton::on_selected, radiobutton_g1b3);
	radiobutton_g1b3->func_unselected().set(this, &RadioButton::on_unselected, radiobutton_g1b3);
	radiobutton_g1b3->func_group_selection_changed().set(this, &RadioButton::on_group_selection_changed, radiobutton_g1b3);

	radiobutton_g2b1 = new clan::RadioButton(this);
	radiobutton_g2b1->set_geometry(clan::Rect(client_area.left + 11, client_area.top + 30, clan::Size(64, 32)));
	radiobutton_g2b1->set_text("Button A");
	radiobutton_g2b1->set_selected(true);
	radiobutton_g2b1->set_group_name("Second Group");
	radiobutton_g2b1->func_selected().set(this, &RadioButton::on_selected, radiobutton_g2b1);
	radiobutton_g2b1->func_unselected().set(this, &RadioButton::on_unselected, radiobutton_g2b1);
	radiobutton_g2b1->func_group_selection_changed().set(this, &RadioButton::on_group_selection_changed, radiobutton_g2b1);

	radiobutton_g2b2 = new clan::RadioButton(this);
	radiobutton_g2b2->set_geometry(clan::Rect(client_area.left + 91, client_area.top + 30, clan::Size(64, 32)));
	radiobutton_g2b2->set_text("Button B");
	radiobutton_g2b2->set_group_name("Second Group");
	radiobutton_g2b2->func_selected().set(this, &RadioButton::on_selected, radiobutton_g2b2);
	radiobutton_g2b2->func_unselected().set(this, &RadioButton::on_unselected, radiobutton_g2b2);
	radiobutton_g2b2->func_group_selection_changed().set(this, &RadioButton::on_group_selection_changed, radiobutton_g2b2);

	int xoffset = client_area.left + 5;
	int yoffset = client_area.top + 70;
	const int gap = 16;

	info_selected = new Info(this);
	info_selected->set(xoffset, yoffset, "Selected");
	yoffset += gap;
	info_unselected = new Info(this);
	info_unselected->set(xoffset, yoffset, "Unselected");
	yoffset += gap;
	info_group_selection_changed = new Info(this);
	info_group_selection_changed->set(xoffset, yoffset, "Group Selection Changed");
	yoffset += gap;

	xoffset = client_area.left + 1;
	yoffset = client_area.top + 126;
	clan::Size label_size(50, 15);

	checkbox_disable = new clan::CheckBox(this);
	checkbox_disable->set_geometry(clan::Rect(xoffset, yoffset, clan::Size(159, 15)));
	checkbox_disable->func_checked().set(this, &RadioButton::on_checked_disable, checkbox_disable);
	checkbox_disable->func_unchecked().set(this, &RadioButton::on_unchecked_disable, checkbox_disable);
	checkbox_disable->set_text("Disable Button 2");
}

void RadioButton::on_selected(clan::RadioButton *radiobutton)
{
	std::string string = decode_button(radiobutton);
	info_selected->set_comment( string );
	info_selected->activate();
}

void RadioButton::on_unselected(clan::RadioButton *radiobutton)
{
	std::string string = decode_button(radiobutton);
	info_unselected->set_comment( string );
	info_unselected->activate();
}

void RadioButton::on_group_selection_changed(clan::RadioButton *radiobutton_group, clan::RadioButton *radiobutton)
{
	std::string string = decode_button(radiobutton_group);

	info_group_selection_changed->set_comment( string );
	info_group_selection_changed->activate();
}

std::string RadioButton::decode_button(clan::RadioButton *radiobutton)
{
	if (radiobutton == radiobutton_g1b1)
		return " (Button 1)";
	if (radiobutton == radiobutton_g1b2)
		return " (Button 2)";
	if (radiobutton == radiobutton_g1b3)
		return " (Button 3)";
	if (radiobutton == radiobutton_g2b1)
		return " (Button A)";
	if (radiobutton == radiobutton_g2b2)
		return " (Button B)";
	return " (unknown)";
}

void RadioButton::on_checked_disable(clan::CheckBox *checkbox)
{
	radiobutton_g1b2->set_enabled(false);
}

void RadioButton::on_unchecked_disable(clan::CheckBox *checkbox)
{
	radiobutton_g1b2->set_enabled(true);
}

bool RadioButton::on_close()
{
	exit_with_code(0);
	return true;
}
