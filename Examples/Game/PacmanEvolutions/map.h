
#pragma once

class Map
{
public:
	Map();
	~Map();

	enum TileFlags // fix: this isn't flags
	{
		flag_nothing     = 0,
		flag_egg         = 1,
		flag_powerup     = 2,
		flag_trail       = 3,
		flag_evolutionup = 3, // to do: fix this
		flag_wall        = 4,
		flag_wall_left   = flag_wall + 1,
		flag_wall_right  = flag_wall + 2,
		flag_wall_top    = flag_wall + 4,
		flag_wall_bottom = flag_wall + 8
	};

	int get_tile(int x, int y) const;
	bool has_wall(int x, int y) const;
	bool has_egg(int x, int y) const;
	bool has_powerup(int x, int y) const;
	bool has_evolutionup(int x, int y) const;
	bool has_trail(int x, int y) const { return false; } // implement this

	int get_eggs_left() const;
	void eat_egg(int x, int y);
	void leave_trail(int x, int y) { } // implement this
	void eat_trail(int x, int y) { } // implement this

	int get_width() const { return width; }
	int get_height() const { return height; }

private:
	void generate_level(int map_width, int map_height, int num_connections);
	bool find_neighbour(int x, int y, int *xx, int *yy);

	int width;
	int height;
	std::vector<int> tiles;
	int eggs_left;
};
