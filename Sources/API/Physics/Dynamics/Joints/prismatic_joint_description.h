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

/// \addtogroup clanPhysics_Dynamics clanPhysics Dynamics
/// \{


#pragma once

#include "../../api_physics.h"
#include "../../../Core/Math/vec2.h"
#include <memory>

namespace clan
{

	class PhysicsWorld;
	class Body;
	class PrismaticJointDescription_Impl;

/// \brief PrismaticJointDescription class.
///
/// \xmlonly !group=Physic/Dynamics/Joints! !header=physics.h! \endxmlonly
class CL_API_PHYSICS PrismaticJointDescription
{

/// \name Construction
/// \{
public:
	/// \brief Constructs a null instance.
	PrismaticJointDescription();

	/// \brief Constructs a PrismaticJointDescription object.
	///
	/// \param pw = Physics World.
	PrismaticJointDescription(const PhysicsWorld &pw);

	virtual ~PrismaticJointDescription();

/// \}
/// \name Attributes
/// \{
public:

	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Short-cut function to determine if either body is inactive. 
	bool is_active() const;
 	
	//Body get_body_a();

	//Body get_body_b();

	//Vec2f get_anchor_a();

	//Vec2f get_anchor_b();

/// \}
/// \name Operations
/// \{

	/// \brief Copy assignment operator.
	PrismaticJointDescription &PrismaticJointDescription::operator =(const PrismaticJointDescription &copy);

	/// \brief Sets two bodies to be connected by the joint.
	///
	/// \param bodyA = First body to be connected.
	/// \param bodyB = Second body to be connected.
	/// \param anchorA = The local anchor point relative to bodyA's origin. 
	/// \param anchorB = The local anchor point relative to bodyB's origin. 
	void set_bodies(const Body &bodyA, const Body &bodyB, const Vec2f &anchorA, const Vec2f &anchorB);

	/// \brief Sets first body to be connected by the joint.
	///
	/// \param body = body to be connected.
	/// \param anchor = The local anchor point relative to body's origin. 
	void set_body_a(const Body &body,const Vec2f &anchor);

	/// \brief Sets second body to be connected by the joint.
	///
	/// \param body = body to be connected.
	/// \param anchor = The local anchor point relative to body's origin. 
	void set_body_b(const Body &body,const Vec2f &anchor);

	/// \brief Sets the local translation unit axis in bodyA.
	///
	/// \param axis = vector representing the translation unit axis.
	void set_axis_a(const Vec2f &axis);

	/// \brief Sets The constrained angle between the bodies: bodyB_angle - bodyA_angle.
	///
	/// \param angle = angle between the bodies.
	void set_reference_angle(const Angle &angle);

	/// Enable/disable the joint limit. 
	void enable_limit(const bool value = true);

	/// \brief Sets the translation limits for the prismatic joint. Usually in meters.
	void set_translation_limits(const float min, const float max);

	/// \brief Sets The lower translation limit, usually in meters. 
	void set_translation_min(const float value);
	
	/// \brief Sets The upper translation limit, usually in meters. 
	void set_translation_max(const float value);

	/// \brief Enable/disable the joint motor. 
	void set_as_motor(const bool value = true);

	/// \brief Sets The maximum motor torque, usually in N-m. 
	void set_max_motor_force(const float value);

	/// \brief Sets the desired motor speed.
	void set_motor_speed(const Angle &angle);

/// \}
/// \name Implementation
/// \{
protected:

	std::shared_ptr<PrismaticJointDescription_Impl> impl;

/// \}

	friend class PrismaticJoint;

};

}

/// \}