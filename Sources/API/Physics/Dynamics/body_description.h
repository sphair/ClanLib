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

#include "../api_physics.h"
#include <memory>
#include "../../Core/Math/vec2.h"

namespace clan
{

class BodyDescription_Impl;
class Pointf;
class Angle;
class PhysicsWorld;

/// \brief Body types.
	enum BodyType
	{
		body_static		= 0,
		body_kinematic	= 1,
		body_dynamic	= 2
	};
/// \brief BodyDescription class
///
/// \xmlonly !group=Physics/Dynamics! !header=physics.h! \endxmlonly
class CL_API_PHYSICS BodyDescription
{
public:

/// \name Construction
/// \{
public:

	/// \brief Constructs a null instance.
	BodyDescription();

	/// \brief Constructs a BodyDescription
	///
	/// \param pw   = Physics World
	BodyDescription(const PhysicsWorld &pw);

	/// \brief Constructs a BodyDescription
	///
	/// \param copy = Body Description
	BodyDescription(const BodyDescription &copy);

	virtual ~BodyDescription();

/// \}
/// \name Attributes
/// \{
public:

	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;


/// \}
/// \name Operations
/// \{
public:
	
	/// \brief Copy assignment operator.
	BodyDescription &operator =(const BodyDescription &copy);

	void set_type(const BodyType type);
	void set_position(const Vec2f &position);
	void set_position(const Pointf &position);

	/// \brief Set the Physics World angle of the body.
	void set_angle(const Angle &angle);

	/// \brief Set the linear velocity of the body's origin in Physics World co-ordinates.
	void set_linear_velocity(const Vec2f &velocity);

	/// \brief Set the angular velocity of the body
	void set_angular_velocity(const Angle &velocity);

	void set_linear_damping(const float damping);
	void set_angular_damping(const float damping);
	void allow_sleep(const bool value = true);
	
	/// \brief Should body initially be awake or sleep?
	void set_awake(const bool value = true);

	/// \brief Should this body be prevented from rotating? Useful for characters.
	void set_fixed_rotation(const bool value);

	/// \brief Set the body to act like a bullet. Gives more accuracy for fast moving bodies.
	void set_as_bullet(const bool value = true);

	/// \brief Does the body start out active?
	void set_active(const bool value = true);

	/// \brief Scale the gravity applied to this body.
	void set_gravity_scale(const float scale);

/// \}
/// \name Implementation
/// \{
private:

	std::shared_ptr<BodyDescription_Impl> impl;
/// \}
	friend class Body;
	friend class Body_Impl;
};

}
/// \}