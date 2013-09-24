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

#include "precomp.h"
#include "texture_packer_view.h"
#include "../texturegroup_component.h"
#include "../texture_packer.h"

TexturePackerView::TexturePackerView(clan::GUIComponent *parent, MainWindow *mainwindow)
: View(parent, mainwindow, "Texture Packer")
{
	func_resized().set(this, &TexturePackerView::on_resized);

	label_selected_resource_file = new clan::Label(this);
	label_selected_resource_file->set_text("Resource file:");

	lineedit_selected_resource_file = new clan::LineEdit(this);

	button_browse_resource_file = new clan::PushButton(this);
	button_browse_resource_file->set_text("...");
	button_browse_resource_file->func_clicked().set(this, &TexturePackerView::on_button_browse_resource_file);

	radio_texture32 = new clan::RadioButton(this);
	radio_texture32->set_text("32x32");
	radio_texture32->set_group_name("TextureSize");

	radio_texture64 = new clan::RadioButton(this);
	radio_texture64->set_text("64x64");
	radio_texture64->set_group_name("TextureSize");

	radio_texture128 = new clan::RadioButton(this);
	radio_texture128->set_text("128x128");
	radio_texture128->set_group_name("TextureSize");

	radio_texture256 = new clan::RadioButton(this);
	radio_texture256->set_text("256x256");
	radio_texture256->set_group_name("TextureSize");

	radio_texture512 = new clan::RadioButton(this);
	radio_texture512->set_text("512x512");
	radio_texture512->set_group_name("TextureSize");

	radio_texture1024 = new clan::RadioButton(this);
	radio_texture1024->set_text("1024x1024");
	radio_texture1024->set_group_name("TextureSize");

	radio_texture2048 = new clan::RadioButton(this);
	radio_texture2048->set_text("2048x2048");
	radio_texture2048->set_group_name("TextureSize");
	radio_texture2048->set_selected(true);

	radio_texture1024x256 = new clan::RadioButton(this);
	radio_texture1024x256->set_text("1024x256");
	radio_texture1024x256->set_group_name("TextureSize");
	radio_texture1024x256->set_selected(true);

	spin_border = new clan::Spin(this);
	spin_border->set_value(1);
	spin_border->set_step_size(1);
	spin_border->set_ranges(0, 8);
	spin_border->set_floating_point_mode(false);

	label_border = new clan::Label(this);
	label_border->set_text("Pixel Border");

	check_sort = new clan::CheckBox(this);
	check_sort->set_text("Sort images by width");

	button_pack = new clan::PushButton(this);
	button_pack->set_text("Generate textures");
	button_pack->func_clicked().set(this, &TexturePackerView::on_button_generate_textures);

	button_save = new clan::PushButton(this);
	button_save->set_text("Save resourcefile");
	button_save->func_clicked().set(this, &TexturePackerView::on_button_save_resources);

	generation_result = new clan::Label(this);

	texturegroup_component = new TextureGroupComponent(this);

	on_resized();
}

TexturePackerView::~TexturePackerView()
{
}

void TexturePackerView::on_resized()
{
	clan::Size size = get_size();

	label_selected_resource_file->set_geometry(clan::Rect(10, 13, 80, 30));
	lineedit_selected_resource_file->set_geometry(clan::Rect(80, 10, size.width - 20 - 20, 30));
	button_browse_resource_file->set_geometry(clan::Rect(10 + size.width - 20 - 30, 10, 10 + size.width - 20, 30));

	radio_texture32->set_geometry(clan::Rect(10, 40, clan::Size(80, 21)));
	radio_texture64->set_geometry(clan::Rect(80+10, 40, clan::Size(80, 21)));
	radio_texture128->set_geometry(clan::Rect(80+80+10, 40, clan::Size(80, 21)));
	radio_texture256->set_geometry(clan::Rect(80+80+80+10, 40, clan::Size(80, 21)));
	radio_texture512->set_geometry(clan::Rect(80+80+80+80+10, 40, clan::Size(80, 21)));
	radio_texture1024->set_geometry(clan::Rect(80+80+80+80+80+10, 40, clan::Size(80, 21)));
	radio_texture2048->set_geometry(clan::Rect(80+80+80+80+80+80+10, 40, clan::Size(80, 21)));
	radio_texture1024x256->set_geometry(clan::Rect(80+80+80+80+80+80+80+10, 40, clan::Size(80, 21)));
	label_border->set_geometry(clan::Rect(10, 70, clan::Size(60, 21)));
	spin_border->set_geometry(clan::Rect(70, 68, clan::Size(50, 21)));
	check_sort->set_geometry(clan::Rect(140, 70, clan::Size(200, 21)));
	button_pack->set_geometry(clan::Rect(10, 100, clan::Size(120, 25)));
	button_save->set_geometry(clan::Rect(size.width - 130, 100, clan::Size(120, 25)));
	generation_result->set_geometry(clan::Rect(170, 104, size.width - 130, 90));
	texturegroup_component->set_geometry(clan::Rect(10, 130, size.width - 10, size.height - 10));
}

void TexturePackerView::on_button_browse_resource_file()
{
	clan::OpenFileDialog dlg(this);
	dlg.add_filter("Resource files", "*.xml", true);
	dlg.add_filter("All files", "*.*");

	dlg.set_initial_directory(clan::System::get_exe_path());

	if(dlg.show())
	{
		std::string filename = dlg.get_filename();

		try
		{
			load_resource_file(filename);
			lineedit_selected_resource_file->set_text(filename);
		}
		catch(clan::Exception error)
		{
			generation_result->set_text(clan::string_format("Error: %1", error.message));
		}
	}
}

void TexturePackerView::load_resource_file(const std::string &file)
{
	packer.load_resources(get_canvas(), file);
}

void TexturePackerView::on_button_save_resources()
{
	try
	{
		clan::SaveFileDialog dlg(this);

		dlg.add_filter("Resource files", "*.xml", true);
		dlg.add_filter("All files", "*.*");

		dlg.set_initial_directory(clan::System::get_exe_path());

		if(dlg.show())
		{
			std::string filename = dlg.get_filename();
			clan::Canvas canvas = get_canvas();
			packer.save_resources(canvas, filename);
			generation_result->set_text(clan::string_format("Saved resources: %1", filename));
		}
	}
	catch(clan::Exception error)
	{
		generation_result->set_text(clan::string_format("Save error: %1", error.message));
	}
}

void TexturePackerView::on_button_generate_textures()
{
	clan::Size texture_size;
	if(radio_texture32->is_selected())
		texture_size = clan::Size(32, 23);
	if(radio_texture64->is_selected())
		texture_size = clan::Size(64, 64);
	if(radio_texture128->is_selected())
		texture_size = clan::Size(128, 128);
	if(radio_texture256->is_selected())
		texture_size = clan::Size(256, 256);
	if(radio_texture512->is_selected())
		texture_size = clan::Size(512, 512);
	if(radio_texture1024x256->is_selected())
		texture_size = clan::Size(1024, 256);
	if(radio_texture1024->is_selected())
		texture_size = clan::Size(1024, 1024);
	if(radio_texture2048->is_selected())
		texture_size = clan::Size(2048, 2048);

	try
	{
		clan::TextureGroup *group = packer.pack(get_canvas(), texture_size, spin_border->get_value(), check_sort->is_checked());
		texturegroup_component->set_texturegroup(group);
		texturegroup_component->request_repaint();

		generation_result->set_text(clan::string_format("Textures generated: %1", group->get_texture_count()));
	}
	catch(clan::Exception error)
	{
		generation_result->set_text(clan::string_format("Error: %1", error.message));
	}
}
