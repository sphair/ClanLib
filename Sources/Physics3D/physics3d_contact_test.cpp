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
**    Magnus Norddahl
*/

#include "Physics3D/precomp.h"
#include "API/Physics3D/physics3d_contact_test.h"
#include "API/Physics3D/physics3d_object.h"
#include "API/Physics3D/physics3d_world.h"
#include "API/Physics3D/physics3d_shape.h"
#include "API/Core/System/exception.h"
#include "physics3d_contact_test_impl.h"
#include "physics3d_world_impl.h"
#include "physics3d_object_impl.h"
#include "physics3d_shape_impl.h"

namespace clan
{

Physics3DContactTest::Physics3DContactTest()
{
}

Physics3DContactTest::Physics3DContactTest(Physics3DWorld &world)
	: impl(std::make_shared<Physics3DContactTest_Impl>(world.impl.get()))
{
}

bool Physics3DContactTest::is_null() const
{
	return !impl;
}

bool Physics3DContactTest::test(const Physics3DObject &object)
{
	impl->contacts.clear();

	Physics3DContactTest_Impl::AllHitsContactResultCallback callback(impl.get(), object.impl->object.get());
	impl->world->dynamics_world->contactTest(object.impl->object.get(), callback);

	return !impl->contacts.empty();
}

bool Physics3DContactTest::test(const Physics3DShape &shape, const Vec3f &position, const Quaternionf &orientation)
{
	btTransform transform(btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w), btVector3(position.x, position.y, position.z));

	btCollisionObject obj;
	obj.setCollisionShape(shape.impl->shape.get());
	obj.setWorldTransform(transform);

	Physics3DContactTest_Impl::AllHitsContactResultCallback callback(impl.get(), &obj);
	impl->world->dynamics_world->contactTest(&obj, callback);

	return !impl->contacts.empty();
}

int Physics3DContactTest::get_hit_count() const
{
	return impl->contacts.size();
}

Physics3DObject Physics3DContactTest::get_hit_object(int index) const
{
	if (!impl->contacts.empty())
		return Physics3DObject(impl->contacts[index].object->shared_from_this());
	else
		return Physics3DObject();
}

Vec3f Physics3DContactTest::get_hit_position(int index) const
{
	btVector3 &v = impl->contacts[index].hit_position;
	return Vec3f(v.x(), v.y(), v.z());
}

Vec3f Physics3DContactTest::get_hit_normal(int index) const
{
	btVector3 &v = impl->contacts[index].hit_normal;
	return Vec3f(v.x(), v.y(), v.z());
}

float Physics3DContactTest::get_hit_distance(int index) const
{
	return impl->contacts[index].hit_distance;
}


/////////////////////////////////////////////////////////////////////////////

Physics3DContactTest_Impl::Physics3DContactTest_Impl(Physics3DWorld_Impl *world)
	: world(world)
{
}

Physics3DContactTest_Impl::~Physics3DContactTest_Impl()
{
}

}
