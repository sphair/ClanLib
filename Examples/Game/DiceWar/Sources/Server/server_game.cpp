
#include "precomp.h"
#include "server.h"
#include "server_player.h"
#include "server_game.h"
#include "server_game_player_collection.h"
#include "server_game_player.h"
#include "server_game_player_ai.h"
#include "../Lib/map_area.h"
#include "../Lib/net_events_game.h"
#include <algorithm>

ServerGame::ServerGame(Server *server, int id, int max_players)
: server(server), active_player(0), id(id), current_turn(0), max_turns(20), max_players(max_players)
{
	players.reset(new ServerGamePlayerCollection(server, this));
}

ServerGame::~ServerGame()
{
	std::vector<ServerPlayer *>::iterator it;
	for (it = players_ai.begin(); it != players_ai.end(); ++it)
		delete (*it);
}

void ServerGame::add_player(ServerPlayer *player)
{
	players->add_player(new ServerGamePlayer(player));
}

void ServerGame::remove_player(ServerGamePlayer *player)
{
	if (active_player == player && players->get_count() > 1)
		end_turn(player);

	players->remove_player(player);
}

bool ServerGame::start()
{
	if(players->get_count() == 0)
		return false;

	// Create dummy/AI players if less than max players
	if(players->get_count() < max_players)
	{
		create_ai_players(max_players - players->get_count());
	}

	if(initialize_map(map_name))
	{
		players->send_event(NetGameEvent(STC_GAME_IS_STARTING));
		players->transfer_players();
		transfer_map();
		start_first_turn();

		return true;
	}

	return false;
}

void ServerGame::create_ai_players(int ai_player_count)
{
	for(int i = 0; i < ai_player_count; ++i)
	{
		// TODO: This will blow up if max_players > 6
		std::string names[] = {"Sam", "Max", "Tom", "Bob", "Sue", "Ron"};
		ServerPlayer *player_ai = server->create_ai_player(names[i]);

		players_ai.push_back(player_ai);
		players->add_player(new ServerGamePlayerAI(server, this, player_ai));
	}
}

bool ServerGame::initialize_map(std::string map_name)
{
	std::string filename = string_format("Resources/Maps/%1/map.xml", map_name);
	map.load(map_name, filename);

	// Randomize which player we start with
	std::vector<ServerGamePlayer *>::iterator it_player = players->get_players().begin();
	int start_player_random = rand() % players->get_players().size();
	while(start_player_random--)
		++it_player;

	// Assign areas to players
	for(unsigned int i = 0; i < map.areas_size(); ++i)
	{
		MapArea *map_area = map.get_area(i);
		map_area->player_id = (*it_player)->player->id;
		map_area->army_strength = 1;

		if(++it_player == players->get_players().end())
			it_player = players->get_players().begin();
	}

	// Assign armies to players
	for(it_player = players->get_players().begin(); it_player != players->get_players().end(); ++it_player)
	{
		int reinforcement_count = (map.areas_size() / players->get_count()) * 2;
		place_reinforcements(reinforcement_count, (*it_player), false);
	}

	// Make sure we got a valid map
	return map.validate();
}

void ServerGame::transfer_map()
{
	players->send_event(NetGameEvent(STC_GAME_LOAD_MAP, map.get_mapname()));

	// Send out map info to clients
	for(unsigned int i = 0; i < map.areas_size(); ++i)
	{
		MapArea *map_area = map.get_area(i);

		players->send_event(NetGameEvent(STC_GAME_SET_MAPAREA_OWNERSHIP, map_area->id, map_area->player_id));
		players->send_event(NetGameEvent(STC_GAME_SET_MAPAREA_ARMY_STRENGTH, map_area->id, map_area->army_strength));
	}
}

void ServerGame::attack_area(ServerGamePlayer *game_player, int map_area_from_id, int map_area_to_id)
{
	if (active_player == game_player)
	{
		MapArea *attack_area = map.get_area_by_id(map_area_from_id);
		MapArea *defense_area = map.get_area_by_id(map_area_to_id);
	
		if (map.is_valid_attack(active_player->player->id, attack_area, defense_area))
		{
			// Set all players to battle mode
			std::vector<ServerGamePlayer *>::iterator it;
			for(it = players->get_players().begin(); it != players->get_players().end(); ++it)
			{
				ServerGamePlayer *server_game_player = (*it);
				server_game_player->in_battle_mode = true;
			}

			// Send attacked area network event
			players->send_event(NetGameEvent(STC_GAME_ATTACKED_AREA, attack_area->id, defense_area->id));

			// Calculate attack dices
			std::vector<int> dice_attack_result(8, 0);
			int attack_strength = 0;
			for(int i = 0; i < attack_area->army_strength; ++i)
			{
				dice_attack_result[i] = rand() % 6 + 1;
				attack_strength += dice_attack_result[i];
			}

			// Send attack dice result network event
			NetGameEvent attack_event(STC_GAME_ATTACK_DICE_RESULT); 
			for(int i = 0; i < 8; ++i)
				attack_event.add_argument(dice_attack_result[i]);
			players->send_event(attack_event);

			// Calculate defense dices
			std::vector<int> dice_defense_result(8, 0);
			int defense_strength = 0;
			for(int i = 0; i < defense_area->army_strength; ++i)
			{
				dice_defense_result[i] = rand() % 6 + 1;
				defense_strength += dice_defense_result[i];
			}

			// Send defense dice result network event
			NetGameEvent defense_event(STC_GAME_DEFENSE_DICE_RESULT); 
			for(int i = 0; i < 8; ++i)
				defense_event.add_argument(dice_defense_result[i]);
			players->send_event(defense_event);

			// Check battle victory
			if(attack_strength >= defense_strength) // Attacker won
			{
				// Claim area
				defense_area->player_id = attack_area->player_id;

				players->send_event(NetGameEvent(STC_GAME_SET_MAPAREA_OWNERSHIP, defense_area->id, attack_area->player_id));

				// Move troops
				defense_area->army_strength = attack_area->army_strength - 1;
				attack_area->army_strength = 1;

				players->send_event(NetGameEvent(STC_GAME_SET_MAPAREA_ARMY_STRENGTH, defense_area->id, defense_area->army_strength));
				players->send_event(NetGameEvent(STC_GAME_SET_MAPAREA_ARMY_STRENGTH, attack_area->id, attack_area->army_strength));
			}
			else // Defender won
			{
				// Reduce attacker to bare minimum
				attack_area->army_strength = 1;

				players->send_event(NetGameEvent(STC_GAME_SET_MAPAREA_ARMY_STRENGTH, attack_area->id, attack_area->army_strength));
			}
		}
		else
		{
			game_player->send_event(NetGameEvent(STC_GAME_INVALID_ATTACK));
		}
	}
}

