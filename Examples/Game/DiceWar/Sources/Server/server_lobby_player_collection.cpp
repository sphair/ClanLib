
#include "precomp.h"
#include "server_lobby_player_collection.h"
#include "server_lobby_game_player_collection.h"
#include "server_lobby_player.h"
#include "server_lobby_game.h"
#include "server_player.h"
#include "server_game.h"
#include "server_game_player.h"
#include "server.h"
#include "../Lib/net_events_lobby.h"
#include <algorithm>

ServerLobbyPlayerCollection::ServerLobbyPlayerCollection(Server *server)
: server(server)
{
}

ServerLobbyPlayerCollection::~ServerLobbyPlayerCollection()
{
}

ServerLobbyPlayer *ServerLobbyPlayerCollection::create_player(ServerPlayer *player, const std::string &player_name)
{
	ServerLobbyPlayer *lobby_player = new ServerLobbyPlayer(player);
	players.push_back(lobby_player);

	player->lobby_player = lobby_player;

	server->get_network_server()->send_event(NetGameEvent(STC_LOBBY_PLAYER_LOGGED_IN, player->id, player_name));
	send_available_players(lobby_player);

	return lobby_player;
}

void ServerLobbyPlayerCollection::remove_player(ServerPlayer *player)
{
	if(player)
	{
		// TODO: Move this out from lobby?
		if(player->game_player)
		{
			ServerGamePlayer *game_player = player->game_player;

			// Remove player from playing game he is in
			if (game_player->current_game)
				game_player->current_game->remove_player(game_player);

			delete player->game_player;
			player->game_player = 0;
		}

		if(player->lobby_player)
		{
			ServerLobbyPlayer *lobby_player = player->lobby_player;

			// Remove player from lobby game he has joined
			if (lobby_player->current_game)
				lobby_player->current_game->get_player_collection()->remove_player(lobby_player);

			// Remove player from player list
			std::vector<ServerLobbyPlayer *>::iterator it;
			it = find(players.begin(), players.end(), lobby_player); 
			if (it != players.end())
				players.erase(it);

			delete player->lobby_player;
			player->lobby_player = 0;
		}

		server->get_network_server()->send_event(NetGameEvent(STC_LOBBY_PLAYER_LOGGED_OUT, player->id));
	}
}

void ServerLobbyPlayerCollection::send_available_players(ServerLobbyPlayer *lobby_player)
{
	for(unsigned i = 0; i < players.size(); ++i)
	{
		if(players[i]->player->id != lobby_player->player->id)		
			lobby_player->send_event(NetGameEvent(STC_LOBBY_PLAYER_LOGGED_IN, players[i]->player->id, players[i]->player->name));
	}
}

void ServerLobbyPlayerCollection::send_event(const NetGameEvent &game_event)
{
	std::vector<ServerLobbyPlayer *>::iterator it;
	for (it = players.begin(); it != players.end(); ++it)
	{
		ServerLobbyPlayer *lobby_player = (*it);

		lobby_player->send_event(game_event);
	}
}
