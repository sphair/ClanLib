/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
**    Magnus Norddahl
*/

#pragma once
namespace clan
{

class LevelDataLight
{
public:
	LevelDataLight() : attenuation_start(), attenuation_end(), casts_shadows(), level_set(0) { }
	LevelDataLight(const Vec3f &position, const Quaternionf &orientation, const Vec3f &color, float attenuation_start, float attenuation_end, float falloff, float hotspot, bool casts_shadows)
		: position(position), orientation(orientation), color(color), attenuation_start(attenuation_start), attenuation_end(attenuation_end), falloff(falloff), hotspot(hotspot), casts_shadows(casts_shadows), level_set(0) { }

	Vec3f position;
	Quaternionf orientation;
	Vec3f color;
	float attenuation_start;
	float attenuation_end;
	float falloff;
	float hotspot;
	bool casts_shadows;
	int level_set;
};

}

