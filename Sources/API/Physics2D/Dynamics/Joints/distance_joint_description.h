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

#include "../../api_physics.h"
#include "../../../Core/Math/vec2.h"
#include <memory>

namespace clan
{

	class PhysicsWorld;
	class Body;
	class DistanceJointDescription_Impl;

/// \brief DistanceJointDescription class.
///
/// \xmlonly !group=Physic/Dynamics/Joints! !header=physics.h! \endxmlonly
class CL_API_PHYSICS DistanceJointDescription
{

/// \name Construction
/// \{
public:
	/// \brief Constructs a null instance.
	DistanceJointDescription();

	/// \brief Constructs a DistanceJointDescription object.
	///
	/// \param pw = Physics World.
	DistanceJointDescription(const PhysicsWorld &pw);

	virtual ~DistanceJointDescription();

/// \}
/// \name Attributes
/// \{
public:

	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Short-cut function to determine if either body is inactive. 
	bool is_active() const;
 	
	//Body get_body_a();

	//Body get_body_b();

	//Vec2f get_anchor_a();

	//Vec2f get_anchor_b();

/// \}
/// \name Operations
/// \{

	/// \brief Copy assignment operator.
	DistanceJointDescription &DistanceJointDescription::operator =(const DistanceJointDescription &copy);

	/// \brief Sets two bodies to be connected by the joint.
	///
	/// \param bodyA = First body to be connected.
	/// \param bodyB = Second body to be connected.
	/// \param anchorA = The local anchor point relative to bodyA's origin. 
	/// \param anchorB = The local anchor point relative to bodyB's origin. 
	void set_bodies(const Body &bodyA, const Body &bodyB, const Vec2f &anchorA, const Vec2f &anchorB);

	/// \brief Sets first body to be connected by the joint.
	///
	/// \param body = body to be connected.
	/// \param anchor = The local anchor point relative to body's origin. 
	void set_body_a(const Body &body,const Vec2f &anchor);

	/// \brief Sets second body to be connected by the joint.
	///
	/// \param body = body to be connected.
	/// \param anchor = The local anchor point relative to body's origin. 
	void set_body_b(const Body &body,const Vec2f &anchor);

	/// Sets the damping ratio. 0 = no damping, 1 = critical damping. 
	void set_damping_ratio(const float damping);

	/// Sets the natural length between the anchor points. 
	void set_length(const float length);

	// void set_frequency(const float frequency);
	//Add us
	// 	Clone the concrete DistanceJointDescription
	//virtual DistanceJointDescription	*Clone () const = 0;

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

	std::shared_ptr<DistanceJointDescription_Impl> impl;

/// \}

	friend class DistanceJoint;

};

}

/// \}