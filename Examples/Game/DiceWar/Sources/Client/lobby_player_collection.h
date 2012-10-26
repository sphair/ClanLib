
#pragma once

class LobbyPlayer;

class LobbyPlayerCollection
{
public:
	LobbyPlayerCollection();
	~LobbyPlayerCollection();

	LobbyPlayer *create_player(int player_id, std::string player_name);

	void remove_player(int player_id);

	LobbyPlayer *get_player(int player_id) const;

private:
	std::map<int, LobbyPlayer *> players;
};
