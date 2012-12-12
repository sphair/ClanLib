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
#include "API/Physics/World/physics_query_assistant.h"
#include "API/Physics/World/physics_world.h"
#include "physics_query_assistant_impl.h"
#include "physics_world_impl.h"
#include "API/Core/Math/point.h"

namespace clan
{

//																						_______________________
//																						C O N S T R U C T O R S
PhysicsQueryAssistant::PhysicsQueryAssistant()
{
}

PhysicsQueryAssistant::PhysicsQueryAssistant(PhysicsWorld &pw)
: impl(new PhysicsQueryAssistant_Impl(pw))
{

}

PhysicsQueryAssistant::~PhysicsQueryAssistant()
{

}

//																						___________________
//																						A T T R I B U T E S

bool PhysicsQueryAssistant::has_raycast_result()
{
	return impl->raycast_result_amount;
}
int  PhysicsQueryAssistant::get_raycast_result_amount()
{
	return impl->raycast_result_amount;
}

RaycastResult PhysicsQueryAssistant::get_raycast_result(int id)
{
	return impl->raycasted_objects[id];
}
//																						___________________
//																						O P E R A T I O N S
void PhysicsQueryAssistant::RaycastFirst(Pointf &p1, Pointf &p2)
{
	float scale = impl->owner_world->get_physic_scale();
	b2Vec2 p1_;
	b2Vec2 p2_;
	p1_.x=p1.x/scale;
	p1_.y=p1.y/scale;

	p2_.x=p2.x/scale;
	p2_.y=p2.y/scale;

	impl->raycast_type = raycast_first;
	impl->raycast_result_amount = 0;
	impl->owner_world->impl->world.RayCast(impl.get(), p1_, p2_);
}

}