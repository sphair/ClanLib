
#ifndef header_object
#define header_object

#if _MSC_VER > 1000
#pragma once
#endif

#include <ClanLib/core.h>

class CL_Surface;

class Object
{
public:
	~Object() { }

	void draw() { surface.draw(pos.x, pos.y); }

	CL_Point pos;
	unsigned int z_value;
	CL_Surface surface;
};

struct object_compare
{
	bool operator()(const Object *a, const Object *b) const
	{
		return a->z_value < b->z_value;
	}
};

#endif
