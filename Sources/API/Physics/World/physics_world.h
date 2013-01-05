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
#include "../../Core/Signals/signal_v0.h"
#include "../../Core/Signals/signal_v1.h"
#include <memory>

namespace clan
{

class Physic;
class PhysicsWorldDescription;
class PhysicsWorld_Impl;
class PhysicsContext;
class PhysicsQueryAssistant;

/// \brief Top-level world class.
///
/// The world class manages all physics entities, dynamic simulation, and asynchronous queries.
/// The world also contains efficient memory management facilities. 
/// \xmlonly !group=Physic/World! !header=physics.h! \endxmlonly
class CL_API_PHYSICS PhysicsWorld
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a null instance.
	PhysicsWorld();

	/// \brief Constructs a world.
	///
	/// \param description = Structure that describes how to create the Physics World.
	PhysicsWorld(const PhysicsWorldDescription &description);
	

	~PhysicsWorld();
	
/// \}
/// \name Attributes
/// \{
public:

	/// \brief Signal emitted when world's step is called.
	Signal_v1<float> &sig_world_step();

	/// \brief Signal emitted when world's destroyed.
	Signal_v0 &sig_world_destroyed();

	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;
	
	/// \brief Returns the name of the Physics World.
	std::string get_name() const;

	/// \brief Returns the timestep for every world step.
	double get_timestep() const;

	/// \brief Returns the amount of velocity iterations on every world tick.
	int get_velocity_iterations() const;

	/// \brief Returns the amount of position iterations on every world tick.
	int get_position_iterations() const;

	/// \brief Return the amount of pixels for 1 meter in the Physics World.
	int get_physic_scale() const;

	/// \brief Return the Physics Context of the world.
	PhysicsContext &get_pc();

	/// \brief Return the Physics Query Assistant of the world.
	PhysicsQueryAssistant &get_qa();

/// \}
/// \name Operations
/// \{
public:

	/// \brief Simulates one step of the physic simulation
	void step();

	/// \brief Simulates one step of the physic simulation
	///
	/// \param timestep how much time lapse the step is going to simulate.
	/// \param velocity_iterations How many times the velocities of the bodies are going to be iterated. Bigger amount gives better quality at a cost of speed.
	/// \param position_iterations How many times the positions of the bodies are going to be iterated. Bigger amount gives better quality at a cost of speed.
	void step(float timestep, int velocity_iterations = 8, int position_iterations = 3);

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<PhysicsWorld_Impl> impl;
	
/// \}

	friend class PhysicsContext;
	friend class PhysicsQueryAssistant;
	friend class DistanceJointDescription;
	friend class RevoluteJointDescription;
	friend class PrismaticJointDescription;

	friend class PhysicsDebugDraw;
	friend class PolygonShape;
	friend class ChainShape;
	friend class CircleShape;
	friend class EdgeShape;
	
	friend class BodyDescription;
	friend class Body;
	friend class FixtureDescription;
	friend class Fixture;

};

}

/// \}
