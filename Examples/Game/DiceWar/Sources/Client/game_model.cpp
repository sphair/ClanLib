
#include "precomp.h"
#include "game_model.h"
#include "game_player_collection.h"

GameModel::GameModel()
{
	players.reset(new GamePlayerCollection());
}

GameModel::~GameModel()
{
}
