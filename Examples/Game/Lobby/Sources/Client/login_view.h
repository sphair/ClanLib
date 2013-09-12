
#pragma once

class Client;

class LoginView : public clan::GUIComponent
{
public:
	LoginView(clan::GUIManager *gui_manager);
	~LoginView();

	void show() { set_visible(true); }
	std::string get_server() const { return edit_server->get_text(); }
	std::string get_port() const { return edit_port->get_text(); }
	std::string get_player_nick() const { return edit_player_nick->get_text(); }

private:
	static clan::GUITopLevelDescription get_toplevel_description();

	void on_button_ok_clicked();
	void on_button_cancel_clicked();
	void on_edit_player_nick_changed(clan::InputEvent &e);

	void update_button_states();

	Client *client;

	clan::Label *label_info;
	clan::Label *label_player_nick;
	clan::Label *label_server;
	clan::LineEdit *edit_player_nick;
	clan::LineEdit *edit_server;
	clan::LineEdit *edit_port;
	clan::PushButton *button_ok;
	clan::PushButton *button_cancel;
};
