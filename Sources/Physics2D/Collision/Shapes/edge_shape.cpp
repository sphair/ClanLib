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
#include "shape_impl.h"
#include "Edge_shape_impl.h"
#include "API/Physics2D/Collision/Shapes/Edge_shape.h"
#include "API/Physics2D/World/physics_world.h"
#include "API/Physics2D/World/physics_context.h"
#include "API/Core/Math/angle.h"
#include "../../World/physics_world_impl.h"
#include "../../World/physics_context_impl.h"

namespace clan
{
//																											_______________________																											
//																											C O N S T R U C T O R S
EdgeShape::EdgeShape()
{
	
}

EdgeShape::EdgeShape(const PhysicsWorld &pw)
: impl(new EdgeShape_Impl(pw.impl.get()))
{
	shape_impl->shape_type = shape_edge;
	shape_impl->shape = dynamic_cast<b2Shape*> (&impl->shape);
}

EdgeShape::EdgeShape(const PhysicsContext &pc)
: impl(new EdgeShape_Impl(pc.impl->get_owner()))
{
	shape_impl->shape_type = shape_edge;
	shape_impl->shape = dynamic_cast<b2Shape*> (&impl->shape);
}

EdgeShape::~EdgeShape()
{

}

//																											___________________																											
//																											A T T R I B U T E S
void EdgeShape::throw_if_null() const
{
	if (!impl)
		throw Exception("EdgeShape is null");
}

//																											___________________																											
//																											O P E R A T I O N S
EdgeShape &EdgeShape::operator =(const EdgeShape &copy)
{
	impl = copy.impl;
	return *this;
}

void EdgeShape::set (const Vec2f &v1, const Vec2f &v2)
{
	impl->shape.Set(b2Vec2(v1.x, v1.y), b2Vec2(v2.x, v2.y));
}

}