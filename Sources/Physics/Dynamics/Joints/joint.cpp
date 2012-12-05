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
#include "Joint_impl.h"
#include "joint_description.h"
#include "API/Physics/Dynamics/Joints/joint.h"

namespace clan
{
//																											_______________________																											
//																											C O N S T R U C T O R S

Joint::Joint()
: joint_impl(new Joint_Impl)
{

}

Joint::Joint(const JointDescription &description)
: joint_impl(new Joint_Impl)
{

}
Joint::~Joint()
{

}

//																											___________________																											
//																											A T T R I B U T E S


void Joint::throw_if_null() const
{
	if (!joint_impl)
		throw Exception("Joint is null");
}

JointType Joint::get_Joint_type () const
{
	return joint_impl->joint_type;
}

bool Joint::is_active() const
{
	return joint_impl->joint->IsActive();
}

Body *Joint::get_body_a()
{
	return static_cast<Body *> (joint_impl->joint->GetBodyA()->GetUserData());
}

Body *Joint::get_body_b()
{
	return static_cast<Body *> (joint_impl->joint->GetBodyB()->GetUserData());
}
Vec2f Joint::get_anchor_a()
{
	b2Vec2 &vec = joint_impl->joint->GetAnchorA();
	return Vec2f(vec.x, vec.y);
}

Vec2f Joint::get_anchor_b()
{
	b2Vec2 &vec = joint_impl->joint->GetAnchorB();
	return Vec2f(vec.x, vec.y);
}

Vec2f Joint::get_reaction_force (float dt)
{
	b2Vec2 &vec = joint_impl->joint->GetReactionForce(dt);
	return Vec2f(vec.x, vec.y);
}
 
float Joint::get_reaction_torque (float dt)
{
	return joint_impl->joint->GetReactionTorque(dt);
}

//																											___________________																											
//																											O P E R A T I O N S

Joint &Joint::operator =(const Joint &copy)
{
	joint_impl = copy.joint_impl;
	return *this;
}

}