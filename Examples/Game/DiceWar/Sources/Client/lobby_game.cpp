
#include "precomp.h"
#include "lobby_game.h"
#include <algorithm>

LobbyGame::LobbyGame(int id, std::string name, std::string map_name, int max_players, State state)
: name(name), id(id), max_players(max_players), map_name(map_name), state(state)
{
}

LobbyGame::~LobbyGame()
{
}

void LobbyGame::add_player(LobbyPlayer *player)
{
	players.push_back(player);
}

void LobbyGame::remove_player(LobbyPlayer *player)
{
	std::vector<LobbyPlayer *>::iterator it;
	it = find(players.begin(), players.end(), player); 
	if (it != players.end())
	{
		players.erase(it);
	}
}

bool LobbyGame::contains_player(LobbyPlayer *player)
{
	return find(players.begin(), players.end(), player) != players.end();
}
