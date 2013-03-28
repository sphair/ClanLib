/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

/// \addtogroup clanPhysics_Collision clanPhysics Collision
/// \{
#pragma once

#include "../api_physics2d.h"

namespace clan
{
	class Body;
	class Fixture;

/// \brief Physics Object class.
///
/// A physics object is a pure virtual class that should be a base for all the game object classes
/// that want to work with collisions.
/// \xmlonly !group=Physic/Collision! !header=physics.h! \endxmlonly
class CL_API_PHYSICS PhysicsObject
{

/// \name Construction
/// \{
public:
	PhysicsObject(){};
	virtual ~PhysicsObject() { return; }
/// \}
/// \name Attributes
/// \{

	virtual bool should_collide_with(Body &body) = 0;

/// \}
/// \name Operations
/// \{

	virtual void on_collision_begin(Body &body) = 0;
	virtual void on_collision_end(Body &body) = 0;

/// \}
	
};

}

/// \}