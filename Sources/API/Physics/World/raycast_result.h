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

#include <memory>

#include "../api_physics.h"
#include "../../Core/Math/vec2.h"

namespace clan
{

	class RaycastResult_Impl;
	class Fixture;
	class Body;
	class Pointf;

/// \brief Raycast Result class.
///
/// Used by physics query assistant.
/// \xmlonly !group=Physic/World! !header=physics.h! \endxmlonly
class CL_API_PHYSICS RaycastResult
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a null instance.
	RaycastResult();

	~RaycastResult();

private:
	/// \brief Constructs a RaycastResult object.
	RaycastResult(Fixture &fixture, Pointf &point, Vec2f &normal, float fraction);

/// \}
/// \name Attributes
/// \{
public:

	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Get the fixture holded by the RaycastResult.
	Fixture &get_fixture() const;

	/// \brief Get the body holded by the RaycastResult.
	Body &get_body() const;

/// \}
/// \name Operations
/// \{
public:

	/// \brief Copy operator.
	RaycastResult &RaycastResult::operator =(const RaycastResult &copy);

/// \}
/// \name Implementation
/// \{
private:

	std::shared_ptr<RaycastResult_Impl> impl;
/// \}

	friend class PhysicsQueryAssistant_Impl;
};

/// \}

}