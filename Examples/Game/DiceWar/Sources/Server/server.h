
#pragma once

class ServerPlayer;
class ServerGameEvents;
class ServerLobbyEvents;
class ServerLoginEvents;
class ServerLobbyModel;

class Server
{
public:
	Server();
	~Server();

	void exec(CL_Event &stop_event);

	CL_NetGameServer *get_network_server() { return &network_server; }

	void login(ServerPlayer *player, CL_String player_name);
	ServerPlayer *create_ai_player(const CL_String &player_name);

	void handle_event(ServerPlayer *player, const CL_NetGameEvent &e);

private:
	void on_client_connected(CL_NetGameConnection *connection);
	void on_client_disconnected(CL_NetGameConnection *connection);

	void on_event_received(CL_NetGameConnection *connection, const CL_NetGameEvent &e);

	int next_player_id;

	CL_NetGameServer network_server;

	CL_AutoPtr<ServerLoginEvents> login_events;
	CL_AutoPtr<ServerLobbyEvents> lobby_events;
	CL_AutoPtr<ServerGameEvents> game_events;

	CL_AutoPtr<ServerLobbyModel> lobby_model;

	CL_SlotContainer slots;
};
