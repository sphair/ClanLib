/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
#include "API/Physics3D/physics3d_contact_test.h"
#include "API/Physics3D/physics3d_object.h"
#include "API/Physics3D/physics3d_world.h"
#include "API/Core/System/exception.h"
#include "physics3d_contact_test_impl.h"
#include "physics3d_world_impl.h"
#include "physics3d_object_impl.h"

namespace clan
{

Physics3DContactTest::Physics3DContactTest()
{
}

Physics3DContactTest::Physics3DContactTest(Physics3DWorld &world)
	: impl(new Physics3DContactTest_Impl(world.impl.get()))
{
}

bool Physics3DContactTest::is_null() const
{
	return !impl;
}

bool Physics3DContactTest::test(const Physics3DObject &object)
{
	Physics3DContactTest_Impl::AllHitsContactResultCallback callback(impl.get(), object.impl->object.get());
	impl->world->dynamics_world->contactTest(object.impl->object.get(), callback);

	impl->contacts.clear();
	for (std::map<Physics3DObject_Impl *, bool>::iterator it = callback.contacts.begin(); it != callback.contacts.end(); ++it)
	{
		impl->contacts.push_back(it->first);
	}

	return !impl->contacts.empty();
}

int Physics3DContactTest::get_hit_count() const
{
	return impl->contacts.size();
}

Physics3DObject Physics3DContactTest::get_hit_object(int index) const
{
	if (!impl->contacts.empty())
		return Physics3DObject(impl->contacts[index]->shared_from_this());
	else
		return Physics3DObject();
}

/////////////////////////////////////////////////////////////////////////////

Physics3DContactTest_Impl::Physics3DContactTest_Impl(Physics3DWorld_Impl *world)
	: world(world)
{
}

Physics3DContactTest_Impl::~Physics3DContactTest_Impl()
{
}

}
