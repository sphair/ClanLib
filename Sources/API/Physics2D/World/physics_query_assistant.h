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

#include "../api_physics2d.h"
#include "query_result.h"
#include "../../Core/Math/rect.h"


namespace clan
{
	class PhysicsWorld;
	class PhysicsQueryAssistant_Impl;
	class Pointf;

class CL_API_PHYSICS PhysicsQueryAssistant
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a null instance.
	PhysicsQueryAssistant();

	/// \brief Constructs a PhysicsQueryAssistant.
	///
	/// \param pw = Physics World.
	PhysicsQueryAssistant(PhysicsWorld &pw);

	~PhysicsQueryAssistant();

/// \}
/// \name Attributes
/// \{

	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Check if last raycast operation has yeld any results.
	bool has_query_result();

	/// \brief Returns the amount of objects found.
	int get_query_result_amount();

	/// \brief Returns the raycast result.
	QueryResult get_query_result(int id = 0);

	/// \brief An useful method that creates a rect from a given pointf that could be used for querying.
	Rectf prepare_rect(const Pointf &point, const float range = 0.001f);

	/// \brief An useful method that creates a rect from two given ints that could be used for querying.
	Rectf prepare_rect(const int x, const int y, const float range = 0.001f);

	/// \brief An useful method that creates a rect from two given floats that could be used for querying.
	Rectf prepare_rect(const float x, const float y, const float range = 0.001f);


/// \}
/// \name Operations
/// \{

	/// \brief Raycast to find the first object on a given line segment.
	void raycast_first(const Pointf &start, const Pointf &end);

	/// \brief Raycast to find any object on a given line segment.
	void raycast_any(const Pointf &start, const Pointf &end);

	/// \brief Raycast to find all objects on a given line segment.
	void raycast_all(const Pointf &start, const Pointf &end);

	/// \brief Query the world for any fixture that potentially overlap the provided rectangle. 
	void query_any(const Rectf &rect);

	/// \brief Query the world for some fixtures that potentially overlap the provided rectangle. 
	void query_some(const Rectf &rect, const int amount);

	/// \brief Query the world for all fixtures that potentially overlap the provided rectangle. 
	void query_all(const Rectf &rect);

protected:

	/// \brief Execute raycasting. 
	inline void raycast(const Pointf &start, const Pointf &end);

	/// \brief Execute aabb querying.
	inline void query(const Rectf &rect);

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<PhysicsQueryAssistant_Impl> impl;

/// \}
};

}

/// \}