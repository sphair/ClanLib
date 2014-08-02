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
#include "joint_impl.h"
#include "joint_description.h"
#include "API/Physics2D/Dynamics/Joints/joint.h"
#include "../../World/physics_world_impl.h"

namespace clan
{
//																											_______________________																											
//																											C O N S T R U C T O R S

Joint_Impl::Joint_Impl(PhysicsWorld_Impl *owner)
: joint(NULL),
  joint_type(Joint_None),
  id(-1),
  joint_occupied(false),
  owner_world(owner)
{

}

Joint_Impl::~Joint_Impl()
{
	if(joint_occupied)
	{
		sig_joint_deletion();
		owner_world->safe_destroy_joint(joint, id);
	}

	return;
}
//																											___________________																											
//																											A T T R I B U T E S


//																											___________________																											
//																											O P E R A T I O N S
void Joint_Impl::create_joint(b2JointDef &joint_def)
{
        std::shared_ptr<Joint_Impl> impl = shared_from_this();
	if(joint_occupied)
	{
		owner_world->destroy_joint(impl);
		sig_joint_deletion();
	}
	else
	{
		joint_occupied = true;
	}

	owner_world->create_joint(impl, joint_def);
	joint->SetUserData(this);
}
void  Joint_Impl::remove_joint()
{
	if(joint_occupied)
	{
	        std::shared_ptr<Joint_Impl> impl = shared_from_this();
		owner_world->destroy_joint(impl);
		//owner_world->world.DestroyJoint(joint);
		joint_occupied = false;
		sig_joint_deletion();
	}
}

}