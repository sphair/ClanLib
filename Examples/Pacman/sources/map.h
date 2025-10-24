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

#ifndef header_map
#define header_map

#if _MSC_VER > 1000
#pragma once
#endif

class Map
{
//!Enums:
public:
	//: Different tile types in the game.
	enum TileType
	{
		tile_empty,   // Tile is nothing.
		tile_egg,     // Tile holds an egg.
		tile_powerup, // Tile holds a powerup.
		tile_trail,   // Tile holds a trail left by pacman.
		tile_wall     // Tile is a wall.
	};

//!Construction:
public:
	//:  Constructor. Loads the resources needed to display the map.
	Map(CL_ResourceManager *resources);
	
	//: Destructor.
	~Map();

//!Attributes:
public:
	//: Get the width of the map.
	int get_width() { return width; }
	
	//: Get the height of the map.
	int get_height() { return height; }
	
	//: Get the tile type of the tile at position (x,y).
	TileType get_tile_type(int x, int y);

	//: Returns the amount of eggs left on the map.
	int get_eggs_left() { return eggs_left; }
	
	//: Returns the pixel width of a tile.
	int get_tile_width() { return tile_images.get_frame_size(0).width; }

	//: Returns the pixel height of a tile.
	int get_tile_height() { return tile_images.get_frame_size(0).height; }

//!Operations:
public:
	//: Generate a level of the specified size.
	//: The num_connections specify the amount of connections between the
	//: tunnels in the maze.
	void generate_level(int width, int height, int num_connections = 20);
	
	//: Eat the egg at position (x,y), if any.
	void eat_egg(int x, int y);
	
	//: Leave a trail at position (x,y), if possible.
	void leave_trail(int x, int y);
	
	//: Eat the trail at position (x,y), if there's one.
	void eat_trail(int x, int y);
	
	//: Draw the map on graphic context, looking from (view_x, view_y).
	void draw(int view_x, int view_y, CL_GraphicContext *gc = 0);

//!Implementation:
private:
	//: Set the tile to the specified type.
	void set_tile_type(int x, int y, TileType type);

	//: Find free neighbour tile. Helper function for the map generation.
	bool find_neighbour(int x, int y, int *result_x, int *result_y);
	
	//: Sprite containing the tile images.
	CL_Sprite tile_images;
	
	//: Width of map.
	int width;
	
	//: Height of map.
	int height;
	
	//: Sprite frame for each tile.
	int *tiles;
	
	//: Number of eggs left on the map.
	int eggs_left;
};

#endif
