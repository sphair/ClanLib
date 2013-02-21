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
#include "API/Physics3D/physics3d_sweep_test.h"
#include "API/Physics3D/physics3d_object.h"
#include "API/Physics3D/physics3d_world.h"
#include "API/Core/System/exception.h"
#include "physics3d_sweep_test_impl.h"
#include "physics3d_world_impl.h"

namespace clan
{

Physics3DSweepTest::Physics3DSweepTest()
{
}

Physics3DSweepTest::Physics3DSweepTest(Physics3DWorld &world)
	: impl(new Physics3DSweepTest_Impl(world.impl.get()))
{
}

bool Physics3DSweepTest::is_null() const
{
	return !impl;
}

bool Physics3DSweepTest::test(const Physics3DShape &shape, const Vec3f &from_pos, const Quaternionf &from_orientation, const Vec3f &to_pos, const Vec3f &to_orientation, float allowed_ccd_penetration)
{
	throw Exception("Physics3DSweepTest::test not implemented");
}

int Physics3DSweepTest::get_hit_count() const
{
	return 0;
}

float Physics3DSweepTest::get_hit_fraction(int index) const
{
	return 0.0f;
}

Vec3f Physics3DSweepTest::get_hit_position(int index) const
{
	return Vec3f();
}

Quaternionf Physics3DSweepTest::get_hit_orientation(int index) const
{
	return Quaternionf();
}

Physics3DObject Physics3DSweepTest::get_hit_object(int index) const
{
	return Physics3DObject();
}

/////////////////////////////////////////////////////////////////////////////

Physics3DSweepTest_Impl::Physics3DSweepTest_Impl(Physics3DWorld_Impl *world)
	: world(world)
{
}

Physics3DSweepTest_Impl::~Physics3DSweepTest_Impl()
{
}


}
