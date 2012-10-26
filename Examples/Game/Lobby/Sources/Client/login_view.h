
#pragma once

class Client;

class LoginView : public CL_GUIComponent
{
public:
	LoginView(CL_GUIManager *gui_manager);
	~LoginView();

	void show() { set_visible(true); }
	CL_String get_server() const { return edit_server->get_text(); }
	CL_String get_port() const { return edit_port->get_text(); }
	CL_String get_player_nick() const { return edit_player_nick->get_text(); }

private:
	static CL_GUITopLevelDescription get_toplevel_description();

	void on_button_ok_clicked();
	void on_button_cancel_clicked();
	void on_edit_player_nick_changed(CL_InputEvent &e);

	void update_button_states();

	Client *client;

	CL_Label *label_info;
	CL_Label *label_player_nick;
	CL_Label *label_server;
	CL_LineEdit *edit_player_nick;
	CL_LineEdit *edit_server;
	CL_LineEdit *edit_port;
	CL_PushButton *button_ok;
	CL_PushButton *button_cancel;
};
