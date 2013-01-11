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
#include "../Box2D/Box2D.h"
#include "body_impl.h"
#include "body_description_impl.h"
#include "../World/physics_world_impl.h"

namespace clan
{

//																						_______________________
//																						C O N S T R U C T O R S
Body_Impl::Body_Impl( PhysicsWorld_Impl *owner)
:	body(owner_world->get_dummy_body()),
	body_occupied(false),
	owner_world(owner),
	id(-1)
{

}

Body_Impl::~Body_Impl() 
{ 
	remove_body();
}
//																						___________________
//																						O P E R A T I O N S
void Body_Impl::create_body(const BodyDescription &description)
{	
	if(body_occupied)	//Add proper handling of Physics World in a case of a deletion
	{
		body->GetWorld()->DestroyBody(body);
	}
	else
	{
		body_occupied = true;
	}

	body = owner_world->create_body(description.impl->bodyDef);
	body->SetUserData(this);
}
void Body_Impl::remove_body()
{
	if(body_occupied)
	{
		sig_body_deletion.invoke();

		body->GetWorld()->DestroyBody(body);
		body_occupied = false;
		body = owner_world->get_dummy_body();
		
	}
}

void Body_Impl::on_begin_collision(Body_Impl &body)
{
	Body collision_body;
	collision_body.impl = shared_from_this();

	sig_begin_collision.invoke(collision_body); //Send the body that this body collided with.
}

void Body_Impl::on_end_collision(Body_Impl &body)
{
	Body collision_body;
	collision_body.impl = shared_from_this();

	sig_end_collision.invoke(collision_body); //Send the body that this body collided with.
}
//																						_____________
//																						S I G N A L S

//																						___________________
//																						A T T R I B U T E S

/*
b2Fixture *Body_Impl::create_fixture(b2FixtureDef &description) // obsolete
{
	return body->CreateFixture(&description);
}
*/

}
