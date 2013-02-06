
#pragma once

#include "../SolutionModel/solution_model.h"
#include "../FileItemType/file_item_type_factory.h"
#include "../TextEditor/text_editor_file_item_type.h"
#include "../UIController/ui_controller.h"
#include "../BuildSystem/build_system.h"

class clan::Ribbon;
class Workspace;
class SolutionExplorer;
class PropertyManager;

class EditorMainWindow : public clan::GUIComponent
{
public:
	EditorMainWindow(clan::GUIManager *manager, const FileItemTypeFactory &file_item_type_factory);
	SolutionModel &get_model() { return model; }

private:
	void on_resized();
	bool on_close();
	void on_item_new();
	void on_item_open();
	void on_item_save();
	void on_item_save_as();
	void on_item_about();
	void on_item_exit();

	static clan::GUITopLevelDescription get_top_level_description();

	clan::Ribbon *ribbon;
	Workspace *workspace;
	std::unique_ptr<UIController> ui_controller;
	SolutionExplorer *explorer;
	PropertyManager *property_manager;
	SolutionModel model;
	BuildSystem build_system;
	FileItemTypeFactory file_item_type_factory;
	TextEditorFileItemType text_editor_file_item_type;
};
