
#pragma once

#include "../Lib/map.h"

class ClientMap : public Map
{
public:
	ClientMap();

	float get_terrain_scale() const { return terrain_scale; }

protected:
	virtual void process(DomDocument &doc);

	float terrain_scale;
};
