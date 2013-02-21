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
#include "API/Physics3D/physics3d_ray_tester.h"
#include "API/Physics3D/physics3d_object.h"
#include "API/Physics3D/physics3d_world.h"
#include "physics3d_ray_tester_impl.h"
#include "physics3d_world_impl.h"

namespace clan
{

Physics3DRayTester::Physics3DRayTester()
{
}

Physics3DRayTester::Physics3DRayTester(Physics3DWorld &world)
	: impl(new Physics3DRayTester_Impl(world.impl.get()))
{
}

bool Physics3DRayTester::is_null() const
{
	return !impl;
}

bool Physics3DRayTester::test(const clan::Vec3f &new_start, const clan::Vec3f &new_end)
{
	return false;
}

bool Physics3DRayTester::has_hit() const
{
	return impl->has_hit;
}

float Physics3DRayTester::get_hit_fraction() const
{
	return impl->hit_fraction;
}

Vec3f Physics3DRayTester::get_hit_position() const
{
	return mix(impl->start, impl->end, impl->hit_fraction);
}

Physics3DObject Physics3DRayTester::get_hit_object() const
{
	return Physics3DObject();
}

/////////////////////////////////////////////////////////////////////////////

Physics3DRayTester_Impl::Physics3DRayTester_Impl(Physics3DWorld_Impl *world)
	: world(world), has_hit(false), hit_fraction(1.0f), hit_object(nullptr)
{
}

Physics3DRayTester_Impl::~Physics3DRayTester_Impl()
{
}


}
