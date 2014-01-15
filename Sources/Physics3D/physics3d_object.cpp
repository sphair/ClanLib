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
#include "API/Physics3D/physics3d_object.h"
#include "API/Physics3D/physics3d_shape.h"
#include "API/Physics3D/physics3d_constraint.h"
#include "API/Physics3D/physics3d_world.h"
#include "physics3d_object_impl.h"
#include "physics3d_shape_impl.h"
#include "physics3d_constraint_impl.h"
#include "physics3d_world_impl.h"

namespace clan
{

Physics3DObject::Physics3DObject()
{
}

Physics3DObject::Physics3DObject(std::shared_ptr<Physics3DObject_Impl> impl)
	: impl(impl)
{
}

Physics3DObject Physics3DObject::collision_body(Physics3DWorld &world, const Physics3DShape &shape, const Vec3f &position, const Quaternionf &orientation)
{
	Physics3DObject instance(std::make_shared<Physics3DObject_Impl>(world.impl.get()));

	btTransform transform(btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w), btVector3(position.x, position.y, position.z));

	instance.impl->shape = shape;
	instance.impl->object.reset(new btCollisionObject());
	instance.impl->object->setUserPointer(instance.impl.get());
	instance.impl->object->setCollisionShape(shape.impl->shape.get());
	instance.impl->object->setWorldTransform(transform);

	world.impl->dynamics_world->addCollisionObject(instance.impl->object.get());

	return instance;
}

Physics3DObject Physics3DObject::rigid_body(Physics3DWorld &world, const Physics3DShape &shape, float mass, const Vec3f &position, const Quaternionf &orientation, const Vec3f &local_inertia)
{
	Physics3DObject instance(std::make_shared<Physics3DObject_Impl>(world.impl.get()));

	btTransform transform(btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w), btVector3(position.x, position.y, position.z));

	Vec3f inertia = local_inertia;
	if (mass != 0.0f && inertia == Vec3f())
	{
		btVector3 bt_inertia;
		shape.impl->shape.get()->calculateLocalInertia(mass, bt_inertia);
		inertia = Vec3f(bt_inertia.x(), bt_inertia.y(), bt_inertia.z());
	}

	instance.impl->shape = shape;
	instance.impl->object.reset(new btRigidBody(mass, 0, shape.impl->shape.get(), btVector3(inertia.x, inertia.y, inertia.z)));
	instance.impl->object->setUserPointer(instance.impl.get());
	instance.impl->object->setWorldTransform(transform);

	world.impl->dynamics_world->addRigidBody(btRigidBody::upcast(instance.impl->object.get()));

	return instance;
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

bool Physics3DObject::is_static() const
{
	return (impl->object->getCollisionFlags() & btCollisionObject::CF_STATIC_OBJECT) == btCollisionObject::CF_STATIC_OBJECT;
}

bool Physics3DObject::is_kinematic() const
{
	return (impl->object->getCollisionFlags() & btCollisionObject::CF_KINEMATIC_OBJECT) == btCollisionObject::CF_KINEMATIC_OBJECT;
}

bool Physics3DObject::is_character_object() const
{
	return (impl->object->getCollisionFlags() & btCollisionObject::CF_CHARACTER_OBJECT) == btCollisionObject::CF_CHARACTER_OBJECT;
}

