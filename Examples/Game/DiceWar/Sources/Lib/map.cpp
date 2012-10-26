
#include "precomp.h"
#include "map.h"
#include "map_area.h"

/*
	The XML file format:

	<map>
		<areas>
			<area id="1">
				<position>
					<x>860</x>
					<y>860</y>
				</position>
				<connections>1,2</connections>
			</area>
			<area id="2">
				<position>
					<x>1000</x>
					<y>430</y>
				</position>
				<connections>0</connections>
			</area>
			<area id="3">
				<position>
					<x>324</x>
					<y>543</y>
				</position>
				<connections>0</connections>
			</area>
		<areas>
	</map>
*/

Map::Map()
{
}

Map::~Map()
{
	clear();
}

void Map::clear()
{
	for (unsigned int i = 0; i < areas.size(); i++)
		delete areas[i];
}

bool Map::validate()
{
	if(areas.size() < 6)
		return false;

	for (unsigned int i = 0; i < areas.size(); i++)
		if(areas[i]->validate() == false)
			return false;

	return true;
}

MapArea *Map::get_area_by_id(int id)
{
	for (unsigned int i = 0; i < areas.size(); i++)
		if(areas[i]->id == id)
			return areas[i];

	return 0;
}

void Map::load(const std::string &map_name, const std::string &file_name)
{
	clear();

	File file(file_name, File::open_existing, File::access_read);
	DomDocument doc(file);
	file.close();

	this->file_name = file_name;
	this->map_name = map_name;

	process(doc);
}

void Map::process(DomDocument &doc)
{
	DomElement map_element = doc.get_document_element();
	if (map_element.get_local_name() != "map")
		throw Exception("XML file not a valid map");

	DomElement areas_element = map_element.named_item("areas").to_element();
	if (areas_element.is_null())
		throw Exception("XML file not a valid map");

	std::map<int, MapArea*> area_id_map = create_areas(areas_element);
	connect_areas(areas_element, area_id_map);
}

std::map<int, MapArea*> Map::create_areas(DomElement &areas_element)
{
	std::map<int, MapArea*> area_id_map;
	DomNode cur_node = areas_element.get_first_child();
	while (!cur_node.is_null())
	{
		DomElement cur_element = cur_node.to_element();
		if (cur_element.get_local_name() == "area")
		{
			int area_id = StringHelp::text_to_int(cur_element.get_attribute("id"));
			std::auto_ptr<MapArea> area(new MapArea(this, area_id));

			DomElement position_element = cur_element.named_item("position").to_element();
			std::string pos_x = position_element.named_item("x").to_element().get_text();
			std::string pos_y = position_element.named_item("y").to_element().get_text();
			area->position = Point(StringHelp::text_to_int(pos_x), StringHelp::text_to_int(pos_y));

			if (area_id_map.find(area_id) != area_id_map.end())
				throw Exception(string_format("Duplicate definitions of area with id %1", area_id));
			area_id_map[area_id] = area.get();

			areas.push_back(area.release());
		}
		else if (cur_element.is_element())
		{
			throw Exception(string_format("Unexpected element %1", cur_element.get_tag_name()));
		}
		cur_node = cur_node.get_next_sibling();
	}
	return area_id_map;
}

void Map::connect_areas(DomElement &areas_element, std::map<int, MapArea*> &area_id_map)
{
	DomNode cur_node = areas_element.get_first_child();
	while (!cur_node.is_null())
	{
		DomElement cur_element = cur_node.to_element();
		if (cur_element.get_local_name() == "area")
		{
			int area_id = StringHelp::text_to_int(cur_element.get_attribute("id"));
			DomElement connections = cur_element.named_item("connections").to_element();
			std::vector<std::string> ids = StringHelp::split_text(connections.get_text(), ",");
			std::map<int, bool> connected_ids;
			for (unsigned int i = 0; i < ids.size(); i++)
			{
				int id = StringHelp::text_to_int(ids[i]);
				std::map<int, MapArea*>::iterator it = area_id_map.find(id);
				if (it == area_id_map.end())
					throw Exception(string_format("Unable to connect area %1 to %2: area not found", area_id, id));
				if (connected_ids.find(id) != connected_ids.end())
					throw Exception(string_format("Area %1 already connected to %2", area_id, id));
				connected_ids[id] = true;
				area_id_map[area_id]->connecting_areas.push_back(it->second);
			}
		}
		cur_node = cur_node.get_next_sibling();
	}
}

bool Map::is_valid_attack(int player_id, MapArea *from_area, MapArea *to_area)
{
	// Check if both areas actually exists
	if(from_area == 0 || to_area == 0)
		return false;

	// Check if player owns the attacking area
	if(from_area->player_id != player_id)
		return false;

	// Check if the attack is between two different players
	if(from_area->player_id == to_area->player_id)
		return false;

	// Check if attack strength is larger than 1
	if(from_area->army_strength < 2)
		return false;

	// Check if target area is a neighbour to attacking area
	if(from_area->has_connecting_area(to_area) == false)
		return false;

	return true;
}
