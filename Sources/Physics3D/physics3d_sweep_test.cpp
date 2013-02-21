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
**    Magnus Norddahl
*/

#include "Physics3D/precomp.h"
#include "API/Physics3D/physics3d_sweep_test.h"
#include "API/Physics3D/physics3d_object.h"
#include "API/Physics3D/physics3d_shape.h"
#include "API/Physics3D/physics3d_world.h"
#include "API/Core/System/exception.h"
#include "physics3d_sweep_test_impl.h"
#include "physics3d_world_impl.h"
#include "physics3d_shape_impl.h"

namespace clan
{

Physics3DSweepTest::Physics3DSweepTest()
{
}

Physics3DSweepTest::Physics3DSweepTest(Physics3DWorld &world)
	: impl(new Physics3DSweepTest_Impl(world.impl.get()))
{
}

bool Physics3DSweepTest::is_null() const
{
	return !impl;
}

bool Physics3DSweepTest::test_any_hit(const Physics3DShape &shape, const Vec3f &from_pos, const Quaternionf &from_orientation, const Vec3f &to_pos, const Quaternionf &to_orientation, float allowed_ccd_penetration)
{
	throw Exception("Physics3DSweepTest::test_any_hit not implemented");
}

bool Physics3DSweepTest::test_first_hit(const Physics3DShape &shape, const Vec3f &from_pos, const Quaternionf &from_orientation, const Vec3f &to_pos, const Quaternionf &to_orientation, float allowed_ccd_penetration)
{
	btConvexShape *convex_shape = dynamic_cast<btConvexShape*>(shape.impl->shape.get());
	if (convex_shape == nullptr)
		throw Exception("Sweep testing is only supported for convex shapes");

	impl->from_pos = from_pos;
	impl->to_pos = to_pos;
	impl->hits.clear();

	btVector3 bt_from_pos(from_pos.x, from_pos.y, from_pos.z);
	btVector3 bt_to_pos(to_pos.x, to_pos.y, to_pos.z);

	btTransform bt_from_transform(btQuaternion(from_orientation.x, from_orientation.y, from_orientation.z, from_orientation.w), bt_from_pos);
	btTransform bt_to_transform(btQuaternion(to_orientation.x, to_orientation.y, to_orientation.z, to_orientation.w), bt_to_pos);

	btCollisionWorld::ClosestConvexResultCallback callback(bt_from_pos, bt_to_pos);
	impl->world->dynamics_world->convexSweepTest(convex_shape, bt_from_transform, bt_to_transform, callback, allowed_ccd_penetration);

	if (callback.hasHit())
	{
		Physics3DSweepTest_Impl::SweepHit hit;
		hit.hit_fraction = callback.m_closestHitFraction;
		hit.hit_normal = Vec3f(callback.m_hitNormalWorld.getX(), callback.m_hitNormalWorld.getY(), callback.m_hitNormalWorld.getZ());
		hit.hit_collision_object = static_cast<Physics3DObject_Impl *>(callback.m_hitCollisionObject->getUserPointer());
		impl->hits.push_back(hit);
	}

	return !impl->hits.empty();
}

bool Physics3DSweepTest::test_all_hits(const Physics3DShape &shape, const Vec3f &from_pos, const Quaternionf &from_orientation, const Vec3f &to_pos, const Quaternionf &to_orientation, float allowed_ccd_penetration)
{
	throw Exception("Physics3DSweepTest::test_all_hits not implemented");
}

int Physics3DSweepTest::get_hit_count() const
{
	return impl->hits.size();
}

float Physics3DSweepTest::get_hit_fraction(int index) const
{
	return impl->hits[index].hit_fraction;
}

Vec3f Physics3DSweepTest::get_hit_position(int index) const
{
	return mix(impl->from_pos, impl->to_pos, impl->hits[index].hit_fraction);
}

Vec3f Physics3DSweepTest::get_hit_normal(int index) const
{
	return impl->hits[index].hit_normal;
}

Physics3DObject Physics3DSweepTest::get_hit_object(int index) const
{
	//Physics3DObject_Impl *object_impl = impl->hits[index].hit_collision_object;
	return Physics3DObject(/*object_impl*/);
}

/////////////////////////////////////////////////////////////////////////////

Physics3DSweepTest_Impl::Physics3DSweepTest_Impl(Physics3DWorld_Impl *world)
	: world(world)
{
}

Physics3DSweepTest_Impl::~Physics3DSweepTest_Impl()
{
}


}
