
#pragma once

#include "../Workspace/dockable_component.h"
#include "../FileItemType/file_item_type_factory.h"
#include "../SolutionModel/Project/project.h"

class SolutionModel;
class Solution;
class Project;
typedef std::shared_ptr<Project> ProjectPtr;
class ProjectItem;
class UIController;
class DragDropOperation;

class SolutionExplorer : public DockableComponent
{
public:
	SolutionExplorer(FileItemTypeFactory &factory, UIController *controller);
	std::vector<std::string> get_selected_filenames();

private:
	SolutionModel &solution_model();
	void on_item_show_solution_explorer();

	class ListViewItemData : public clan::ListViewItemUserData
	{
	public:
		ListViewItemData() : solution(), project_item() { }
		ListViewItemData(Solution *solution) : solution(solution), project_item() { }
		ListViewItemData(ProjectItem *project_item) : solution(), project_item(project_item) { }

		Solution *solution;
		ProjectItem *project_item;
	};
	typedef std::shared_ptr<ListViewItemData> ListViewItemDataPtr;

	void populate();
	void populate_item(clan::ListViewItem parent_item, ProjectItem *item);
	void create_icon_list();
	clan::ListViewItem create_item(Solution *solution);
	clan::ListViewItem create_item(ProjectPtr project);
	clan::ListViewItem create_item(ProjectItem *project_item);
	ListViewItemDataPtr get_item_data(clan::ListViewItem item);

	void on_resized();
	void on_listview_mouse_right_up(clan::Point pos);
	void on_listview_item_doubleclick(const clan::ListViewItem &item);
	void on_listview_item_opened(const clan::ListViewItem &item);
	void on_listview_item_closed(const clan::ListViewItem &item);
	void on_listview_begin_drag();
	void on_add_new_item();
	void on_add_existing_item();
	void on_remove();
	void on_open();
	void on_open_with();
	void on_new_folder();

	int get_icon_index(std::string filename);

	FileItemTypeFactory &factory;
	UIController *controller;
	clan::ListView *listview;
	DragDropOperation *drag_drop;
	std::map<std::string, int> extension_to_icon;
	clan::PopupMenu popup;
	clan::SlotContainer slots;

	friend class DragDropOperation;
};
