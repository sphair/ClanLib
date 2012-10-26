
#include "precomp.h"
#include "game_map_selection.h"
#include "client.h"
#include "game_model.h"
#include "game_player.h"
#include "game_player_collection.h"
#include "../Lib/map.h"
#include "../Lib/map_area.h"

///////////////////////////////////////////////////////////////////////////////
// Construction:

MapSelection::MapSelection(Client *client) : client(client)
{
	color_highlighted = Color::white;
 	color_highlighted.set_alpha(50);

	color_selected_source = Color::blue;
	color_selected_source.set_alpha(50);

	color_selected_destination = Color::blue;
	color_selected_destination.set_alpha(50);

	original_area_colors.resize(client->get_game_model()->get_map()->areas_size() + 1, 0);

	is_initialized = true;

	update_areas_with_player_ownership();
	clear_selection();
}

///////////////////////////////////////////////////////////////////////////////
// Attributes:

bool MapSelection::is_modified() const
{
	return is_dirty;
}

bool MapSelection::is_complete() const
{
	return is_area_id_valid(get_selected_source_area_id()) && is_area_id_valid(get_selected_destination_area_id());
}

bool MapSelection::has_highlighted_area() const
{
	return highlighted_area_id != -1;
}

int MapSelection::get_highlighted_area_id() const
{
	return highlighted_area_id;
}

int MapSelection::get_selected_source_area_id() const
{
	return selected_source_area_id;
}

int MapSelection::get_selected_destination_area_id() const
{
	return selected_destination_area_id;
}

///////////////////////////////////////////////////////////////////////////////
// Operations:

void MapSelection::update_areas_with_player_ownership()
{
	if(is_initialized == false)
		return;

	GameModel *game_model = client->get_game_model();
	ClientMap *map = client->get_game_model()->get_map();
	for(unsigned int i = 0; i < map->areas_size(); ++i)
	{
		MapArea *area = map->get_area(i);

		GamePlayer *player = game_model->get_players()->get_player(area->player_id);
		if(player)
			original_area_colors[i + 1] = (player->color.get_red() << 24) + (player->color.get_green() << 16) + (player->color.get_blue() << 8) + 0x42;
		else
			original_area_colors[i + 1] = 0;
	}
}

void MapSelection::set_highlighted_area(int area_id)
{
	// Clear any previous highlight
	if(is_area_id_valid(highlighted_area_id))
	{
		if(highlighted_area_id == selected_destination_area_id)
			current_area_colors[highlighted_area_id] = color_selected_destination.get_rgba8();
		else if(highlighted_area_id == selected_source_area_id)
			current_area_colors[highlighted_area_id] = color_selected_source.get_rgba8();
		else
			current_area_colors[highlighted_area_id] = original_area_colors[highlighted_area_id];

		highlighted_area_id = -1;
		is_dirty = true;
	}

	// Set new highlighted area
	if (is_area_id_valid(area_id))
	{
		// Select destination area (we have a source area selected already)
		if(is_area_id_valid(get_selected_source_area_id()))
		{
			MapArea *source_area = client->get_game_model()->get_map()->get_area_by_id(get_selected_source_area_id());
			MapArea *destination_area = client->get_game_model()->get_map()->get_area_by_id(area_id);

			// Check if destination area is not owner by player
			if(destination_area->player_id == client->get_player_id())
				return;

			// Check if destination area is connected to source area
			if(source_area->has_connecting_area(destination_area) == false)
				return;
		}
		// Select source area
		else
		{

			MapArea *source_area = client->get_game_model()->get_map()->get_area_by_id(area_id);

			// Check if source area is owner by player
			if(source_area->player_id != client->get_player_id())
				return;

			// Check if source area has more than two armies
			if(source_area->army_strength < 2)
				return;
		}

		current_area_colors[area_id] = color_highlighted.get_rgba8();
		highlighted_area_id = area_id;
		is_dirty = true;
	}
}

void MapSelection::select_highlighted_area()
{
	if(has_highlighted_area())
	{
		int highlighted_area_id = get_highlighted_area_id();

		if(get_selected_source_area_id() == highlighted_area_id)
		{
			clear_selected_source_area();
		}
		else
		{
			set_selected_area(highlighted_area_id);
		}
	}
	else
	{
		clear_selection();
	}
}

void MapSelection::clear_selected_source_area()
{
	if (is_area_id_valid(selected_source_area_id))
	{
		if(current_area_colors[selected_source_area_id] != original_area_colors[selected_source_area_id])
		{
			current_area_colors[selected_source_area_id] = original_area_colors[selected_source_area_id];

			is_dirty = true;
		}		

		selected_source_area_id = -1;
	}
}

void MapSelection::set_selected_area(int area_id)
{
	if (is_area_id_valid(area_id))
	{
		// select destination area
		if(is_area_id_valid(get_selected_source_area_id()))
		{
			current_area_colors[area_id] = color_selected_destination.get_rgba8();
			selected_destination_area_id = area_id;
		}
		// select source area
		else
		{
			current_area_colors[area_id] = color_selected_source.get_rgba8();
			selected_source_area_id = area_id;
		}

		is_dirty = true;
	}
}

void MapSelection::clear_selection()
{
	current_area_colors = original_area_colors;

	highlighted_area_id = -1;
	selected_source_area_id = -1;
	selected_destination_area_id = -1;

	is_dirty = true;
}

const std::vector<unsigned int> &MapSelection::get_result_colors()
{
	is_dirty = false;

	return current_area_colors;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation:

bool MapSelection::is_area_id_valid(int area_id) const
{
	return area_id >= 1 && (unsigned int)area_id < current_area_colors.size();
}
