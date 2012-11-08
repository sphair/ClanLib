
#pragma once

class ServerLobbyPlayer;
class ServerLobbyGameCollection;
class ServerLobby;

class ServerLobbyEvents
{
public:
	ServerLobbyEvents(CL_NetGameServer *server, ServerLobby *lobby, ServerLobbyGameCollection *games);
	~ServerLobbyEvents() {};

	CL_NetGameEventDispatcher_v1<ServerLobbyPlayer *> &get_dispatcher() { return lobby_events; }

private:
	void on_event_create_game(const CL_NetGameEvent &e, ServerLobbyPlayer *player);
	void on_event_change_game_settings(const CL_NetGameEvent &e, ServerLobbyPlayer *player);
	void on_event_get_available_games(const CL_NetGameEvent &e, ServerLobbyPlayer *player);
	void on_event_join_game(const CL_NetGameEvent &e, ServerLobbyPlayer *player);
	void on_event_leave_game(const CL_NetGameEvent &e, ServerLobbyPlayer *player);
	void on_event_start_game(const CL_NetGameEvent &e, ServerLobbyPlayer *player);
	void on_event_add_lobby_message(const CL_NetGameEvent &e, ServerLobbyPlayer *player);

	CL_NetGameEventDispatcher_v1<ServerLobbyPlayer *> lobby_events;

	ServerLobbyGameCollection *games;
	ServerLobby *lobby;
	CL_NetGameServer *server;
};
