
#pragma once

#include "view.h"

class ServerListView : public View
{
public:
	ServerListView(CL_GUIComponent *parent, MainFrame *mainframe);
	~ServerListView();

private:
	void on_resized();
	void on_button_add_clicked();
	void on_button_remove_clicked();
	void on_button_connect_clicked();
	void on_button_edit_clicked();
	CL_Rect get_button_geometry(int index, int separators);
	void on_server_list_selection_changed(CL_ListViewSelection selection);

	CL_ListView *server_list;
	CL_PushButton *button_add;
	CL_PushButton *button_remove;
	CL_PushButton *button_connect;
	CL_PushButton *button_edit;
};
