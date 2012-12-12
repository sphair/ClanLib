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
#include "physics_query_assistant_impl.h"
#include "../Dynamics/fixture_impl.h"
#include "API/Physics/Dynamics/fixture.h"
#include "API/Core/Math/vec2.h"
#include "API/Core/Math/point.h"

namespace clan
{
PhysicsQueryAssistant_Impl::PhysicsQueryAssistant_Impl(PhysicsWorld &pw)
: owner_world(&pw),
  raycast_result_amount(0),
  max_raycasted_objects(200), //Make it customisable
  raycasted_objects(max_raycasted_objects)
{
}

bool PhysicsQueryAssistant_Impl::ReportFixture(b2Fixture* fixture)
{
	return true;
}

float32 PhysicsQueryAssistant_Impl::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
{
	if(raycast_result_amount >= max_raycasted_objects) return 0;

	Fixture_Impl &fixture_impl = *static_cast<Fixture_Impl *> (fixture->GetUserData());
	Fixture fixture_result(fixture_impl.shared_from_this());

	Pointf pointf(point.x, point.y);
	Vec2f normal_vec(normal.x, normal.y);

	switch(raycast_type)
	{
	case raycast_all:
		raycasted_objects[raycast_result_amount] = RaycastResult(fixture_result, pointf, normal_vec, fraction);
		raycast_result_amount++;
		return 1;

	case raycast_any:
		raycasted_objects[raycast_result_amount] = RaycastResult(fixture_result, pointf, normal_vec, fraction);
		raycast_result_amount++;
		return 0;

	case raycast_first:
		raycasted_objects[0] = RaycastResult(fixture_result, pointf, normal_vec, fraction);
		raycast_result_amount++;
		return fraction;

	default:
		return 0;
	}
	
}

}