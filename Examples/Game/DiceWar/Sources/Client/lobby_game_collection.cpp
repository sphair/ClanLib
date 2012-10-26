
#include "precomp.h"
#include "lobby_game_collection.h"
#include "lobby_game.h"

LobbyGameCollection::LobbyGameCollection()
{
}

LobbyGameCollection::~LobbyGameCollection()
{
	for (std::map<int, LobbyGame *>::iterator it = games.begin(); it != games.end(); ++it)
		delete it->second;
}

LobbyGame *LobbyGameCollection::get_game(int game_id) const
{
	std::map<int, LobbyGame *>::const_iterator it = games.find(game_id);
	if (it != games.end())
		return it->second;
	else
		return 0;
}

LobbyGame *LobbyGameCollection::create_game(int game_id, const std::string &game_name, const std::string &map_name, int max_players, LobbyGame::State state)
{
	if(games[game_id] == 0)
	{
		LobbyGame *game = new LobbyGame(game_id, game_name, map_name, max_players, state);
		games[game_id] = game;
		return game;
	}
	else
	{
		return 0;
	}
}

void LobbyGameCollection::remove_game(int game_id)
{
	games.erase(games.find(game_id));
}

void LobbyGameCollection::remove_player(LobbyPlayer *player)
{
	// Remove player from games list
	std::map<int, LobbyGame *>::iterator it;
	for(it = games.begin(); it != games.end(); ++it)
	{
		(*it).second->remove_player(player);
	}
}
