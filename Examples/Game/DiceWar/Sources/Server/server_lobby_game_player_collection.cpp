
#include "precomp.h"
#include "server_lobby_game_player_collection.h"
#include "server.h"
#include "server_lobby_game.h"
#include "server_lobby_player.h"
#include "server_player.h"
#include "../Lib/net_events_lobby.h"
#include <algorithm>

ServerLobbyGamePlayerCollection::ServerLobbyGamePlayerCollection(Server *server, ServerLobbyGame *game, ServerLobbyPlayer *owner)
: server(server), game(game), owner(owner)
{
}

ServerLobbyGamePlayerCollection::~ServerLobbyGamePlayerCollection()
{
	std::vector<ServerLobbyPlayer *>::iterator it;
	for (it = players.begin(); it != players.end(); ++it)
		(*it)->current_game = 0;
}

void ServerLobbyGamePlayerCollection::add_player(ServerLobbyPlayer *lobby_player)
{
	if(lobby_player->current_game)
		return;

	players.push_back(lobby_player);
	lobby_player->current_game = game;

	server->get_network_server()->send_event(NetGameEvent(STC_LOBBY_PLAYER_JOINED_GAME, lobby_player->player->id, game->get_id()));
}

void ServerLobbyGamePlayerCollection::remove_player(ServerLobbyPlayer *lobby_player)
{
	if(lobby_player->current_game != game)
		return;

	std::vector<ServerLobbyPlayer *>::iterator it;
	it = find(players.begin(), players.end(), lobby_player); 
	if (it != players.end())
	{
		server->get_network_server()->send_event(NetGameEvent(STC_LOBBY_PLAYER_LEFT_GAME, lobby_player->player->id, lobby_player->current_game->get_id()));

		players.erase(it);
		
		lobby_player->current_game = 0;

		if (owner == lobby_player)
		{
			if (players.empty())
				owner = 0;
			else
				owner = players.front();
		}
	}
}

bool ServerLobbyGamePlayerCollection::contains_player(ServerLobbyPlayer *player)
{
	return find(players.begin(), players.end(), player) != players.end();
}

void ServerLobbyGamePlayerCollection::send_available_players(ServerLobbyPlayer *destination_player)
{
	std::vector<ServerLobbyPlayer *>::iterator it;
	for(it = players.begin(); it != players.end(); ++it)
	{
		ServerLobbyPlayer *lobby_player = (*it);

		destination_player->send_event(NetGameEvent(STC_LOBBY_PLAYER_JOINED_GAME, lobby_player->player->id, game->get_id()));
	}
}
