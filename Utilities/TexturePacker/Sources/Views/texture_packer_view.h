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

class TextureGroupComponent;

class TexturePackerView : public View
{
public:
	TexturePackerView(clan::GUIComponent *parent, MainWindow *mainwindow);
	~TexturePackerView();

private:
	void on_resized();

	void on_button_generate_textures();
	void on_button_save_resources();
	void on_button_browse_resource_file();

	void load_resource_file(const std::string &file);

	TexturePacker packer;

	clan::Label *label_selected_resource_file;
	clan::LineEdit *lineedit_selected_resource_file;
	clan::PushButton *button_browse_resource_file;

	clan::PushButton *button_pack;
	clan::PushButton *button_save;
	clan::Spin *spin_border;
	clan::Label *label_border;

	clan::CheckBox *check_sort;

	clan::RadioButton *radio_texture32;
	clan::RadioButton *radio_texture64;
	clan::RadioButton *radio_texture128;
	clan::RadioButton *radio_texture256;
	clan::RadioButton *radio_texture512;
	clan::RadioButton *radio_texture1024x256;
	clan::RadioButton *radio_texture1024;
	clan::RadioButton *radio_texture2048;

	clan::Label *generation_result;

	TextureGroupComponent *texturegroup_component;
};
