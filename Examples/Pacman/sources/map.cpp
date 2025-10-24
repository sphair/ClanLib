/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    (if your name is missing here, please add it)
*/

#include "precomp.h"
#include "map.h"

/////////////////////////////////////////////////////////////////////////////
// Map construction:

Map::Map(CL_ResourceManager *resources) : width(0), height(0), tiles(0), eggs_left(0)
{
	tile_images = CL_Sprite("Game/spr_maptiles", resources);
}
	
Map::~Map()
{
	delete[] tiles;
}

/////////////////////////////////////////////////////////////////////////////
// Map attributes:

Map::TileType Map::get_tile_type(int x, int y)
{
	int tile = tiles[x + y*width];
	switch (tile)
	{
	case 0:
		return tile_empty;
	case 1:
		return tile_egg;
	case 2:
		return tile_powerup;
	case 3:
		return tile_trail;
	default:
		return tile_wall;		
	}
}

/////////////////////////////////////////////////////////////////////////////
// Map operations:

void Map::generate_level(int map_width, int map_height, int num_connections)
{
	// Clean up any possible earlier map:
	delete[] tiles;
	
	// Allocate map array and setup variables:
	width = map_width;
	height = map_height;
	tiles = new int[width * height];
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
	int *tunnel_x[4];
	int *tunnel_y[4];
	for (i=0; i<4; i++)
	{
		tunnel_x[i] = new int[num_tiles];
		tunnel_y[i] = new int[num_tiles];
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
	
	// Clean up.
	for (i=0; i<4; i++)
	{
		delete[] tunnel_x[i];
		delete[] tunnel_y[i];
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
			if (tiles[x + y*width] <= 2) continue;

			int new_image = 0; 
			if (x>0	       && tiles[(x-1) + y*width] > 3) new_image |= 1; // left flag
			if (x<width-1  && tiles[(x+1) + y*width] > 3) new_image |= 2; // right flag
			if (y>0	       && tiles[x + (y-1)*width] > 3) new_image |= 4; // up flag
			if (y<height-1 && tiles[x + (y+1)*width] > 3) new_image |= 8; // down flag

			tiles[x + y*width] = new_image+4;
		}
	}
	
	// Place powerups in each corner of map:
	tiles[1         + 1*width] = 2;
	tiles[(width-2) + 1*width] = 2;
	tiles[1         + (height-2)*width] = 2;
	tiles[(width-2) + (height-2)*width] = 2;

	// Count the number of eggs
	eggs_left = 0;
	for (y=0; y<height; y++)
		for (x=0; x<width; x++)       
			if ((tiles[x + y*width] == 1) || (tiles[x + y*width] == 2))
				eggs_left++;
	
}

void Map::eat_egg(int x, int y)
{
	// Only eat egg if its actually an egg!
	if (get_tile_type(x, y) != tile_egg && get_tile_type(x, y) != tile_powerup) return;

	set_tile_type(x, y, tile_empty);
	eggs_left--;
}

void Map::leave_trail(int x, int y)
{
	// Only leave trail if tile doesn't contain something else.
	if (get_tile_type(x, y) != tile_empty) return;

	set_tile_type(x, y, tile_trail);
}

void Map::eat_trail(int x, int y)
{
	// Only eat trail if its a trail.
	if (get_tile_type(x, y) != tile_trail) return;

	set_tile_type(x, y, tile_empty);
}

void Map::draw(int center_x, int center_y, CL_GraphicContext *gc)
{
	static int start_time = -1;
	if (start_time == -1) start_time = CL_System::get_time();
	
	int powerup_spr = ((CL_System::get_time()-start_time)/250)%2;
	
	int tile_width = get_tile_width()-6;
	int tile_height = get_tile_height()-6;

	for (int y=0; y<height; y++)
	{
		for (int x=0; x<width; x++)
		{
			int scr_x = x*tile_width-6 - center_x;
			int scr_y = y*tile_height-6 - center_y;
			
			int frame = tiles[x + y*width];
			if (frame == 2) frame = powerup_spr + 1; // powerup blinking.

			tile_images.set_frame(frame);
			tile_images.draw(scr_x, scr_y, gc);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Map implementation:

void Map::set_tile_type(int x, int y, TileType type)
{
	int tile_frame = 0;
	switch(type)
	{
	case tile_empty:
		tile_frame = 0;
		break;
		
	case tile_egg:
		tile_frame = 1;
		break;
		
	case tile_powerup:
		tile_frame = 2;
		break;
		
	case tile_trail:
		tile_frame = 3;
		break;
		
	case tile_wall:
		tile_frame = 4;
		break;
	}
	
	tiles[x + y * width] = tile_frame;
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
