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
#include "../World/physics_world_impl.h"
#include "../World/physics_context_impl.h"
#include "API/Physics/Dynamics/body_description.h"
#include "API/Physics/World/physics_world.h"
#include "API/Core/Math/point.h"
#include "API/Core/Math/angle.h"

namespace clan
{
//																											_______________________																											
//																											C O N S T R U C T O R S

BodyDescription::BodyDescription()
{

}
BodyDescription::BodyDescription(const PhysicsWorld &pw)
: impl(new BodyDescription_Impl(pw.impl.get()))
{

}

BodyDescription::BodyDescription(const PhysicsContext &pc)
: impl(new BodyDescription_Impl(pc.impl->get_owner()))
{

}

BodyDescription::BodyDescription(const BodyDescription &copy)
: impl(copy.impl)
{
}

BodyDescription::~BodyDescription()
{
}

//																											___________________																											
//																											A T T R I B U T E S

//																											___________________																											
//																											O P E R A T I O N S
BodyDescription &BodyDescription::operator =(const BodyDescription &copy)
{
	impl = copy.impl;
	return *this;
}

void BodyDescription::throw_if_null() const
{
	if (!impl)
	throw Exception("BodyDescription is null");
}

void BodyDescription::set_type(const BodyType type)
{
	b2BodyType body_type;
	switch(type)
	{
		case body_dynamic:
		body_type = b2_dynamicBody;
		break;
		case body_kinematic:
		body_type = b2_kinematicBody;
		break;
		case body_static:
		body_type = b2_staticBody;
		break;
	}

	impl->bodyDef.type = body_type;
		
}

void BodyDescription::set_position(const Vec2f &position)
{
	float scale = impl->owner_world->physic_scale;
	impl->bodyDef.position = b2Vec2(position.x/scale, position.y/scale);
}
void BodyDescription::set_position(const Pointf &position)
{
	float scale = impl->owner_world->physic_scale;
	impl->bodyDef.position = b2Vec2(position.x/scale, position.y/scale);
}

void BodyDescription::set_angle(const Angle &angle)
{
	impl->bodyDef.angle = angle.to_radians();
}

void BodyDescription::set_linear_velocity(const Vec2f &velocity)
{
	float scale = impl->owner_world->physic_scale;
	impl->bodyDef.linearVelocity = b2Vec2(velocity.x/scale, velocity.y/scale);
}

void BodyDescription::set_angular_velocity(const Angle &velocity)
{
	impl->bodyDef.angularVelocity = velocity.to_radians();
}

void BodyDescription::set_linear_damping(const float damping)
{
	impl->bodyDef.linearDamping = damping;
}
void BodyDescription::set_angular_damping(const float damping)
{
	impl->bodyDef.angularDamping = damping;
}
void BodyDescription::allow_sleep(const bool value)
{
	impl->bodyDef.allowSleep = value;
}
	
void BodyDescription::set_awake(const bool value)
{
	impl->bodyDef.awake = value;
}

void BodyDescription::set_fixed_rotation(const bool value)
{
	impl->bodyDef.fixedRotation = value;
}

void BodyDescription::set_as_bullet(const bool value)
{
	impl->bodyDef.bullet = value;
}

void BodyDescription::set_active(const bool value)
{
	impl->bodyDef.active = value;
}

void BodyDescription::set_gravity_scale(const float scale)
{
	impl->bodyDef.gravityScale = scale;
}

}