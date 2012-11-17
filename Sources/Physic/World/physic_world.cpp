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

#include "Physic/precomp.h"
#include "physic_world_impl.h"
namespace clan
{

//																											_______________________																											
//																											C O N S T R U C T O R S
PhysicWorld::PhysicWorld()
: impl(new PhysicWorld_Impl())
{

}

PhysicWorld::PhysicWorld(const PhysicWorldDescription &description)
: impl(new PhysicWorld_Impl())
{
	impl->create(description);
}

PhysicWorld::~PhysicWorld()
{
}

//																											___________________																											
//																											A T T R I B U T E S
void PhysicWorld::throw_if_null() const
{
	if (!impl)
		throw Exception("PhysicWorld is null");
}

Signal_v1<float> &PhysicWorld::sig_world_step()
{
	return impl->sig_world_step;
}

//																											___________________																											
//																											O P E R A T I O N S

void PhysicWorld::step()
{
	impl->step();
}

void PhysicWorld::step(float timestep, int velocity_iterations, int position_iterations)
{
	impl->step( timestep, velocity_iterations, position_iterations);
}

}
