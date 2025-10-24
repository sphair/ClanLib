
#pragma once

class GamePlayer;

class GamePlayerCollection
{
public:
	GamePlayerCollection();
	~GamePlayerCollection();

	GamePlayer *create_player(CL_String &player_name, int player_id, int visual_id);

	void remove_player(GamePlayer *player);

	GamePlayer *get_player(int player_id);

	std::vector<GamePlayer *> players;

private:
	// An array of player colors - represented by the visual_id in GamePlayers
	std::vector<CL_Color> player_starting_colors;
};
