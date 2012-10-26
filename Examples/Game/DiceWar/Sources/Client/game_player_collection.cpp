	
#include "precomp.h"
#include "game_player_collection.h"
#include "game_player.h"
#include <algorithm>

GamePlayerCollection::GamePlayerCollection()
{
	// Create default colors for max 6 players
	// The visual_id is an index into this array
	player_starting_colors.push_back(Color::red);
	player_starting_colors.push_back(Color::green);
	player_starting_colors.push_back(Color::blue);
	player_starting_colors.push_back(Color::yellow);
	player_starting_colors.push_back(Color::purple);
	player_starting_colors.push_back(Color::magenta);
}

GamePlayerCollection::~GamePlayerCollection()
{
}

GamePlayer *GamePlayerCollection::create_player(std::string &player_name, int player_id, int visual_id)
{
	if(visual_id >= 0 && visual_id < (int)player_starting_colors.size())
	{
		Color color = player_starting_colors[visual_id];
		GamePlayer *player = new GamePlayer(player_name, player_id, color);

		players.push_back(player);

		return player;
	}
	else
	{
		// Unable to create the player
		return 0;
	}
}

void GamePlayerCollection::remove_player(GamePlayer *player)
{
	std::vector<GamePlayer *>::iterator it;
	it = find(players.begin(), players.end(), player); 
	if (it != players.end())
	{
		players.erase(it);
	}
}

GamePlayer *GamePlayerCollection::get_player(int player_id)
{
	std::vector<GamePlayer *>::iterator it;
	for(it = players.begin(); it != players.end(); ++it)
	{
		GamePlayer *player = (*it);

		if(player->id == player_id)
			return player;
	}

	// Unable to find player
	return 0;
}
