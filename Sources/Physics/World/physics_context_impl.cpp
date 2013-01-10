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

#include "Physics/precomp.h"
#include "physics_context_impl.h"
#include "../Dynamics/body_impl.h"
#include "../Dynamics/fixture_impl.h"
#include "../Dynamics/Joints/joint_impl.h"
#include "API/Physics/World/physics_world.h"
#include "API/Core/Text/string_format.h"
#include "physics_world_impl.h"

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
//																						___________________
//																						O P E R A T I O N S

int PhysicsContext_Impl::create_in_context(std::shared_ptr<Body_Impl> body)
{
	if(free_body_slots.size()>0)
	{
		int slot = free_body_slots.front();
		free_body_slots.pop_front();
		bodies[slot] = body;

		return slot;
	}
	else
	throw Exception(string_format("Exceded the current maximum bodies amount that is equal %1.", max_body_amount));
}

int PhysicsContext_Impl::create_in_context(std::shared_ptr<Fixture_Impl> fixture)
{
	if(free_fixture_slots.size()>0)
	{
		int slot = free_fixture_slots.front();
		free_fixture_slots.pop_front();

		fixtures[slot] = fixture;

		return slot;
	}
	else
	throw Exception(string_format("Exceded the current maximum fixtures amount that is equal %1.", max_fixture_amount));

}
int PhysicsContext_Impl::create_in_context(std::shared_ptr<Joint_Impl> joint)
{
		if(free_joint_slots.size()>0)
	{
		int slot = free_joint_slots.front();
		free_joint_slots.pop_front();

		joints[slot] = joint;

		return slot;
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
		}
		else
		throw Exception("Couldn't delete the Joint from the context. The id of the Joint is different than the one stored in the PhysicsContext."); //TODO: expand this.
	}
	else
	throw Exception("Tried to remove a Joint from the context, but the Joint has no ID number"); 
}



}