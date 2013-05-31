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

#include "precomp.h"
#include "tilemap.h"

void TileMap::load(Canvas &canvas, const std::string &level, DisplayCache &resources)
{
	GraphicContext gc = canvas.get_gc();
	Resource resource = resources.get_resource(level);
	
	if (resource.get_type() != "tilemap")
		throw Exception(string_format("Resource %1 is not of type tilemap!", level));

	DomElement element = resource.get_element();
	std::string level_name = element.get_attribute("name");
	std::string resource_name = element.get_attribute("resource");
	map_width = element.get_attribute_int("width");
	map_height = element.get_attribute_int("height");
	
	cl_log_event("Debug", "Loading level %1 (%2x%3)", level_name, map_width, map_height);

	sprite_tiles = Sprite(gc, resource_name, &resources);

	tile_width = sprite_tiles.get_width();
	tile_height = sprite_tiles.get_height();

	cl_log_event("Debug", "Loaded resource %1 with %2 tiles", resource_name, sprite_tiles.get_frame_count());

	std::vector<DomNode> layer_nodes = element.select_nodes("layer");
	for (size_t layer_index = 0; layer_index < layer_nodes.size(); layer_index++)
	{
		DomElement layer_element = layer_nodes[layer_index].to_element();
		std::string layer_name = layer_element.get_attribute("name");

		std::string layer_tiles = layer_element.get_first_child().get_node_value();
		std::vector<std::string> tile_indices = StringHelp::split_text(layer_tiles, ",");

		TileMapLayer layer;
		layer.map.reserve(tile_indices.size());
		for(size_t i = 0; i < tile_indices.size(); ++i)
			layer.map.push_back(StringHelp::text_to_int(tile_indices[i]));
	
		layers.push_back(layer);

		cl_log_event("Debug", "Loaded layer %1 with %2 tiles", layer_name, layer.map.size());
	}

	current_map_position_x = 0;
	current_map_position_y = 0;
}

void TileMap::draw(Canvas &canvas)
{
	int screen_width = canvas.get_width();
	int screen_height = canvas.get_height();

	int start_tile_x = max(0, current_map_position_x / tile_width); 
	int start_tile_y = max(0, current_map_position_y / tile_height); 

	int scrolling_pixel_offset_x = current_map_position_x - start_tile_x * tile_width;
	int scrolling_pixel_offset_y = current_map_position_y - start_tile_y * tile_height;

	int tiles_on_screen_horizontally = screen_width / tile_width + 1;
	int tiles_on_screen_vertically = screen_height / tile_height + 1; 

	// since we might show half tiles on edges, make sure we display one more tile to fill screen
	tiles_on_screen_horizontally++;
	tiles_on_screen_vertically++;

	// make sure we don't draw more than the level size
	if(tiles_on_screen_horizontally + start_tile_x > map_width)
		tiles_on_screen_horizontally = map_width - start_tile_x;
	if(tiles_on_screen_vertically + start_tile_y > map_height)
		tiles_on_screen_vertically = map_height - start_tile_y;

	for(size_t l = 0; l < layers.size(); ++l)
	{
		for(int current_tile_y = 0; current_tile_y < tiles_on_screen_vertically; ++current_tile_y)
		{
			for(int current_tile_x = 0; current_tile_x < tiles_on_screen_horizontally; ++current_tile_x)
			{
				int index = (start_tile_y + current_tile_y) * map_width + (start_tile_x + current_tile_x);

				int sprite_index = layers[l].map[index];

				int tile_position_screen_x = current_tile_x * tile_width - scrolling_pixel_offset_x;
				int tile_position_screen_y = current_tile_y * tile_height - scrolling_pixel_offset_y;

				sprite_tiles.set_frame(sprite_index);
				sprite_tiles.draw(canvas, tile_position_screen_x, tile_position_screen_y); 
			}
		}
	}
}

void TileMap::set_scroll(int x, int y)
{
	current_map_position_x = x;
	current_map_position_y = y;
}
