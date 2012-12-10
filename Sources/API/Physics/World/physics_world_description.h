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
#include "../../Core/Math/vec2.h"
#include <memory>

namespace clan
{

class Size;
class Rect;
class PhysicsWorld;
class PhysicsWorldDescription_Impl;

class PhysicsWorldDescriptionData
{
public:
	virtual ~PhysicsWorldDescriptionData() { }
};

/// \brief Physics World description class.
///
/// This class allows you to setup a more advanced description when creating a Physics World.
/// \xmlonly !group=Physic/World! !header=physics.h! \endxmlonly
class CL_API_PHYSICS PhysicsWorldDescription
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a world description with default values.
	PhysicsWorldDescription();

	/// \brief Constructs a PhysicsWorldDescription
	/// \param copy = Physics World Description
	PhysicsWorldDescription(const PhysicsWorldDescription &copy);

	virtual ~PhysicsWorldDescription();

/// \}
/// \name Attributes
/// \{
public:

	/// \brief Returns if bodies are allowed to sleep when they come to rest.
	bool is_sleep_enabled() const;

	/// \brief Returns the world gravity
	Vec2f get_gravity() const;

	/// \brief Returns the tick timestep.
	float get_timestep() const;

	/// \brief Returns the amount of velocity iterations on every world tick.
	int get_velocity_iterations() const;

	/// \brief Returns the amount of position iterations on every world tick.
	int get_position_iterations() const;

	/// \brief Return the amount of pixels for 1 meter in the Physics World.
	int get_physic_scale() const;

	/// \brief Returns the object stored in the given data name.
	std::shared_ptr<PhysicsWorldDescriptionData> get_data(const std::string &data_name) const;

/// \}
/// \name Operations
/// \{
public:
	/// \brief Copy assignment operator.
	PhysicsWorldDescription &operator =(const PhysicsWorldDescription &copy);

	/// \brief toggles whether the bodies are able to sleep when they come to rest.
	void set_sleep(bool value = true);

	/// \brief sets the world gravity
	void set_gravity(double xAcceleration, double yAcceleration);

	/// \brief sets the tick timestep.
	void set_timestep(float timestep);

	/// \brief sets the amount of velocity iterations on every world tick.
	void set_velocity_iterations(int iterations);

	/// \brief sets the amount of position iterations on every world tick.
	void set_position_iterations(int iterations);

	/// \brief Store object in description.
	void set_data(const std::string &data_name, const std::shared_ptr<PhysicsWorldDescriptionData> &ptr);

	/// \brief Change the amount of pixels for 1 meter in the Physics World. Simulation works best for objects that are larger than 0.1 meter but smaller than 10 meters.
	void set_physic_scale(int pixels);


/// \}
/// \name Implementation
/// \{
private:
	std::shared_ptr<PhysicsWorldDescription_Impl> impl;
/// \}
	friend class PhysicsWorld_Impl;
};

}

/// \}
