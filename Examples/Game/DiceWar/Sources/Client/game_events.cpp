
#include "precomp.h"
#include "game_view.h"
#include "game_events.h"
#include "game_player.h"
#include "game_player_collection.h"
#include "client.h"
#include "game_model.h"
#include "../Lib/map_area.h"
#include "../Lib/net_events_game.h"

GameEvents::GameEvents(Client *client)
: client(client)
{
	game_events.func_event(STC_GAME_IS_STARTING).set(this, &GameEvents::on_event_game_is_starting);
	game_events.func_event(STC_GAME_LOAD_MAP).set(this, &GameEvents::on_event_game_load_map);
	game_events.func_event(STC_GAME_PLAYER_JOINED_GAME).set(this, &GameEvents::on_event_player_joined_game);
	game_events.func_event(STC_GAME_PLAYER_LEFT_GAME).set(this, &GameEvents::on_event_player_left_game);
	game_events.func_event(STC_GAME_SET_MAPAREA_OWNERSHIP).set(this, &GameEvents::on_event_set_maparea_ownership);
	game_events.func_event(STC_GAME_SET_MAPAREA_ARMY_STRENGTH).set(this, &GameEvents::on_event_set_maparea_army_strength);
	game_events.func_event(STC_GAME_HAS_STARTED).set(this, &GameEvents::on_event_game_has_started);
	game_events.func_event(STC_GAME_INVALID_ATTACK).set(this, &GameEvents::on_event_invalid_attack);
	game_events.func_event(STC_GAME_ATTACKED_AREA).set(this, &GameEvents::on_event_attacked_area);
	game_events.func_event(STC_GAME_ATTACK_DICE_RESULT).set(this, &GameEvents::on_event_attack_dice_result);
	game_events.func_event(STC_GAME_DEFENSE_DICE_RESULT).set(this, &GameEvents::on_event_defense_dice_result);
	game_events.func_event(STC_GAME_PLAYER_TURN_STARTED).set(this, &GameEvents::on_event_player_turn_started);
	game_events.func_event(STC_GAME_OVER).set(this, &GameEvents::on_event_game_over);
	game_events.func_event(STC_GAME_ATTACK_DONE).set(this, &GameEvents::on_event_game_attack_done);
	game_events.func_event(STC_GAME_PLAYER_MESSAGE).set(this, &GameEvents::on_event_game_player_message);
	game_events.func_event(STC_GAME_SYSTEM_MESSAGE).set(this, &GameEvents::on_event_game_system_message);
}

GameEvents::~GameEvents()
{
}

void GameEvents::on_event_game_load_map(const NetGameEvent &e)
{
	std::string mapname = e.get_argument(0);

	client->load_map(mapname);
}

void GameEvents::on_event_player_joined_game(const NetGameEvent &e)
{
	int player_id = e.get_argument(0);
	std::string player_name = e.get_argument(1);
	int visual_id = e.get_argument(2);

	GamePlayer *player = client->get_game_model()->get_players()->create_player(player_name, player_id, visual_id);
	if(player)
		client->get_game_view()->player_joined_game(player);
	else
		cl_log_event("error", "Invalid event: %1", e.to_string());
}

void GameEvents::on_event_player_left_game(const NetGameEvent &e)
{
	int player_id = e.get_argument(0);
	
	GamePlayer *player = client->get_game_model()->get_players()->get_player(player_id);
	if(player)
	{
		client->get_game_view()->player_left_game(player);

		client->get_game_model()->get_players()->remove_player(player);
	}
	else
		cl_log_event("error", "Invalid event: %1", e.to_string());
}

void GameEvents::on_event_set_maparea_ownership(const NetGameEvent &e)
{
	int map_area_id = e.get_argument(0);
	int player_id = e.get_argument(1);

	MapArea *map_area = client->get_game_model()->get_map()->get_area_by_id(map_area_id);
	GamePlayer *player = client->get_game_model()->get_players()->get_player(player_id);

	if(map_area != 0 && player != 0)
	{
		map_area->player_id = player->id;
		client->get_game_view()->update_map();
	}
	else
		cl_log_event("error", "Invalid event: %1", e.to_string());
}

void GameEvents::on_event_set_maparea_army_strength(const NetGameEvent &e)
{
	int map_area_id = e.get_argument(0);
	int army_strength = e.get_argument(1);

	MapArea *map_area = client->get_game_model()->get_map()->get_area_by_id(map_area_id);

	if(map_area != 0 && army_strength >= 1 && army_strength <= 8)
		map_area->army_strength = army_strength;
	else
		cl_log_event("error", "Invalid event: %1", e.to_string());
}

void GameEvents::on_event_game_is_starting(const NetGameEvent &e)
{
	client->create_game();
}

void GameEvents::on_event_game_has_started(const NetGameEvent &e)
{
	if (client->get_game_model()->get_map()->validate())
	{
		client->get_game_view()->start_game();
	}
	else
	{
		// TODO: Show an error to user
		cl_log_event("error", "Unable to start game: Map is invalid", e.to_string());
	}
}

void GameEvents::on_event_invalid_attack(const NetGameEvent &e)
{
	client->get_game_view()->invalid_attack();
}

void GameEvents::on_event_attacked_area(const NetGameEvent &e)
{
	int map_area_from_id = e.get_argument(0);
	int map_area_to_id = e.get_argument(1);
	client->get_game_view()->attacked_area(map_area_from_id, map_area_to_id);
}

void GameEvents::on_event_attack_dice_result(const NetGameEvent &e)
{
	std::vector<int> dice_results;
	dice_results.reserve(8);
	for (int i = 0; i < 8; i++)
	{
		int result = e.get_argument(i);
		if (result == 0)
			break;
		dice_results.push_back(result);
	}
	client->get_game_view()->attack_dice_result(dice_results);
}

void GameEvents::on_event_defense_dice_result(const NetGameEvent &e)
{
	std::vector<int> dice_results;
	dice_results.reserve(8);
	for (int i = 0; i < 8; i++)
	{
		int result = e.get_argument(i);
		if (result == 0)
			break;
		dice_results.push_back(result);
	}
	client->get_game_view()->defense_dice_result(dice_results);
}

void GameEvents::on_event_game_attack_done(const NetGameEvent &e)
{
	// TODO: Respect this and only allow attacks after this has been received
}

void GameEvents::on_event_player_turn_started(const NetGameEvent &e)
{
	int player_id = e.get_argument(0);

	if(player_id == client->get_player_id())
		client->get_game_view()->set_active_turn();
	else
		client->get_game_view()->set_waiting_turn();
}

void GameEvents::on_event_game_over(const NetGameEvent &e)
{
	std::string title = e.get_argument(0);
	std::string message = e.get_argument(1);

	client->get_game_view()->game_over(title, message);
}

void GameEvents::on_event_game_player_message(const NetGameEvent &e)
{
	int player_id = e.get_argument(0);
	std::string message = e.get_argument(1);

	GamePlayer *player = client->get_game_model()->get_players()->get_player(player_id);
	if(player)
	{
		client->get_game_view()->add_player_message(player, message);
	}
}

void GameEvents::on_event_game_system_message(const NetGameEvent &e)
{
	std::string message = e.get_argument(0);

	client->get_game_view()->add_system_message(message);
}
