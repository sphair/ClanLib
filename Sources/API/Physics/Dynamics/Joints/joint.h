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

	class JointDescription;
	class Joint_Impl;
	class Body;

/// \brief Joint types.
enum JointType
{
	Joint_Distance =	0,
	Joint_Friction =	1,
	Joint_Gear =		2,
	Joint_Mouse =		3,
	Joint_Prismatic =	4,
	Joint_Pulley =		5,
	Joint_Revolute =	6,
	Joint_Rope =		7,
	Joint_Weld =		8,
	Joint_Wheel =		9,
	Joint_None =		1337
};

/// \brief Joint class.
///
/// \xmlonly !group=Physic/Dynamics/Joints! !header=physics.h! \endxmlonly
class CL_API_PHYSICS Joint
{

/// \name Construction
/// \{
public:
	/// \brief Constructs a null instance.
	Joint();

	/// \brief Constructs a Joint object.
	///
	/// \param description = a Joint Description object.
	Joint(const JointDescription &description);

	virtual ~Joint();

/// \}
/// \name Attributes
/// \{
public:

	JointType get_Joint_type () const;

	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !joint_impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Short-cut function to determine if either body is inactive. 
	bool is_active() const;

	/// \brief Get the reaction force on bodyB at the joint anchor in Newtons.
	virtual Vec2f get_reaction_force (float dt);
 
	/// \brief Get the reaction torque on bodyB in N*m. 
	virtual float get_reaction_torque (float dt);
 	
	/// \brief Return the first body attached to this joint.
	virtual Body* get_body_a();

	/// \brief Return the second body attached to this joint.
	virtual Body* get_body_b();

	/// \brief Return the anchor point on first body in world coordinates. 
	virtual Vec2f get_anchor_a();

	/// \brief Return the second body attached to this joint.
	virtual Vec2f get_anchor_b();

	/// \brief Return the world's id of the joint.
	int get_id() const;

/// \}
/// \name Operations
/// \{
public:
	Joint &Joint::operator =(const Joint &copy);

	//Add us
	// 	Clone the concrete Joint
	//virtual Joint	*Clone () const = 0;

	//virtual int 	GetChildCount () const = 0;
	//virtual bool 	TestPoint (const Vec2f &transformation, const Vec2f &point) const = 0;
	
	//virtual bool 	RayCast (b2RayCastOutput *output, const b2RayCastInput &input, const b2Transform &transform, int32 childIndex) const = 0;
	//virtual void 	ComputeAABB (b2AABB *aabb, const b2Transform &xf, int32 childIndex) const = 0;
	//virtual void 	ComputeMass (b2MassData *massData, float32 density) const = 0;
	//Add us
protected:

	virtual std::shared_ptr<Joint> create_null_derived();

	/// \brief Set the world's id of the joint.
	void set_id(int value);
	
/// \}
/// \name Implementation
/// \{
protected:

	std::shared_ptr<Joint_Impl> joint_impl;

/// \}

	friend class PhysicsContext;
	friend class PhysicsContext_Impl;
};

}

/// \}