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
#include "physics_world_description_impl.h"
#include "physics_world_impl.h"
#include "physics_context_impl.h"
#include "API/Physics2D/Dynamics/body_description.h"
#include "../Dynamics/body_impl.h"
#include "../Dynamics/fixture_impl.h"
#include "../Dynamics/Joints/joint_impl.h"


namespace clan
{

//																											_______________________																											
//																											C O N S T R U C T O R S

PhysicsWorld_Impl::PhysicsWorld_Impl()
:	name				("Unnamed Clanlib Physics World"),
	world				(b2Vec2(0.0f,10.0f)),
	timestep			(1.0f/60.0f),
	velocity_iterations	(8),
	position_iterations (3),
	physic_scale		(100),
	olm_enabled			(false)
{	
	world.SetContactListener(&listener); //Uncomment after finishing physics context.
	world.SetContactFilter(&listener); //Uncomment after finishing physics context.
}

PhysicsWorld_Impl::~PhysicsWorld_Impl() 
{ 
	if(joints_for_checking.size()>0) check_joints();
	if(fixtures_for_checking.size()>0) check_fixtures();
	if(bodies_for_checking.size()>0) check_bodies();

	if(joints_for_destroying.size()>0) remove_joints();
	if(fixtures_for_destroying.size()>0) remove_fixtures();
	if(bodies_for_destroying.size()>0) remove_bodies();

	world.SetContactListener(NULL);
	world.SetContactFilter(NULL);

	return; 
}
//																											___________________																											
//																											A T T R I B U T E S

b2Body *PhysicsWorld_Impl::get_dummy_body()
{
	return pc.impl->dummy_body_b2;
}

b2Fixture *PhysicsWorld_Impl::get_dummy_fixture()
{
	return pc.impl->dummy_fixture_b2;
}
//																											___________________																											
//																											O P E R A T I O N S

void PhysicsWorld_Impl::create(const PhysicsWorldDescription &description)
{
	PhysicsWorldDescription_Impl &desc_impl = *description.impl;
	name = desc_impl.name;
	timestep = desc_impl.timestep;
	velocity_iterations = desc_impl.velocity_iterations;
	position_iterations = desc_impl.position_iterations;
	physic_scale = desc_impl.physic_scale;
	olm_enabled = desc_impl.olm_enabled;

	world.SetAllowSleeping(desc_impl.sleep_enabled);
	world.SetGravity(desc_impl.gravity);

}
void PhysicsWorld_Impl::step()
{
	if(joints_for_checking.size()>0) check_joints();
	if(fixtures_for_checking.size()>0) check_fixtures();
	if(bodies_for_checking.size()>0) check_bodies();

	if(joints_for_destroying.size()>0) remove_joints();
	if(fixtures_for_destroying.size()>0) remove_fixtures();
	if(bodies_for_destroying.size()>0) remove_bodies();

	world.Step(timestep,velocity_iterations,position_iterations);
	listener.emit_collision_signals();

	sig_world_step.invoke(timestep);


}
void PhysicsWorld_Impl::step(float timestep, int velocity_iterations, int position_iterations)
{
	if(joints_for_destroying.size()>0) remove_joints();
	if(fixtures_for_destroying.size()>0) remove_fixtures();
	if(bodies_for_destroying.size()>0) remove_bodies();

	world.Step(timestep,velocity_iterations,position_iterations);
	listener.emit_collision_signals();

	sig_world_step.invoke(timestep);
}

void PhysicsWorld_Impl::create_joint(std::shared_ptr<Joint_Impl> &joint, const b2JointDef &description)
{
	pc.impl->create_in_context(joint);
	joint->joint = world.CreateJoint(&description);
}

void PhysicsWorld_Impl::create_fixture(std::shared_ptr<Fixture_Impl> &fixture, b2Body *body, const b2FixtureDef &description)
{
	pc.impl->create_in_context(fixture);
	fixture->fixture = body->CreateFixture(&description);
}

void PhysicsWorld_Impl::create_body(std::shared_ptr<Body_Impl> &body, const b2BodyDef &description)
{
	pc.impl->create_in_context(body);
	body->body = world.CreateBody(&description);
}

void PhysicsWorld_Impl::destroy_joint(std::shared_ptr<Joint_Impl> &joint)
{
	joints_for_destroying.push_back(joint->joint);
	//joint->joint = dummy_joint; //Set dummy joint here.
	pc.impl->remove_from_context(joint);
}
void PhysicsWorld_Impl::destroy_fixture(std::shared_ptr<Fixture_Impl> &fixture)
{
	fixtures_for_destroying.push_back(fixture->fixture);
	fixture->fixture = pc.impl->dummy_fixture_b2;
	pc.impl->remove_from_context(fixture);
}
void PhysicsWorld_Impl::destroy_body(std::shared_ptr<Body_Impl> &body)
{
	bodies_for_destroying.push_back(body->body);
	body->body = pc.impl->dummy_body_b2;
	pc.impl->remove_from_context(body);
}

void PhysicsWorld_Impl::safe_destroy_joint(b2Joint *joint, int id)
{
	joints_for_destroying.push_back(joint);

	//pc.impl->remove_joint_from_context(id);
}
void PhysicsWorld_Impl::safe_destroy_fixture(b2Fixture *fixture, int id)
{
	fixtures_for_destroying.push_back(fixture);

	//pc.impl->remove_fixture_from_context(id);
}
void PhysicsWorld_Impl::safe_destroy_body(b2Body *body, int id)
{
	bodies_for_destroying.push_back(body);

	//pc.impl->remove_body_from_context(id);
}

void PhysicsWorld_Impl::check_joint(int id)
{
	if(olm_enabled)
	joints_for_checking.push_back(id);
}

void PhysicsWorld_Impl::check_fixture(int id)
{
	if(olm_enabled)
	fixtures_for_checking.push_back(id);
}

void PhysicsWorld_Impl::check_body(int id)
{
	if(olm_enabled)
	bodies_for_checking.push_back(id);
}
void PhysicsWorld_Impl::remove_joints()
{
	std::list<b2Joint *>::iterator it;
	for(it = joints_for_destroying.begin() ; it != joints_for_destroying.end() ;)
	{
		world.DestroyJoint(*it);

		it = joints_for_destroying.erase(it);
	}
}

void PhysicsWorld_Impl::remove_fixtures()
{
	std::list<b2Fixture *>::iterator it;
	for(it = fixtures_for_destroying.begin() ; it != fixtures_for_destroying.end() ;)
	{
		(*it)->GetBody()->DestroyFixture(*it);

		it = fixtures_for_destroying.erase(it);
	}
}

void PhysicsWorld_Impl::remove_bodies()
{
	std::list<b2Body *>::iterator it;
	for(it = bodies_for_destroying.begin() ; it != bodies_for_destroying.end() ;)
	{
		world.DestroyBody(*it);

		it = bodies_for_destroying.erase(it);
	}
}

void PhysicsWorld_Impl::check_joints()
{
	std::list<int>::iterator it;
	for(it = joints_for_checking.begin() ; it != joints_for_checking.end() ;)
	{
		pc.impl->check_joint(*it); //Optimise by sending a whole list.

		it = joints_for_checking.erase(it);
	}
}

void PhysicsWorld_Impl::check_fixtures()
{
	std::list<int>::iterator it;
	for(it = fixtures_for_checking.begin() ; it != fixtures_for_checking.end() ;)
	{
		pc.impl->check_fixture(*it); //Optimise by sending a whole list.

		it = fixtures_for_checking.erase(it);
	}
	
}

void PhysicsWorld_Impl::check_bodies()
{
	std::list<int>::iterator it;
	for(it = bodies_for_checking.begin() ; it != bodies_for_checking.end() ;)
	{
		pc.impl->check_body(*it); //Optimise by sending a whole list.

		it = bodies_for_checking.erase(it);
	}
	
}

}