bool Physics3DObject::is_debug_drawn() const
{
	return (impl->object->getCollisionFlags() & btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT) != btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT;
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

void Physics3DObject::set_static(bool enable)
{
	if (enable)
		impl->object->setCollisionFlags(impl->object->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
	else
		impl->object->setCollisionFlags(impl->object->getCollisionFlags() & (~btCollisionObject::CF_STATIC_OBJECT));
}

void Physics3DObject::set_kinematic(bool enable)
{
	if (enable)
		impl->object->setCollisionFlags(impl->object->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	else
		impl->object->setCollisionFlags(impl->object->getCollisionFlags() & (~btCollisionObject::CF_KINEMATIC_OBJECT));
}

void Physics3DObject::set_character_object(bool enable)
{
	if (enable)
		impl->object->setCollisionFlags(impl->object->getCollisionFlags() | btCollisionObject::CF_CHARACTER_OBJECT);
	else
		impl->object->setCollisionFlags(impl->object->getCollisionFlags() & (~btCollisionObject::CF_CHARACTER_OBJECT));
}

void Physics3DObject::set_debug_drawn(bool enable)
{
	if (!enable)
		impl->object->setCollisionFlags(impl->object->getCollisionFlags() | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);
	else
		impl->object->setCollisionFlags(impl->object->getCollisionFlags() & (~btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT));
}

UserDataOwner *Physics3DObject::get_userdata_owner()
{
	return &impl->userdata_owner;
}

void Physics3DObject::set_mass(float mass, const Vec3f &local_inertia)
{
	btRigidBody *body = btRigidBody::upcast(impl->object.get());
	if (body)
		body->setMassProps(mass, btVector3(local_inertia.x, local_inertia.y, local_inertia.z));
}

void Physics3DObject::set_sleeping_thresholds(float linear, float angular)
{
	btRigidBody *body = btRigidBody::upcast(impl->object.get());
	if (body)
		body->setSleepingThresholds(linear, angular);
}

void Physics3DObject::set_ccd_swept_sphere_radius(float radius)
{
	btRigidBody *body = btRigidBody::upcast(impl->object.get());
	if (body)
		body->setCcdSweptSphereRadius(radius);
}

void Physics3DObject::set_ccd_motion_threshold(float motion_threshold)
{
	btRigidBody *body = btRigidBody::upcast(impl->object.get());
	if (body)
		body->setCcdMotionThreshold(motion_threshold);
}

void Physics3DObject::activate(bool force_activation)
{
	btRigidBody *body = btRigidBody::upcast(impl->object.get());
	if (body)
		body->activate(force_activation);
}

void Physics3DObject::apply_central_force(const Vec3f &force)
{
	btRigidBody *body = btRigidBody::upcast(impl->object.get());
	if (body)
	{
		body->activate();
		body->applyCentralForce(btVector3(force.x, force.y, force.z));
	}
}

void Physics3DObject::apply_torque(const Vec3f &torque)
{
	btRigidBody *body = btRigidBody::upcast(impl->object.get());
	if (body)
	{
		body->activate();
		body->applyTorque(btVector3(torque.x, torque.y, torque.z));
	}
}

void Physics3DObject::apply_force(const Vec3f &force, const Vec3f &relative_pos)
{
	btRigidBody *body = btRigidBody::upcast(impl->object.get());
	if (body)
	{
		body->activate();
		body->applyForce(btVector3(force.x, force.y, force.z), btVector3(relative_pos.x, relative_pos.y, relative_pos.z));
	}
}

void Physics3DObject::apply_central_impulse(const Vec3f &force)
{
	btRigidBody *body = btRigidBody::upcast(impl->object.get());
	if (body)
	{
		body->activate();
		body->applyCentralImpulse(btVector3(force.x, force.y, force.z));
	}
}

void Physics3DObject::apply_torque_impulse(const Vec3f &torque)
{
	btRigidBody *body = btRigidBody::upcast(impl->object.get());
	if (body)
	{
		body->activate();
		body->applyTorqueImpulse(btVector3(torque.x, torque.y, torque.z));
	}
}

void Physics3DObject::apply_impulse(const Vec3f &impulse, const Vec3f &relative_pos)
{
	btRigidBody *body = btRigidBody::upcast(impl->object.get());
	if (body)
	{
		body->activate();
		body->applyImpulse(btVector3(impulse.x, impulse.y, impulse.z), btVector3(relative_pos.x, relative_pos.y, relative_pos.z));
	}
}

void Physics3DObject::clear_forces()
{
	btRigidBody *body = btRigidBody::upcast(impl->object.get());
	if (body)
		body->clearForces();
}

void Physics3DObject::add_constraint(const Physics3DConstraint &constraint)
{
	btRigidBody *body = btRigidBody::upcast(impl->object.get());
	if (body)
		body->addConstraintRef(constraint.impl->constraint.get());
}

void Physics3DObject::remove_constraint(const Physics3DConstraint &constraint)
{
	btRigidBody *body = btRigidBody::upcast(impl->object.get());
	if (body)
		body->removeConstraintRef(constraint.impl->constraint.get());
}

///////////////////////////////////////////////////////////////////////////

Physics3DObject_Impl::Physics3DObject_Impl(Physics3DWorld_Impl *world)
	: world(world)
{
}

Physics3DObject_Impl::~Physics3DObject_Impl()
{
	if (object != 0)
	{
		world->dynamics_world->removeCollisionObject(object.get());
	}
}

}
