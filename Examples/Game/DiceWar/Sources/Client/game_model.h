
#pragma once

class GamePlayerCollection;

#include "game_map.h"

class GameModel
{
public:
	GameModel();
	~GameModel();

	GamePlayerCollection *get_players() { return players; }

	ClientMap *get_map() { return &map; }

private:
	CL_AutoPtr<GamePlayerCollection> players;

	ClientMap map;
};
