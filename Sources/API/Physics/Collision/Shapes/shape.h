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

/// \addtogroup clanPhysics_Collision clanPhysics Collision
/// \{
#pragma once

#include "../../api_physics.h"

namespace clan
{

	class Shape_Impl;
/// \brief Shape types.
enum ShapeType
{
	shape_circle	= 0,
	shape_edge		= 1,
	shape_polygon	= 2,
	shape_chain		= 3
};

/// \brief Shape class.
///
/// \xmlonly !group=Physic/Collision/Shapes! !header=physics.h! \endxmlonly
class CL_API_PHYSICS Shape
{

/// \name Construction
/// \{
public:
	/// \brief Constructs a Shape object.
	Shape();

	virtual ~Shape();

/// \}
/// \name Attributes
/// \{
public:

	ShapeType get_shape_type () const;

	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !shape_impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;
/// \}
/// \name Operations
/// \{

	/// \brief Copy operator.
	Shape &Shape::operator =(const Shape &copy);
	//Add us
	// 	Clone the concrete shape
	//virtual Shape	*Clone () const = 0;

	//virtual int 	GetChildCount () const = 0;
	//virtual bool 	TestPoint (const Vec2f &transformation, const Vec2f &point) const = 0;
	
	//virtual bool 	RayCast (b2RayCastOutput *output, const b2RayCastInput &input, const b2Transform &transform, int32 childIndex) const = 0;
	//virtual void 	ComputeAABB (b2AABB *aabb, const b2Transform &xf, int32 childIndex) const = 0;
	//virtual void 	ComputeMass (b2MassData *massData, float32 density) const = 0;
	//Add us

	
/// \}
/// \name Implementation
/// \{
protected:

	std::shared_ptr<Shape_Impl> shape_impl;

/// \}
	friend class FixtureDescription;
};

}

/// \}