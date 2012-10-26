
#include "precomp.h"
#include "server_lobby_game.h"
#include "server_lobby_game_player_collection.h"
#include "server_lobby_player.h"
#include "server_lobby_net_events.h"
#include "server_lobby_game_information.h"

ServerLobbyGame::ServerLobbyGame(CL_NetGameServer *server, ServerLobbyPlayer *owner, int id, ServerLobbyGameInformation *extra_information)
: server(server), id(id), state(lobby), extra_information(extra_information)
{
	player_collection.reset(new ServerLobbyGamePlayerCollection(server, this, owner));
}

void ServerLobbyGame::set_state(State state)
{
	this->state = state;

	if(state == playing)
	{
		server->send_event(CL_NetGameEvent(STC_LOBBY_GAME_STARTED, id));
	}
}

void ServerLobbyGame::send_game_info_to_all()
{
	CL_NetGameEvent event(STC_LOBBY_GAME_INFO, get_id(), get_state());

	if(extra_information)
		extra_information->append_network_game_info(event);

	server->send_event(event);
}

void ServerLobbyGame::send_game_info(ServerLobbyPlayer *destination_player)
{
	CL_NetGameEvent event(STC_LOBBY_GAME_INFO, get_id(), get_state());

	if(extra_information)
		extra_information->append_network_game_info(event);

	destination_player->send_event(event);
}

void ServerLobbyGame::add_player(ServerLobbyPlayer *lobby_player)
{
	player_collection->add_player(lobby_player);
}

void ServerLobbyGame::remove_player(ServerLobbyPlayer *lobby_player)
{
	player_collection->remove_player(lobby_player);
}

void ServerLobbyGame::send_available_players(ServerLobbyPlayer *destination_player)
{
	player_collection->send_available_players(destination_player);
}

bool ServerLobbyGame::has_players() const
{
	return player_collection->get_count() > 0;
}

ServerLobbyPlayer *ServerLobbyGame::get_owner() const
{
	return player_collection->get_owner();
}

void ServerLobbyGame::modify(const std::vector<CL_NetGameEventValue> &extra_arguments)
{
	if(extra_information)
		extra_information->apply_network_game_info(extra_arguments);

	send_game_info_to_all();
}
