/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    Magnus Norddahl
**    (if your name is missing here, please add it)
*/

#include "precomp.h"
#include "filedialog_generic.h"
//#include "API/GUI/filedialog.h"
#include "API/GUI/treenode.h"
#include "API/GUI/treeitem.h"

#include "API/Core/XML/dom_element.h"
#include "API/Core/IOData/directory_scanner.h"
#include "API/Display/display.h"
#include "API/Core/System/error.h"
#include "API/Core/IOData/directory.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_FileDialog_Generic::CL_FileDialog_Generic(
	CL_FileDialog *self,
	const std::string &title,
	const std::string &file,
	const std::string &filter)
	: filedialog(self)
{
	behavior = CL_FileDialog::quit_always;
	button = CL_FileDialog::button_none;
	// TODO: Calculate proper size
	int width = 400;
	int height = 315;
	int x = (CL_Display::get_width() - width) / 2;
	int y = (CL_Display::get_height() - height) / 2;
	filedialog->set_position(CL_Rect(x, y, x + width, y + height));
	filedialog->set_title(title);

	CL_Component *client_area = filedialog->get_client_area();

	width = client_area->get_width();

	label_dir = new CL_Label(CL_Point(10, 12), "Directory:", client_area);
	input_dir = new CL_InputBox(CL_Rect(65, 10, width - 120, 30), client_area);
	input_dir->enable(false);

	button_parent = new CL_Button(CL_Rect(width - 115, 9, width - 65, 28), "Parent", client_area);
	button_createdir = new CL_Button(CL_Rect(width - 60, 9, width - 10, 28), "New", client_area);

	treeview_files = new CL_TreeView(CL_Rect(10, 32, width - 10, 197), client_area);
	treeview_files->add_column("Filename", 200);
	treeview_files->add_column("Size", 80);
	treeview_files->add_column("Type", 100);
	treeview_files->show_root_decoration(false);

	label_file = new CL_Label(CL_Point(10, 207), "Filename:", client_area);
	input_file = new CL_InputBox(CL_Rect(65, 205, width - 10, 225), client_area);

	label_filter = new CL_Label(CL_Point(10, 232), "Filter:", client_area);
	input_filter = new CL_InputBox(CL_Rect(65, 230, width - 10, 250), client_area);

	button_ok = new CL_Button(CL_Rect(10, 260, 100, 280), "OK", client_area);
	button_cancel = new CL_Button(CL_Rect(width - 110, 260, width - 10, 280), "Cancel", client_area);

	slots.connect(self->sig_set_options(), this, &CL_FileDialog_Generic::on_set_options);
	slots.connect(treeview_files->sig_selection_changed(), this, &CL_FileDialog_Generic::on_file_activated);
	slots.connect(button_ok->sig_clicked(), this, &CL_FileDialog_Generic::on_button_quit, true);
	slots.connect(button_cancel->sig_clicked(), this, &CL_FileDialog_Generic::on_button_quit, false);
	slots.connect(button_parent->sig_clicked(), this, &CL_FileDialog_Generic::on_button_parent);
	slots.connect(button_createdir->sig_clicked(), this, &CL_FileDialog_Generic::on_button_createdir);
	slots.connect(input_file->sig_return_pressed(), this, &CL_FileDialog_Generic::on_edit_file);
	slots.connect(input_filter->sig_return_pressed(), this, &CL_FileDialog_Generic::on_edit_filter);
	slots.connect(input_dir->sig_return_pressed(), this, &CL_FileDialog_Generic::on_edit_dir);
	
	set_file(file, false);
	set_filter(filter, false);
	show_hidden = false;

	read_dir();
}

CL_FileDialog_Generic::~CL_FileDialog_Generic()
{
	delete label_dir;
	delete input_dir;
	delete button_parent;
	delete button_createdir;
	delete treeview_files;
	delete label_file;
	delete input_file;
	delete label_filter;
	delete input_filter;
	delete button_ok;
	delete button_cancel;
}

