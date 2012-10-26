
#pragma once

class Client;

class GameEvents
{
public:
	GameEvents(Client *client);
	~GameEvents();

	NetGameEventDispatcher_v0 &get_dispatcher() { return game_events; }

private:
	void on_event_game_load_map(const NetGameEvent &e);
	void on_event_player_joined_game(const NetGameEvent &e);
	void on_event_player_left_game(const NetGameEvent &e);
	void on_event_set_maparea_ownership(const NetGameEvent &e);
	void on_event_set_maparea_army_strength(const NetGameEvent &e);
	void on_event_game_is_starting(const NetGameEvent &e);
	void on_event_game_has_started(const NetGameEvent &e);
	void on_event_invalid_attack(const NetGameEvent &e);
	void on_event_attacked_area(const NetGameEvent &e);
	void on_event_attack_dice_result(const NetGameEvent &e);
	void on_event_defense_dice_result(const NetGameEvent &e);
	void on_event_game_attack_done(const NetGameEvent &e);
	void on_event_player_turn_started(const NetGameEvent &e);
	void on_event_game_over(const NetGameEvent &e);
	void on_event_game_player_message(const NetGameEvent &e);
	void on_event_game_system_message(const NetGameEvent &e);

	NetGameEventDispatcher_v0 game_events;

	Client *client;
};
