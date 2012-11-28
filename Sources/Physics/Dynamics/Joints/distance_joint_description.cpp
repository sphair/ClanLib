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
#include "distance_joint_description_impl.h"
#include "../body_impl.h"
#include "../../World/physic_world_impl.h"
#include "API/Physics/World/physic_world.h"
#include "API/Physics/Dynamics/body.h"
#include "API/Physics/Dynamics/Joints/distance_joint_description.h"

namespace clan
{
//																											_______________________																											
//																											C O N S T R U C T O R S
DistanceJointDescription::DistanceJointDescription()
{
}

DistanceJointDescription::DistanceJointDescription(const PhysicWorld &pw)
: impl(new DistanceJointDescription_Impl(*pw.impl))
{

}

DistanceJointDescription::~DistanceJointDescription()
{

}

//																											___________________																											
//																											A T T R I B U T E S


void DistanceJointDescription::throw_if_null() const
{
	if (!impl)
		throw Exception("DistanceJointDescription is null");
}

//																											___________________																											
//																											O P E R A T I O N S

DistanceJointDescription &DistanceJointDescription::operator =(const DistanceJointDescription &copy)
{
	impl = copy.impl;
	return *this;
}

void DistanceJointDescription::set_bodies(const Body &bodyA, const Body &bodyB, const Vec2f &anchorA, const Vec2f &anchorB)
{
	float scale = impl->owner->physic_scale;
	impl->joint_def.Initialize(	bodyA.impl->body, 
								bodyB.impl->body, 
								b2Vec2(anchorA.x/scale, anchorA.y/scale), 
								b2Vec2(anchorB.x/scale, anchorB.y/scale));
}

void DistanceJointDescription::set_body_a(const Body &body, const Vec2f &anchor)
{
	float scale = impl->owner->physic_scale;
	impl->joint_def.bodyA = body.impl->body;
	impl->joint_def.localAnchorA = b2Vec2(anchor.x/scale, anchor.y/scale);
}
void DistanceJointDescription::set_body_b(const Body &body, const Vec2f &anchor)
{
	float scale = impl->owner->physic_scale;
	impl->joint_def.bodyB = body.impl->body;
	impl->joint_def.localAnchorB = b2Vec2(anchor.x/scale, anchor.y/scale);
}

void DistanceJointDescription::set_damping_ratio(float damping)
{
	impl->joint_def.dampingRatio = damping;
}

void DistanceJointDescription::set_length(float length)
{
	float scale = impl->owner->physic_scale;
	impl->joint_def.length = length/scale;
}

}