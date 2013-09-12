
#include "precomp.h"
#include "server_lobby_game.h"
#include "server_lobby_game_collection.h"
#include "server_lobby_game.h"
#include "server_lobby_game_player_collection.h"
#include "server_lobby_player.h"
#include "server_lobby_net_events.h"
#include <algorithm>

ServerLobbyGameCollection::ServerLobbyGameCollection(clan::NetGameServer *server)
: server(server), next_lobby_game_id(1)
{
}

void ServerLobbyGameCollection::remove_empty_games()
{
	std::vector<ServerLobbyGame *>::iterator it;
	for(it = lobby_games.begin(); it != lobby_games.end();)
	{
		ServerLobbyGame *lobby_game = (*it);
		if(lobby_game->has_players() == false)
		{
			server->send_event(clan::NetGameEvent(STC_LOBBY_GAME_REMOVED, lobby_game->get_id()));

			it = lobby_games.erase(it);

			delete lobby_game;
		}
		else
		{
			++it;
		}
	}
}

ServerLobbyGame *ServerLobbyGameCollection::create_lobby_game(ServerLobbyPlayer *owner, clan::Callback_1<ServerLobbyGameInformation *, clan::NetGameConnection *> &func_create_lobby_game)
{
	ServerLobbyGameInformation *lobby_game_extra_information = 0;
	if(func_create_lobby_game.is_null())
		lobby_game_extra_information = func_create_lobby_game.invoke(owner->get_connection());

	ServerLobbyGame *lobby_game = new ServerLobbyGame(server, owner, next_lobby_game_id++, lobby_game_extra_information);

	lobby_game->add_player(owner);
	lobby_games.push_back(lobby_game);

	lobby_game->send_game_info_to_all();

	return lobby_game;
}

void ServerLobbyGameCollection::send_available_lobby_games(ServerLobbyPlayer *destination_player)
{
	if(lobby_games.size() > 0)
	{
		for(unsigned i = 0; i < lobby_games.size(); ++i)
		{
			ServerLobbyGame *game = lobby_games[i];
			game->send_game_info(destination_player);
			game->send_available_players(destination_player);
		}
	}
	else
	{
		destination_player->send_event(clan::NetGameEvent(STC_LOBBY_NO_GAMES_AVAILABLE));
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
