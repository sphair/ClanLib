
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

	NetGameEventDispatcher_v0 &get_dispatcher() { return lobby_events; }
	LobbyPlayer *get_player(int player_id) const;

private:
	void on_event_player_logged_in(const NetGameEvent &e);
	void on_event_player_logged_out(const NetGameEvent &e);
	void on_event_game_created(const NetGameEvent &e);
	void on_event_game_info(const NetGameEvent &e);
	void on_event_game_removed(const NetGameEvent &e);
	void on_event_no_games_available(const NetGameEvent &e);
	void on_event_player_joined_game(const NetGameEvent &e);
	void on_event_player_left_game(const NetGameEvent &e);
	void on_event_game_started(const NetGameEvent &e);
	void on_event_player_message(const NetGameEvent &e);
	void on_event_error_message(const NetGameEvent &e);

	NetGameEventDispatcher_v0 lobby_events;

	Client *client;

	std::auto_ptr<LobbyPlayerCollection> player_collection;
	std::auto_ptr<LobbyGameCollection> game_collection;
};
