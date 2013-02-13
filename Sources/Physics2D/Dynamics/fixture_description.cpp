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
#include "Physics/Dynamics/fixture_description_impl.h"
#include "API/Physics2D/World/physics_context.h"
#include "API/Physics2D/Dynamics/fixture_description.h"
#include "API/Physics2D/Collision/Shapes/shape.h"
#include "API/Physics2D/Collision/Shapes/polygon_shape.h"
#include "../Collision/Shapes/shape_impl.h"
#include "../World/physics_context_impl.h"

namespace clan
{
//																											_______________________																											
//																											C O N S T R U C T O R S
FixtureDescription::FixtureDescription()
{
}

FixtureDescription::FixtureDescription(const PhysicsWorld &pw)
: impl(new FixtureDescription_Impl(pw.impl.get()))
{

}

FixtureDescription::FixtureDescription(const PhysicsContext &pc)
:impl(new FixtureDescription_Impl(pc.impl->get_owner()))
{

}
FixtureDescription::FixtureDescription(const FixtureDescription &copy)
: impl(copy.impl)
{
}

FixtureDescription::~FixtureDescription()
{
}
//																											___________________																											
//																											A T T R I B U T E S

void FixtureDescription::throw_if_null() const
{
	if (!impl)
		throw Exception("FixtureDescription is null");
}

//																											___________________																											
//																											O P E R A T I O N S

void FixtureDescription::set_shape(const Shape &shape)
{
	impl->fixtureDef.shape = shape.shape_impl->shape; //Extend me.
}
	
void FixtureDescription::set_friction(const float friction)
{
	impl->fixtureDef.friction = friction;
}

void FixtureDescription::set_restitution(const float restitution)
{
	impl->fixtureDef.restitution = restitution;
}

void FixtureDescription::set_density(const float density)
{
	impl->fixtureDef.density = density;
}

void FixtureDescription::set_as_sensor(const bool value)
{
	impl->fixtureDef.isSensor = value;
}

void FixtureDescription::set_filter()
{
	//Add me.
}
	

FixtureDescription &FixtureDescription::operator =(const FixtureDescription &copy)
{
	impl = copy.impl;
	return *this;
}

}