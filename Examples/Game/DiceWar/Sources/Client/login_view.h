
#pragma once

class Client;

class LoginView : public GUIComponent
{
public:
	LoginView(Client *client);
	~LoginView();

	void show() { set_visible(true); }
	std::string get_server() const { return edit_server->get_text(); }
	std::string get_port() const { return edit_port->get_text(); }
	std::string get_player_nick() const { return edit_player_nick->get_text(); }

private:
	static GUITopLevelDescription get_toplevel_description();

	void on_button_ok_clicked();
	void on_button_cancel_clicked();
	void on_edit_player_nick_changed(InputEvent &e);

	Client *client;

	Label *label_info;
	Label *label_player_nick;
	Label *label_server;
	LineEdit *edit_player_nick;
	LineEdit *edit_server;
	LineEdit *edit_port;
	PushButton *button_ok;
	PushButton *button_cancel;
};
