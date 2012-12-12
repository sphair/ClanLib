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
#include "../World/physics_world.h"
#include "../../Core/Math/vec2.h"
#include <memory>
#include "../../Core/Signals/signal_v0.h"
#include "../../Core/Signals/signal_v1.h"

namespace clan
{

class Body;
class FixtureDescription;
class Fixture_Impl;

/// \brief Fixture class
///
/// \xmlonly !group=Physics/Dynamics! !header=physics.h! \endxmlonly
class CL_API_PHYSICS Fixture
{
public:
	/// \brief Constructs a null instance.
	Fixture();

	/// \brief Constructs a World
	///
	/// \param pc = Physics Context of the physics world.
	/// \param body = Body
	/// \param description = Fixture Description
	Fixture(PhysicsContext &pc, Body &body, const FixtureDescription &description);

	~Fixture();

private:
	/// \brief Constructs a Fixture with impl.
	Fixture(std::shared_ptr<Fixture_Impl> impl);

/// \}
/// \name Attributes
/// \{
public:

	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Check if Fixture is a sensor.
	bool is_sensor() const;

	/// \brief Return the world's id of the fixture.
	int get_id() const;

	/// \brief Return the body that this fixture is owned by.
	Body &get_owner_body() const;

/// \}
/// \name Operations
/// \{
public:

	/// \brief Copy operator.
	Fixture &Fixture::operator =(const Fixture &copy);

	/// \brief Set the fixture as a sensor.
	void set_as_sensor (const bool value = true);

	/// \brief Checks given point for collision with the fixture.
	bool test_point (const Vec2f &p) const;

private:
		
	/// \brief set the world's id of the fixture.
	void set_id(int value);

/// \}
/// \name Signals
/// \{
public:
	/// \brief Signal emitted when a collision with other fixture occurs
	///
	/// \return Signal_v1<Fixture>
	Signal_v1<Fixture> &sig_begin_collision();

	/// \brief Signal emitted when a collision with other fixture ends
	///
	/// \return Signal_v1<Fixture>
	Signal_v1<Fixture> &sig_end_collision();

	/// \brief Signal emitted when the fixture is deleted.
	///
	/// \return Signal_v0
	Signal_v0 &sig_fixture_deletion();

/// \}
/// \name Implementation
/// \{
private:

	std::shared_ptr<Fixture_Impl> impl;
/// \}

	friend class Fixture_Impl;
	friend class PhysicsQueryAssistant_Impl;
	friend class PhysicsContext;
};

}
/// \}