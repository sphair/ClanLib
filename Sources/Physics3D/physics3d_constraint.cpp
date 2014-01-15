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
#include "API/Physics3D/physics3d_constraint.h"
#include "API/Physics3D/physics3d_object.h"
#include "API/Core/System/exception.h"
#include "API/Core/Math/vec3.h"
#include "API/Core/Math/quaternion.h"
#include "physics3d_constraint_impl.h"
#include "physics3d_object_impl.h"

namespace clan
{

Physics3DConstraint::Physics3DConstraint()
{
}

bool Physics3DConstraint::is_null() const
{
	return !impl;
}

Physics3DConstraint Physics3DConstraint::point_to_point(const Physics3DObject &a, const Vec3f &pivot_in_a)
{
	btRigidBody &body_a = *btRigidBody::upcast(a.impl->object.get());

	Physics3DConstraint constraint;
	constraint.impl = std::shared_ptr<Physics3DConstraint_Impl>(new Physics3DConstraint_Impl());
	constraint.impl->constraint.reset(new btPoint2PointConstraint(body_a, btVector3(pivot_in_a.x, pivot_in_a.y, pivot_in_a.z)));
	constraint.impl->constraint->setUserConstraintPtr(constraint.impl.get());
	return constraint;
}

Physics3DConstraint Physics3DConstraint::point_to_point(const Physics3DObject &a, const Physics3DObject &b, const Vec3f &pivot_in_a, const Vec3f &pivot_in_b)
{
	btRigidBody &body_a = *btRigidBody::upcast(a.impl->object.get());
	btRigidBody &body_b = *btRigidBody::upcast(b.impl->object.get());

	Physics3DConstraint constraint;
	constraint.impl = std::shared_ptr<Physics3DConstraint_Impl>(new Physics3DConstraint_Impl());
	constraint.impl->constraint.reset(new btPoint2PointConstraint(body_a, body_b, btVector3(pivot_in_a.x, pivot_in_a.y, pivot_in_a.z), btVector3(pivot_in_b.x, pivot_in_b.y, pivot_in_b.z)));
	constraint.impl->constraint->setUserConstraintPtr(constraint.impl.get());
	return constraint;
}

Physics3DConstraint Physics3DConstraint::hinge(const Physics3DObject &a, const Vec3f &pivot_in_a, const Quaternionf &axis_in_a, bool use_reference_frame_a)
{
	btRigidBody &body_a = *btRigidBody::upcast(a.impl->object.get());

	btTransform transform_a(btQuaternion(axis_in_a.x, axis_in_a.y, axis_in_a.z, axis_in_a.w), btVector3(pivot_in_a.x, pivot_in_a.y, pivot_in_a.z));

	Physics3DConstraint constraint;
	constraint.impl = std::shared_ptr<Physics3DConstraint_Impl>(new Physics3DConstraint_Impl());
	constraint.impl->constraint.reset(new btHingeConstraint(body_a, transform_a, use_reference_frame_a));
	constraint.impl->constraint->setUserConstraintPtr(constraint.impl.get());
	return constraint;
}

Physics3DConstraint Physics3DConstraint::hinge(const Physics3DObject &a, const Physics3DObject &b, const Vec3f &pivot_in_a, const Vec3f &pivot_in_b, const Quaternionf &axis_in_a, const Quaternionf &axis_in_b, bool use_reference_frame_a)
{
	btRigidBody &body_a = *btRigidBody::upcast(a.impl->object.get());
	btRigidBody &body_b = *btRigidBody::upcast(b.impl->object.get());

	btTransform transform_a(btQuaternion(axis_in_a.x, axis_in_a.y, axis_in_a.z, axis_in_a.w), btVector3(pivot_in_a.x, pivot_in_a.y, pivot_in_a.z));
	btTransform transform_b(btQuaternion(axis_in_b.x, axis_in_b.y, axis_in_b.z, axis_in_b.w), btVector3(pivot_in_b.x, pivot_in_b.y, pivot_in_b.z));

	Physics3DConstraint constraint;
	constraint.impl = std::shared_ptr<Physics3DConstraint_Impl>(new Physics3DConstraint_Impl());
	constraint.impl->constraint.reset(new btHingeConstraint(body_a, body_b, transform_a, transform_b, use_reference_frame_a));
	constraint.impl->constraint->setUserConstraintPtr(constraint.impl.get());
	return constraint;
}

Physics3DConstraint Physics3DConstraint::slider(const Physics3DObject &a, const Physics3DObject &b, const Vec3f &pivot_in_a, const Vec3f &pivot_in_b, const Quaternionf &axis_in_a, const Quaternionf &axis_in_b, bool use_reference_frame_a)
{
	btRigidBody &body_a = *btRigidBody::upcast(a.impl->object.get());
	btRigidBody &body_b = *btRigidBody::upcast(b.impl->object.get());

	btTransform transform_a(btQuaternion(axis_in_a.x, axis_in_a.y, axis_in_a.z, axis_in_a.w), btVector3(pivot_in_a.x, pivot_in_a.y, pivot_in_a.z));
	btTransform transform_b(btQuaternion(axis_in_b.x, axis_in_b.y, axis_in_b.z, axis_in_b.w), btVector3(pivot_in_b.x, pivot_in_b.y, pivot_in_b.z));

	Physics3DConstraint constraint;
	constraint.impl = std::shared_ptr<Physics3DConstraint_Impl>(new Physics3DConstraint_Impl());
	constraint.impl->constraint.reset(new btSliderConstraint(body_a, body_b, transform_a, transform_b, use_reference_frame_a));
	constraint.impl->constraint->setUserConstraintPtr(constraint.impl.get());
	return constraint;
}

Physics3DConstraint Physics3DConstraint::cone_twist(const Physics3DObject &a, const Vec3f &pivot_in_a, const Quaternionf &axis_in_a)
{
	btRigidBody &body_a = *btRigidBody::upcast(a.impl->object.get());

	btTransform transform_a(btQuaternion(axis_in_a.x, axis_in_a.y, axis_in_a.z, axis_in_a.w), btVector3(pivot_in_a.x, pivot_in_a.y, pivot_in_a.z));

	Physics3DConstraint constraint;
	constraint.impl = std::shared_ptr<Physics3DConstraint_Impl>(new Physics3DConstraint_Impl());
	constraint.impl->constraint.reset(new btConeTwistConstraint(body_a, transform_a));
	constraint.impl->constraint->setUserConstraintPtr(constraint.impl.get());
	return constraint;
}

Physics3DConstraint Physics3DConstraint::cone_twist(const Physics3DObject &a, const Physics3DObject &b, const Vec3f &pivot_in_a, const Vec3f &pivot_in_b, const Quaternionf &axis_in_a, const Quaternionf &axis_in_b)
{
	btRigidBody &body_a = *btRigidBody::upcast(a.impl->object.get());
	btRigidBody &body_b = *btRigidBody::upcast(b.impl->object.get());

	btTransform transform_a(btQuaternion(axis_in_a.x, axis_in_a.y, axis_in_a.z, axis_in_a.w), btVector3(pivot_in_a.x, pivot_in_a.y, pivot_in_a.z));
	btTransform transform_b(btQuaternion(axis_in_b.x, axis_in_b.y, axis_in_b.z, axis_in_b.w), btVector3(pivot_in_b.x, pivot_in_b.y, pivot_in_b.z));

	Physics3DConstraint constraint;
	constraint.impl = std::shared_ptr<Physics3DConstraint_Impl>(new Physics3DConstraint_Impl());
	constraint.impl->constraint.reset(new btConeTwistConstraint(body_a, body_b, transform_a, transform_b));
	constraint.impl->constraint->setUserConstraintPtr(constraint.impl.get());
	return constraint;
}

Physics3DConstraint Physics3DConstraint::six_degrees_of_freedom(const Physics3DObject &a, const Physics3DObject &b, const Vec3f &pivot_in_a, const Vec3f &pivot_in_b, const Quaternionf &axis_in_a, const Quaternionf &axis_in_b, bool use_reference_frame_a)
{
	btRigidBody &body_a = *btRigidBody::upcast(a.impl->object.get());
	btRigidBody &body_b = *btRigidBody::upcast(b.impl->object.get());

	btTransform transform_a(btQuaternion(axis_in_a.x, axis_in_a.y, axis_in_a.z, axis_in_a.w), btVector3(pivot_in_a.x, pivot_in_a.y, pivot_in_a.z));
	btTransform transform_b(btQuaternion(axis_in_b.x, axis_in_b.y, axis_in_b.z, axis_in_b.w), btVector3(pivot_in_b.x, pivot_in_b.y, pivot_in_b.z));

	Physics3DConstraint constraint;
	constraint.impl = std::shared_ptr<Physics3DConstraint_Impl>(new Physics3DConstraint_Impl());
	constraint.impl->constraint.reset(new btGeneric6DofConstraint(body_a, body_b, transform_a, transform_b, use_reference_frame_a));
	constraint.impl->constraint->setUserConstraintPtr(constraint.impl.get());
	return constraint;
}

void Physics3DConstraint::set_linear_lower_limit(const Vec3f &limit)
{
	static_cast<btGeneric6DofConstraint*>(impl->constraint.get())->setLinearLowerLimit(btVector3(limit.x, limit.y, limit.z));
}

void Physics3DConstraint::set_linear_upper_limit(const Vec3f &limit)
{
	static_cast<btGeneric6DofConstraint*>(impl->constraint.get())->setLinearUpperLimit(btVector3(limit.x, limit.y, limit.z));
}

/////////////////////////////////////////////////////////////////////////////

Physics3DConstraint_Impl::Physics3DConstraint_Impl()
{
}

Physics3DConstraint_Impl::~Physics3DConstraint_Impl()
{
}

}
