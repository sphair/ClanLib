
#include "precomp.h"
#include "server_lobby_player_collection.h"
#include "server_lobby_player.h"
#include "server_lobby_game.h"
#include "server_lobby_net_events.h"
#include <algorithm>

ServerLobbyPlayerCollection::ServerLobbyPlayerCollection(CL_NetGameServer *server)
: server(server)
{
}

ServerLobbyPlayer *ServerLobbyPlayerCollection::get_player(int player_id)
{
	for(unsigned i = 0; i < players.size(); ++i)
	{
		if(players[i]->get_id() == player_id)
			return players[i];
	}
	return 0;
}

ServerLobbyPlayer *ServerLobbyPlayerCollection::create_player(int player_id, CL_NetGameConnection *connection, CL_Callback_1<ServerLobbyPlayerInformation *, CL_NetGameConnection *> &func_create_lobby_player)
{
	ServerLobbyPlayerInformation *lobby_player_extra_information = 0;
	if(func_create_lobby_player.is_null())
		lobby_player_extra_information = func_create_lobby_player.invoke(connection);

	ServerLobbyPlayer *lobby_player = new ServerLobbyPlayer(server, connection, player_id, lobby_player_extra_information);
	players.push_back(lobby_player);

	lobby_player->send_player_info_to_all();

	send_available_players(lobby_player);

	return lobby_player;
}

void ServerLobbyPlayerCollection::send_available_players(ServerLobbyPlayer *destination_player)
{
	for(unsigned i = 0; i < players.size(); ++i)
	{
		if(players[i]->get_id() != destination_player->get_id())		
		{
			players[i]->send_player_info(destination_player);
		}
	}
}

void ServerLobbyPlayerCollection::remove_player(ServerLobbyPlayer *lobby_player)
{
	if(lobby_player)
	{
		// Remove player from lobby game he has joined
		if (lobby_player->get_current_game())
			lobby_player->get_current_game()->remove_player(lobby_player);

		// Remove player from player list
		std::vector<ServerLobbyPlayer *>::iterator it;
		it = find(players.begin(), players.end(), lobby_player); 
		if (it != players.end())
			players.erase(it);

		server->send_event(CL_NetGameEvent(STC_LOBBY_PLAYER_LOGGED_OUT, lobby_player->get_id()));

		delete lobby_player;
	}
}

void ServerLobbyPlayerCollection::send_event(const CL_NetGameEvent &game_event)
{
	std::vector<ServerLobbyPlayer *>::iterator it;
	for (it = players.begin(); it != players.end(); ++it)
	{
		ServerLobbyPlayer *lobby_player = (*it);

		lobby_player->send_event(game_event);
	}
}
