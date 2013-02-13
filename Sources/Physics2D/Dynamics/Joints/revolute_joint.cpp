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
#include "../../Box2D/Box2D.h"
#include "joint_impl.h"
#include "revolute_joint_impl.h"
#include "revolute_joint_description_impl.h"
#include "../../World/physics_world_impl.h"
#include "../../World/physics_context_impl.h"
#include "API/Physics/Dynamics/Joints/revolute_joint_description.h"
#include "API/Physics/Dynamics/Joints/revolute_joint.h"
#include "API/Physics/World/physics_context.h"


namespace clan
{

//																											_______________________																											
//																											C O N S T R U C T O R S
RevoluteJoint::RevoluteJoint()
{

}

RevoluteJoint::RevoluteJoint(PhysicsContext &pc, const RevoluteJointDescription &description)
{
	impl = std::shared_ptr<RevoluteJoint_Impl> (new RevoluteJoint_Impl(pc.impl->get_owner()));

	if(impl->owner_world)
	{
		impl->joint_type = Joint_Distance;
		impl->create_joint(description.impl->joint_def);
	}
	else
	throw Exception("Tried to create a distance joint with a null PhysicsWorld object");

}

RevoluteJoint::~RevoluteJoint()
{

}

//																											___________________																											
//																											A T T R I B U T E S


void RevoluteJoint::throw_if_null() const
{
	if (!impl)
		throw Exception("RevoluteJoint is null");
}

bool RevoluteJoint::is_active() const
{
	return impl->joint->IsActive();
}

//																											___________________																											
//																											O P E R A T I O N S

RevoluteJoint &RevoluteJoint::operator =(const RevoluteJoint &copy)
{
	impl = copy.impl;
	return *this;
}

}