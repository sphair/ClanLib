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
#include "../../../Core/Math/vec2.h"
#include "../../Collision/Shapes/shape.h"

namespace clan
{

class PhysicsWorld;
class PolygonShape_Impl;
class Angle;

/// \brief Polygon Shape class.
///
/// A convex polygon. It is assumed that the interior of the polygon is to the left of each edge.
/// Polygons have a maximum number of vertices equal to b2_maxPolygonVertices.
/// In most cases you should not need many vertices for a convex polygon. 
/// \xmlonly !group=Physic/Collision/Shapes! !header=physics.h! \endxmlonly
class CL_API_PHYSICS PolygonShape : public Shape
{
/// \name Construction
/// \{
public:

	/// \brief Constructs a null instance.
	PolygonShape();

	/// \brief Constructs a Polygon Shape.
	/// 
	/// \param pw = Physics World.
	PolygonShape(const PhysicsWorld &pw);

	~PolygonShape();

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
	PolygonShape &operator =(const PolygonShape &copy);

	//Add us
	//b2Shape * 	Clone (b2BlockAllocator *allocator) const
	//int32 	GetChildCount () const
	//void 	Set (const b2Vec2 *vertices, int32 vertexCount)

	void 	set_as_box (float width, float height);
	void 	set_as_box (float width, float height, const Vec2f &center, Angle &angle);
	//bool 	TestPoint (const b2Transform &transform, const b2Vec2 &p) const
	//bool 	RayCast (b2RayCastOutput *output, const b2RayCastInput &input, const b2Transform &transform, int32 childIndex) const
	//void 	ComputeAABB (b2AABB *aabb, const b2Transform &transform, int32 childIndex) const
	//void 	ComputeMass (b2MassData *massData, float32 density) const
	//int32 	GetVertexCount () const
	//const b2Vec2 & 	GetVertex (int32 index) const
	//Add us

/// \}
/// \name Implementation
/// \{
private:

	std::shared_ptr<PolygonShape_Impl> impl;

/// \}
	friend class FixtureDescription;
};

}

/// \}