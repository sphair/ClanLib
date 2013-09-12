
#pragma once

class ServerLobbyPlayer;
class ServerLobbyGameCollection;
class ServerLobby;

class ServerLobbyEvents
{
public:
	ServerLobbyEvents(clan::NetGameServer *server, ServerLobby *lobby, ServerLobbyGameCollection *games);
	~ServerLobbyEvents() {};

	clan::NetGameEventDispatcher_v1<ServerLobbyPlayer *> &get_dispatcher() { return lobby_events; }

private:
	void on_event_create_game(const clan::NetGameEvent &e, ServerLobbyPlayer *player);
	void on_event_change_game_settings(const clan::NetGameEvent &e, ServerLobbyPlayer *player);
	void on_event_get_available_games(const clan::NetGameEvent &e, ServerLobbyPlayer *player);
	void on_event_join_game(const clan::NetGameEvent &e, ServerLobbyPlayer *player);
	void on_event_leave_game(const clan::NetGameEvent &e, ServerLobbyPlayer *player);
	void on_event_start_game(const clan::NetGameEvent &e, ServerLobbyPlayer *player);
	void on_event_add_lobby_message(const clan::NetGameEvent &e, ServerLobbyPlayer *player);

	clan::NetGameEventDispatcher_v1<ServerLobbyPlayer *> lobby_events;

	ServerLobbyGameCollection *games;
	ServerLobby *lobby;
	clan::NetGameServer *server;
};
