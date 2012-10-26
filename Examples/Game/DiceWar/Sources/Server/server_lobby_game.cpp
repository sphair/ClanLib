
#include "precomp.h"
#include "server_lobby_game.h"
#include "server.h"
#include "server_lobby_game_player_collection.h"
#include "server_lobby_player.h"
#include "server_player.h"
#include "../Lib/net_events_lobby.h"

ServerLobbyGame::ServerLobbyGame(Server *server, ServerLobbyPlayer *owner, int id)
: server(server), id(id), max_players(6), state(lobby)
{
	player_collection.reset(new ServerLobbyGamePlayerCollection(server, this, owner));
}

ServerLobbyGame::~ServerLobbyGame()
{
}

void ServerLobbyGame::set_max_players(int max_players)
{
	if(max_players >= 1 && max_players <= 6)
		this->max_players = max_players;
}

void ServerLobbyGame::set_state(State state)
{
	this->state = state;

	if(state == playing)
		server->get_network_server()->send_event(NetGameEvent(STC_LOBBY_GAME_STARTED, id));
}

void ServerLobbyGame::send_game_info()
{
	server->get_network_server()->send_event(
		NetGameEvent(STC_LOBBY_GAME_INFO, get_id(), get_name(), get_map_name(), get_max_players(), get_state()));
}

void ServerLobbyGame::send_game_info(ServerLobbyPlayer *destination_player)
{
	destination_player->send_event(
		NetGameEvent(STC_LOBBY_GAME_INFO, get_id(), get_name(), get_map_name(), get_max_players(), get_state()));
}
