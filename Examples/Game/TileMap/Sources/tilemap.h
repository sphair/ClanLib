/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
**    Kenneth Gangstoe
*/

#pragma once

using namespace clan;

class TileMapLayer
{
public:
	std::vector<int> map;
};

class TileMap
{
public:
	void load(Canvas &canvas, const std::string &level, ResourceManager &resources);

	void draw(Canvas &canvas);
	void set_scroll(int x, int y);

	Sprite sprite_tiles;
	std::vector<TileMapLayer> layers;

	int map_width;
	int map_height;

	int tile_width;
	int tile_height;

	// current pixel position on map
	int current_map_position_x;
	int current_map_position_y;
};
