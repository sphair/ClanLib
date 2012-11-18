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

#include "Physics/precomp.h"
#include "physic_world_description_impl.h"
#include "physic_world_impl.h"
#include "API/Physics/Dynamics/body_description.h"


namespace clan
{

//																											_______________________																											
//																											C O N S T R U C T O R S

PhysicWorld_Impl::PhysicWorld_Impl()
:	name				("Unnamed Clanlib Physic World"),
	world				(b2Vec2(0.0f,10.0f)),
	timestep			(1.0f/60.0f),
	velocity_iterations	(8),
	position_iterations (3),
	physic_scale		(100)
{	
	//world.Step(timestep,velocity_iterations,position_iterations); //Doing so inits the world and allows adding bodies to it.
}

//																											___________________																											
//																											O P E R A T I O N S

void PhysicWorld_Impl::create(const PhysicWorldDescription &description)
{
	PhysicWorldDescription_Impl &desc_impl = *description.impl;
	name = desc_impl.name;
	world.SetAllowSleeping(desc_impl.sleep_enabled);
	world.SetGravity(desc_impl.gravity);

}
void PhysicWorld_Impl::step()
{
		world.Step(timestep,velocity_iterations,position_iterations);
		sig_world_step.invoke(timestep);
}
void PhysicWorld_Impl::step(float timestep, int velocity_iterations, int position_iterations)
{
		world.Step(timestep,velocity_iterations,position_iterations);
		sig_world_step.invoke(timestep);
}

b2Body *PhysicWorld_Impl::create_body(const b2BodyDef &description)
{
	//b2BodyDef def = description;
	//def.position = (1.0f/physic_scale) * def.position; //change to a better physic_scale handling

	return world.CreateBody(&description);
}
//																											___________________																											
//																											A T T R I B U T E S

}