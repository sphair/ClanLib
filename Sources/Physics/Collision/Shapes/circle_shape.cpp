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
#include "shape_impl.h"
#include "circle_shape_impl.h"
#include "API/Physics/Collision/Shapes/Circle_shape.h"
#include "API/Physics/World/physic_world.h"
#include "../../World/physic_world_impl.h"
#include "API/Core/Math/angle.h"

namespace clan
{
//																											_______________________																											
//																											C O N S T R U C T O R S
CircleShape::CircleShape()
{
	
}

CircleShape::CircleShape(const PhysicWorld &pw)
: impl(new CircleShape_Impl(*pw.impl))
{
	shape_impl->shape_type = shape_circle;
	shape_impl->shape = dynamic_cast<b2Shape*> (&impl->shape);
}

CircleShape::~CircleShape()
{

}

//																											___________________																											
//																											A T T R I B U T E S
void CircleShape::throw_if_null() const
{
	if (!impl)
		throw Exception("CircleShape is null");
}

//																											___________________																											
//																											O P E R A T I O N S

CircleShape &CircleShape::operator =(const CircleShape &copy)
{
	impl = copy.impl;
	return *this;
}

void CircleShape::set_radius(float radius)
{
	impl->shape.m_radius = radius;
}


}