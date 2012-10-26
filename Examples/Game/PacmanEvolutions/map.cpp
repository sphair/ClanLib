
#include "precomp.h"
#include "map.h"

Map::Map()
{
	generate_level(51, 51, 20);
}

Map::~Map()
{
}

int Map::get_tile(int x, int y) const
{
	return tiles[cl_clamp(x, 0, width-1) + cl_clamp(y, 0, height-1) * width];
}

bool Map::has_wall(int x, int y) const
{
	if (x >= 0 && y >= 0 && x < width && y < height)
		return get_tile(x, y) >= 4;
	else
		return false;
}

bool Map::has_egg(int x, int y) const
{
	return get_tile(x, y) == flag_egg;
}

bool Map::has_powerup(int x, int y) const
{
	return get_tile(x, y) == flag_powerup;
}

bool Map::has_evolutionup(int x, int y) const
{
	return get_tile(x, y) == flag_evolutionup;
}

int Map::get_eggs_left() const
{
	return eggs_left;
}

void Map::eat_egg(int x, int y)
{
	if (has_egg(x, y) || has_evolutionup(x, y) || has_powerup(x, y))
	{
		eggs_left--;
		tiles[cl_clamp(x, 0, width-1) + cl_clamp(y, 0, height-1) * width] = 0;
	}
}

