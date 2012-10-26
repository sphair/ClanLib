
#pragma once

#include "lobby_game.h"

class LobbyPlayer;
class Client;

class LobbyGameCollection
{
public:
	LobbyGameCollection();
	~LobbyGameCollection();

	LobbyGame *get_game(int game_id) const;

	LobbyGame *create_game(int game_id, const std::string &game_name, const std::string &map_name, int max_players, LobbyGame::State state);

	void remove_game(int game_id);

	void remove_player(LobbyPlayer *player);

private:
	std::map<int, LobbyGame *> games;
};
