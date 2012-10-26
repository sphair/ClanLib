
#pragma once

class LobbyPlayer;

class LobbyGame
{
public:
	enum State
	{
		lobby,
		playing
	};

public:
	LobbyGame(int id, std::string name, std::string map_name, int max_players, State state);
	~LobbyGame();

	State get_state() { return state; }
	void set_state(State state) { this->state = state; }

	void add_player(LobbyPlayer *player);
	void remove_player(LobbyPlayer *player);
	bool contains_player(LobbyPlayer *player);

	int get_id() const { return id; }
	std::string get_name() const { return name; }
	int get_player_count() const { return players.size(); }
	int get_max_players() const { return max_players; }
	std::string get_map_name() const { return map_name; }

	const std::vector<LobbyPlayer *> &get_players() const { return players; }

private:
	int id;
	std::string name;
	int max_players;
	std::string map_name;
	State state;

	std::vector<LobbyPlayer *> players;
};
