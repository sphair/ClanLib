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

class Canvas;
class PhysicsWorld;
class PhysicsDebugDraw_Impl;

/// \brief Physic Debug Draw flags.
enum DebugFlags
{
	///\brief Draw shapes flag.
    f_shape = 0x0001,

	///\brief Draw joint connections flag.
    f_joint = 0x0002,

   	///\brief Draw axis aligned bonding boxes flag
	f_aabb = 0x0004,

   	///\brief Draw braod-phase pairs flag.
    f_pair	= 0x0008,

   	///\brief Draw center of mass frame flag.
    f_center_of_mass = 0x0010

};
   
/// \brief PhysicsDebugDraw class.
///
/// \xmlonly !group=Physic/World! !header=physics.h! \endxmlonly
class CL_API_PHYSICS PhysicsDebugDraw
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a null instance.
	PhysicsDebugDraw();

	/// \brief Constructs a debug draw object.
	///
	/// \param pw = Physics World.
	PhysicsDebugDraw(const PhysicsWorld &pw);
	
	~PhysicsDebugDraw();
	
/// \}
/// \name Attributes
/// \{
public:

	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Return currently set up draw flags.
	unsigned int get_flags() const;

/// \}
/// \name Operations
/// \{
public:

	/// \brief Draws physic objects on the canvas.
	void draw(Canvas &canvas);

	/// \brief Set draw flags.
	void set_flags(unsigned int flags);

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<PhysicsDebugDraw_Impl> impl;
	
/// \}

};

}

/// \}
