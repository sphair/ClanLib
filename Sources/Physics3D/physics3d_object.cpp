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
#include "API/Physics3D/physics3d_object.h"
#include "physics3d_object_impl.h"

namespace clan
{

Physics3DObject::Physics3DObject()
{
}

Physics3DObject::Physics3DObject(Physics3DWorld &world, const Physics3DShape &shape, const Vec3f &position, const Quaternionf &orientation, const Vec3f &scale)
	: impl(new Physics3DObject_Impl())
{
}

Vec3f Physics3DObject::get_position() const
{
	return Vec3f();
}

Quaternionf Physics3DObject::get_orientation() const
{
	return Quaternionf();
}

Vec3f Physics3DObject::get_scale() const
{
	return Vec3f();
}

void Physics3DObject::set_position(const Vec3f &position)
{
}

void Physics3DObject::set_orientation(const Quaternionf &orientation)
{
}

void Physics3DObject::set_scale(const Vec3f &scale)
{
}

}
