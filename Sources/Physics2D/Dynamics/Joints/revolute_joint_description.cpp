/*
**  ClanLib SDK
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

#include "Physics2D/precomp.h"
#include "revolute_joint_description_impl.h"
#include "../body_impl.h"
#include "../../World/physics_world_impl.h"
#include "API/Physics2D/World/physics_world.h"
#include "API/Physics2D/Dynamics/body.h"
#include "API/Physics2D/Dynamics/Joints/revolute_joint_description.h"
#include "API/Core/Math/angle.h"

namespace clan
{
//																											_______________________																											
//																											C O N S T R U C T O R S
RevoluteJointDescription::RevoluteJointDescription()
{
}

RevoluteJointDescription::RevoluteJointDescription(const PhysicsWorld &pw)
: impl(std::make_shared<RevoluteJointDescription_Impl>(*pw.impl))
{

}

RevoluteJointDescription::~RevoluteJointDescription()
{

}

//																											___________________																											
//																											A T T R I B U T E S


void RevoluteJointDescription::throw_if_null() const
{
	if (!impl)
		throw Exception("RevoluteJointDescription is null");
}

bool RevoluteJointDescription::is_motor() const
{
	return impl->joint_def.enableMotor;
}

float RevoluteJointDescription::get_motor_speed() const
{
	return impl->joint_def.motorSpeed;
}

Angle RevoluteJointDescription::get_reference_angle() const
{
	return Angle(impl->joint_def.referenceAngle, angle_radians);
}

Angle RevoluteJointDescription::get_lower_angle() const
{
	return Angle(impl->joint_def.lowerAngle, angle_radians);
}

Angle RevoluteJointDescription::get_upper_angle() const
{
	return Angle(impl->joint_def.upperAngle, angle_radians);
}

//																											___________________																											
//																											O P E R A T I O N S

RevoluteJointDescription &RevoluteJointDescription::operator =(const RevoluteJointDescription &copy)
{
	impl = copy.impl;
	return *this;
}

void RevoluteJointDescription::set_bodies(const Body &bodyA, const Body &bodyB, const Vec2f &anchor)
{
	float scale = impl->owner->physic_scale;
	impl->joint_def.Initialize(	bodyA.impl->body, 
								bodyB.impl->body, 
								b2Vec2(anchor.x/scale, anchor.y/scale));
}

void RevoluteJointDescription::set_body_a(const Body &body, const Vec2f &anchor)
{
	float scale = impl->owner->physic_scale;
	impl->joint_def.bodyA = body.impl->body;
	impl->joint_def.localAnchorA = b2Vec2(anchor.x/scale, anchor.y/scale);
}
void RevoluteJointDescription::set_body_b(const Body &body, const Vec2f &anchor)
{
	float scale = impl->owner->physic_scale;
	impl->joint_def.bodyB = body.impl->body;
	impl->joint_def.localAnchorB = b2Vec2(anchor.x/scale, anchor.y/scale);
}

 
void RevoluteJointDescription::set_as_motor(const bool value)
{
	impl->joint_def.enableMotor = value;
}
void  RevoluteJointDescription::set_motor_speed(const Angle speed)
{
	impl->joint_def.motorSpeed = speed.to_radians();
}

void  RevoluteJointDescription::set_reference_angle(const Angle angle)
{
	impl->joint_def.referenceAngle = angle.to_radians();
}

void  RevoluteJointDescription::set_max_motor_torque(const float value)
{
	impl->joint_def.maxMotorTorque = value;
}

void  RevoluteJointDescription::set_angle_limit(const Angle lower, const Angle upper)
{
	impl->joint_def.lowerAngle = lower.to_radians();
	impl->joint_def.upperAngle = upper.to_radians();
}

void  RevoluteJointDescription::set_lower_angle(const Angle angle)
{
	impl->joint_def.lowerAngle = angle.to_radians();
}

void  RevoluteJointDescription::set_upper_angle(const Angle angle)
{
	impl->joint_def.upperAngle = angle.to_radians();
}



}