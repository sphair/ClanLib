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

#pragma once

#include <vector>
#include <list>
#include <memory>

namespace clan
{
	class Body;
	class Fixture;
	class Joint;
	class PhysicWorld;

class PhysicsContext_Impl
{
public:

//																						_______________________
//																						C O N S T R U C T O R S
	PhysicsContext_Impl(PhysicWorld &pw);
	virtual ~PhysicsContext_Impl() { return; }


//																						___________________
//																						A T T R I B U T E S


//																						___________________
//																						O P E R A T I O N S
	int create_in_context(Body &body);
	int create_in_context(Fixture &fixture);
	//void create_in_context(Joint &joint); //TODO

	void remove_from_context(Body &body);
	void remove_from_context(Fixture &fixture);
	//void remove_from_context(Joint &joint){}; //TODO

//																						___________________________
//																						I M P L E M E N T A T I O N
	PhysicWorld *owner;
	const int max_body_amount;
	const int max_fixture_amount;
	const int max_joint_amount;
	std::vector<std::shared_ptr<Body>> bodies;
	std::vector<std::shared_ptr<Fixture>> fixtures;
	std::vector<std::shared_ptr<Joint>> joints;
	std::list<int> free_body_slots;
	std::list<int> free_fixture_slots;
	std::list<int> free_joint_slots;


};

}