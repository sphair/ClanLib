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

	void exec(clan::Event &stop_event);

	clan::NetGameServer *get_network_server() { return &network_server; }

private:
	void on_client_connected(clan::NetGameConnection *connection);
	void on_client_disconnected(clan::NetGameConnection *connection, const std::string &);
	void on_event_received(clan::NetGameConnection *connection, const clan::NetGameEvent &e);
	void on_event_login(const clan::NetGameEvent &e, ServerPlayer *player);

	ServerLobbyGameInformation *on_create_lobby_game(clan::NetGameConnection *owner_player_connection);
	ServerLobbyPlayerInformation *on_create_lobby_player(clan::NetGameConnection *player_connection);

	void login(ServerPlayer *player, std::string player_name);
	
	int next_player_id;
	int next_game_id;

	clan::NetGameServer network_server;

	ServerLobby lobby;

	clan::NetGameEventDispatcher_v1<ServerPlayer *> login_events;

	std::vector<ServerGame *> running_games;

	clan::SlotContainer slots;
};
