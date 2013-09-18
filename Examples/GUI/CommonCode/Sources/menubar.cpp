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
#include "menubar.h"

MenuBar::MenuBar(clan::GUIManager &manager, clan::ResourceManager &application_resources) :
	clan::Window(&manager, clan::GUITopLevelDescription("Menu Bar & Status Bar", clan::Rect(512 + 24, 256 + 16, clan::Size(256, 180)), false))
{
	func_close().set(this, &MenuBar::on_close);
	tux_image = clan::ImageProviderFactory::load("../CommonCode/Resources/tux.png");

	clan::Canvas canvas = get_canvas();
	tux_head = clan::Image::resource(canvas, "tux_head", application_resources);



	clan::Rect client_area = get_client_area();

	menubar1 = new clan::MenuBar(this);
	menubar1->set_geometry(clan::Rect(client_area.left, client_area.top, client_area.right, client_area.top + 25));

	menu_file.insert_item("New").func_clicked().set(this, &MenuBar::on_item_selected);
	menu_file.insert_item("Open").func_clicked().set(this, &MenuBar::on_item_selected);
	menu_file.insert_item("Save").func_clicked().set(this, &MenuBar::on_item_selected);
	clan::PopupMenuItem tux_item = menu_file.insert_item("Tux");
	tux_item.func_clicked().set(this, &MenuBar::on_item_selected);
	tux_item.set_icon(tux_image);

	menu_file.insert_item("Exit").func_clicked().set(this, &MenuBar::on_item_selected);
	menubar1->add_menu("File", menu_file);

	menu_edit.insert_item("Undo").func_clicked().set(this, &MenuBar::on_item_selected);
	clan::PopupMenuItem redo_item = menu_edit.insert_item("Redo");
	redo_item.func_clicked().set(this, &MenuBar::on_item_selected);
	redo_item.set_enabled(false);
	menu_edit.insert_separator();
	menu_edit.insert_item("Cut").func_clicked().set(this, &MenuBar::on_item_selected);
	menu_edit.insert_item("Copy").func_clicked().set(this, &MenuBar::on_item_selected);
	menu_edit.insert_separator();
	item_submenu = menu_edit.insert_item("Submenu");
	item_submenu.func_clicked().set(this, &MenuBar::on_item_selected);
	menu_edit.insert_separator();
	menu_edit.insert_item("Paste").func_clicked().set(this, &MenuBar::on_item_selected);
	menu_edit.insert_item("Delete").func_clicked().set(this, &MenuBar::on_item_selected);
	menu_edit.insert_separator();
	menu_edit.insert_item("Select All").func_clicked().set(this, &MenuBar::on_item_selected);

	menu_submenu.insert_item("foo").func_clicked().set(this, &MenuBar::on_item_selected);
	menu_submenu.insert_item("bar").func_clicked().set(this, &MenuBar::on_item_selected);
	menu_submenu.insert_item("foobar").func_clicked().set(this, &MenuBar::on_item_selected);
	clan::PopupMenuItem check_item1 = menu_submenu.insert_item("Checkable 1");
	check_item1.func_clicked().set(this, &MenuBar::on_item_selected);
	check_item1.set_checkable(true);
	check_item1.set_checked(true);
	clan::PopupMenuItem check_item2 = menu_submenu.insert_item("Checkable 2");
	check_item2.func_clicked().set(this, &MenuBar::on_item_selected);
	check_item2.set_checkable(true);
	check_item2.set_checked(false);
	clan::PopupMenuItem check_item3 = menu_submenu.insert_item("Disabled Checkable 1");
	check_item3.func_clicked().set(this, &MenuBar::on_item_selected);
	check_item3.set_checkable(true);
	check_item3.set_checked(true);
	check_item3.set_enabled(false);
	clan::PopupMenuItem check_item4 = menu_submenu.insert_item("Disabled Checkable 2");
	check_item4.func_clicked().set(this, &MenuBar::on_item_selected);
	check_item4.set_checkable(true);
	check_item4.set_checked(false);
	check_item4.set_enabled(false);
	item_submenu.set_submenu(menu_submenu);

	menubar1->add_menu("Edit", menu_edit);

	int xoffset = client_area.left + 80;
	int yoffset = client_area.top + 30;
	const int gap = 16;
	clan::Size label_size(50, 15);

	info_item_selected = new Info(this);
	info_item_selected->set(xoffset, yoffset, "Item Selected");
	yoffset += gap;

	statusbar1 = new clan::StatusBar(this);
	int statusbar_height = statusbar1->get_height();
	clan::Rect statusbar_rect(0, client_area.get_height() - statusbar_height, clan::Size(client_area.get_width(), statusbar_height));
	statusbar_rect.translate(client_area.left, client_area.top);
	statusbar1->set_geometry(statusbar_rect);
	statusbar1->set_status_text("Status bar");
	statusbar1->show_size_grip(true);

	pushbutton1 = new clan::PushButton(statusbar1);
	pushbutton1->set_icon(tux_head);
	pushbutton1->func_clicked().set(this, &MenuBar::on_clicked, pushbutton1);
	statusbar1->add_part(0, 48, pushbutton1);
	statusbar1->func_part_double_clicked(0).set(this, &MenuBar::on_part_double_clicked_0, statusbar1);

	component1 = new clan::GUIComponent(statusbar1, "A");
	statusbar1->add_part(1, 48, component1);
	statusbar1->set_part_text(1, "ClanTest");
	statusbar1->func_part_double_clicked(1).set(this, &MenuBar::on_part_double_clicked_1, statusbar1);

	clan::GUIComponent *component2 = (new clan::GUIComponent(statusbar1, "B"));
	statusbar1->add_part(2, 48, component2);
	statusbar1->set_part_text(2, tux_head, "");
	statusbar1->func_part_double_clicked(2).set(this, &MenuBar::on_part_double_clicked_2, statusbar1);

	xoffset = client_area.left + 5;
	yoffset = client_area.top + 40;

	label_status = new clan::Label(this);
	label_status->set_geometry(clan::Rect(xoffset, yoffset, label_size));
	label_status->set_text("Status Bar:");
	yoffset += gap;

	checkbox_status_size_grip = new clan::CheckBox(this);
	checkbox_status_size_grip->set_geometry(clan::Rect(xoffset, yoffset, clan::Size(150, 15)));
	checkbox_status_size_grip->func_checked().set(this, &MenuBar::on_checked_status_size_grip, checkbox_status_size_grip);
	checkbox_status_size_grip->func_unchecked().set(this, &MenuBar::on_unchecked_status_size_grip, checkbox_status_size_grip);
	checkbox_status_size_grip->set_text("Show Size Grip");
	checkbox_status_size_grip->set_checked(true);
	yoffset += gap;

	checkbox_status_text = new clan::CheckBox(this);
	checkbox_status_text->set_geometry(clan::Rect(xoffset, yoffset, clan::Size(150, 15)));
	checkbox_status_text->func_checked().set(this, &MenuBar::on_checked_status_text, checkbox_status_text);
	checkbox_status_text->func_unchecked().set(this, &MenuBar::on_unchecked_status_text, checkbox_status_text);
	checkbox_status_text->set_text("Change Status Text");
	checkbox_status_text->set_checked(true);
	yoffset += gap;

	checkbox_status_show_clantest = new clan::CheckBox(this);
	checkbox_status_show_clantest->set_geometry(clan::Rect(xoffset, yoffset, clan::Size(150, 15)));
	checkbox_status_show_clantest->func_checked().set(this, &MenuBar::on_checked_show_clantest, checkbox_status_show_clantest);
	checkbox_status_show_clantest->func_unchecked().set(this, &MenuBar::on_unchecked_show_clantest, checkbox_status_show_clantest);
	checkbox_status_show_clantest->set_text("Show ClanTest");
	checkbox_status_show_clantest->set_checked(true);
	yoffset += gap;

	checkbox_status_remove_clantest = new clan::CheckBox(this);
	checkbox_status_remove_clantest->set_geometry(clan::Rect(xoffset, yoffset, clan::Size(150, 15)));
	checkbox_status_remove_clantest->func_checked().set(this, &MenuBar::on_checked_remove_clantest, checkbox_status_remove_clantest);
	checkbox_status_remove_clantest->func_unchecked().set(this, &MenuBar::on_unchecked_remove_clantest, checkbox_status_remove_clantest);
	checkbox_status_remove_clantest->set_text("Remove ClanTest");
	checkbox_status_remove_clantest->set_checked(false);
	yoffset += gap;

	xoffset = client_area.left + 110;
	yoffset -= (gap*2);

	info_clicked = new Info(this);
	info_clicked->set(xoffset, yoffset, "Push Button Clicked");

	yoffset += gap;

	info_part_clicked = new Info(this);
	info_part_clicked->set(xoffset, yoffset, "Part Dbl Clicked");

}

