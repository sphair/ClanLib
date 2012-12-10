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

namespace clan
{

class FixtureDescription_Impl;
class Shape;

/// \brief FixtureDescription class
///
/// \xmlonly !group=Physics/Dynamics! !header=physics.h! \endxmlonly
class CL_API_PHYSICS FixtureDescription
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a null instance.
	FixtureDescription();

	/// \brief Constructs a Fixture Description
	///
	/// \param pw = Physics World
	FixtureDescription(const PhysicsWorld &pw);

	FixtureDescription(const FixtureDescription &copy);
	
	virtual ~FixtureDescription();

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

	/// \Brief Set the shape of the Fixture.
	void set_shape(const Shape &shape);
	
	/// \Brief The friction coefficient, usually in the range [0,1]. 
	void set_friction(const float friction); 

	/// \Brief The restitution (elasticity) usually in the range [0,1].
	void set_restitution(const float restitution);
	
	/// \Brief The density, usually in kg/m^2. 
	void set_density(const float density);

	void set_as_sensor(const bool value = true);

	/// \Brief Contact filtering data.
	void set_filter(); //Add me.

	FixtureDescription &FixtureDescription::operator =(const FixtureDescription &copy);

/// \}
/// \name Implementation
/// \{
private:

	std::shared_ptr<FixtureDescription_Impl> impl;
/// \}
	friend class Fixture;
	friend class Fixture_Impl; // Think this over
	friend class Body;
};

}

/// \}