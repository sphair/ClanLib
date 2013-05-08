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
#include "prismatic_joint_description_impl.h"
#include "../body_impl.h"
#include "../../World/physics_world_impl.h"
#include "API/Physics2D/World/physics_world.h"
#include "API/Physics2D/Dynamics/body.h"
#include "API/Physics2D/Dynamics/Joints/prismatic_joint_description.h"
#include "API/Core/Math/angle.h"

namespace clan
{
//																											_______________________																											
//																											C O N S T R U C T O R S
PrismaticJointDescription::PrismaticJointDescription()
{
}

PrismaticJointDescription::PrismaticJointDescription(const PhysicsWorld &pw)
: impl(new PrismaticJointDescription_Impl(*pw.impl))
{

}

PrismaticJointDescription::~PrismaticJointDescription()
{

}

//																											___________________																											
//																											A T T R I B U T E S


void PrismaticJointDescription::throw_if_null() const
{
	if (!impl)
		throw Exception("PrismaticJointDescription is null");
}

//																											___________________																											
//																											O P E R A T I O N S

PrismaticJointDescription &PrismaticJointDescription::operator =(const PrismaticJointDescription &copy)
{
	impl = copy.impl;
	return *this;
}

void PrismaticJointDescription::set_bodies(const Body &bodyA, const Body &bodyB, const Vec2f &anchorA, const Vec2f &anchorB)
{
	float scale = impl->owner->physic_scale;
	impl->joint_def.Initialize(	bodyA.impl->body, 
								bodyB.impl->body, 
								b2Vec2(anchorA.x/scale, anchorA.y/scale), 
								b2Vec2(anchorB.x/scale, anchorB.y/scale));
}

void PrismaticJointDescription::set_body_a(const Body &body, const Vec2f &anchor)
{
	float scale = impl->owner->physic_scale;
	impl->joint_def.bodyA = body.impl->body;
	impl->joint_def.localAnchorA = b2Vec2(anchor.x/scale, anchor.y/scale);
}
void PrismaticJointDescription::set_body_b(const Body &body, const Vec2f &anchor)
{
	float scale = impl->owner->physic_scale;
	impl->joint_def.bodyB = body.impl->body;
	impl->joint_def.localAnchorB = b2Vec2(anchor.x/scale, anchor.y/scale);
}

void PrismaticJointDescription::set_axis_a(const Vec2f &axis)
{
	float scale = impl->owner->physic_scale;
	impl->joint_def.localAxisA = b2Vec2(axis.x/scale, axis.y/scale);
}

void PrismaticJointDescription::set_reference_angle(const Angle &angle)
{
	impl->joint_def.referenceAngle = angle.to_radians();
}

void PrismaticJointDescription::enable_limit(const bool value)
{
	impl->joint_def.enableLimit = value;
}
void PrismaticJointDescription::set_translation_limits(const float min, const float max)
{
	float scale = impl->owner->physic_scale;
	impl->joint_def.lowerTranslation = min/scale;
	impl->joint_def.upperTranslation = max/scale;
}
void PrismaticJointDescription::set_translation_min(const float value)
{
	float scale = impl->owner->physic_scale;
	impl->joint_def.lowerTranslation = value/scale;
}
void PrismaticJointDescription::set_translation_max(const float value)
{
	float scale = impl->owner->physic_scale;
	impl->joint_def.upperTranslation = value/scale;
}
void PrismaticJointDescription::set_as_motor(const bool value)
{
	impl->joint_def.enableMotor = value;
}
void PrismaticJointDescription::set_max_motor_force(const float value)
{
	impl->joint_def.maxMotorForce = value;
}
void PrismaticJointDescription::set_motor_speed(const Angle &angle)
{
	impl->joint_def.motorSpeed = angle.to_radians();
}

}