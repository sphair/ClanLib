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

	class QueryResult_Impl;
	class Fixture;
	class Body;
	class Pointf;

/// \brief Query types
///
/// \xmlonly !group=Physic/World! !header=physics.h! \endxmlonly
enum QueryResultType
{
	query_raycast,
	query_aabb
};

///
/// Used by physics query assistant.
/// \xmlonly !group=Physic/World! !header=physics.h! \endxmlonly

class CL_API_PHYSICS QueryResult
{
public:



/// \name Construction
/// \{
public:
	/// \brief Constructs a null instance.
	QueryResult();

	~QueryResult();

private:

	/// \brief Constructs a QueryResult object.
	QueryResult(Fixture &fixture);

	/// \brief Constructs a QueryResult object.
	QueryResult(Fixture &fixture, Pointf &point, Vec2f &normal, float fraction);

/// \}
/// \name Attributes
/// \{
public:

	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Get the fixture holded by the QueryResult.
	Fixture &get_fixture() const;

	/// \brief Get the body holded by the QueryResult.
	Body &get_body() const;

	/// =brief Get the query type of the QueryResult.
	QueryResultType get_query_type() const;

/// \}
/// \name Operations
/// \{
public:

	/// \brief Copy operator.
	QueryResult &QueryResult::operator =(const QueryResult &copy);

/// \}
/// \name Implementation
/// \{
private:

	std::shared_ptr<QueryResult_Impl> impl;
/// \}

	friend class PhysicsQueryAssistant_Impl;
};

/// \}

}