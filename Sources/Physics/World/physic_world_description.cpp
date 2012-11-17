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
#include "API/Physics/World/physic_world_description.h"
#include "physic_world_description_impl.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// PhysicWorldDescription construction:

PhysicWorldDescription::PhysicWorldDescription() 
: impl(new PhysicWorldDescription_Impl)
{
}

PhysicWorldDescription::PhysicWorldDescription(const PhysicWorldDescription &copy) : impl(copy.impl)
{
}

PhysicWorldDescription::~PhysicWorldDescription()
{
}

/////////////////////////////////////////////////////////////////////////////
// PhysicWorldDescription attributes:

std::shared_ptr<PhysicWorldDescriptionData> PhysicWorldDescription::get_data(const std::string &name) const
{
	std::map<std::string, std::shared_ptr<PhysicWorldDescriptionData> >::const_iterator it;
	it = impl->data_objects.find(name);
	if (it != impl->data_objects.end())
		return it->second;
	else
		return std::shared_ptr<PhysicWorldDescriptionData>();
}

/////////////////////////////////////////////////////////////////////////////
// PhysicWorldDescription operations:

PhysicWorldDescription &PhysicWorldDescription::operator =(const PhysicWorldDescription &copy)
{
	impl = copy.impl;
	return *this;
}

void PhysicWorldDescription::set_sleep(bool value)
{
	impl->sleep_enabled = value;
}

void PhysicWorldDescription::set_gravity(double xAcceleration, double yAcceleration)
{
	impl->gravity = b2Vec2(xAcceleration, yAcceleration);
}

void PhysicWorldDescription::set_timestep(float timestep)
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

void PhysicWorldDescription::set_velocity_iterations(int iterations)
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

void PhysicWorldDescription::set_position_iterations(int iterations)
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

void PhysicWorldDescription::set_data(const std::string &name, const std::shared_ptr<PhysicWorldDescriptionData> &ptr)
{
	impl->data_objects[name] = ptr;
}

void PhysicWorldDescription::set_physic_scale(int pixels)
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

/////////////////////////////////////////////////////////////////////////////
// PhysicWorldDescription implementation:

}