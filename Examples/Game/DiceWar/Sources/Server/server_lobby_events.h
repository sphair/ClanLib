
#pragma once

class Server;
class ServerLobbyPlayer;

class ServerLobbyEvents
{
public:
	ServerLobbyEvents(Server *server, ServerLobbyModel *lobby_model);
	~ServerLobbyEvents();

	NetGameEventDispatcher_v1<ServerLobbyPlayer *> &get_dispatcher() { return lobby_events; }

private:
	void on_event_create_game(const NetGameEvent &e, ServerLobbyPlayer *player);
	void on_event_change_game_settings(const NetGameEvent &e, ServerLobbyPlayer *player);
	void on_event_get_available_games(const NetGameEvent &e, ServerLobbyPlayer *player);
	void on_event_join_game(const NetGameEvent &e, ServerLobbyPlayer *player);
	void on_event_leave_game(const NetGameEvent &e, ServerLobbyPlayer *player);
	void on_event_start_game(const NetGameEvent &e, ServerLobbyPlayer *player);
	void on_event_add_lobby_message(const NetGameEvent &e, ServerLobbyPlayer *player);

	NetGameEventDispatcher_v1<ServerLobbyPlayer *> lobby_events;

	Server *server;
	ServerLobbyModel *lobby_model;
};
