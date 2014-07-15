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
#include "API/Physics2D/World/physics_query_assistant.h"
#include "API/Physics2D/World/physics_world.h"
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
: impl(std::make_shared<PhysicsQueryAssistant_Impl>(pw))
{

}

PhysicsQueryAssistant::~PhysicsQueryAssistant()
{

}

//																						___________________
//																						A T T R I B U T E S

bool PhysicsQueryAssistant::has_query_result()
{
	return impl->query_result_amount;
}
int  PhysicsQueryAssistant::get_query_result_amount()
{
	return impl->query_result_amount;
}

QueryResult PhysicsQueryAssistant::get_query_result(int id)
{
	return impl->queried_objects[id];
}

Rectf PhysicsQueryAssistant::prepare_rect(const Pointf &point, const float range)
{
	return Rectf(point.x-range, point.y-range, point.x+range, point.y+range);
}
Rectf PhysicsQueryAssistant::prepare_rect(const int x, const int y, const float range)
{
	return Rectf((float)x-range, (float)y-range, (float)x+range, (float)y+range);
}
Rectf PhysicsQueryAssistant::prepare_rect(const float x, const float y, const float range)
{
	return Rectf(x-range, y-range, x+range, y+range);
}

//																						___________________
//																						O P E R A T I O N S
void PhysicsQueryAssistant::raycast_first(const Pointf &p1, const Pointf &p2)
{
	impl->query_type =  query_raycast_first;
	raycast(p1, p2);
}

void PhysicsQueryAssistant::raycast_any(const Pointf &p1, const Pointf &p2)
{
	impl->query_type =  query_raycast_any;
	raycast(p1, p2);
}

void PhysicsQueryAssistant::raycast_all(const Pointf &p1, const Pointf &p2)
{
	impl->query_type = query_raycast_all;
	raycast(p1, p2);
}

void PhysicsQueryAssistant::raycast(const Pointf &p1, const Pointf &p2)
{
	float scale = impl->owner_world->get_physic_scale();
	b2Vec2 p1_(p1.x/scale, p1.y/scale);
	b2Vec2 p2_(p2.x/scale, p2.y/scale);

	impl->query_result_amount = 0;
	impl->owner_world->impl->world.RayCast(impl.get(), p1_, p2_);
}

void PhysicsQueryAssistant::query_any(const Rectf &rect)
{
	impl->query_type = query_aabb_any;
	query(rect);
}
void PhysicsQueryAssistant::query_some(const Rectf &rect,const int amount)
{
	impl->query_type = query_aabb_some;
	impl->query_result_limit = amount;
	query(rect);
}
void PhysicsQueryAssistant::query_all(const Rectf &rect)
{
	impl->query_type = query_aabb_all;
	query(rect);
}
void PhysicsQueryAssistant::query(const Rectf &rect)
{
	float scale = impl->owner_world->get_physic_scale();
	b2AABB aabb;

	aabb.lowerBound = b2Vec2(rect.left/scale, rect.top/scale);
	aabb.upperBound = b2Vec2(rect.right/scale, rect.bottom/scale);

	impl->query_result_amount = 0;
	impl->owner_world->impl->world.QueryAABB(impl.get(), aabb);
}

}