void ServerGame::battle_view_over(ServerGamePlayer *player)
{
	player->in_battle_mode = false;

	// Set all players to battle mode
	std::vector<ServerGamePlayer *>::iterator it;
	for(it = players->get_players().begin(); it != players->get_players().end(); ++it)
	{
		ServerGamePlayer *server_game_player = (*it);
		if(server_game_player->in_battle_mode == true)
			return;
	}

	active_player->send_event(NetGameEvent(STC_GAME_ATTACK_DONE));
}

void ServerGame::start_first_turn()
{
	players->send_event(NetGameEvent(STC_GAME_HAS_STARTED));

	active_player = players->get_players().front();
	players->send_event(NetGameEvent(STC_GAME_PLAYER_TURN_STARTED, active_player->player->id));
}

void ServerGame::end_turn(ServerGamePlayer *game_player)
{
	if(active_player == game_player)
	{
		if(check_for_winner() == false)
		{
			std::vector<ServerGamePlayer *>::iterator it = std::find(players->get_players().begin(), players->get_players().end(), active_player);
			if (it != players->get_players().end())
			{
				it++;

				bool full_round_completed = false;

				if (it == players->get_players().end())
				{
					current_turn++;

					it = players->get_players().begin();
					if(it == players->get_players().begin())
						full_round_completed = true;
				}

				if(full_round_completed && current_turn == max_turns)
				{
					active_player = 0;
					players->send_event(NetGameEvent(STC_GAME_OVER, "No winner", string_format("You have played %1 turns.", max_turns)));
				}
				else
				{
					if(full_round_completed)
					{
						calculate_reinforcements();
					}

					active_player = *it;
					players->send_event(NetGameEvent(STC_GAME_PLAYER_TURN_STARTED, active_player->player->id));
				}
			}
			else
			{
				active_player = 0;
			}
		}
	}
}

// Each player get one army per area owned. The reinforcement are randomly placed on owned areas.
// TODO: Both the algorithm and implementation of this sucks :)
void ServerGame::calculate_reinforcements()
{
	std::vector<ServerGamePlayer *>::iterator it;
	for(it = players->get_players().begin(); it != players->get_players().end(); ++it)
	{
		ServerGamePlayer *game_player = (*it);

		// Calculate how many reinforcements this player will get
		int reinforcement_count = 0;
		int areas_owned = 0;
		for(unsigned int i = 0; i < map.areas_size(); ++i)
		{
			if(map.get_area(i)->player_id == game_player->player->id)
			{
				reinforcement_count++;
				areas_owned++;
			}
		}

		if(areas_owned > 0)
		{
			place_reinforcements(reinforcement_count, game_player, true);
		}
	}
}

void ServerGame::place_reinforcements(int reinforcement_count, ServerGamePlayer *game_player, bool send_events)
{
	while(reinforcement_count > 0)
	{
		MapArea *map_area = map.get_area(rand() % map.areas_size());
		if(map_area->player_id == game_player->player->id)
		{
			// Increase an areas army if less than 8 in strength
			if(map_area->army_strength < 8)
			{
				map_area->army_strength = map_area->army_strength++;

				reinforcement_count--;

				if(send_events)
				{
					players->send_event(NetGameEvent(STC_GAME_SET_MAPAREA_ARMY_STRENGTH, map_area->id, map_area->army_strength));

					log_event("gameplay", "Reinforcing area %1 for player %2", map_area->id, game_player->player->name);
				}
			}
			else
			{
				// Otherwise, check if *all* areas are full
				int total_army_strength = 0;
				int areas_owned = 0;
				for(unsigned int i = 0; i < map.areas_size(); ++i)
				{
					if(map.get_area(i)->player_id == game_player->player->id)
					{
						areas_owned++;
						total_army_strength += map.get_area(i)->army_strength;
					}
				}

				// Player is all full, so clear rest of reinforcements
				if(total_army_strength == areas_owned * 8)
					reinforcement_count = 0;
			}
		}
	}	
}

// Checks if a player owns the entire map.
// Returns true if we have a winner, false otherwise.
bool ServerGame::check_for_winner()
{
	// TODO: This check can be moved into Map class
	int player_id = -1;
	for(unsigned int i = 0; i < map.areas_size(); ++i)
	{
		MapArea *area = map.get_area(i);

		if(player_id == -1)
		{
			player_id = area->player_id;
		}
		else
		{
			if(area->player_id != player_id)
				return false;
		}
	}

	// We have a winner!
	active_player = 0;
	ServerGamePlayer *server_player = players->get_player(player_id);
	players->send_event(NetGameEvent(STC_GAME_OVER, "Winner!", string_format("%1 won!", server_player->player->name)));

	return true;
}
