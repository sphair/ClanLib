
#pragma once

#include "server_lobby_game_collection.h"
#include "server_lobby_player_collection.h"
#include "server_lobby_events.h"

class ServerLobbyGameInformation;
class ServerLobbyPlayerInformation;

class ServerLobby
{
public:
	ServerLobby(CL_NetGameServer *server);
	~ServerLobby() {};

	bool dispatch_event(const CL_NetGameEvent &e, int player_id);

	void add_player(int player_id, CL_NetGameConnection *connection);
	void remove_player(int player_id);

	CL_Callback_1<ServerLobbyGameInformation *, CL_NetGameConnection *> func_create_lobby_game;
	CL_Callback_1<ServerLobbyPlayerInformation *, CL_NetGameConnection *> func_create_lobby_player;

private:
	std::auto_ptr<ServerLobbyEvents> lobby_events;

	ServerLobbyPlayerCollection players;
	ServerLobbyGameCollection games;

	CL_NetGameServer *server;
	CL_SlotContainer slots;
};
