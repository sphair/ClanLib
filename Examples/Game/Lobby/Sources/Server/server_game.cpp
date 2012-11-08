
#include "precomp.h"
#include "server.h"
#include "server_player.h"
#include "server_game.h"
#include "server_game_player_collection.h"
#include "server_game_player.h"

ServerGame::ServerGame(Server *server, int id)
: server(server), active_player(0), id(id), current_turn(0), max_turns(20)
{
	players.reset(new ServerGamePlayerCollection(server, this));
}

ServerGame::~ServerGame()
{
	std::vector<ServerPlayer *>::iterator it;
	for (it = players_ai.begin(); it != players_ai.end(); ++it)
		delete (*it);
}

void ServerGame::add_player(ServerPlayer *player)
{
	players->add_player(new ServerGamePlayer(player));
}

void ServerGame::remove_player(ServerGamePlayer *player)
{
	players->remove_player(player);
}

bool ServerGame::start()
{
	if(players->get_count() == 0)
		return false;

	return true;
}
