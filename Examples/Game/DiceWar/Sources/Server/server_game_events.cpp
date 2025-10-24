
#include "precomp.h"
#include "server.h"
#include "server_game.h"
#include "server_game_events.h"
#include "server_game_player.h"
#include "server_player.h"

ServerGameEvents::ServerGameEvents(Server *server)
: server(server)
{
	game_events.func_event("game-attack-area").set(this, &ServerGameEvents::on_event_attack_area);
	game_events.func_event("game-end-turn").set(this, &ServerGameEvents::on_event_end_turn);
	game_events.func_event("game-battle-view-over").set(this, &ServerGameEvents::on_event_battle_view_over);
	game_events.func_event("game-add-message").set(this, &ServerGameEvents::on_event_game_add_message);
}

ServerGameEvents::~ServerGameEvents()
{
}

void ServerGameEvents::on_event_attack_area(const CL_NetGameEvent &e, ServerGame *game, ServerGamePlayer *game_player)
{
	int map_area_from_id = e.get_argument(0);
	int map_area_to_id = e.get_argument(1);

	game->attack_area(game_player, map_area_from_id, map_area_to_id);
}

void ServerGameEvents::on_event_end_turn(const CL_NetGameEvent &e, ServerGame *game, ServerGamePlayer *game_player)
{
	game->end_turn(game_player);
}

void ServerGameEvents::on_event_battle_view_over(const CL_NetGameEvent &e, ServerGame *game, ServerGamePlayer *game_player)
{
	game->battle_view_over(game_player);
}

void ServerGameEvents::on_event_game_add_message(const CL_NetGameEvent &e, ServerGame *game, ServerGamePlayer *game_player)
{
	CL_String message = e.get_argument(0);
	if(message.length() > 0)
		server->get_network_server()->send_event(CL_NetGameEvent("game-player-message", game_player->player->id, message));
}
