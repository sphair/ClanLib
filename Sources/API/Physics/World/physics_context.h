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

/// \addtogroup clanphysics_world clanPhysics World
/// \{

#pragma once

#include "../api_physics.h"
#include <memory>

namespace clan
{
	class PhysicsWorld;
	class PhysicsContext_Impl;
	class Body_Impl;
	class Fixture_Impl;
	class Joint_Impl;

class CL_API_PHYSICS PhysicsContext
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a null instance.
	PhysicsContext();

	/// \brief Constructs a PhysicsContext.
	///
	/// \param pw = Physics World.
	PhysicsContext(PhysicsWorld &pw);

/// \}
/// \name Attributes
/// \{

	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Returns maximum amount of bodies allowed.
	int max_bodies() const;

	/// \brief Returns maximum amount of fixtures allowed.
	int max_fixtures() const;

	/// \brief Returns maximum amount of joints allowed.
	int max_joints() const;

/// \}
/// \name Operations
/// \{

private:
	/// \brief Add Body to the context.
	void create_in_context(std::shared_ptr<Body_Impl> body);

	/// \brief Add Fixture to the context.
	void create_in_context(std::shared_ptr<Fixture_Impl> fixture);

	/// \brief Add Joint to the context.
	void create_in_context(std::shared_ptr<Joint_Impl> joint);

	/// \brief Remove Body from the context.
	void remove_from_context(std::shared_ptr<Body_Impl> body);

	/// \brief Remove Fixture from the context.
	void remove_from_context(std::shared_ptr<Fixture_Impl> fixture);

	/// \brief Remove Joint from the context.
	void remove_from_context(std::shared_ptr<Joint_Impl> joint);

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<PhysicsContext_Impl> impl;

/// \}

	friend class BodyDescription;
	friend class FixtureDescription;

	friend class Body;
	friend class Fixture;
	friend class Joint;

	friend class DistanceJoint;
	friend class RevoluteJoint;
	friend class PrismaticJoint;
	friend class MouseJoint;
};

}

/// \}