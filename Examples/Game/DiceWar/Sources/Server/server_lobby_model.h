
#pragma once

#include "server_lobby_player_collection.h"
#include "server_lobby_game_collection.h"

class ServerLobbyModel
{
public:
	ServerLobbyModel(Server *server);
	~ServerLobbyModel();

public:
	ServerLobbyPlayerCollection players;
	ServerLobbyGameCollection games;
};
