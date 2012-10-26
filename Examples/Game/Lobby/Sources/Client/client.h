
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

	CL_NetGameClient *get_network_client() { return &network_client; }

	void send_event(const CL_NetGameEvent &game_event) { network_client.send_event(game_event); }

private:
	void on_window_close();

	void on_connected();
	void on_disconnected();
	void on_event_received(const CL_NetGameEvent &e);
	void on_gui_paint();

	State state;

	CL_DisplayWindow display_window;
	CL_GUIManager gui;

	CL_NetGameClient network_client;

	CL_String player_nick;
	int player_id;

	CL_Image image_desktop;

	LoginEvents *login_events;

	CL_SlotContainer slots;
};
