/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

#include "Physics2D/precomp.h"
#include "physics_world_impl.h"
#include "physics_context_impl.h"
#include "API/Physics2D/World/physics_world.h"


namespace clan
{

//																											_______________________																											
//																											C O N S T R U C T O R S
PhysicsWorld::PhysicsWorld()
{

}

PhysicsWorld::PhysicsWorld(const PhysicsWorldDescription &description)
: impl(new PhysicsWorld_Impl)
{
	impl->pc = PhysicsContext(*this);
	impl->assistant = PhysicsQueryAssistant(*this);

	impl->create(description);

	impl->pc.impl->setup_dummy_objects();
}

PhysicsWorld::~PhysicsWorld()
{
	impl->sig_world_destroyed.invoke();
}

//																											___________________																											
//																											A T T R I B U T E S
void PhysicsWorld::throw_if_null() const
{
	if (!impl)
		throw Exception("PhysicsWorld is null");
}
bool PhysicsWorld::is_olm_enabled() const
{
	return impl->olm_enabled;
}
int PhysicsWorld::get_physic_scale() const
{
	return (int)impl->physic_scale;
}

PhysicsContext &PhysicsWorld::get_pc()
{
	return impl->pc;
}

PhysicsQueryAssistant &PhysicsWorld::get_qa()
{
	return impl->assistant;
}

//																											_____________																							
//																											S I G N A L S

Signal<float> &PhysicsWorld::sig_world_step()
{
	return impl->sig_world_step;
}

Signal<> &PhysicsWorld::sig_world_destroyed()
{
	return impl->sig_world_destroyed;
}

//																											___________________																											
//																											O P E R A T I O N S

void PhysicsWorld::set_olm(const bool value)
{
	impl->olm_enabled = value;
}
void PhysicsWorld::step()
{
	impl->step();
	
}

void PhysicsWorld::step(float timestep, int velocity_iterations, int position_iterations)
{
	impl->step( timestep, velocity_iterations, position_iterations);
}

}
