
#pragma once

#include "../Lib/map.h"

class ServerGamePlayerCollection;
class ServerGamePlayer;
class ServerGamePlayerAI;
class ServerPlayer;
class Server;

class ServerGame
{
public:
	ServerGame(Server *server, int id, int max_players);
	~ServerGame();

	void add_player(ServerPlayer *player);
	void remove_player(ServerGamePlayer *player);

	void set_map_name(std::string map_name) { this->map_name = map_name; }

	bool start();

	int get_id() const { return id; }
	
	Map *get_map() { return &map; }
	
	void attack_area(ServerGamePlayer *player, int map_area_from_id, int map_area_to_id);
	void end_turn(ServerGamePlayer *player);

	void battle_view_over(ServerGamePlayer *player);

private:
	void create_ai_players(int ai_player_count);
	bool initialize_map(std::string map_name);
	void transfer_map();
	void start_first_turn();
	void calculate_reinforcements();
	void place_reinforcements(int reinforcement_count, ServerGamePlayer* game_player, bool send_events);
	bool check_for_winner();

	Server *server;

	std::auto_ptr<ServerGamePlayerCollection> players;

	ServerGamePlayer *active_player;

	std::vector<ServerPlayer *> players_ai;

	Map map;
	std::string map_name;

	int id;

	int max_players;
	int max_turns;
	int current_turn;
};
