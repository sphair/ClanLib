
#include "precomp.h"
#include "game_map.h"

/*
	The XML file format:

	<map>
		<client-settings>
			<terrain>
				<scale>0.3</scale>
			</terrain>
		</client-settings>
	</map>
*/

ClientMap::ClientMap()
{
	terrain_scale = 0.2f;
}

void ClientMap::process(CL_DomDocument &doc)
{
	Map::process(doc);

	CL_DomElement map_element = doc.get_document_element();
	if (map_element.get_local_name() != "map")
		throw CL_Exception("XML file not a valid map");

	CL_DomElement client_settings_element = map_element.named_item("client-settings").to_element();
	if (client_settings_element.is_element())
	{
		CL_DomElement terrain_element = client_settings_element.named_item("terrain").to_element();
		if (terrain_element.is_element())
		{
			CL_DomElement scale_element = terrain_element.named_item("scale").to_element();
			if (scale_element.is_element())
			{
				terrain_scale = CL_StringHelp::text_to_float(scale_element.get_text());
			}
		}	
	}
}
