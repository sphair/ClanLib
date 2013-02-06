/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
*/
#include "GameIDE/precomp.h"
#include "dlg_add_new_project.h"
#include "../SolutionModel/solution_model.h"
namespace clan
{


DlgAddNewProject::DlgAddNewProject(GUIComponent *owner, SolutionModel &model)
: GUIComponent(owner, get_toplevel_description()), model(model)
{
	label_name = new Label(this);
	lineedit_name = new LineEdit(this);
	label_location = new Label(this);
	lineedit_location = new LineEdit(this);
	button_browse_location = new PushButton(this);
	label_solution_name = new Label(this);
	lineedit_solution_name = new LineEdit(this);
	check_create_solution_directory = new CheckBox(this);
	button_ok = new PushButton(this);
	button_cancel = new PushButton(this);

	label_name->set_text("Name:");
	label_location->set_text("Location:");
	button_browse_location->set_text("Browse...");
	label_solution_name->set_text("Solution name:");
	check_create_solution_directory->set_text("Create directory for solution");
	button_ok->set_text("OK");
	button_ok->set_default(true);
	button_cancel->set_text("Cancel");
	button_cancel->set_cancel(true);

	func_resized().set(this, &DlgAddNewProject::on_resized);
	func_close().set(this, &DlgAddNewProject::on_close);
	button_ok->func_clicked().set(this, &DlgAddNewProject::on_button_ok_clicked);
	button_cancel->func_clicked().set(this, &DlgAddNewProject::on_button_cancel_clicked);
	button_browse_location->func_clicked().set(this, &DlgAddNewProject::on_button_browse_location_clicked);
	check_create_solution_directory->func_state_changed().set(this, &DlgAddNewProject::on_check_create_solution_directory_state_changed);

	on_check_create_solution_directory_state_changed();
	on_resized();
	lineedit_name->set_focus();
}

void DlgAddNewProject::on_resized()
{
	Rect client_box = get_size();
	client_box.shrink(11);

	Rect button_area_box(client_box.left, client_box.bottom - 22, client_box.right, client_box.bottom);
	Rect cancel_box(button_area_box.right - 75, button_area_box.top, button_area_box.right, button_area_box.bottom);
	Rect ok_box(cancel_box.left - 5 - 75, button_area_box.top, cancel_box.left - 5, button_area_box.bottom);

	button_ok->set_geometry(ok_box);
	button_cancel->set_geometry(cancel_box);

	int label_width = 75;
	int browse_width = 75;
	int edit_width = client_box.get_width() - label_width - browse_width - 5;
	int y = client_box.top;

	Label *labels[] = { label_name, label_location, label_solution_name, 0 };
	GUIComponent *edits[] = { lineedit_name, lineedit_location, lineedit_solution_name, check_create_solution_directory };
	for (int i = 0; i < 4; i++)
	{
		Rect label_box(client_box.left, y + 2, client_box.left + label_width, y + 22);
		Rect edit_box(client_box.left + label_width, y, client_box.left + label_width + edit_width, y + 22);
		if (labels[i])
			labels[i]->set_geometry(label_box);
		edits[i]->set_geometry(edit_box);
		if (i == 1)
		{
			Rect browse_box(client_box.left + label_width + edit_width + 5, y, client_box.left + label_width + edit_width + 5 + browse_width, y + 22);
			button_browse_location->set_geometry(browse_box);
		}
		y += 22 + 5;
	}
}

bool DlgAddNewProject::on_close()
{
	on_button_cancel_clicked();
	return true;
}

void DlgAddNewProject::on_button_ok_clicked()
{
	std::string name = lineedit_name->get_text();
	std::string folder = lineedit_location->get_text();

	model.create_solution(folder, name);

	exit_with_code(1);
}

void DlgAddNewProject::on_button_cancel_clicked()
{
	exit_with_code(0);
}

void DlgAddNewProject::on_button_browse_location_clicked()
{
	BrowseFolderDialog dlg(this);
	dlg.set_title("Select Location Folder");
	if (dlg.show())
	{
		lineedit_location->set_text(dlg.get_selected_path());
	}
}

void DlgAddNewProject::on_check_create_solution_directory_state_changed()
{
	bool enabled = check_create_solution_directory->is_checked();
	lineedit_solution_name->set_enabled(enabled);
}

GUITopLevelDescription DlgAddNewProject::get_toplevel_description()
{
	GUITopLevelDescription desc;
	desc.show_minimize_button(false);
	desc.show_maximize_button(false);
	desc.show_sysmenu(false);
	desc.set_allow_resize(true);
	desc.set_title("New Project");
	desc.set_size(Size(480, 150), true);
	return desc;
}

}

