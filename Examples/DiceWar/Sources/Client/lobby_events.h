
#pragma once

class Client;
class LobbyPlayer;
class LobbyGame;
class LobbyPlayerCollection;
class LobbyGameCollection;

class LobbyEvents
{
public:
	LobbyEvents(Client *client);
	~LobbyEvents();

	CL_NetGameEventDispatcher_v0 &get_dispatcher() { return lobby_events; }
	LobbyPlayer *get_player(int player_id) const;

private:
	void on_event_player_logged_in(const CL_NetGameEvent &e);
	void on_event_player_logged_out(const CL_NetGameEvent &e);
	void on_event_game_created(const CL_NetGameEvent &e);
	void on_event_game_info(const CL_NetGameEvent &e);
	void on_event_game_removed(const CL_NetGameEvent &e);
	void on_event_no_games_available(const CL_NetGameEvent &e);
	void on_event_player_joined_game(const CL_NetGameEvent &e);
	void on_event_player_left_game(const CL_NetGameEvent &e);
	void on_event_game_started(const CL_NetGameEvent &e);
	void on_event_player_message(const CL_NetGameEvent &e);
	void on_event_error_message(const CL_NetGameEvent &e);

	CL_NetGameEventDispatcher_v0 lobby_events;

	Client *client;

	CL_AutoPtr<LobbyPlayerCollection> player_collection;
	CL_AutoPtr<LobbyGameCollection> game_collection;
};