void MenuBar::on_item_selected()
{
	info_item_selected->activate();
}

void MenuBar::on_checked_status_size_grip(clan::CheckBox *checkbox)
{
	statusbar1->show_size_grip(true);
}

void MenuBar::on_unchecked_status_size_grip(clan::CheckBox *checkbox)
{
	statusbar1->show_size_grip(false);
}

void MenuBar::on_checked_status_text(clan::CheckBox *checkbox)
{
	statusbar1->set_status_text("This Status");
}

void MenuBar::on_unchecked_status_text(clan::CheckBox *checkbox)
{
	statusbar1->set_status_text("Alternate Status");
}

void MenuBar::on_checked_show_clantest(clan::CheckBox *checkbox)
{
	statusbar1->show_part(1, true);
}

void MenuBar::on_unchecked_show_clantest(clan::CheckBox *checkbox)
{
	statusbar1->show_part(1, false);
}

void MenuBar::on_checked_remove_clantest(clan::CheckBox *checkbox)
{
	statusbar1->remove_part(1);
	checkbox_status_show_clantest->set_enabled(false);
}

void MenuBar::on_unchecked_remove_clantest(clan::CheckBox *checkbox)
{
	statusbar1->add_part(1, 48, component1);
	statusbar1->set_part_text(1, "ClanTest");
	checkbox_status_show_clantest->set_enabled(true);
	checkbox_status_show_clantest->set_checked(true);
}

void MenuBar::on_clicked(clan::PushButton *pushbutton)
{
	info_clicked->activate();
}

void MenuBar::on_part_double_clicked_0( clan::StatusBar *statusbar)
{
	std::string value="0";
	info_part_clicked->set_comment(value);
	info_part_clicked->activate();
}
void MenuBar::on_part_double_clicked_1( clan::StatusBar *statusbar)
{
	std::string value="1";
	info_part_clicked->set_comment(value);
	info_part_clicked->activate();
}
void MenuBar::on_part_double_clicked_2( clan::StatusBar *statusbar)
{
	std::string value="2";
	info_part_clicked->set_comment(value);
	info_part_clicked->activate();
}


bool MenuBar::on_close()
{
	exit_with_code(0);
	return true;
}
