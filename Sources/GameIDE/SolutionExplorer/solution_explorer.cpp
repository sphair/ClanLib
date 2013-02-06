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
#include "solution_explorer.h"
#include "dlg_add_new_item.h"
#include "dlg_open_with.h"
#include "drag_drop_operation.h"
#include "SolutionModel/solution_model.h"
#include "FileItemType/file_item_type.h"
#include "Workspace/workspace.h"
#include "UIController/ui_controller.h"
#include "UIController/document_editor_type.h"

namespace clan
{

SolutionExplorer::SolutionExplorer(FileItemTypeFactory &factory, UIController *controller)
: listview(0), drag_drop(0), factory(factory), controller(controller)
{
	set_title("Solution Explorer");
	set_dockable_window_geometry(Rect(Point(50, 50), Size(300, 400)));
	slots.connect(solution_model().sig_model_updated, this, &SolutionExplorer::populate);

//	controller->component<UIRibbonPushButton>("view.solution-explorer").func_clicked(0).set(this, &SolutionExplorer::on_item_show_solution_explorer);

	listview = new ListView(this);
	listview->func_mouse_right_up().set(this, &SolutionExplorer::on_listview_mouse_right_up);
	listview->func_item_doubleclick().set(this, &SolutionExplorer::on_listview_item_doubleclick);
	listview->func_item_opened().set(this, &SolutionExplorer::on_listview_item_opened);
	listview->func_item_closed().set(this, &SolutionExplorer::on_listview_item_closed);
	listview->func_begin_drag().set(this, &SolutionExplorer::on_listview_begin_drag);
	listview->set_class("borderless");
	listview->get_header()->set_class("hidden");
	listview->get_header()->append(listview->get_header()->create_column("name", "Name")).set_width(110);

	drag_drop = new DragDropOperation(this);

	create_icon_list();
	populate();
}

SolutionModel &SolutionExplorer::solution_model()
{
	return controller->get_solution_model();
}

void SolutionExplorer::on_item_show_solution_explorer()
{
	show();
}

std::vector<std::string> SolutionExplorer::get_selected_filenames()
{
	std::vector<std::string> filenames;
	ListViewSelectedItem cur_selected_item = listview->get_selected_items().get_first();
	while (cur_selected_item.is_item())
	{
		ListViewItemDataPtr data = get_item_data(cur_selected_item.get_item());
		if (data && data->project_item)
		{
			filenames.push_back(data->project_item->get_filename());
		}
		cur_selected_item = cur_selected_item.get_next_sibling();
	}
	return filenames;
}

void SolutionExplorer::on_resized()
{
	listview->set_geometry(get_size());
}

void SolutionExplorer::populate()
{
	listview->clear();
	if (!solution_model().solution.name.empty())
	{
		ListViewItem solution_item = create_item(&solution_model().solution);
		listview->get_document_item().append_child(solution_item);
		for (size_t i = 0; i < solution_model().solution.projects.size(); i++)
		{
			ProjectPtr project = solution_model().get_project(solution_model().solution.projects[i]);
			if (project)
			{
				ListViewItem project_item = create_item(project);
				solution_item.append_child(project_item);

				populate_item(project_item, &project->items);
			}
			else
			{
				// To do: create unloaded project item
			}
		}
	}
}

void SolutionExplorer::populate_item(ListViewItem listview_item, ProjectItem *item)
{
	for (size_t i = 0; i < item->children().size(); i++)
	{
		ListViewItem child_item = create_item(item->children()[i]);
		populate_item(child_item, item->children()[i]);
		listview_item.append_child(child_item);
	}
}

void SolutionExplorer::on_listview_mouse_right_up(Point pos)
{
	ListViewItemDataPtr item_data = get_item_data(listview->get_selected_item());
	if (item_data)
	{
		if (item_data->solution)
		{
		}
		else if (item_data->project_item)
		{
			popup = PopupMenu();
			if (dynamic_cast<ProjectFileItem*>(item_data->project_item))
			{
				popup.insert_item("Open").func_clicked().set(this, &SolutionExplorer::on_open);
				popup.insert_item("Open With...").func_clicked().set(this, &SolutionExplorer::on_open_with);
			}
			else
			{
				popup.insert_item("Add New Item...").func_clicked().set(this, &SolutionExplorer::on_add_new_item);
				popup.insert_item("Add Existing Item...").func_clicked().set(this, &SolutionExplorer::on_add_existing_item);
				popup.insert_item("New Folder").func_clicked().set(this, &SolutionExplorer::on_new_folder);
				popup.insert_item("New Filter").set_enabled(false);
			}
			popup.insert_separator();
			popup.insert_item("Cut").set_enabled(false);
			popup.insert_item("Copy").set_enabled(false);
			popup.insert_item("Remove").func_clicked().set(this, &SolutionExplorer::on_remove);
			popup.insert_item("Rename").set_enabled(false);
			popup.insert_separator();
			popup.insert_item("Open Folder in Windows Explorer").set_enabled(false);
			popup.insert_separator();
			popup.insert_item("Properties").set_enabled(false);
			popup.start(this, component_to_screen_coords(pos));
		}
	}
}

void SolutionExplorer::on_listview_item_doubleclick(const ListViewItem &item)
{
	on_open();
}

void SolutionExplorer::on_listview_item_opened(const ListViewItem &item)
{
	ListViewItemDataPtr item_data = get_item_data(item);
	if (item_data && item_data->project_item)
		solution_model().set_open(item_data->project_item, true);
}

void SolutionExplorer::on_listview_item_closed(const ListViewItem &item)
{
	ListViewItemDataPtr item_data = get_item_data(item);
	if (item_data && item_data->project_item)
		solution_model().set_open(item_data->project_item, false);
}

void SolutionExplorer::on_listview_begin_drag()
{
	drag_drop->begin_drag();
}

void SolutionExplorer::on_add_new_item()
{
	ListViewItemDataPtr item_data = get_item_data(listview->get_selected_item());
	if (item_data)
	{
		if (item_data->project_item)
		{
			DlgAddNewItem dlg(this, factory, item_data->project_item->get_filename());
			if(dlg.exec() == 1)
			{
				std::string name = dlg.get_name();
				std::string location = dlg.get_location();
				FileItemType *type = dlg.get_fileitemtype();
				std::string filename = PathHelp::combine(location, PathHelp::get_basename(name) + "." + type->get_extension());

				if (type->create(this, filename)) // To do: use MainFrame as parent instead
				{
					solution_model().add_file(item_data->project_item, filename);
					controller->open(filename, type->get_editors().front());
				}
			}
		}
	}
}

void SolutionExplorer::on_add_existing_item()
{
	ListViewItemDataPtr item_data = get_item_data(listview->get_selected_item());
	if (item_data)
	{
		if (item_data->project_item)
		{
			OpenFileDialog dlg(this);
			dlg.set_initial_directory(item_data->project_item->get_filename());
			if(dlg.show())
			{
				std::vector<std::string> filenames = dlg.get_filenames();
				for(size_t i = 0; i < filenames.size(); ++i)
					solution_model().add_file(item_data->project_item, filenames[i]);
			}
		}
	}
}

void SolutionExplorer::on_open()
{
	ListViewItemDataPtr item_data = get_item_data(listview->get_selected_item());
	if (item_data)
	{
		if (dynamic_cast<ProjectFileItem*>(item_data->project_item))
		{
			ProjectFileItem *file = dynamic_cast<ProjectFileItem*>(item_data->project_item);
			FileItemType *file_type = factory.find_from_filename(file->filename);
			if (file_type)
			{
				controller->open(file->get_filename(), file_type->get_editors().front());
			}
		}
	}
}

void SolutionExplorer::on_open_with()
{
	ListViewItemDataPtr item_data = get_item_data(listview->get_selected_item());
	if (item_data)
	{
		if (dynamic_cast<ProjectFileItem*>(item_data->project_item))
		{
			ProjectFileItem *file = dynamic_cast<ProjectFileItem*>(item_data->project_item);
			FileItemType *file_type = factory.find_from_filename(file->filename);
			if (file_type)
			{
				DlgOpenWith dlg(this, file_type->get_editors());
				if (dlg.exec() == 1)
				{
					controller->open(file->get_filename(), dlg.get_editor_type());
				}
			}
		}
	}
}

void SolutionExplorer::on_remove()
{
	ListViewItemDataPtr item_data = get_item_data(listview->get_selected_item());
	if (item_data && item_data->project_item)
	{
		solution_model().remove_item(item_data->project_item);
	}
}

void SolutionExplorer::on_new_folder()
{
	ListViewItemDataPtr item_data = get_item_data(listview->get_selected_item());
	if (item_data)
	{
		ProjectFolderItem *folder = dynamic_cast<ProjectFolderItem*>(item_data->project_item);
		BrowseFolderDialog browse_folder(this);
		browse_folder.set_initial_directory(item_data->project_item->get_filename());
		browse_folder.set_title("Select folder to add");
		if (browse_folder.show())
			solution_model().add_folder(item_data->project_item, browse_folder.get_selected_path());
	}
}

ListViewItem SolutionExplorer::create_item(Solution *solution)
{
	ListViewItem item = listview->create_item();
	item.set_open(true);
	item.set_icon(4);
	item.set_column_text("name", string_format("Solution '%1'", solution->name));
	item.set_userdata(ListViewItemDataPtr(new ListViewItemData(solution)));
	return item;
}

ListViewItem SolutionExplorer::create_item(ProjectPtr project)
{
	ListViewItem item = listview->create_item();
	item.set_open(true);
	item.set_icon(5);
	item.set_column_text("name", project->name);
	item.set_userdata(ListViewItemDataPtr(new ListViewItemData(&project->items)));
	return item;
}

ListViewItem SolutionExplorer::create_item(ProjectItem *project_item)
{
	ListViewItem item = listview->create_item();
	if (dynamic_cast<ProjectFolderItem*>(project_item))
	{
		item.set_open(solution_model().is_open(project_item));
		item.set_icon(1);
	}
	else if (dynamic_cast<ProjectFilterItem*>(project_item))
	{
		item.set_open(solution_model().is_open(project_item));
		item.set_icon(2);
	}
	else if (dynamic_cast<ProjectFileItem*>(project_item))
	{
		item.set_icon(get_icon_index(dynamic_cast<ProjectFileItem*>(project_item)->filename));
	}
	else
	{
		item.set_icon(5);
	}
	item.set_column_text("name", project_item->get_name());
	item.set_userdata(ListViewItemDataPtr(new ListViewItemData(project_item)));
	return item;
}

SolutionExplorer::ListViewItemDataPtr SolutionExplorer::get_item_data(ListViewItem item)
{
	if (!item.is_null())
		return std::dynamic_pointer_cast<ListViewItemData>(item.get_userdata());
	else
		return ListViewItemDataPtr();
}

void SolutionExplorer::create_icon_list()
{
	ResourceManager resources = get_resources();

	Canvas canvas = get_canvas();
	ListViewIcon icon;
	icon.set_sprite(Sprite(canvas, "IconFolderOpen", &resources), listview_mode_details);
	listview->get_icon_list().set_icon(1, icon);

	ListViewIcon icon2;
	icon2.set_sprite(Sprite(canvas, "IconFolderClosed", &resources), listview_mode_details);
	listview->get_icon_list().set_icon(2, icon2);

	ListViewIcon icon3;
	icon3.set_sprite(Sprite(canvas, "IconDocument", &resources), listview_mode_details);
	listview->get_icon_list().set_icon(3, icon3);

	ListViewIcon icon4;
	icon4.set_sprite(Sprite(canvas, "IconSolution", &resources), listview_mode_details);
	listview->get_icon_list().set_icon(4, icon4);

	ListViewIcon icon5;
	icon5.set_sprite(Sprite(canvas, "IconProject", &resources), listview_mode_details);
	listview->get_icon_list().set_icon(5, icon5);

	int current_index = 6;
	for (size_t i = 0; i < factory.types().size(); i++)
	{
		Sprite sprite = factory.types()[i]->get_icon(canvas, &resources);
		if (!sprite.is_null())
		{
			extension_to_icon[StringHelp::text_to_lower(factory.types()[i]->get_extension())] = current_index;

			ListViewIcon icon;
			icon.set_sprite(sprite, listview_mode_details);
			listview->get_icon_list().set_icon(current_index, icon);
			current_index++;
		}
		else
		{
			extension_to_icon[StringHelp::text_to_lower(factory.types()[i]->get_extension())] = 3;
		}
	}
}

int SolutionExplorer::get_icon_index(std::string filename)
{
	std::map<std::string, int>::iterator it = extension_to_icon.find(StringHelp::text_to_lower(PathHelp::get_extension(filename)));
	if (it != extension_to_icon.end())
		return it->second;
	else
		return 3; // IconDocument
}

}

