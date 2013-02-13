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

#include "../../api_physics2d.h"
#include "../../../Core/Math/vec2.h"
#include <memory>

namespace clan
{

	class PhysicsWorld;
	class Body;
	class MouseJointDescription_Impl;

/// \brief MouseJointDescription class.
///
/// \xmlonly !group=Physic/Dynamics/Joints! !header=physics.h! \endxmlonly
class CL_API_PHYSICS MouseJointDescription
{

/// \name Construction
/// \{
public:
	/// \brief Constructs a null instance.
	MouseJointDescription();

	/// \brief Constructs a MouseJointDescription object.
	///
	/// \param pw = Physics World.
	MouseJointDescription(const PhysicsWorld &pw);

	virtual ~MouseJointDescription();

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


/// \}
/// \name Operations
/// \{

	/// \brief Copy assignment operator.
	MouseJointDescription &MouseJointDescription::operator =(const MouseJointDescription &copy);

	/// \brief Sets two bodies to be connected by the joint.
	///
	/// \param bodyA = First body to be connected.
	/// \param bodyB = Second body to be connected. 
	void set_bodies(const Body &bodyA, const Body &bodyB);

	/// \brief Sets first body to be connected by the joint.
	///
	/// \param body = body to be connected.
	void set_body_a(const Body &body);

	/// \brief Sets second body to be connected by the joint.
	///
	/// \param body = body to be connected.
	void set_body_b(const Body &body);

	/// \brief Sets the target position of the mouse.
	void set_target(const Vec2f &target);

	/// \brief Sets the maximum force the mouse joint is allowed to have.
	void set_max_force(const float value);

	/// \brief Sets the damping ratio. 0 = no damping, 1 = critical damping. 
	void set_damping_ratio(const float damping);

	/// \brief Sets the response speed in Hz.
	void set_frequency(const float frequency);
	
/// \}
/// \name Implementation
/// \{
protected:

	std::shared_ptr<MouseJointDescription_Impl> impl;

/// \}

	friend class MouseJoint;

};

}

/// \}