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
**    Arkadiusz Kalinowski
*/

#pragma once

#include "../api_physics2d.h"
#include "body_description.h"
#include <memory>
#include "../../Core/Math/vec2.h"
#include "../../Core/Signals/signal.h"

namespace clan
{
/// \addtogroup clanPhysics_Dynamics clanPhysics Dynamics
/// \{

class XMLResourceDocument;
class PhysicsWorld;
class BodyDescription;
class Body_Impl;
class FixtureDescription;
class PhysicsContext;
class PhysicsObject;

/// \brief Body class
class CL_API_PHYSICS Body
{

/// \name Construction
/// \{
public:
	/// \brief Constructs a null instance.
	Body();

	/// \brief Constructs a Body
	///
	/// \param pc = Physics Context
	/// \param description = Body Description
	Body(PhysicsContext &pc, const BodyDescription &description);

	/// \brief Constructs a Body
	///
	/// \param pc = Physics Context
	/// \param resource_id = String Ref
	/// \param resources = Resource Manager
	Body(PhysicsContext &pc, const std::string &resource_id, const XMLResourceDocument &resources);

	~Body();

/// \}
/// \name Attributes
/// \{
public:

	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Get the position of the body.
	Vec2f get_position() const;

	/// \brief Get the angle of the body
	Angle get_angle() const;

	/// \brief Return the linear velocity of the body.
	Vec2f get_linear_velocity() const;

	/// \brief Return the angular velocity of the body.
	Angle get_angular_velocity() const;

	/// \brief Get the gravity scale of the body. 
	float get_gravity_scale();
	
	/// \brief Get the type of this body.
	BodyType get_type() const;

	/// \brief Returns true if the body has fixed rotation. 
	bool has_fixed_rotation() const;

	/// \brief Get the linear damping of the body. 
	float get_linear_damping() const;

	/// \briefGet the angular damping of the body. 
	float get_angular_damping() const;

	/// \brief Return the world's id of the body.
	int get_id() const;

	/// \brief Returns true if this body is a dummy body.
	bool is_dummy() const;

	/// \brief Returns true if this body is not sleeping.
	bool is_awake() const;

	/// \brief Returns true if this body is active in the simulation.
	bool is_active() const;

	/// \brief Returns true if this body is a bullet.
	bool is_bullet() const;

	/// \brief Returns the physics object data.
	PhysicsObject *get_data();
/// \}
/// \name Operations
/// \{
public:

	/// \brief Copy operator.
	Body &operator =(const Body &copy);

	/// \brief add a fixture to the body.
	//void create_fixture(const FixtureDescription &fixture);

	/// \brief set the position of the body.
	void set_position(const Vec2f &pos);

	/// \brief set the angle of the body
	void set_angle(const Angle &angle);

	/// \brief set the linear velocity of the body.
	void set_linear_velocity(const Vec2f &velocity);

	/// \brief set the angular velocity of the body.
	void set_angular_velocity(const Angle &velocity);

	/// \brief Apply a force at a world point.
	///
	///  If the force is not applied at the center of mass, 
	///  it will generate a torque and affect the angular velocity. This wakes up the body.
	/// \param force = the world force vector, usually in Newtons (N).
    /// \param point = the world position of the point of application. 
	void apply_force (const Vec2f &force, const Vec2f &point);

	/// \brief Apply a force to the center of mass.
	///
	/// This wakes up the body.
	void apply_force_to_center (const Vec2f &force);

	/// \brief Apply a force to the center of mass.
	///
	/// This wakes up the body.
	/// \param force = the world force vector, usually in Newtons (N).
	void apply_torque (const float torque);

	/// \brief Apply an impulse at a point.
	///
	/// This immediately modifies the velocity. It also modifies the angular velocity 
	/// if the point of application is not at the center of mass. This wakes up the body. 
	void apply_linear_impulse (const Vec2f &impulse, const Vec2f &point);

	/// \brief Apply an angular impulse.
	///
	/// \param impulse = the angular impulse in units of kg*m*m/s 
	void apply_angular_impulse (const float impulse);

	/// \brief Set the gravity scale of the body. 
	void set_gravity_scale(const float scale);

	/// \brief Set the type of this body. This may alter the mass and velocity.
	void set_type(const BodyType type);

	/// \brief Set this body to be treated like a bullet for continuous collision detection.
	void set_as_bullet(const bool value = true);

	/// \brief Set this body to sleep.
	void set_awake(const bool value = true);

	/// \brief Set this body to be active in the simulation.
	void set_active(const bool value = true);

	/// \brief Set this body to use fixed rotation. This causes the mass to be reset.
	void set_fixed_rotation(const bool value = true);

	/// \brief Set the linear damping of the body. 
	void set_linear_damping(const float damping);

	/// \brief Set the angular damping of the body. 
	void set_angular_damping(const float damping);

	/// \brief This resets the mass properties to the sum of the mass properties of the fixtures. 
	///
	/// Normally it does not need to be called unless you called set_mass_data to override the mass and you later want to reset the mass. 
	void reset_mass_data();

	/// \brief Remove the body from the simulation. The body gets swapped with a dummy body.
	void kill();

	/// \brief Set the physics object object that is going to process collision events.
	void set_data(PhysicsObject *data);

/// \}
/// \name Signals
/// \{
public:
	/// \brief Signal emitted when a collision with other body occurs
	///
	/// \return Signal<void(Body)>
	Signal<void(Body)> &sig_begin_collision();

	/// \brief Signal emitted when a collision with other body ends
	///
	/// \return Signal<void(Body)>
	Signal<void(Body)> &sig_end_collision();

	/// \brief Signal emitted when the body is deleted.
	///
	/// \return Signal<void()>
	Signal<void()> &sig_body_deletion();

/// \}
/// \name Implementation
/// \{
private:

	std::shared_ptr<Body_Impl> impl;
/// \}
	friend class PhysicsContext_Impl;
	
	friend class Fixture;
	friend class Fixture_Impl;
	friend class Body_Impl;
	friend class PhysicsContext;
	friend class PhysicsListener;

	friend class DistanceJointDescription;
	friend class RevoluteJointDescription;
	friend class PrismaticJointDescription;
	friend class MouseJointDescription;

};

}
/// \}