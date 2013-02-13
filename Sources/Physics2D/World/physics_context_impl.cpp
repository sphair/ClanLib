/*
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
#include "physics_world_impl.h"
#include "physics_context_impl.h"
#include "API/Physics2D/World/physics_world.h"
#include "API/Core/Text/string_format.h"
#include "API/Physics2D/Dynamics/body_description.h"
#include "API/Physics2D/Dynamics/fixture_description.h"
#include "API/Physics2D/Dynamics/body.h"
#include "API/Physics2D/Dynamics/fixture.h"
#include "API/Physics2D/Collision/Shapes/edge_shape.h"
#include "../Dynamics/body_impl.h"
#include "../Dynamics/fixture_impl.h"
#include "../Dynamics/Joints/joint_impl.h"

namespace clan
{

//																						_______________________
//																						C O N S T R U C T O R S
PhysicsContext_Impl::PhysicsContext_Impl(PhysicsWorld_Impl *owner)
: max_body_amount(200), 
  max_fixture_amount(400),
  max_joint_amount(100),
  bodies(max_body_amount), //Add customising options
  fixtures(max_fixture_amount), //Add customising options
  joints(max_joint_amount), //Add customising options
  owner_world(owner)
{
	if(owner_world)
	{
		for(int i=0; i<max_body_amount; i++)
		{
			free_body_slots.push_back(i);
		}

		for(int i=0; i<max_fixture_amount; i++)
		{
			free_fixture_slots.push_back(i);
		}

		for(int i=0; i<max_joint_amount; i++)
		{
			free_joint_slots.push_back(i);
		}	
	}
	else
	throw Exception("Tried to create a PhysicsContext using a null PhysicWorld object.");
}

void PhysicsContext_Impl::setup_dummy_objects()
{
	//Setup b2 dummy objects
	b2BodyDef body_def;
	body_def.active = false;
	dummy_body_b2 = owner_world->world.CreateBody(&body_def);

	b2FixtureDef fixture_def;
	b2EdgeShape shape;
	shape.Set(b2Vec2(0.0f, 0.0f), b2Vec2(0.0f, 0.0f));
	fixture_def.shape = &shape;
	dummy_fixture_b2 = dummy_body_b2->CreateFixture(&fixture_def);

		//Setup impl dummy objects
	PhysicsContext pc = get_API();
	BodyDescription body_desc(pc);
	body_desc.set_active(false);

	Body dummy_body(pc,body_desc);

	FixtureDescription fixture_desc(pc);
	EdgeShape edge_shape(pc);
	edge_shape.set(Vec2f(0.0f, 0.0f), Vec2f(0.0f, 0.0f));
	fixture_desc.set_shape(edge_shape);

	Fixture dummy_fixture(pc, dummy_body, fixture_desc);

	dummy_body.kill();
	dummy_fixture.kill();

	dummy_body_impl = dummy_body.impl;
	dummy_fixture_impl = dummy_fixture.impl;
}
//																						___________________
//																						O P E R A T I O N S

void PhysicsContext_Impl::create_in_context(std::shared_ptr<Body_Impl> body)
{
	if(free_body_slots.size()>0)
	{
		int slot = free_body_slots.front();
		free_body_slots.pop_front();
		bodies[slot] = body;
		body->set_id(slot);
	}
	else
	throw Exception(string_format("Exceded the current maximum bodies amount that is equal %1.", max_body_amount));
}

void PhysicsContext_Impl::create_in_context(std::shared_ptr<Fixture_Impl> fixture)
{
	if(free_fixture_slots.size()>0)
	{
		int slot = free_fixture_slots.front();
		free_fixture_slots.pop_front();
		fixtures[slot] = fixture;
		fixture->set_id(slot);
	}
	else
	throw Exception(string_format("Exceded the current maximum fixtures amount that is equal %1.", max_fixture_amount));

}
void PhysicsContext_Impl::create_in_context(std::shared_ptr<Joint_Impl> joint)
{
		if(free_joint_slots.size()>0)
	{
		int slot = free_joint_slots.front();
		free_joint_slots.pop_front();
		joints[slot] = joint;
		joint->set_id(slot);
	}
	else
	throw Exception(string_format("Exceded the current maximum joints amount that is equal %1.", max_joint_amount));

}

void PhysicsContext_Impl::remove_from_context(std::shared_ptr<Body_Impl> body)
{
	int id = body->get_id();

	if(id>=0)
	{
		if( bodies[id]->get_id() == id)
		{
			bodies[id].reset();
			free_body_slots.push_back(id);
		}
		else
		throw Exception("Couldn't delete the Body from the context. The id of the Body is different than the one stored in the PhysicsContext."); //TODO: expand this.
	}
	else
	throw Exception("Tried to remove a Body from the context, but the Body has no ID number"); 
}

void PhysicsContext_Impl::remove_from_context(std::shared_ptr<Fixture_Impl> fixture)
{
	int id = fixture->get_id();
	
	if(id>=0)
	{
		if( fixtures[id]->get_id() == id)
		{
			fixtures[id].reset();
			free_fixture_slots.push_back(id);
		}
		else
		throw Exception("Couldn't delete the Fixture from the context. The id of the Fixture is different than the one stored in the PhysicsContext."); //TODO: expand this.
	}
	else
	throw Exception("Tried to remove a Fixture from the context, but the Fixture has no ID number"); 
}


void PhysicsContext_Impl::remove_from_context(std::shared_ptr<Joint_Impl> joint)
{
	int id = joint->get_id();
	
	if(id>=0)
	{
		if( joints[id]->get_id() == id)
		{
			joints[id].reset();
			free_joint_slots.push_back(id);
		}
		else
		throw Exception("Couldn't delete the Joint from the context. The id of the Joint is different than the one stored in the PhysicsContext."); //TODO: expand this.
	}
	else
	throw Exception("Tried to remove a Joint from the context, but the Joint has no ID number"); 
}

void PhysicsContext_Impl::remove_body_from_context(int id)
{
	if(id>=0)
	{
		if( bodies[id]->get_id() == id)
		{
			bodies[id].reset();
			free_body_slots.push_back(id);
		}
		else
		throw Exception("Couldn't delete the Body from the context. The id of the Body is different than the one stored in the PhysicsContext."); //TODO: expand this.
	}
	else
	throw Exception("Tried to remove a Body from the context, but the Body has no ID number"); 
}

void PhysicsContext_Impl::remove_fixture_from_context(int id)
{
	if(id>=0)
	{
		if( fixtures[id]->get_id() == id)
		{
			fixtures[id].reset();
			free_fixture_slots.push_back(id);
		}
		else
		throw Exception("Couldn't delete the Fixture from the context. The id of the Fixture is different than the one stored in the PhysicsContext."); //TODO: expand this.
	}
	else
	throw Exception("Tried to remove a Fixture from the context, but the Fixture has no ID number"); 
}


void PhysicsContext_Impl::remove_joint_from_context(int id)
{
	if(id>=0)
	{
		if( joints[id]->get_id() == id)
		{
			joints[id].reset();
			free_joint_slots.push_back(id);
		}
		else
		throw Exception("Couldn't delete the Joint from the context. The id of the Joint is different than the one stored in the PhysicsContext."); //TODO: expand this.
	}
	else
	throw Exception("Tried to remove a Joint from the context, but the Joint has no ID number"); 
}

void PhysicsContext_Impl::check_body(int id)
{
	if(id>=0)
	{
		if(bodies[id].get())
		{
			if( bodies[id]->get_id() == id)
			{
				if (bodies[id].use_count()==1)
				{
					bodies[id].reset();
					free_body_slots.push_back(id);
				}
			}
			else
			throw Exception("Couldn't check the Body in the context. The id of the Body is different than the one stored in the PhysicsContext."); //TODO: expand this.
		}
	}
	else
	throw Exception("Tried to check a Body from the context, but the Body has no ID number"); 
}
void PhysicsContext_Impl::check_fixture(int id)
{
	if(id>=0)
	{
		if(fixtures[id].get())
		{
			if( fixtures[id]->get_id() == id)
			{
				if (fixtures[id].use_count()==1)
				{
					fixtures[id].reset();
					free_fixture_slots.push_back(id);
				}
			}
			else
			throw Exception("Couldn't check the Fixture in the context. The id of the Fixture is different than the one stored in the PhysicsContext."); //TODO: expand this.
		}
	}
	else
	throw Exception("Tried to check a Fixture from the context, but the Fixture has no ID number"); 
}
void PhysicsContext_Impl::check_joint(int id)
{
	if(id>=0)
	{
		if(joints[id].get())
		{
			if( joints[id]->get_id() == id)
			{
				if (joints[id].use_count()==1)
				{
					joints[id].reset();
					free_joint_slots.push_back(id);
				}
			}
			else
			throw Exception("Couldn't check the Joint in the context. The id of the Joint is different than the one stored in the PhysicsContext."); //TODO: expand this.
		}
	}
	else
	throw Exception("Tried to check a Joint from the context, but the Joint has no ID number"); 
}

Body PhysicsContext_Impl::get_body_from_context(int id)
{
	if(id>=0 && id<max_body_amount)
	{
		if(bodies[id].get())
		{
			Body body;
			body.impl = bodies[id];
			return body;
		}
	}
	
	Body body;
	body.impl = dummy_body_impl;
	return body;
}
Fixture PhysicsContext_Impl::get_fixture_from_context(int id)
{
	if(id>=0 && id<max_fixture_amount)
	{
		if(fixtures[id].get())
		{
			Fixture fixture;
			fixture.impl = fixtures[id];
			return fixture;
		}
	}
	
	Fixture fixture;
	fixture.impl = dummy_fixture_impl;
	return fixture;
}
Joint PhysicsContext_Impl::get_joint_from_context(int id)
{
	if(id>=0 && id<max_joint_amount)
	{
		if(joints[id].get())
		{
			Joint joint;
			joint.impl = joints[id];
			return joint;
		}
	}
	
	Joint joint;
	//joint.impl = dummy_joint_impl; //Add me.
	return joint;
}

}