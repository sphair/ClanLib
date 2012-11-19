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
#include "API/Physics/Dynamics/body.h"
#include "API/Physics/Dynamics/body_description.h"
#include "API/Physics/World/physic_world.h"
#include <memory>
namespace clan
{

class Body_Impl
{
//																						_______________________
//																						C O N S T R U C T O R S
public:
	Body_Impl(PhysicWorld_Impl &pw_impl);

	~Body_Impl() { if(body_occupied) body->GetWorld()->DestroyBody(body); }
//																						___________________
//																						O P E R A T I O N S
	void create_body(const BodyDescription &description);

	//b2Fixture *create_fixture(b2FixtureDef &description); //obsolete
//																						_____________
//																						S I G N A L S

	//Body deletion signal here?
	//Maybe fixture attachment signal?

//																						___________________
//																						A T T R I B U T E S
public:
	PhysicWorld_Impl *owner;
	b2Body *body;
	bool body_occupied;

};

}