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
	class RevoluteJointDescription_Impl;

/// \brief RevoluteJointDescription class.
///
/// \xmlonly !group=Physic/Dynamics/Joints! !header=physics.h! \endxmlonly
class CL_API_PHYSICS RevoluteJointDescription
{

/// \name Construction
/// \{
public:
	/// \brief Constructs a null instance.
	RevoluteJointDescription();

	/// \brief Constructs a RevoluteJointDescription object.
	///
	/// \param pw = Physics World.
	RevoluteJointDescription(const PhysicsWorld &pw);

	virtual ~RevoluteJointDescription();

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
 	
	bool is_motor() const;

	float get_motor_speed() const;

	Angle get_reference_angle() const;

	Angle get_lower_angle() const;

	Angle get_upper_angle() const;

	//Body get_body_a();

	//Body get_body_b();

	//Vec2f get_anchor_a();

	//Vec2f get_anchor_b();

/// \}
/// \name Operations
/// \{

	/// \brief Copy assignment operator.
	RevoluteJointDescription &RevoluteJointDescription::operator =(const RevoluteJointDescription &copy);

	/// \brief Sets two bodies to be connected by the joint.
	///
	/// \param bodyA = First body to be connected.
	/// \param bodyB = Second body to be connected.
	/// \param anchor = The anchor point relative to world. 
	void set_bodies(const Body &bodyA, const Body &bodyB, const Vec2f &anchor);

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

	/// \brief Enables the joint motor. 
	void set_as_motor(const bool value = true);

	/// \brief Apply the desired motor speed. 
	void set_motor_speed(const Angle speed);

	/// \brief The bodyB angle minus bodyA angle in the reference state.
	void set_reference_angle(const Angle angle);

	/// \brief Set the maximum motor torque used to achieve the desired motor speed. Usually in N-m. 
	void set_max_motor_torque(const float value);

	/// \brief Set limits for the angle of the joint.
	void set_angle_limit(const Angle lower, const Angle upper);

	/// \brief The lower angle for the joint limit.
	void set_lower_angle(const Angle angle);

	/// \brief The upper angle for the joint limit.
	void set_upper_angle(const Angle angle);
	
/// \}
/// \name Implementation
/// \{
protected:

	std::shared_ptr<RevoluteJointDescription_Impl> impl;

/// \}

	friend class RevoluteJoint;

};

}

/// \}