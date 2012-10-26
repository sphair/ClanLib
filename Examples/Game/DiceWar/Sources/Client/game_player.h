
#pragma once

class GamePlayer
{
public:
	GamePlayer(std::string &name, int id, Color &color);

	std::string name;
	int id;
	Color color;
};
