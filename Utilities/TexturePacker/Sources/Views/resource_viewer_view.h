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
**    Kenneth Gangstoe
*/

#pragma once

#include "../view.h"
#include "../texture_packer.h"

class SpriteComponent;
class ResourceItem;

class ResourceViewerView : public View
{
public:
	ResourceViewerView(clan::GUIComponent *parent, MainWindow *mainwindow);
	~ResourceViewerView();

private:
	void on_resized();
	void on_button_browse_resource_file();
	void on_selection_changed(clan::ListViewSelection selection);
	void on_button_edit_resource();
	void on_button_create_collision_data();

	ResourceItem *get_selected_resource_item();

	void load_resource_file(const std::string &file);

	void show_resource(ResourceItem *resource_item);

	TexturePacker packer;

	clan::Label *label_selected_resource_file;
	clan::LineEdit *lineedit_selected_resource_file;
	clan::PushButton *button_browse_resource_file;

	clan::PushButton *button_edit_resource;
	clan::PushButton *button_create_collision_data;

	clan::ListView *resource_list;

	SpriteComponent *sprite_component;
};
