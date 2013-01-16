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
#include <list>
#include "API/Physics/Dynamics/fixture.h"
#include "API/Physics/Dynamics/body.h"
#include "API/Physics/Dynamics/body_description.h"
#include "API/Core/Signals/signal_v0.h"
#include "API/Core/Signals/signal_v1.h"

namespace clan
{

class PhysicsWorld_Impl;
class PhysicsObject;

class Body_Impl : public std::enable_shared_from_this<Body_Impl>
{
//																						_______________________
//																						C O N S T R U C T O R S
public:
	Body_Impl(PhysicsWorld_Impl *owner);
	~Body_Impl();
//																						___________________
//																						A T T R I B U T E S
	int get_id() const {return id;}

//																						___________________
//																						O P E R A T I O N S
	void add_fixture(Fixture &fixture);
	void set_id(int value) { id = value;}

	void create_body(const BodyDescription &description);
	void remove_body();
	void remove_body_safetly();
	void remove_fixtures();
	void on_begin_collision(Body_Impl &body);
	void on_end_collision(Body_Impl &body);
	//b2Fixture *create_fixture(b2FixtureDef &description); //obsolete
//																						_____________
//																						S I G N A L S

	Signal_v1 <Body> sig_begin_collision;
	Signal_v1 <Body> sig_end_collision;
	Signal_v0 sig_body_deletion;
	//Maybe fixture attachment signal?

//																						___________________________
//																						I M P L E M E N T A T I O N
public:
	PhysicsObject *data;
	std::list<Fixture> owned_fixtures;
	int id;
	PhysicsWorld_Impl *owner_world;
	b2Body *body;
	bool body_occupied;

};

}