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

#include "Physics/precomp.h"
#include "physics_listener.h"
#include "../Dynamics/body_impl.h"
#include "../Dynamics/fixture_impl.h"
#include "API/Physics/Dynamics/body.h"
#include "API/Physics/Dynamics/fixture.h"

namespace clan
{

PhysicsListener::PhysicsListener()
{
	
}
void PhysicsListener::BeginContact(b2Contact *contact)
{

	//optimise me
	Fixture_Impl &fixtureA = *static_cast<Fixture_Impl *> (contact->GetFixtureA()->GetUserData());
	Fixture_Impl &fixtureB = *static_cast<Fixture_Impl *> (contact->GetFixtureB()->GetUserData());

	fixtureA.on_begin_collision(fixtureB);
	fixtureB.on_begin_collision(fixtureA);

	/////Type cast the two colliding objects to a Body_Impl////////////////
	Body_Impl &bodyA = *static_cast<Body_Impl *> (contact->GetFixtureA()->GetBody()->GetUserData());
	Body_Impl &bodyB = *static_cast<Body_Impl *> (contact->GetFixtureB()->GetBody()->GetUserData());
	bodyA.on_begin_collision(bodyB);
	bodyB.on_begin_collision(bodyA);
}

void PhysicsListener::EndContact(b2Contact *contact)
{
	//optimise me
	Fixture_Impl &fixtureA = *static_cast<Fixture_Impl *> (contact->GetFixtureA()->GetUserData());
	Fixture_Impl &fixtureB = *static_cast<Fixture_Impl *> (contact->GetFixtureB()->GetUserData());

	fixtureA.on_end_collision(fixtureB);
	fixtureB.on_end_collision(fixtureA);

	/////Type cast the two colliding objects to a Body_Impl////////////////
	Body_Impl &bodyA = *static_cast<Body_Impl *> (contact->GetFixtureA()->GetBody()->GetUserData());
	Body_Impl &bodyB = *static_cast<Body_Impl *> (contact->GetFixtureB()->GetBody()->GetUserData());
	bodyA.on_end_collision(bodyB);
	bodyB.on_end_collision(bodyA);
}

void PhysicsListener::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse)
{
	//TODO
}

void PhysicsListener::PreSolve(b2Contact *contact, const b2Manifold *oldManifold)
{
	//TODO
}

bool PhysicsListener::ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB)
{
	//TODO
	return true;
}

}