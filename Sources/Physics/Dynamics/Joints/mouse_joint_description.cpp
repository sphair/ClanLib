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
#include "mouse_joint_description_impl.h"
#include "../body_impl.h"
#include "../../World/physics_world_impl.h"
#include "API/Physics/World/physics_world.h"
#include "API/Physics/Dynamics/body.h"
#include "API/Physics/Dynamics/Joints/mouse_joint_description.h"

namespace clan
{
//																											_______________________																											
//																											C O N S T R U C T O R S
MouseJointDescription::MouseJointDescription()
{
}

MouseJointDescription::MouseJointDescription(const PhysicsWorld &pw)
: impl(new MouseJointDescription_Impl(*pw.impl))
{

}

MouseJointDescription::~MouseJointDescription()
{

}

//																											___________________																											
//																											A T T R I B U T E S


void MouseJointDescription::throw_if_null() const
{
	if (!impl)
		throw Exception("MouseJointDescription is null");
}

//																											___________________																											
//																											O P E R A T I O N S

MouseJointDescription &MouseJointDescription::operator =(const MouseJointDescription &copy)
{
	impl = copy.impl;
	return *this;
}

void MouseJointDescription::set_bodies(const Body &bodyA, const Body &bodyB)
{
	float scale = impl->owner->physic_scale;
	impl->joint_def.bodyA = bodyA.impl->body;
	impl->joint_def.bodyB = bodyB.impl->body;
}

void MouseJointDescription::set_body_a(const Body &body)
{
	impl->joint_def.bodyA = body.impl->body;
}
void MouseJointDescription::set_body_b(const Body &body)
{
	impl->joint_def.bodyB = body.impl->body;
}

void MouseJointDescription::set_target(const Vec2f &target)
{
	float scale = impl->owner->physic_scale;
	impl->joint_def.target = b2Vec2 (target.x/scale, target.y/scale);
}

void MouseJointDescription::set_max_force(const float value)
{
	impl->joint_def.maxForce = value;
}

void MouseJointDescription::set_damping_ratio(float damping)
{
	impl->joint_def.dampingRatio = damping;
}

void MouseJointDescription::set_frequency(const float frequency)
{
	impl->joint_def.frequencyHz = frequency;
}

}