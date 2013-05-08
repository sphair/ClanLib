/*
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

#include <vector>
#include <list>
#include <memory>
#include "API/Physics2D/World/physics_context.h"

namespace clan
{
	class Body_Impl;
	class Fixture_Impl;
	class Joint_Impl;
	class PhysicsWorld_Impl;
	class B2Body;
	class B2Fixture;

class PhysicsContext_Impl : public std::enable_shared_from_this<PhysicsContext_Impl>
{
public:

//																						_______________________
//																						C O N S T R U C T O R S
	PhysicsContext_Impl(PhysicsWorld_Impl *owner);
	virtual ~PhysicsContext_Impl() { return; }

	void setup_dummy_objects();
//																						___________________
//																						A T T R I B U T E S
	PhysicsContext get_API() { PhysicsContext pc; pc.impl =  shared_from_this(); return pc; }
	
	PhysicsWorld_Impl *get_owner(){return owner_world;}

	Body get_body_from_context(int id);
	Fixture get_fixture_from_context(int id);
	Joint get_joint_from_context(int id);
//																						___________________
//																						O P E R A T I O N S
	void create_in_context(std::shared_ptr<Body_Impl> body);
	void create_in_context(std::shared_ptr<Fixture_Impl> fixture);
	void create_in_context(std::shared_ptr<Joint_Impl> joint);

	void remove_from_context(std::shared_ptr<Body_Impl> body);
	void remove_from_context(std::shared_ptr<Fixture_Impl> fixture);
	void remove_from_context(std::shared_ptr<Joint_Impl> joint);

	void remove_body_from_context(int id);
	void remove_fixture_from_context(int id);
	void remove_joint_from_context(int id);

	void check_body(int id);
	void check_fixture(int id);
	void check_joint(int id);

//																						___________________________
//																						I M P L E M E N T A T I O N
	PhysicsWorld_Impl *owner_world;

	std::shared_ptr<Body_Impl> dummy_body_impl;
	std::shared_ptr<Fixture_Impl> dummy_fixture_impl;

	b2Body *dummy_body_b2;
	b2Fixture *dummy_fixture_b2;

	bool has_something_to_delete;
	
	const int max_body_amount;
	const int max_fixture_amount;
	const int max_joint_amount;
	std::vector<std::shared_ptr<Body_Impl>> bodies;
	std::vector<std::shared_ptr<Fixture_Impl>> fixtures;
	std::vector<std::shared_ptr<Joint_Impl>> joints;
	std::list<int> free_body_slots;
	std::list<int> free_fixture_slots;
	std::list<int> free_joint_slots;



};

}