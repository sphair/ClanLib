
#ifndef header_main
#define header_main

class FormView;
class PropertyList;

class GUIEditor : public CL_ClanApplication
{
public:
	virtual int main(int argc, char **argv);

private:
	void on_window_close();

	void on_paint();

	void on_resize(int old_width, int old_height);

	CL_GUIManager *gui;

	CL_Menu *menubar;
	CL_MenuNode *menunode_file;
	CL_MenuItem *menuitem_file;
	CL_MenuNode *menunode_edit;
	CL_MenuItem *menuitem_edit;
	CL_MenuNode *menunode_view;
	CL_MenuItem *menuitem_view;
	CL_MenuNode *menunode_help;
	CL_MenuItem *menuitem_help;

	CL_Menu *menu_file;
	CL_MenuNode *menunode_new;
	CL_MenuItem *menuitem_new;
	CL_MenuNode *menunode_open;
	CL_MenuItem *menuitem_open;
	CL_MenuNode *menunode_save;
	CL_MenuItem *menuitem_save;
	CL_MenuNode *menunode_exit;
	CL_MenuItem *menuitem_exit;

	CL_Menu *menu_edit;
	CL_MenuNode *menunode_undo;
	CL_MenuItem *menuitem_undo;
	CL_MenuNode *menunode_redo;
	CL_MenuItem *menuitem_redo;
	CL_MenuNode *menunode_cut;
	CL_MenuItem *menuitem_cut;
	CL_MenuNode *menunode_copy;
	CL_MenuItem *menuitem_copy;
	CL_MenuNode *menunode_paste;
	CL_MenuItem *menuitem_paste;
	CL_MenuNode *menunode_delete;
	CL_MenuItem *menuitem_delete;

	CL_Menu *menu_view;
	CL_MenuNode *menunode_components;
	CL_MenuItem *menuitem_components;
	CL_MenuNode *menunode_connections;
	CL_MenuItem *menuitem_connections;
	CL_MenuNode *menunode_properties;
	CL_MenuItem *menuitem_properties;
	CL_MenuNode *menunode_tools;
	CL_MenuItem *menuitem_tools;

	CL_Menu *menu_help;
	CL_MenuNode *menunode_about;
	CL_MenuItem *menuitem_about;

	CL_Frame *frame_toolbox;
	CL_Label *label_toolbox;
	CL_ListBox *list_tools;

	CL_Frame *frame_form;
	CL_Label *label_form;
	FormView *form_view;

	CL_Frame *frame_propertylist;
	CL_Label *label_properties;
	PropertyList *property_list;
};

#endif
