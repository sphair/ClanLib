
#pragma once

#include "server_lobby_game_collection.h"
#include "server_lobby_player_collection.h"
#include "server_lobby_events.h"

class ServerLobbyGameInformation;
class ServerLobbyPlayerInformation;

class ServerLobby
{
public:
	ServerLobby(clan::NetGameServer *server);
	~ServerLobby() {};

	bool dispatch_event(const clan::NetGameEvent &e, int player_id);

	void add_player(int player_id, clan::NetGameConnection *connection);
	void remove_player(int player_id);

	clan::Callback_1<ServerLobbyGameInformation *, clan::NetGameConnection *> func_create_lobby_game;
	clan::Callback_1<ServerLobbyPlayerInformation *, clan::NetGameConnection *> func_create_lobby_player;

private:
	std::auto_ptr<ServerLobbyEvents> lobby_events;

	ServerLobbyPlayerCollection players;
	ServerLobbyGameCollection games;

	clan::NetGameServer *server;
	clan::SlotContainer slots;
};
