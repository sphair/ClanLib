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
#include "body_description_impl.h"
#include "body_impl.h"
#include "fixture_description_impl.h"
#include "../World/physic_world_impl.h"
#include "API/Physics/World/physic_world.h"
#include "API/Physics/Dynamics/fixture_description.h"
#include "API/Physics/Dynamics/body.h"
#include "API/Core/Math/angle.h"


namespace clan
{
//																											_______________________																											
//																											C O N S T R U C T O R S
Body::Body()
{
}

Body::Body(const BodyDescription &description)
: impl(new Body_Impl(*this, *description.impl->owner))
{
	if(impl->owner)
	{
		impl->create_body(description);
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
	float scale = impl->owner_world->physic_scale;
	b2Vec2 vec = impl->body->GetPosition();

	return Vec2f(vec.x*scale, vec.y*scale);
}

Angle Body::get_angle() const
{
	float angle = impl->body->GetAngle();
	impl->body->GetAngle();

	return Angle(angle,angle_radians);
}

Vec2f Body::get_linear_velocity() const
{
	float scale = impl->owner_world->physic_scale;
	b2Vec2 vec = impl->body->GetLinearVelocity();

	return Vec2f(vec.x*scale, vec.y*scale);
}

Angle Body::get_angular_velocity() const
{
	float velocity = impl->body->GetAngularVelocity();

	return Angle(velocity,angle_radians);
}
//																											___________________																											
//																											O P E R A T I O N S
/*
Fixture Body::create_fixture(const FixtureDescription &description)
{
	Fixture fixture(*this,description);
 	
	return fixture;
}
*/
void Body::set_position(const Vec2f &pos)
{
	float scale = impl->owner_world->physic_scale;
	impl->body->SetTransform(b2Vec2(pos.x/scale, pos.y/scale), impl->body->GetAngle());
}

void Body::set_angle(const Angle &angle)
{
	impl->body->SetTransform(impl->body->GetPosition(), angle.to_radians());
}

void Body::set_linear_velocity(const Vec2f &velocity)
{
	float scale = impl->owner_world->physic_scale;
	impl->body->SetLinearVelocity(b2Vec2(velocity.x/scale, velocity.y/scale));
}

void Body::set_angular_velocity(const Angle &velocity)
{
	impl->body->SetAngularVelocity(velocity.to_radians());
}


//																											_____________																										
//																											S I G N A L S

Signal_v1<Body &> &Body::sig_begin_collision()
{
	return impl->sig_begin_collision;
}

Signal_v1<Body &> &Body::sig_end_collision()
{
	return impl->sig_end_collision;
}

Signal_v0 &Body::sig_body_deletion()
{
	return impl->sig_body_deletion;
}

}