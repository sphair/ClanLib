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
**    Magnus Norddahl
*/

#include "Physics3D/precomp.h"
#include "API/Physics3D/physics3d_world.h"
#include "physics3d_world_impl.h"
#include "API/Core/System/exception.h"

namespace clan
{

Physics3DWorld::Physics3DWorld()
	: impl(new Physics3DWorld_Impl())
{
}

bool Physics3DWorld::is_null() const
{
	return !impl;
}

int Physics3DWorld::step_simulation(float time_step, int max_sub_steps, float fixed_time_step)
{
	int num_sub_steps = impl->dynamics_world->stepSimulation(time_step, max_sub_steps, fixed_time_step);
	return num_sub_steps;
}

void Physics3DWorld::step_simulation_once(float time_step)
{
	int steps = impl->dynamics_world->stepSimulation(time_step, 0, time_step);
	if (steps == 0)
		throw Exception("btDynamicsWorld::stepSimulation failed");
}

/////////////////////////////////////////////////////////////////////////////

Physics3DWorld_Impl::Physics3DWorld_Impl()
{
	collision_configuration.reset(new btDefaultCollisionConfiguration());
	dispatcher.reset(new btCollisionDispatcher(collision_configuration.get()));
	broadphase.reset(new btDbvtBroadphase());
	ghost_pair_callback.reset(new btGhostPairCallback());
	broadphase->getOverlappingPairCache()->setInternalGhostPairCallback(ghost_pair_callback.get());
	constraint_solver.reset(new btSequentialImpulseConstraintSolver());
	dynamics_world.reset(new btDiscreteDynamicsWorld(dispatcher.get(), broadphase.get(), constraint_solver.get(), collision_configuration.get()));
	dynamics_world->getDispatchInfo().m_allowedCcdPenetration=0.0001f; // This line is needed by the character controller
}

Physics3DWorld_Impl::~Physics3DWorld_Impl()
{
	// To do: dispose all collision user objects
}

}
