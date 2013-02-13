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
#include <memory>
#include "physics_listener.h"
#include "API/Physics/World/physics_world.h"
#include "API/Physics/World/physics_world_description.h"
#include "API/Physics/World/physics_context.h"
#include "API/Physics/World/physics_query_assistant.h"
#include "API/Core/Signals/signal_v0.h"
#include "API/Core/Signals/signal_v1.h"

namespace clan
{
	class BodyDescription;
	class PhysicsWorld;
	class Joint_Impl;
	class Fixture_Impl;
	class Body_Impl;

class PhysicsWorld_Impl : public std::enable_shared_from_this<PhysicsWorld_Impl>
{
public:

//																						_______________________
//																						C O N S T R U C T O R S
/// \name Construction
/// \{
	PhysicsWorld_Impl();

	virtual ~PhysicsWorld_Impl();

//																						___________________																											
//																						A T T R I B U T E S
/// \}
/// \name Attributes
/// \{

	PhysicsWorld get_API() { PhysicsWorld world; world.impl = shared_from_this(); return world; }
	b2Body *get_dummy_body();
	b2Fixture *get_dummy_fixture();

//																						___________________
//																						O P E R A T I O N S
/// \}
/// \name Operations
/// \{
public:
	void create(const PhysicsWorldDescription &description);
	void step();
	void step(float timestep, int velocity_iterations, int position_iterations);

	void create_joint(std::shared_ptr<Joint_Impl> &joint, const b2JointDef &description);
	void create_fixture(std::shared_ptr<Fixture_Impl> &fixture, b2Body *body, const b2FixtureDef &description);
	void create_body(std::shared_ptr<Body_Impl> &body, const b2BodyDef &description);

	void destroy_joint(std::shared_ptr<Joint_Impl> &joint);
	void destroy_fixture(std::shared_ptr<Fixture_Impl> &fixture);
	void destroy_body(std::shared_ptr<Body_Impl> &body);
	
	void safe_destroy_joint(b2Joint *joint, int id);
	void safe_destroy_fixture(b2Fixture *fixture, int id);
	void safe_destroy_body(b2Body *body, int id);

	//Check if the Physics Context's one is the last one alive.
	void check_body(int id);
	void check_fixture(int id);
	void check_joint(int id);
private:

	void remove_joints();
	void remove_fixtures();
	void remove_bodies();

	void check_joints();
	void check_fixtures();
	void check_bodies();

//																						_____________
//																						S I G N A L S
public:
	Signal_v1<float> sig_world_step;
	Signal_v0 sig_world_destroyed;
//																						___________________________
//																						I M P L E M E N T A T I O N
/// \}
/// \name Implementation
/// \{
public:
	
	//Physics World state information

	bool olm_enabled; //If true this state tells the Physics World to remove physics objects that have no API objects pointing to them anymore.

	std::list<int> joints_for_checking;
	std::list<int> fixtures_for_checking;
	std::list<int> bodies_for_checking;

	std::list<b2Joint *> joints_for_destroying;
	std::list<b2Fixture *> fixtures_for_destroying;
	std::list<b2Body *> bodies_for_destroying;

	b2World world;
	PhysicsListener listener;
	PhysicsContext pc;
	PhysicsQueryAssistant assistant;

	float physic_scale; // in pixels per 1 Box2D meter. Defaults at 100.
	float timestep;
	int velocity_iterations;
	int position_iterations;
	std::string name;
/// \}

};

}
