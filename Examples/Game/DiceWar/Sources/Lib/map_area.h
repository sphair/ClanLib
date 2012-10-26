
#pragma once

class Map;

class MapArea
{
public:
	MapArea(Map *map, int id);

	bool validate();

	bool has_connecting_area(MapArea *area);

	Map *map;
	int id;
	int player_id;
	int army_strength;
	Point position;
	std::vector<MapArea *> connecting_areas;
};
