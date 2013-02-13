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

#include "Physics2D/precomp.h"
#include "physics_listener.h"
#include "../Dynamics/body_impl.h"
#include "../Dynamics/fixture_impl.h"
#include "API/Physics2D/Dynamics/body.h"
#include "API/Physics2D/Dynamics/fixture.h"
#include "API/Physics2D/Collision/physics_object.h"

namespace clan
{

PhysicsListener::PhysicsListener()
{
	
}
void PhysicsListener::BeginContact(b2Contact *contact)
{

	CollisionData data;
	data.collide_fixtureA = static_cast<Fixture_Impl *> (contact->GetFixtureA()->GetUserData());
	data.collide_fixtureB = static_cast<Fixture_Impl *> (contact->GetFixtureB()->GetUserData());
	data.collide_bodyA = static_cast<Body_Impl *> (contact->GetFixtureA()->GetBody()->GetUserData());
	data.collide_bodyB = static_cast<Body_Impl *> (contact->GetFixtureB()->GetBody()->GetUserData());
	data.is_end_collision = false;

	collision_data.push_back(data);	
}

void PhysicsListener::EndContact(b2Contact *contact)
{
	//End collisions are currently turned out due to bugs that need to be fixed before enabling the end collisions back.
	//CollisionData data;
	//data.collide_fixtureA = static_cast<Fixture_Impl *> (contact->GetFixtureA()->GetUserData());
	//data.collide_fixtureB = static_cast<Fixture_Impl *> (contact->GetFixtureB()->GetUserData());
	//data.collide_bodyA = static_cast<Body_Impl *> (contact->GetFixtureA()->GetBody()->GetUserData());
	//data.collide_bodyB = static_cast<Body_Impl *> (contact->GetFixtureB()->GetBody()->GetUserData());
	//data.is_end_collision = true;

	//collision_data.push_back(data);
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
	Body_Impl *bodyA_impl = static_cast<Body_Impl *> (fixtureA->GetBody()->GetUserData());
	Body_Impl *bodyB_impl = static_cast<Body_Impl *> (fixtureB->GetBody()->GetUserData());

	PhysicsObject *objectA = bodyA_impl->data;
	PhysicsObject *objectB = bodyB_impl->data;
	
	if(objectA!=nullptr && objectB!=nullptr)
	{	
		Body bodyA;
		bodyA.impl = bodyA_impl->shared_from_this();

		Body bodyB;
		bodyB.impl = bodyB_impl->shared_from_this();

		return (objectA->should_collide_with(bodyB) && objectB->should_collide_with(bodyA));
	}
	else
	{
		return true;
	}
}

void PhysicsListener::emit_collision_signals()
{
	std::list<CollisionData>::iterator it;
	for(it = collision_data.begin(); it!= collision_data.end() ;)
	{
		CollisionData &data = (*it);
		PhysicsObject *objectA = data.collide_bodyA->data;
		PhysicsObject *objectB =  data.collide_bodyB->data;

		if(objectA!=nullptr && objectB!=nullptr)
		{	
			Body bodyA;
			bodyA.impl = data.collide_bodyA->shared_from_this();

			Body bodyB;
			bodyB.impl = data.collide_bodyB->shared_from_this();

			//Might optimise by splitting emit_collision_signals to emit_begin_collision_signals and emit_end_collision_signals.
			//if(!data.is_end_collision)
			//{
				objectA->on_collision_begin(bodyB);
				objectB->on_collision_begin(bodyA);
			//}
			//else
			//{
			//	objectA->on_collision_end(bodyB);
			//	objectB->on_collision_end(bodyA);
			//}
		}

		it = collision_data.erase(it);
	}

}

}