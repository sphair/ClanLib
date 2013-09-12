
#include "precomp.h"
#include "server_lobby_player.h"
#include "server_lobby_game.h"
#include "server_lobby_net_events.h"
#include "server_lobby_player_information.h"

ServerLobbyPlayer::ServerLobbyPlayer(clan::NetGameServer *server, clan::NetGameConnection *connection, int id, ServerLobbyPlayerInformation *extra_information)
: server(server), connection(connection), id(id), extra_information(extra_information), current_game(0)
{
}

void ServerLobbyPlayer::send_event(const clan::NetGameEvent &game_event)
{
	connection->send_event(game_event);
}

void ServerLobbyPlayer::send_player_info_to_all()
{
	clan::NetGameEvent event(STC_LOBBY_PLAYER_LOGGED_IN, id);

	if(extra_information)
		extra_information->append_network_game_info(event);

	server->send_event(event);
}

void ServerLobbyPlayer::send_player_info(ServerLobbyPlayer *destination_player)
{
	clan::NetGameEvent event(STC_LOBBY_PLAYER_LOGGED_IN, id);

	if(extra_information)
		extra_information->append_network_game_info(event);

	destination_player->send_event(event);
}

void ServerLobbyPlayer::set_current_game(ServerLobbyGame *game)
{
	current_game = game;

	if(current_game)
		current_game->add_player(this);
}

void ServerLobbyPlayer::leave_current_game()
{
	if(current_game)
		current_game->remove_player(this);

	current_game = 0;
}
