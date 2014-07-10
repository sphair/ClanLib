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

#pragma once

#include "Physics3D/Bullet/btBulletDynamicsCommon.h"
#include "API/Core/Math/vec3.h"
#include <map>

namespace clan
{

class Physics3DObject_Impl;
class Physics3DWorld_Impl;

class Physics3DContactTest_Impl
{
public:
	Physics3DContactTest_Impl(Physics3DWorld_Impl *world);
	~Physics3DContactTest_Impl();

	Physics3DWorld_Impl *world;

	struct Contact
	{
		Contact(Physics3DObject_Impl *object, const btVector3 &hit_point, const btVector3 &hit_normal, float hit_distance) : object(object), hit_position(hit_point), hit_normal(hit_normal), hit_distance(hit_distance) { }

		Physics3DObject_Impl *object;
		btVector3 hit_position;
		btVector3 hit_normal;
		float hit_distance;
	};

	std::vector<Contact> contacts;

	class AllHitsContactResultCallback : public btCollisionWorld::ContactResultCallback
	{
	public:
		AllHitsContactResultCallback(Physics3DContactTest_Impl *impl, btCollisionObject *test_object) : impl(impl), test_object(test_object)
		{
		}

		btScalar addSingleResult(btManifoldPoint &cp, const btCollisionObjectWrapper *colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper *colObj1Wrap, int partId1, int index1)
		{
			if (colObj0Wrap->getCollisionObject() == test_object)
			{
				impl->contacts.push_back(Contact(static_cast<Physics3DObject_Impl*>(colObj1Wrap->getCollisionObject()->getUserPointer()), cp.getPositionWorldOnA(), -cp.m_normalWorldOnB, cp.getDistance()));
			}
			else
			{
				impl->contacts.push_back(Contact(static_cast<Physics3DObject_Impl*>(colObj0Wrap->getCollisionObject()->getUserPointer()), cp.getPositionWorldOnB(), cp.m_normalWorldOnB, cp.getDistance()));
			}

			return btScalar(1.);
		}

	private:
		Physics3DContactTest_Impl *impl;
		btCollisionObject *test_object;
	};
};

}
