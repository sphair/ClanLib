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
#include <vector>

namespace clan
{

class Physics3DObject_Impl;
class Physics3DWorld_Impl;

class Physics3DSweepTest_Impl
{
public:
	Physics3DSweepTest_Impl(Physics3DWorld_Impl *world);
	~Physics3DSweepTest_Impl();

	Physics3DWorld_Impl *world;

	Vec3f from_pos, to_pos;

	struct SweepHit
	{
		SweepHit() : hit_fraction(0.0f), hit_collision_object(0) { }

		bool operator<(const SweepHit &other) const { return hit_fraction < other.hit_fraction; }

		float hit_fraction;
		Vec3f hit_normal;
		Physics3DObject_Impl *hit_collision_object;
	};

	std::vector<SweepHit> hits;

	class AllHitsConvexResultCallback : public btCollisionWorld::ConvexResultCallback
	{
	public:
		AllHitsConvexResultCallback(Physics3DSweepTest_Impl *impl) : impl(impl)
		{
		}

		btAlignedObjectArray<btVector3>	m_hitNormalWorld;
		btAlignedObjectArray<btVector3>	m_hitPointWorld;
		btAlignedObjectArray<const btCollisionObject*> m_hitCollisionObjects;
		btAlignedObjectArray<btScalar> m_hitFractions;

		virtual	btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult,bool normalInWorldSpace)
		{
			SweepHit hit;
			hit.hit_fraction = convexResult.m_hitFraction;
			hit.hit_collision_object = static_cast<Physics3DObject_Impl*>(convexResult.m_hitCollisionObject->getUserPointer());

			if (normalInWorldSpace)
			{
				hit.hit_normal = to_vec3f(convexResult.m_hitNormalLocal);
			}
			else
			{
				///need to transform normal into worldspace
				hit.hit_normal = to_vec3f(convexResult.m_hitCollisionObject->getWorldTransform().getBasis() * convexResult.m_hitNormalLocal);
			}

			//hit.hit_point = to_vec3f(convexResult.m_hitPointLocal);

			impl->hits.push_back(hit);

			return btScalar(1.);
		}

	private:
		static Vec3f to_vec3f(const btVector3 &v)
		{
			return Vec3f(v.getX(), v.getY(), v.getZ());
		}

		Physics3DSweepTest_Impl *impl;
	};
};

}
