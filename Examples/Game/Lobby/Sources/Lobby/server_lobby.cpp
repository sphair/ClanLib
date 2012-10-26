
#include "precomp.h"
#include "server_lobby.h"
#include "server_lobby_player.h"
#include "server_lobby_events.h"
#include "server_lobby_game.h"
#include "server_lobby_net_events.h"

ServerLobby::ServerLobby(CL_NetGameServer *server)
: players(server), games(server), server(server)
{
	lobby_events.reset(new ServerLobbyEvents(server, this, &games));
}

bool ServerLobby::dispatch_event(const CL_NetGameEvent &e, int player_id)
{
	bool handled_event = false;

	ServerLobbyPlayer *lobby_player = players.get_player(player_id);
	if (lobby_player)
		handled_event = lobby_events->get_dispatcher().dispatch(e, lobby_player);

	return handled_event;
}

void ServerLobby::add_player(int player_id, CL_NetGameConnection *connection)
{
	players.create_player(player_id, connection, func_create_lobby_player);
}

void ServerLobby::remove_player(int player_id)
{
	cl_log_event("system", "Client disconnected");

	ServerLobbyPlayer *lobby_player = players.get_player(player_id);

	if(lobby_player)
	{
		players.remove_player(lobby_player);
		games.remove_empty_games();
	}
}