FileInfo::FileInfo(std::string name, bool directory, bool readable, bool writable, bool hidden, int size) 
: name(name), directory(directory), readable(readable), writable(writable), hidden(hidden), size(size)
{
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:
const CL_FileDialog::ButtonPressed &CL_FileDialog_Generic::get_pressed_button(void) const
{
	return button;
}

const std::string &CL_FileDialog_Generic::get_file() const
{
	return file;
}

const std::string &CL_FileDialog_Generic::get_filter() const
{
	return filter;
}

const std::string &CL_FileDialog_Generic::get_dir() const
{
	return dir;
}

const std::string &CL_FileDialog_Generic::get_path() const
{
	return path;
}

bool CL_FileDialog_Generic::is_hidden_files_visible() const
{
	return show_hidden;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:
void CL_FileDialog_Generic::set_behavior(CL_FileDialog::Behavior p_behavior)
{
	behavior = p_behavior;
}

void CL_FileDialog_Generic::set_file(const std::string &new_file, bool perform_refresh)
{
	input_file->set_text(new_file);
	file = new_file;

	if(perform_refresh)
		refresh();
}

void CL_FileDialog_Generic::set_dir(const std::string &new_dir, bool perform_refresh)
{
	input_dir->set_text(new_dir);
	dir = new_dir;
	CL_Directory::change_to(dir);
	path = CL_Directory::get_current();

	if(perform_refresh)
		refresh();
}

void CL_FileDialog_Generic::set_filter(const std::string &new_filter, bool perform_refresh)
{
	input_filter->set_text(new_filter);
	filter = new_filter;

	if(perform_refresh)
		refresh();
}

void CL_FileDialog_Generic::show_hidden_files(bool enable)
{
	show_hidden = enable;
}

void CL_FileDialog_Generic::refresh()
{
	CL_DirectoryScanner scanner;
	unsigned int i;
	bool parent = false;

	treeview_files->clear();
	Directories.clear();
	Files.clear();

	scanner.scan (".", "*");
	while (scanner.next())
	{
		std::string file = scanner.get_name();

		if(file == "." || file == "..")
			parent = true;
		else 
		{
			if (!scanner.is_hidden() || show_hidden == true)
			{	
				if (scanner.is_directory())
				{
					Directories.push_back(FileInfo(file, scanner.is_directory(), scanner.is_readable(), scanner.is_writable(), scanner.is_hidden(), 0));
				}
			}
		}
	}
	
	scanner.scan (".", filter);
	while (scanner.next())
	{
		std::string file = scanner.get_name();

		if(file == "." || file == "..")
			parent = true;
		else 
		{
			if (!scanner.is_hidden() || show_hidden == true)
			{	
				if (!scanner.is_directory())
				{
					Files.push_back(FileInfo(file, scanner.is_directory(), scanner.is_readable(), scanner.is_writable(), scanner.is_hidden(), scanner.get_size()));
				}
			}
		}
	}

	std::sort(Directories.begin(), Directories.end(), CL_FileDialog_Generic::sort);
	std::sort(Files.begin(), Files.end(), CL_FileDialog_Generic::sort);

	for(i = 0; i < Directories.size(); i++)
		treeview_files->insert_item(Directories[i].name, "", "Directory");

	for(i = 0; i < Files.size(); i++)
	{
		std::string AppendName = Files[i].name;
		int index = AppendName.rfind('.');
		std::string file_size;
		
		file_size = CL_String::format("%1 bytes", Files[i].size);

		if(Files[i].size > 1024)
			file_size = CL_String::format("%1 KB", Files[i].size / 1024);

		if(Files[i].size > 1024 * 1024)
			file_size = CL_String::format("%1 MB", Files[i].size / (1024 * 1024));


		if(index >= 0)
		{
			AppendName = AppendName.substr(index);
			AppendName = CL_String::format("%1 file", AppendName);
		}

		treeview_files->insert_item(Files[i].name, file_size, AppendName);
	}

	button_parent->enable(parent);
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_FileDialog_Generic::on_file_activated(const CL_TreeNode &node)
{
	CL_TreeItem *treeitem = (CL_TreeItem *)node.get_component();

	std::string item = treeitem->get_text(0);

	if(treeitem->get_text(2) == "Directory")
	{
		CL_Directory::change_to(item);
		read_dir();
	}
	else
	{
		set_file(item, false);
	}
}

void CL_FileDialog_Generic::on_button_quit(bool ok)
{
	if(ok == false)
	{
		button = CL_FileDialog::button_cancel;
		file = "";
	}
	else if(ok)
	{
		button = CL_FileDialog::button_ok;
		
		if (behavior != CL_FileDialog::quit_file_selected)
		{
			//accept manually edited/typed entry as the file	
			file = input_file->get_text();
		}
	}

	// Quit only if behaviour say that
	if( (behavior == CL_FileDialog::quit_always) || (button == CL_FileDialog::button_cancel) || ( (behavior == CL_FileDialog::quit_file_selected) && !file.empty()) )
		filedialog->quit();
}

void CL_FileDialog_Generic::on_button_parent()
{
	CL_Directory::change_to("..");
	read_dir();
}

void CL_FileDialog_Generic::on_button_createdir()
{
	CL_InputDialog dlg("Create new directory", "Ok", "Cancel", "", filedialog);
	CL_InputBox *dir_name = dlg.add_input_box("New directory:", "", 200);
	dlg.set_position(10, 10);
	dlg.run();

	if(dlg.get_result_button() == 0)
	{
		if(dir_name->get_text() != "")
		{
			CL_Directory::create(dir_name->get_text());
			refresh();
		}
	}
}

void CL_FileDialog_Generic::on_edit_dir()
{
	set_dir(input_dir->get_text());
}

void CL_FileDialog_Generic::on_edit_file()
{
	set_file(input_file->get_text());
}

void CL_FileDialog_Generic::on_edit_filter()
{
	set_filter(input_filter->get_text());
}

void CL_FileDialog_Generic::on_set_options(const CL_DomElement &options)
{
	if (options.has_attribute("file"))
		set_file(options.get_attribute("file"), false);

	if (options.has_attribute("filter"))
		set_filter(options.get_attribute("filter"), false);

	if (options.has_attribute("show_hidden"))
		show_hidden = CL_String::to_bool(options.get_attribute("show_hidden"));

	read_dir();
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:

void CL_FileDialog_Generic::read_dir()
{
	std::string cwd = CL_Directory::get_current();
	path = cwd;

	int find_last1 = cwd.rfind('/');
	int find_last2 = cwd.rfind('\\');

	int index = 0;
	if(find_last1 != -1)
		index = find_last1 + 1;
	else if(find_last2 != -1)
		index = find_last2 + 1;

	cwd = cwd.substr(cwd.length() - (cwd.length() - index));

	set_dir(cwd);
}

bool CL_FileDialog_Generic::sort(const FileInfo &a, const FileInfo &b)
{
   return a.name < b.name;
}
