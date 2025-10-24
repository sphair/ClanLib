
#include "precomp.h"
#include "map_area.h"

MapArea::MapArea(Map *map, int id)
: map(map), id(id), player_id(0), army_strength(0)
{
}

bool MapArea::validate()
{
	if (connecting_areas.size() < 1)
		return false;

	if(map == 0 || id == 0 || player_id == 0 || army_strength == 0)
		return false;

	return true;
}

bool MapArea::has_connecting_area(MapArea *area)
{
	for (unsigned int i = 0; i < connecting_areas.size(); i++)
	{
		if(connecting_areas[i]->id == area->id)
		{
			return true;
		}
	}
	return false;
}
