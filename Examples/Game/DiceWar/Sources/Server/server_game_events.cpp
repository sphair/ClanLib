
#include "precomp.h"
#include "server.h"
#include "server_game.h"
#include "server_game_events.h"
#include "server_game_player.h"
#include "server_player.h"
#include "../Lib/net_events_game.h"

ServerGameEvents::ServerGameEvents(Server *server)
: server(server)
{
	game_events.func_event(CTS_GAME_ATTACK_AREA).set(this, &ServerGameEvents::on_event_attack_area);
	game_events.func_event(CTS_GAME_END_TURN).set(this, &ServerGameEvents::on_event_end_turn);
	game_events.func_event(CTS_GAME_BATTLE_VIEW_OVER).set(this, &ServerGameEvents::on_event_battle_view_over);
	game_events.func_event(CTS_GAME_ADD_MESSAGE).set(this, &ServerGameEvents::on_event_game_add_message);
}

ServerGameEvents::~ServerGameEvents()
{
}

void ServerGameEvents::on_event_attack_area(const NetGameEvent &e, ServerGame *game, ServerGamePlayer *game_player)
{
	int map_area_from_id = e.get_argument(0);
	int map_area_to_id = e.get_argument(1);

	game->attack_area(game_player, map_area_from_id, map_area_to_id);
}

void ServerGameEvents::on_event_end_turn(const NetGameEvent &e, ServerGame *game, ServerGamePlayer *game_player)
{
	game->end_turn(game_player);
}

void ServerGameEvents::on_event_battle_view_over(const NetGameEvent &e, ServerGame *game, ServerGamePlayer *game_player)
{
	game->battle_view_over(game_player);
}

void ServerGameEvents::on_event_game_add_message(const NetGameEvent &e, ServerGame *game, ServerGamePlayer *game_player)
{
	std::string message = e.get_argument(0);
	if(message.length() > 0)
		server->get_network_server()->send_event(NetGameEvent(STC_GAME_PLAYER_MESSAGE, game_player->player->id, message));
}
