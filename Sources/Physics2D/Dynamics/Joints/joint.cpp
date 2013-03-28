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

#include "Physics2D/precomp.h"
#include "joint_impl.h"
#include "joint_description.h"
#include "API/Physics2D/Dynamics/Joints/joint.h"
#include "../../World/physics_world_impl.h"

namespace clan
{
//																											_______________________																											
//																											C O N S T R U C T O R S

Joint::Joint()
{

}

Joint::~Joint()
{
	if(impl)
	{
		if(impl->joint_occupied)
		{
			impl->owner_world->check_joint(impl->id);
		}
	}
}

//																											___________________																											
//																											A T T R I B U T E S

void Joint::throw_if_null() const
{
	throw Exception("Joint is null");
}

JointType Joint::get_Joint_type () const
{
	return Joint_None;
}

bool Joint::is_active() const
{
	return false;
}

Body *Joint::get_body_a()
{
	return static_cast<Body *> (impl->joint->GetBodyA()->GetUserData());
}

Body *Joint::get_body_b()
{
	return static_cast<Body *> (impl->joint->GetBodyB()->GetUserData());
}
Vec2f Joint::get_anchor_a()
{
	b2Vec2 vec = impl->joint->GetAnchorA();
	return Vec2f(vec.x, vec.y);
}

Vec2f Joint::get_anchor_b()
{
	b2Vec2 vec = impl->joint->GetAnchorB();
	return Vec2f(vec.x, vec.y);
}

Vec2f Joint::get_reaction_force (float dt)
{
	b2Vec2 vec = impl->joint->GetReactionForce(dt);
	return Vec2f(vec.x, vec.y);
}
 
float Joint::get_reaction_torque (float dt)
{
	return impl->joint->GetReactionTorque(dt);
}

int Joint::get_id() const
{
	return impl->id;
}

bool Joint::is_dummy() const
{
	return !impl->joint_occupied;
}
//																											___________________																											
//																											O P E R A T I O N S

Joint &Joint::operator =(const Joint &copy)
{
	if(impl)
	{
		if(impl->joint_occupied)
		{
			impl->owner_world->check_joint(impl->id);
		}
	}
	
	impl = copy.impl;
	return *this;
	
}


}