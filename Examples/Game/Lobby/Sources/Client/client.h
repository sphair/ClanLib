
#pragma once

class LoginEvents;

class Client
{
public:
	enum State
	{
		not_logged_in,
		in_lobby,
		playing
	};

public:
	Client();

	void initialize_network();

	void initialize_display();

	void initialize_gui();

	~Client();

	void exec();

	State get_state() const { return state; }
	void set_state(const State &value) { state = value; }

	void set_player_id(int id) { player_id = id; }
	int get_player_id() const { return player_id; }

	clan::NetGameClient *get_network_client() { return &network_client; }

	void send_event(const clan::NetGameEvent &game_event) { network_client.send_event(game_event); }

private:
	void on_window_close();

	void on_connected();
	void on_disconnected();
	void on_event_received(const clan::NetGameEvent &e);
	void on_gui_paint();

	State state;

	clan::DisplayWindow display_window;
	clan::GUIManager gui;

	clan::NetGameClient network_client;

	std::string player_nick;
	int player_id;

	clan::Image image_desktop;

	LoginEvents *login_events;

	clan::SlotContainer slots;
};
