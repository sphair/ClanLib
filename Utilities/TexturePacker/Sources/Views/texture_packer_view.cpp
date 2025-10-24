/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

TexturePackerView::TexturePackerView(CL_GUIComponent *parent, MainWindow *mainwindow)
: View(parent, mainwindow, "Texture Packer")
{
	func_resized().set(this, &TexturePackerView::on_resized);

	label_selected_resource_file = new CL_Label(this);
	label_selected_resource_file->set_text("Resource file:");

	lineedit_selected_resource_file = new CL_LineEdit(this);

	button_browse_resource_file = new CL_PushButton(this);
	button_browse_resource_file->set_text("...");
	button_browse_resource_file->func_clicked().set(this, &TexturePackerView::on_button_browse_resource_file);

	radio_texture32 = new CL_RadioButton(this);
	radio_texture32->set_text("32x32");
	radio_texture32->set_group_name("TextureSize");

	radio_texture64 = new CL_RadioButton(this);
	radio_texture64->set_text("64x64");
	radio_texture64->set_group_name("TextureSize");

	radio_texture128 = new CL_RadioButton(this);
	radio_texture128->set_text("128x128");
	radio_texture128->set_group_name("TextureSize");

	radio_texture256 = new CL_RadioButton(this);
	radio_texture256->set_text("256x256");
	radio_texture256->set_group_name("TextureSize");

	radio_texture512 = new CL_RadioButton(this);
	radio_texture512->set_text("512x512");
	radio_texture512->set_group_name("TextureSize");

	radio_texture1024 = new CL_RadioButton(this);
	radio_texture1024->set_text("1024x1024");
	radio_texture1024->set_group_name("TextureSize");

	radio_texture2048 = new CL_RadioButton(this);
	radio_texture2048->set_text("2048x2048");
	radio_texture2048->set_group_name("TextureSize");
	radio_texture2048->set_selected(true);

	radio_texture512x256 = new CL_RadioButton(this);
	radio_texture512x256->set_text("512x256");
	radio_texture512x256->set_group_name("TextureSize");
	radio_texture512x256->set_selected(true);

	spin_border = new CL_Spin(this);
	spin_border->set_value(1);
	spin_border->set_step_size(1);
	spin_border->set_ranges(0, 8);
	spin_border->set_floating_point_mode(false);

	label_border = new CL_Label(this);
	label_border->set_text("Pixel Border");

	button_pack = new CL_PushButton(this);
	button_pack->set_text("Generate textures");
	button_pack->func_clicked().set(this, &TexturePackerView::on_button_generate_textures);

	button_save = new CL_PushButton(this);
	button_save->set_text("Save resourcefile");
	button_save->func_clicked().set(this, &TexturePackerView::on_button_save_resources);

	generation_result = new CL_Label(this);

	texturegroup_component = new TextureGroupComponent(this);

	on_resized();
}

TexturePackerView::~TexturePackerView()
{
}

void TexturePackerView::on_resized()
{
	CL_Size size = get_size();

	label_selected_resource_file->set_geometry(CL_Rect(10, 13, 80, 30));
	lineedit_selected_resource_file->set_geometry(CL_Rect(80, 10, size.width - 20 - 20, 30));
	button_browse_resource_file->set_geometry(CL_Rect(10 + size.width - 20 - 30, 10, 10 + size.width - 20, 30));

	radio_texture32->set_geometry(CL_Rect(10, 40, CL_Size(80, 21)));
	radio_texture64->set_geometry(CL_Rect(80+10, 40, CL_Size(80, 21)));
	radio_texture128->set_geometry(CL_Rect(80+80+10, 40, CL_Size(80, 21)));
	radio_texture256->set_geometry(CL_Rect(80+80+80+10, 40, CL_Size(80, 21)));
	radio_texture512->set_geometry(CL_Rect(80+80+80+80+10, 40, CL_Size(80, 21)));
	radio_texture1024->set_geometry(CL_Rect(80+80+80+80+80+10, 40, CL_Size(80, 21)));
	radio_texture2048->set_geometry(CL_Rect(80+80+80+80+80+80+10, 40, CL_Size(80, 21)));
	radio_texture512x256->set_geometry(CL_Rect(80+80+80+80+80+80+80+10, 40, CL_Size(80, 21)));
	spin_border->set_geometry(CL_Rect(650, 40, CL_Size(50, 21)));
	label_border->set_geometry(CL_Rect(710, 40, CL_Size(100, 21)));

	button_pack->set_geometry(CL_Rect(10, 70, CL_Size(120, 25)));
	button_save->set_geometry(CL_Rect(size.width - 130, 70, CL_Size(120, 25)));
	generation_result->set_geometry(CL_Rect(170, 74, size.width - 130, 90));
	texturegroup_component->set_geometry(CL_Rect(10, 100, size.width - 10, size.height - 10));
}

void TexturePackerView::on_button_browse_resource_file()
{
	CL_OpenFileDialog dlg(this);
	dlg.add_filter("Resource files", "*.xml", true);
	dlg.add_filter("All files", "*.*");

	dlg.set_initial_directory(CL_System::get_exe_path());

	if(dlg.show())
	{
		CL_String filename = dlg.get_filename();

		try
		{
			load_resource_file(filename);
			lineedit_selected_resource_file->set_text(filename);
		}
		catch(CL_Exception error)
		{
			generation_result->set_text(cl_format("Error: %1", error.message));
		}
	}
}

void TexturePackerView::load_resource_file(const CL_String &file)
{
	packer.load_resources(get_gc(), file);
}

void TexturePackerView::on_button_save_resources()
{
	try
	{
		CL_SaveFileDialog dlg(this);

		dlg.add_filter("Resource files", "*.xml", true);
		dlg.add_filter("All files", "*.*");

		dlg.set_initial_directory(CL_System::get_exe_path());

		if(dlg.show())
		{
			CL_String filename = dlg.get_filename();
			packer.save_resources(filename);
			generation_result->set_text(cl_format("Saved resources: %1", filename));
		}
	}
	catch(CL_Exception error)
	{
		generation_result->set_text(cl_format("Save error: %1", error.message));
	}
}

void TexturePackerView::on_button_generate_textures()
{
	CL_Size texture_size;
	if(radio_texture32->is_selected())
		texture_size = CL_Size(32, 23);
	if(radio_texture64->is_selected())
		texture_size = CL_Size(64, 64);
	if(radio_texture128->is_selected())
		texture_size = CL_Size(128, 128);
	if(radio_texture256->is_selected())
		texture_size = CL_Size(256, 256);
	if(radio_texture512->is_selected())
		texture_size = CL_Size(512, 512);
	if(radio_texture512x256->is_selected())
		texture_size = CL_Size(512, 256);
	if(radio_texture1024->is_selected())
		texture_size = CL_Size(1024, 1024);
	if(radio_texture2048->is_selected())
		texture_size = CL_Size(2048, 2048);

	try
	{
		CL_TextureGroup *group = packer.pack(get_gc(), texture_size, spin_border->get_value());
		texturegroup_component->set_texturegroup(group);
		texturegroup_component->request_repaint();

		generation_result->set_text(cl_format("Textures generated: %1", group->get_texture_count()));
	}
	catch(CL_Exception error)
	{
		generation_result->set_text(cl_format("Error: %1", error.message));
	}
}