void Map::generate_level(int map_width, int map_height, int num_connections)
{
	// Allocate map array and setup variables:
	width = map_width;
	height = map_height;
	tiles.resize(width*height);
	eggs_left = 0;

	// Random generate map:
	int x, y, center_x, center_y, num_tiles, i;
	
	// 1. clear map with walls.
	for (y=0; y<height; y++)
		for (x=0; x<width; x++)
			tiles[x + y*width] = 4;

	// 2. make gaps in all four directions at starting point in map.
	center_x = width/2;
	center_y = height/2;
	
	tiles[center_x   + center_y     * width] = 0;
	tiles[center_x-1 + center_y     * width] = 1;
	tiles[center_x+1 + center_y     * width] = 1;
	tiles[center_x   + (center_y-1) * width] = 1;
	tiles[center_x   + (center_y+1) * width] = 1;

	// 3. build tunnels in level
	num_tiles = width * height;
	std::vector<int> tunnel_x[4];
	std::vector<int> tunnel_y[4];
	for (i=0; i<4; i++)
	{
		tunnel_x[i].resize(num_tiles);
		tunnel_y[i].resize(num_tiles);
	}

	// Set the four tunnel starting points:
	tunnel_x[0][0] = center_x;
	tunnel_y[0][0] = center_y - 2;
	tunnel_x[1][0] = center_x;
	tunnel_y[1][0] = center_y + 2;
	tunnel_x[2][0] = center_x - 2;
	tunnel_y[2][0] = center_y;
	tunnel_x[3][0] = center_x + 2;
	tunnel_y[3][0] = center_y;

	// Mark beginning tiles as tunnel:
	for (i=0; i<4; i++)
		tiles[tunnel_x[i][0] + tunnel_y[i][0]*width] = 1;

	// Start creating the tunnels. We do this up to num_tiles times, since this is the
	// theoretically maximum length any tunnel could possibly have.
	for (int pos=1; pos<num_tiles; pos++)
	{
		// For each round, the tunnel finds a neighbour tile that it can move into.
		int filled_level = 0;
		for (int tun=0; tun<4; tun++)
		{
			// Find free neighbours to this tile. Update which one was chosen
			// to the current position in the tunnel_x and tunnel_y arrays.
			if (find_neighbour(
				tunnel_x[tun][pos-1],
				tunnel_y[tun][pos-1],
				&tunnel_x[tun][pos],
				&tunnel_y[tun][pos])==false)
			{
				// Try and figure out if we moved into a total dead end.
				bool total_dead_end = true;
				for (int pos2=pos-2; pos2>=0; pos2--)
				{
					if (find_neighbour(
						tunnel_x[tun][pos2],
						tunnel_y[tun][pos2],
						&tunnel_x[tun][pos],
						&tunnel_y[tun][pos]))
					{
						total_dead_end = false;
						break;
					}
				}

				// If we moved into a total dead end, this tunnel can move no
				// further.
				if (total_dead_end)
				{
					tunnel_x[tun][pos] = tunnel_x[tun][pos-1];
					tunnel_y[tun][pos] = tunnel_y[tun][pos-1];
					filled_level++;
				}
			}
			
			// Mark newly found tile as egg:
			tiles[tunnel_x[tun][pos] + tunnel_y[tun][pos]*width] = 1;
		}
		
		// All four tunnels reached a dead end. We're done tunneling.
		if (filled_level == 4) break;
	}
	
	// 4. connect gaps random in the tunnels:
	for (i=0; i<num_connections; i++)
	{
		int x = rand()%(width/2-3);
		int y = rand()%(height/2-3);
		
		x *= 2; y *= 2;
		x += 3; y += 3;
		
		switch (rand()%4)
		{
		case 0:
			tiles[x + (y-1)*width] = 1;
			break;
		case 1:
			tiles[x + (y+1)*width] = 1;
			break;
		case 2:
			tiles[(x-1) + y*width] = 1;
			break;
		case 3:
			tiles[(x+1) + y] = 1;
			break;
		}
	}

	// Random generation is complete.
	
	// Now change the map wall tiles to correct sprite frame:
	// (we got different images depending which walls are next to a wall)
	for (y=0; y<height; y++)
	{
		for (x=0; x<width; x++)
		{
			// If tile is not a wall, dont change tile sprite frame.
			if (tiles[x + y*width] <= 3) continue;

			int new_image = 0; 
			if (x>0	       && tiles[(x-1) + y*width] > 3) new_image |= 1; // left flag
			if (x<width-1  && tiles[(x+1) + y*width] > 3) new_image |= 2; // right flag
			if (y>0	       && tiles[x + (y-1)*width] > 3) new_image |= 4; // up flag
			if (y<height-1 && tiles[x + (y+1)*width] > 3) new_image |= 8; // down flag

			tiles[x + y*width] = new_image+4;
		}
	}
	
	// Place powerups in each corner of map:
	tiles[1         + 1*width] = flag_powerup;
	tiles[(width-2) + 1*width] = flag_powerup;
	tiles[1         + (height-2)*width] = flag_powerup;
	tiles[(width-2) + (height-2)*width] = flag_powerup;

	// Count the number of eggs
	eggs_left = 0;
	for (y=0; y<height; y++)
		for (x=0; x<width; x++)       
			if ((tiles[x + y*width] == 1) || (tiles[x + y*width] == 2))
				eggs_left++;
	
}

bool Map::find_neighbour(int x, int y, int *xx, int *yy)
{
	*xx = x;
	*yy = y;

	int num_dirs = 0;
	if (x>1        && tiles[(x-2) + y*width]     >= 4) num_dirs++;
	if (y>1        && tiles[x     + (y-2)*width] >= 4) num_dirs++;
	if (x<width-2  && tiles[(x+2) + y*width]     >= 4) num_dirs++;
	if (y<height-2 && tiles[x     + (y+2)*width] >= 4) num_dirs++;
	
	int sel = rand()%(num_dirs+1);

	if (x>1        && tiles[(x-2)+y*width] >= 4 && (sel--) == 0) { tiles[(x-1)+y*width] = 1; *xx = x-2; return true; }
	if (y>1        && tiles[x+(y-2)*width] >= 4 && (sel--) == 0) { tiles[x+(y-1)*width] = 1; *yy = y-2; return true; }
	if (x<width-3  && tiles[(x+2)+y*width] >= 4 && (sel--) == 0) { tiles[(x+1)+y*width] = 1; *xx = x+2; return true; }
	if (y<height-3 && tiles[x+(y+2)*width] >= 4 && (sel--) == 0) { tiles[x+(y+1)*width] = 1; *yy = y+2; return true; }

	return false;
}
