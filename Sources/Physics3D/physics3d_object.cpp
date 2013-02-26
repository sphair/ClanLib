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
#include "API/Physics3D/physics3d_object.h"
#include "API/Physics3D/physics3d_shape.h"
#include "API/Physics3D/physics3d_world.h"
#include "physics3d_object_impl.h"
#include "physics3d_shape_impl.h"
#include "physics3d_world_impl.h"

namespace clan
{

Physics3DObject::Physics3DObject()
{
}

Physics3DObject::Physics3DObject(Physics3DWorld &world, const Physics3DShape &shape, const Vec3f &position, const Quaternionf &orientation)
	: impl(new Physics3DObject_Impl(world.impl.get()))
{
	btTransform transform(btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w), btVector3(position.x, position.y, position.z));

	impl->shape = shape;
	impl->object.reset(new btCollisionObject());
	impl->object->setCollisionShape(shape.impl->shape.get());
	impl->object->setWorldTransform(transform);

	world.impl->dynamics_world->addCollisionObject(impl->object.get());
}

Vec3f Physics3DObject::get_position() const
{
	btTransform transform = impl->object->getWorldTransform();
	btVector3 origin = transform.getOrigin();
	return Vec3f(origin.getX(), origin.getY(), origin.getZ());
}

Quaternionf Physics3DObject::get_orientation() const
{
	btTransform transform = impl->object->getWorldTransform();
	btQuaternion rotation = transform.getRotation();
	return Quaternionf(rotation.getW(), rotation.getX(), rotation.getY(), rotation.getZ());
}

bool Physics3DObject::is_kinematic() const
{
	return (impl->object->getCollisionFlags() & btCollisionObject::CF_KINEMATIC_OBJECT) == btCollisionObject::CF_KINEMATIC_OBJECT;
}

void Physics3DObject::set_position(const Vec3f &position)
{
	btTransform transform = impl->object->getWorldTransform();
	transform.setOrigin(btVector3(position.x, position.y, position.z));
	impl->object->setWorldTransform(transform);
}

void Physics3DObject::set_orientation(const Quaternionf &orientation)
{
	btTransform transform = impl->object->getWorldTransform();
	transform.setRotation(btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w));
	impl->object->setWorldTransform(transform);
}

void Physics3DObject::set_transform(const Vec3f &position, const Quaternionf &orientation)
{
	btTransform transform(btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w), btVector3(position.x, position.y, position.z));
	impl->object->setWorldTransform(transform);
}

void Physics3DObject::set_kinematic(bool enable)
{
	if (enable)
		impl->object->setCollisionFlags(impl->object->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	else
		impl->object->setCollisionFlags(impl->object->getCollisionFlags() & (~btCollisionObject::CF_KINEMATIC_OBJECT));
}

UserDataOwner *Physics3DObject::get_userdata_owner()
{
	return &impl->userdata_owner;
}

///////////////////////////////////////////////////////////////////////////

Physics3DObject_Impl::Physics3DObject_Impl(Physics3DWorld_Impl *world)
	: world(world)
{
}

Physics3DObject_Impl::~Physics3DObject_Impl()
{
	if (object != nullptr)
	{
		world->dynamics_world->removeCollisionObject(object.get());
	}
}

}
