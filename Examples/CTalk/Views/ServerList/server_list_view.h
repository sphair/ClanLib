
#pragma once

#include "Framework/view.h"
#include "Document/irc_session.h"

class XMLSettings;

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
	void on_server_list_selection_changed(CL_ListViewSelection selection);
	void on_session_created(IRCSession *session);
	void on_session_destroyed(IRCSession *session);
	void on_session_connect_status_changed(IRCSession::ConnectStatus status, IRCSession *session);

	CL_Rect get_button_geometry(int index, int separators);

	XMLSettings get_settings();

	CL_ListView *server_list;
	CL_PushButton *button_add;
	CL_PushButton *button_remove;
	CL_PushButton *button_connect;
	CL_PushButton *button_edit;

	CL_SlotContainer slots;
};
