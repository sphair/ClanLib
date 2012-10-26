
#include "precomp.h"
#include "server.h"
#include "server_player.h"
#include "server_game_player.h"
#include "server_game_player_collection.h"
#include "../Lib/net_events_game.h"
#include <algorithm>

ServerGamePlayerCollection::ServerGamePlayerCollection(Server *server, ServerGame *game)
: server(server), game(game), next_visual_id(0)
{
}

ServerGamePlayerCollection::~ServerGamePlayerCollection()
{
	std::vector<ServerGamePlayer *>::iterator it;
	for (it = players.begin(); it != players.end(); ++it)
		(*it)->current_game = 0;
}

void ServerGamePlayerCollection::add_player(ServerGamePlayer *game_player)
{
	if(game_player->current_game)
		return;

	ServerPlayer *player = game_player->player;

	game_player->current_game = game;
	game_player->visual_id = next_visual_id++;

	players.push_back(game_player);
}

void ServerGamePlayerCollection::remove_player(ServerGamePlayer *game_player)
{
	if(game_player->current_game != game)
		return;

	std::vector<ServerGamePlayer *>::iterator it;
	it = find(players.begin(), players.end(), game_player); 
	if (it != players.end())
	{
		server->get_network_server()->send_event(NetGameEvent(STC_GAME_PLAYER_LEFT_GAME, game_player->player->id));

		players.erase(it);
		
		game_player->current_game = 0;
	}
}

bool ServerGamePlayerCollection::contains_player(ServerGamePlayer *game_player)
{
	return find(players.begin(), players.end(), game_player) != players.end();
}

ServerGamePlayer *ServerGamePlayerCollection::get_player(int player_id)
{
	std::vector<ServerGamePlayer *>::iterator it;
	for (it = players.begin(); it != players.end(); ++it)
	{
		if((*it)->player->id == player_id)
			return (*it);
	}
	throw Exception(string_format("Unable to find player id %1 in game player collection", player_id));
}

void ServerGamePlayerCollection::send_event(const NetGameEvent &game_event)
{
	std::vector<ServerGamePlayer *>::iterator it;
	for (it = players.begin(); it != players.end(); ++it)
	{
		ServerGamePlayer *game_player = (*it);

		game_player->send_event(game_event);
	}
}

void ServerGamePlayerCollection::transfer_players()
{
	std::vector<ServerGamePlayer *>::iterator it;
	for (it = players.begin(); it != players.end(); ++it)
	{
		ServerGamePlayer *game_player = (*it);
		send_event(NetGameEvent(STC_GAME_PLAYER_JOINED_GAME, game_player->player->id, game_player->player->name, game_player->visual_id));
	}
}
