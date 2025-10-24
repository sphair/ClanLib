#pragma once

#include "../view.h"
#include "../texture_packer.h"

class SpriteComponent;
class ResourceItem;

class ResourceViewerView : public View
{
public:
	ResourceViewerView(CL_GUIComponent *parent, MainWindow *mainwindow);
	~ResourceViewerView();

private:
	void on_resized();
	void on_button_browse_resource_file();
	void on_selection_changed(CL_ListViewSelection selection);

	void load_resource_file(const CL_String &file);

	void show_resource(ResourceItem *resource_item);

	TexturePacker packer;

	CL_Label *label_selected_resource_file;
	CL_LineEdit *lineedit_selected_resource_file;
	CL_PushButton *button_browse_resource_file;

	CL_ListView *resource_list;

	SpriteComponent *sprite_component;
};
