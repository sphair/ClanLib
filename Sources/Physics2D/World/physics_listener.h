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

#pragma once

#include "../Box2D/Box2D.h"
#include "API/Core/Signals/signal.h"
#include <list>

namespace clan
{

class Fixture_Impl;
class Body_Impl;
class Fixture;

class PhysicsListener : public b2ContactListener, public b2ContactFilter
{

	struct CollisionData
	{
		bool is_end_collision;

		Fixture_Impl *collide_fixtureA;
		Fixture_Impl *collide_fixtureB;

		Body_Impl *collide_bodyA;
		Body_Impl *collide_bodyB;
	};
//																							_______________________
//																							C O N S T R U C T I O N
public:

	PhysicsListener();
	virtual ~PhysicsListener(){};

//																							___________________
//																							O P E R A T I O N S
	void emit_collision_signals();
	
	virtual void BeginContact(b2Contact* contact);
	virtual void EndContact(b2Contact* contact);
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
	virtual bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB);

	std::list<CollisionData> collision_data;
	
};

}