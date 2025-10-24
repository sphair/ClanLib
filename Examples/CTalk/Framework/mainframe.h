
#pragma once

#include <vector>
#include "Document/irc_entity.h"
#include "view_workspace.h"
#include "notification_area_icon.h"

class Document;
class View;
class ChatView;
class IRCSession;
class IRCText;
class XMLSettings;

class MainFrame : public CL_Window
{
public:
	MainFrame(CL_GUIManager *gui, CL_ResourceManager *resources);
	~MainFrame();

	Document *get_document();
	int get_num_views();
	View *get_view(int index);
	View *get_active_view();
	CL_Rect get_view_area();
	CL_ResourceManager *get_resources();

	void set_active_view(View *view);
	void set_active_view(int index);
	void add_view(const IRCEntity &filter, IRCSession *connection);
	void remove_view(View *view);
	int find_view_index(View *view);
	ChatView *find_view(const IRCEntity &filter);
	void flag_activity(View *view);

	void connect_to_server(XMLSettings &connection);
	void open_conversation(const IRCNick &nick, IRCSession *session);

private:
	void auto_connect_to_servers();
	void add_server_list_view();
	void arrange_buttons();
	void flash_window(View *view);
	void on_resized();
	void on_close();
	void on_minimized();
	void on_activation_gained();

	void on_view_close(View *view);
	void on_connection_join(const IRCChannel &channel, IRCSession *session);
	void on_connection_private_text(const IRCNick &nick, const IRCText &text, IRCSession *session);
	void on_notification_icon_activate();

	static CL_GUITopLevelDescription get_window_description();
	CL_ResourceManager *resources;
	std::auto_ptr<Document> document;
	std::vector<View *> views;
	ViewWorkspace *workspace;
	CL_SlotContainer slots;
	NotificationAreaIcon notification_icon;
};
