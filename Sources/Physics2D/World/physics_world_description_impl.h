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
**    Arkadiusz Kalinowski
*/

#pragma once

#include "../Box2D/Box2D.h"
#include "API/Physics2D/World/physics_world.h"
#include "API/Physics2D/World/physics_world_description.h"
#include <map>

namespace clan
{

class PhysicsWorldDescription_Impl
{
/// \name Construction
/// \{
public:
	PhysicsWorldDescription_Impl():
		name("Unnamed Clanlib Physics World"),
		sleep_enabled(true),
		timestep(1.0f/60.0f),
		gravity(b2Vec2(0.0f,10.0f)),
		velocity_iterations(8),
		position_iterations(3),
		physic_scale(100),
		olm_enabled(false)
	{
	}

	virtual ~PhysicsWorldDescription_Impl() { return; }

/// \}
/// \name Attributes
/// \{
public:

	bool olm_enabled;

	std::string name;
	float physic_scale; // in pixels per 1 Box2D meter. Defaults at 100.
	bool sleep_enabled;
	b2Vec2 gravity;
	float timestep;
	int velocity_iterations;
	int position_iterations;

	std::map<std::string, std::shared_ptr<PhysicsWorldDescriptionData> > data_objects;

/// \}
};

}
