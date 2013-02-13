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
	
#include "Physics2D/precomp.h"
#include "API/Physics2D/World/physics_world_description.h"
#include "physics_world_description_impl.h"

namespace clan
{

//																											_______________________																											
//																											C O N S T R U C T O R S

PhysicsWorldDescription::PhysicsWorldDescription() 
: impl(new PhysicsWorldDescription_Impl)
{
}

PhysicsWorldDescription::PhysicsWorldDescription(const PhysicsWorldDescription &copy) : impl(copy.impl)
{
}

PhysicsWorldDescription::~PhysicsWorldDescription()
{
}

//																											___________________																											
//																											A T T R I B U T E S


Vec2f PhysicsWorldDescription::get_gravity() const
{
	return Vec2f(impl->gravity.x,impl->gravity.y);
}

float PhysicsWorldDescription::get_timestep() const
{
	return impl->timestep;
}

int PhysicsWorldDescription::get_velocity_iterations() const
{
	return impl->velocity_iterations;
}

int PhysicsWorldDescription::get_position_iterations() const
{
	return impl->position_iterations;
}

int PhysicsWorldDescription::get_physic_scale() const
{
	return impl->physic_scale;
}

std::shared_ptr<PhysicsWorldDescriptionData> PhysicsWorldDescription::get_data(const std::string &name) const
{
	std::map<std::string, std::shared_ptr<PhysicsWorldDescriptionData> >::const_iterator it;
	it = impl->data_objects.find(name);
	if (it != impl->data_objects.end())
		return it->second;
	else
		return std::shared_ptr<PhysicsWorldDescriptionData>();
}

//																											___________________																											
//																											O P E R A T I O N S

PhysicsWorldDescription &PhysicsWorldDescription::operator =(const PhysicsWorldDescription &copy)
{
	impl = copy.impl;
	return *this;
}

void PhysicsWorldDescription::set_sleep(bool value)
{
	impl->sleep_enabled = value;
}

void PhysicsWorldDescription::set_gravity(double xAcceleration, double yAcceleration)
{
	impl->gravity = b2Vec2(xAcceleration, yAcceleration);
}

void PhysicsWorldDescription::set_timestep(float timestep)
{
	if(timestep>0) //Someone might want to go back in the future
	{
		impl->timestep = timestep;
	}
	else
	{
		impl->timestep = 0;
	}

}

void PhysicsWorldDescription::set_velocity_iterations(int iterations)
{
	if(iterations>=0)
	{
		impl->velocity_iterations = iterations;
	}
	else
	{
		impl->velocity_iterations = 8;
	}
}

void PhysicsWorldDescription::set_position_iterations(int iterations)
{
	if(iterations>=0)
	{
		impl->position_iterations = iterations;
	}
	else
	{
		impl->position_iterations = 3;
	}
}

void PhysicsWorldDescription::set_data(const std::string &name, const std::shared_ptr<PhysicsWorldDescriptionData> &ptr)
{
	impl->data_objects[name] = ptr;
}

void PhysicsWorldDescription::set_physic_scale(int pixels)
{
	if(pixels>0)
	{
		impl->physic_scale = (float) pixels;
	}
	else
	{
		impl->physic_scale = 100.0f;
	}
}

void PhysicsWorldDescription::set_olm(const bool value)
{
	impl->olm_enabled = value;
}

}