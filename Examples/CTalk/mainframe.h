
#pragma once

#include <vector>

class Document;
class View;
class IRCConnection;
class CL_PushButton;

class MainFrame : public CL_Window
{
//! Construction:
public:
	MainFrame(CL_GUIManager *gui, CL_ResourceManager *resources);
	~MainFrame();

//! Attributes:
public:
	Document *get_document();
	int get_num_views();
	View *get_view(int index);
	View *get_active_view();
	CL_Rect get_view_area();
	CL_ResourceManager *get_resources();

//! Operations:
public:
	void set_active_view(View *view);
	void set_active_view(int index);
	void add_view(const CL_String &channel, IRCConnection *connection);
	void remove_view(View *view);
	int find_view_index(View *view);
	void connect_to_server(const CL_String &server_name, const CL_String &nick, const CL_String &username, const CL_String &full_name, const CL_String &password = CL_String());

//! Implementation:
private:
	void add_server_list_view();
	void arrange_buttons();
	void on_resized();
	void on_close();
	void on_view_button_clicked(View *view);
	void on_connection_join(const CL_String &nick, const CL_String &channel, IRCConnection *connection);
	void on_timer_expired();
	static CL_GUITopLevelDescription get_window_description();
	CL_ResourceManager *resources;
	std::auto_ptr<Document> document;
	std::vector<View *> views;
	CL_Tab *tab;
	CL_SlotContainer slots;
	CL_Timer timer;
};
