#pragma once

#include "server_player.h"
#include "../Lobby/server_lobby.h"

class ServerLobbyGameInformation;
class ServerGame;

class Server
{
public:
	Server();
	~Server();

	void exec(CL_Event &stop_event);

	CL_NetGameServer *get_network_server() { return &network_server; }

private:
	void on_client_connected(CL_NetGameConnection *connection);
	void on_client_disconnected(CL_NetGameConnection *connection);
	void on_event_received(CL_NetGameConnection *connection, const CL_NetGameEvent &e);
	void on_event_login(const CL_NetGameEvent &e, ServerPlayer *player);

	ServerLobbyGameInformation *on_create_lobby_game(CL_NetGameConnection *owner_player_connection);
	ServerLobbyPlayerInformation *on_create_lobby_player(CL_NetGameConnection *player_connection);

	void login(ServerPlayer *player, CL_String player_name);
	
	int next_player_id;
	int next_game_id;

	CL_NetGameServer network_server;

	ServerLobby lobby;

	CL_NetGameEventDispatcher_v1<ServerPlayer *> login_events;

	std::vector<ServerGame *> running_games;

	CL_SlotContainer slots;
};
