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
class ChainShape_Impl;
class CollisionOutline;

/// \brief Chain Shape class.
///
/// A chain shape is a free form sequence of line segments. The chain has two-sided collision,
/// so you can use inside and outside collision. Therefore, you may use any winding order.
/// Since there may be many vertices, they are allocated using b2Alloc.
/// Connectivity information is used to create smooth collisions.
/// WARNING: The chain will not collide properly if there are self-intersections. 
/// \xmlonly !group=Physic/Collision/Shapes! !header=physics.h! \endxmlonly
class CL_API_PHYSICS ChainShape : public Shape
{
/// \name Construction
/// \{
public:

	/// \brief Constructs a null instance.
	ChainShape();

	/// \brief Constructs a Chain Shape.
	/// 
	/// \param pw = Physics World.
	ChainShape(const PhysicsWorld &pw);

	~ChainShape();

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
	ChainShape &operator =(const ChainShape &copy);

	//add us
	/// \brief Create a loop. This automatically adjusts connectivity. 
	///
	/// \param vertices	= an array of vertices, these are copied
	/// \param count = the vertex count 
	void	create_loop (const Vec2f *vertices,const int count);


	void	create_loop (const CollisionOutline &outline);
	/// \brief Create a chain with isolated end vertices. 
	///
	/// \param vertices	= an array of vertices, these are copied
	/// \param count = the vertex count 
	void 	create_chain (const Vec2f *vertices,const int count);

	/// \brief Establish connectivity to a vertex that precedes the first vertex. Don't call this for loops. 
	void 	set_prev_vertex (const Vec2f &prev_vertex);

	/// \brief Establish connectivity to a vertex that follows the last vertex. Don't call this for loops. 
	void 	set_next_vertex (const Vec2f &next_vertex);
	//b2Shape * 	Clone (b2BlockAllocator *allocator) const
	//int32 	GetChildCount () const
	//void 	GetChildEdge (b2EdgeShape *edge, int32 index) const
	//bool 	TestPoint (const b2Transform &transform, const b2Vec2 &p) const
	//bool 	RayCast (b2RayCastOutput *output, const b2RayCastInput &input, const b2Transform &transform, int32 childIndex) const
	//void 	ComputeAABB (b2AABB *aabb, const b2Transform &transform, int32 childIndex) const
	//void 	ComputeMass (b2MassData *massData, float32 density) const 
	//add us
/// \}
/// \name Implementation
/// \{
private:

	std::shared_ptr<ChainShape_Impl> impl;

/// \}
	friend class FixtureDescription;
};

}

/// \}