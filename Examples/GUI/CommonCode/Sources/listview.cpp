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
#include "listview.h"

ListView::ListView(clan::GUIManager &manager) :
	clan::Window(&manager, clan::GUITopLevelDescription("List View & Toolbar", clan::Rect(8, 256*1 + 180*1 + 24, clan::Size(256*2, 180*2)), false))
{

	func_close().set(this, &ListView::on_close);

	clan::Rect client_area = get_client_area();

	clan::Canvas canvas = get_canvas();
	if (clan::FileHelp::file_exists("../../../Resources/GUIThemeLuna/Images/icon_mycomputer.png"))
	{
		sprite_mycomputer = clan::Sprite(canvas, "../../../Resources/GUIThemeLuna/Images/icon_mycomputer.png");
		sprite_folder = clan::Sprite(canvas, "../../../Resources/GUIThemeLuna/Images/icon_folder_32x32.png");
		sprite_overlay = clan::Sprite(canvas, "../../../Resources/GUIThemeLuna/Images/overlay_96x96.png");
	}
	else
	{
		sprite_mycomputer = clan::Sprite(canvas, "../CommonCode/Resources/tux.png");
		sprite_folder = clan::Sprite(canvas, "../CommonCode/Resources/tux.png");
		sprite_overlay = clan::Sprite(canvas, "../CommonCode/Resources/tux.png");
	}

	toolbar = new clan::ToolBar(this);
	toolbar->set_geometry(clan::Rect(client_area.left, client_area.top, clan::Size(32, 180)));
	toolbar->add_item(clan::Sprite(canvas, "../CommonCode/Resources/tux.png"), 0, "Tux1", 0);
	toolbar->add_item(clan::Sprite(canvas, "../CommonCode/Resources/tux.png"), 0, "Tux2", 0);

	listview1 = new clan::ListView(this);
	listview1->set_geometry(clan::Rect(client_area.left + 100, client_area.top + 10, clan::Size(360, 180)));
	listview1->set_display_mode(clan::listview_mode_details);

	listview1->func_selection_changed().set(this, &ListView::on_selection_changed, listview1);
	listview1->func_item_edited().set(this, &ListView::on_item_edited, listview1);
	listview1->func_key_pressed().set(this, &ListView::on_key_pressed, listview1);
	listview1->func_key_released().set(this, &ListView::on_key_released, listview1);
	listview1->func_mouse_right_up().set(this, &ListView::on_mouse_right_up, listview1);
	clan::ListViewItem doc_item = listview1->get_document_item(); 

	clan::ListViewHeader *lv_header = listview1->get_header();  
	lv_header->append(lv_header->create_column("col1_id", "col1")).set_width(130);
	lv_header->append(lv_header->create_column("col2_id", "col2")).set_width(80);
	lv_header->append(lv_header->create_column("col3_id", "col3")).set_width(80);

	clan::ListViewIcon icon;
	icon.set_sprite(sprite_mycomputer, clan::listview_mode_details);
	icon.set_sprite(sprite_folder, clan::listview_mode_icons);
	icon.set_sprite(sprite_folder, clan::listview_mode_thumbnails);

	clan::ListViewIcon olay;
	olay.set_sprite(sprite_overlay, clan::listview_mode_thumbnails);
	olay.set_offset(clan::Point(0, 96-sprite_overlay.get_height()), clan::listview_mode_thumbnails);
	olay.set_scalable(false);

	clan::ListViewIconList ico_list = listview1->get_icon_list();
	ico_list.set_icon(1, icon);
	ico_list.set_icon(1001, olay);

	clan::ListViewItem i1 = listview1->create_item();
	{
		i1.set_column_text("col1_id", "i1");
		i1.set_column_text("col2_id", "col2 text!");
		i1.set_editable(true);
		i1.set_icon(1);
		i1.add_overlay_icon(1001);
		i1.set_open(false);
		doc_item.append_child(i1);

		clan::ListViewItem i1c1 = listview1->create_item();  
		{ 
			i1c1.set_column_text("col1_id", "i1c1");
			i1c1.set_column_text("col2_id", "BOOYAA!");
			i1c1.set_icon(1);
			i1.append_child(i1c1);
		}
	}

	clan::ListViewItem i2 = listview1->create_item();
	{
		i2.set_column_text("col1_id", "i2");
		i2.set_column_text("col2_id", "bar2");
		i2.set_column_text("col3_id", "foobar!");
		i2.set_icon(1);
		i2.set_open(true);

		clan::ListViewItem i2c1 = listview1->create_item();  
		{
			i2c1.set_column_text("col1_id", "i2c1"); 
			i2c1.set_column_text("col2_id", "BOOYAA!");
			i2c1.set_icon(1);
		}
		i2.append_child(i2c1); 

		clan::ListViewItem i2c2 = listview1->create_item();
		{
			i2c2.set_column_text("col1_id", "i2c2");
			i2c2.set_column_text("col2_id", "BOOYAA!");
			i2c2.set_icon(1);
			i2c2.set_open(true); 

			clan::ListViewItem i2c2c1 = listview1->create_item();
			{
				i2c2c1.set_column_text("col1_id", "i2c2c1"); 
				i2c2c1.set_column_text("col2_id", "BOOYAA!");
				i2c2c1.set_icon(1);
			}
			i2c2.append_child(i2c2c1); 
		}
		i2.append_child(i2c2);
	}
	doc_item.append_child(i2);


	for (int j = 0;j < 15; j++)
	{  
		clan::ListViewItem i = listview1->create_item();
		i.set_editable(true);
		i.set_column_text("col1_id", clan::string_format("item %1", j)); 
		i.set_icon(1);       
		if ((j%4) == 0)
		{ 
			i.set_column_text("col3_id", clan::string_format("item %1, col3", j));
		}
		if (( j%2)==0)
		{
			i.set_column_text("col2_id", clan::string_format("item %1, col2", j));
		} 
		doc_item.append_child(i);
	}

	int xoffset = client_area.left + 100;
	int yoffset = client_area.top + 200;
	const int gap = 16;

	info_selection_changed = new Info(this);
	info_selection_changed->set(xoffset, yoffset, "Selection Changed");
	yoffset += gap;

	info_item_edited = new Info(this);
	info_item_edited->set(xoffset, yoffset, "Item Edited");
	yoffset += gap;

	info_key_pressed = new Info(this);
	info_key_pressed->set(xoffset, yoffset, "Key Pressed");
	yoffset += gap;

	info_key_released = new Info(this);
	info_key_released->set(xoffset, yoffset, "Key Released");
	yoffset += gap;

	info_mouse_right_up = new Info(this);
	info_mouse_right_up->set(xoffset, yoffset, "Mouse Right Up");
}

void ListView::on_selection_changed(clan::ListViewSelection selection, clan::ListView *listview)
{
	info_selection_changed->activate();
}

bool ListView::on_item_edited(clan::ListViewItem item, std::string &string, clan::ListView *listview)
{
	std::string new_string = clan::string_format(" (%1)", string);

	info_item_edited->set_comment( new_string );
	info_item_edited->activate();
	return false;
}

void ListView::on_key_pressed(clan::InputEvent &input_event, clan::ListView *listview)
{
	info_key_pressed->activate();
}

void ListView::on_key_released(clan::InputEvent &input_event, clan::ListView *listview)
{
	info_key_released->activate();
}

void ListView::on_mouse_right_up(clan::Point point, clan::ListView *listview)
{
	std::string string = clan::string_format(" (%1,%2)", point.x, point.y);
	info_mouse_right_up->set_comment( string );
	info_mouse_right_up->activate();
}

bool ListView::on_close()
{
	exit_with_code(0);
	return true;
}
