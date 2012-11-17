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

#include "Physic/precomp.h"
#include "Box2D/Box2D.h"
#include "body_description_impl.h"
#include "body_impl.h"
#include "fixture_description_impl.h"
#include "../World/physic_world_impl.h"
#include "API/Physic/World/physic_world.h"
#include "API/Physic/Dynamics/fixture_description.h"
#include "API/Core/Math/angle.h"


namespace clan
{
//																											_______________________																											
//																											C O N S T R U C T O R S
Body::Body()
{
}

Body::Body(PhysicWorld &pw, const BodyDescription &description)
: impl(new Body_Impl())
{
	if(!pw.is_null())
	{
		impl->create_body(pw,description);
	}
	else
	throw Exception("Tried to create a body with a null PhysicWorld object");
}

Body::~Body()
{
}

//																											___________________																											
//																											A T T R I B U T E S
void Body::throw_if_null() const
{
	if (!impl)
		throw Exception("Body is null");
}

Vec2f Body::get_position() const
{
	b2Vec2 vec = impl->body->GetPosition();
	return Vec2f(vec.x, vec.y);
}

Angle Body::get_angle() const
{
	float angle = impl->body->GetAngle();
	impl->body->GetAngle();
	return Angle(angle,angle_radians);
}

//																											___________________																											
//																											O P E R A T I O N S

void Body::add_fixture(const FixtureDescription &description)
{
 	impl->body->CreateFixture(&description.impl->fixtureDef); //Return a fixture object here.

}

}