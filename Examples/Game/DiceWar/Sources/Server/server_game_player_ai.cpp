
#include "precomp.h"
#include "server_game_player_ai.h"
#include "server_player.h"
#include "server.h"
#include "server_game.h"
#include "../Lib/map.h"
#include "../Lib/map_area.h"
#include "../Lib/net_events_game.h"

#include <stdlib.h>	// For rand()

// TODO: How to properly send an event to the "server" ?
// Currently the AI player just routes event into server->handle_event, which is a function who shouldn't be public!

ServerGamePlayerAI::ServerGamePlayerAI(Server *server, ServerGame *game, ServerPlayer *player)
: ServerGamePlayer(player), server(server), game(game)
{
	game_events.func_event(STC_GAME_PLAYER_TURN_STARTED).set(this, &ServerGamePlayerAI::on_event_player_turn_started);
	game_events.func_event(STC_GAME_ATTACKED_AREA).set(this, &ServerGamePlayerAI::on_event_attacked_area);
	game_events.func_event(STC_GAME_ATTACK_DONE).set(this, &ServerGamePlayerAI::on_event_attack_done);
	game_events.func_event(STC_GAME_INVALID_ATTACK).set(this, &ServerGamePlayerAI::on_event_invalid_attack);
}

// This function is called whenever we are supposed to send an event to this player.
// Instead of sending it physically over a network, we just dispatch it directly to ourself.
void ServerGamePlayerAI::send_event(const NetGameEvent &game_event)
{
	cl_log_event("AI", "AI received event: %1", game_event.to_string());

	game_events.dispatch(game_event);
}

void ServerGamePlayerAI::on_event_player_turn_started(const NetGameEvent &e)
{
	int player_id = e.get_argument(0);

	// My turn! Try to take over the world!
	if(player_id == player->id)
	{
		perform_next_attack();
	}
}

void ServerGamePlayerAI::on_event_attacked_area(const NetGameEvent &e)
{
	// Notify server we are finished with our game battle 
	// (we don't have a view so we're instantly finished)
	cl_log_event("AI", "AI sent event: %1", NetGameEvent(CTS_GAME_BATTLE_VIEW_OVER).to_string());
	server->handle_event(player, NetGameEvent(CTS_GAME_BATTLE_VIEW_OVER));
}

void ServerGamePlayerAI::on_event_invalid_attack(const NetGameEvent &e)
{
	// Oops, we screwed up something with our planning - lets bail out
	cl_log_event("AI", "AI sent event: %1", NetGameEvent(CTS_GAME_END_TURN).to_string());
	server->handle_event(player, NetGameEvent(CTS_GAME_END_TURN));
}

void ServerGamePlayerAI::on_event_attack_done(const NetGameEvent &e)
{
	perform_next_attack();
}

// This implements some really crappy AI for deciding which areas to attack	
void ServerGamePlayerAI::perform_next_attack()
{
	Map *map = game->get_map();

	for(unsigned int i = 0; i < map->areas_size(); ++i)
	{
		MapArea *area = map->get_area(i);

		// Check if I own this area
		if(area->player_id == player->id)
		{
			std::vector<MapArea *>::iterator it;
			for(it = area->connecting_areas.begin(); it != area->connecting_areas.end(); ++it)
			{
				MapArea *target_area = (*it);

				bool should_attack = false;

				if(target_area->player_id != player->id)
				{
					if(target_area->army_strength < area->army_strength)
					{
						should_attack = true;
					}
					else
					{
						if(target_area->army_strength == area->army_strength && rand() % 4 == 0)
						{
							should_attack = true;
						}
						else
						{
							if(rand() % 10 == 0)
								should_attack = true;
						}
					}

					if(should_attack)
					{
						cl_log_event("AI", "AI sent event: %1", NetGameEvent(CTS_GAME_ATTACK_AREA, area->id, target_area->id).to_string());
						server->handle_event(player, NetGameEvent(CTS_GAME_ATTACK_AREA, area->id, target_area->id));
						return;
					}
				}
			}
		}
	}
	
	cl_log_event("AI", "AI sent event: %1", NetGameEvent(CTS_GAME_END_TURN).to_string());
	server->handle_event(player, NetGameEvent(CTS_GAME_END_TURN));
}
