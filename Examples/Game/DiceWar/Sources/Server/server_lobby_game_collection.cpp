
#include "precomp.h"
#include "server_lobby_game_collection.h"
#include "server_lobby_game_player_collection.h"
#include "server_lobby_player.h"
#include "server_game_player_ai.h"
#include "server_player.h"
#include "server_lobby_game.h"
#include "server_game.h"
#include "server.h"
#include "../Lib/net_events_lobby.h"
#include <algorithm>

ServerLobbyGameCollection::ServerLobbyGameCollection(Server *server)
: server(server), next_lobby_game_id(1), next_game_id(1)
{
}

ServerLobbyGameCollection::~ServerLobbyGameCollection()
{
}

void ServerLobbyGameCollection::remove_empty_games()
{
	std::vector<ServerLobbyGame *>::iterator it;
	for(it = lobby_games.begin(); it != lobby_games.end();)
	{
		ServerLobbyGame *lobby_game = (*it);
		if(lobby_game->get_player_collection()->get_count() == 0)
		{
			server->get_network_server()->send_event(NetGameEvent(STC_LOBBY_GAME_REMOVED, lobby_game->get_id()));

			it = lobby_games.erase(it);

			delete lobby_game;
		}
		else
		{
			++it;
		}
	}
}

ServerLobbyGame *ServerLobbyGameCollection::create_lobby_game(ServerLobbyPlayer *owner)
{
	if (owner->current_game)
	{
		owner->send_event(NetGameEvent(STC_LOBBY_ERROR_MESSAGE, "Cannot create game when already joined a game"));
		return 0;
	}

	// Create a new game
	ServerLobbyGame *lobby_game = new ServerLobbyGame(server, owner, next_lobby_game_id++);
	lobby_game->set_name(string_format("%1s game", owner->player->name));
	lobby_game->set_map_name("Island");
//	lobby_game->set_map_name("map1");
	lobby_game->set_max_players(6);

	lobby_games.push_back(lobby_game);

	owner->send_event(
		NetGameEvent(STC_LOBBY_GAME_CREATED, lobby_game->get_id(), lobby_game->get_name(), lobby_game->get_map_name(), lobby_game->get_max_players()));
	lobby_game->send_game_info();

	// Add lobby_player to game
	lobby_game->get_player_collection()->add_player(owner);

	return lobby_game;
}

ServerGame *ServerLobbyGameCollection::create_game(ServerLobbyGame *lobby_game)
{
	// Guard against exceptions between creation and putting it into collection
	std::auto_ptr<ServerGame> game(new ServerGame(server, next_game_id++, lobby_game->get_max_players()));

	game->set_map_name(lobby_game->get_map_name());

	// Add lobby players to game
	const std::vector<ServerLobbyPlayer *> &players = lobby_game->get_player_collection()->get_players();
	std::vector<ServerLobbyPlayer *>::const_iterator it;
	for(it = players.begin(); it != players.end(); ++it)
		game->add_player((*it)->player);

	running_games.push_back(game.get());

	return game.release();
}

void ServerLobbyGameCollection::send_available_lobby_games(ServerLobbyPlayer *destination_player)
{
	if(lobby_games.size() > 0)
	{
		for(unsigned i = 0; i < lobby_games.size(); ++i)
		{
			ServerLobbyGame *game = lobby_games[i];

			game->send_game_info(destination_player);

			game->get_player_collection()->send_available_players(destination_player);
		}
	}
	else
	{
		destination_player->send_event(NetGameEvent(STC_LOBBY_NO_GAMES_AVAILABLE));
	}
}

ServerLobbyGame *ServerLobbyGameCollection::get_lobby_game(int lobby_game_id)
{
	for(unsigned i = 0; i < lobby_games.size(); ++i)
	{
		if(lobby_games[i]->get_id() == lobby_game_id)
			return lobby_games[i];
	}
	return 0;
}
