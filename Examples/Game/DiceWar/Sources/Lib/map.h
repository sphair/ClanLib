
#pragma once

#include <map>

class MapArea;

class Map
{
public:
	Map();
	~Map();

	void load(const CL_String &map_name, const CL_String &file_name);

	unsigned int areas_size() const { return areas.size(); }
	MapArea *get_area(unsigned int index) { return areas[index]; }
	MapArea *get_area_by_id(int id);

	CL_String get_mapname() const { return map_name; }
	CL_String get_filename() const { return file_name; }

	// Validates a map is properly initialized (with players, armies, etc)
	bool validate();

	bool is_valid_attack(int player_id, MapArea *from_area, MapArea *to_area);

protected:
	virtual void process(CL_DomDocument &doc);

	void clear();
	std::map<int, MapArea*> create_areas(CL_DomElement &areas_element);
	void connect_areas(CL_DomElement &areas_element, std::map<int, MapArea*> &area_id_map);

	std::vector<MapArea *> areas;

	CL_String file_name;
	CL_String map_name;
};
