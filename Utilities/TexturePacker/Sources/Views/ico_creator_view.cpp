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
#include "ico_creator_view.h"

IcoCreatorView::IcoCreatorView(GUIComponent *parent, MainWindow *mainwindow)
: View(parent, mainwindow, "ICO Creator")
{
	func_resized().set(this, &IcoCreatorView::on_resized);

	for(int i = 0; i < 11; ++i)
	{
		label_selected_file[i] = new Label(this);
		label_selected_file[i]->set_text("Input image file:");

		lineedit_selected_file[i] = new LineEdit(this);

		button_browse_file[i] = new PushButton(this);
		button_browse_file[i]->set_text("...");
		button_browse_file[i]->func_clicked().set(this, &IcoCreatorView::on_button_browse, i);
	}

	button_save = new PushButton(this);
	button_save->set_text("Create ICO file");
	button_save->func_clicked().set(this, &IcoCreatorView::on_button_save);

	label_result = new Label(this);

	on_resized();
}

IcoCreatorView::~IcoCreatorView()
{
}

void IcoCreatorView::on_resized()
{
	Size size = get_size();

	int y = 0;
	for(int i = 0; i < 11; ++i)
	{
		label_selected_file[i]->set_geometry(Rect(10, y + 13, 90, y + 30));
		lineedit_selected_file[i]->set_geometry(Rect(96, y + 10, size.width - 20 - 20, y + 30));
		button_browse_file[i]->set_geometry(Rect(10 + size.width - 20 - 30, y + 10, 10 + size.width - 20, y + 30));

		y += 25;
	}

	label_result->set_geometry(Rect(10, y + 70, size.width - 130, y + 95));
	button_save->set_geometry(Rect(size.width - 130, y + 70, Size(120, 25)));
}

void IcoCreatorView::on_button_browse(int index)
{
	OpenFileDialog dlg(this);
	dlg.add_filter("PNG files", "*.png", true);
	dlg.add_filter("All files", "*.*");

	dlg.set_initial_directory(System::get_exe_path());

	if(dlg.show())
	{
		std::string filename = dlg.get_filename();

		try
		{
			lineedit_selected_file[index]->set_text(filename);
		}
		catch(Exception error)
		{
			label_result->set_text(string_format("Error: %1", error.message));
		}
	}
}

void IcoCreatorView::on_button_save()
{
	SaveFileDialog dlg(this);

	dlg.add_filter("ICO files", "*.ico", true);
	dlg.add_filter("All files", "*.*");

	dlg.set_initial_directory(System::get_exe_path());

	if(dlg.show())
	{
		std::string filename = dlg.get_filename();

		try
		{
			IconSet icon_set;

			int count = 0;
			for(int i = 0; i < 11; ++i)
			{
				if(lineedit_selected_file[i]->get_text().empty() == false)
				{
					PixelBuffer pb(lineedit_selected_file[i]->get_text());
					icon_set.add_image(pb);
					count++;
				}
			}

			DataBuffer databuffer = icon_set.create_win32_icon();

			File ico_file(filename, File::create_always, File::access_write);
			ico_file.write(databuffer.get_data(), databuffer.get_size());
			
			ico_file.close();

			label_result->set_text(string_format("Saved %1 images into ico file: %2", count, filename));
		}
		catch(Exception error)
		{
			label_result->set_text(string_format("Save error: %1", error.message));
		}
	}
}
