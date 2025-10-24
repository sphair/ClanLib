
#pragma once

class Client;

class GameEvents
{
public:
	GameEvents(Client *client);
	~GameEvents();

	CL_NetGameEventDispatcher_v0 &get_dispatcher() { return game_events; }

private:
	void on_event_game_load_map(const CL_NetGameEvent &e);
	void on_event_player_joined_game(const CL_NetGameEvent &e);
	void on_event_player_left_game(const CL_NetGameEvent &e);
	void on_event_set_maparea_ownership(const CL_NetGameEvent &e);
	void on_event_set_maparea_army_strength(const CL_NetGameEvent &e);
	void on_event_game_is_starting(const CL_NetGameEvent &e);
	void on_event_game_has_started(const CL_NetGameEvent &e);
	void on_event_invalid_attack(const CL_NetGameEvent &e);
	void on_event_attacked_area(const CL_NetGameEvent &e);
	void on_event_attack_dice_result(const CL_NetGameEvent &e);
	void on_event_defense_dice_result(const CL_NetGameEvent &e);
	void on_event_game_attack_done(const CL_NetGameEvent &e);
	void on_event_player_turn_started(const CL_NetGameEvent &e);
	void on_event_game_over(const CL_NetGameEvent &e);
	void on_event_game_player_message(const CL_NetGameEvent &e);
	void on_event_game_system_message(const CL_NetGameEvent &e);

	CL_NetGameEventDispatcher_v0 game_events;

	Client *client;
};
