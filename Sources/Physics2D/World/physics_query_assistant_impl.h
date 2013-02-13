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

#pragma once

#include "../Box2D/Box2D.h"
#include "API/Physics/World/query_result.h"
#include "API/Core/Math/rect.h"

namespace clan
{
	class PhysicsWorld;

	enum QueryType
	{
		query_raycast_first,
		query_raycast_any,
		query_raycast_all,
		query_aabb_any,
		query_aabb_some,
		query_aabb_all
	};

class PhysicsQueryAssistant_Impl : public b2QueryCallback, public b2RayCastCallback
{
public:
//																						_______________________
//																						C O N S T R U C T O R S

	PhysicsQueryAssistant_Impl(PhysicsWorld &pw);

	~PhysicsQueryAssistant_Impl() { return; }

//																						___________________
//																						O P E R A T I O N S
	virtual bool ReportFixture(b2Fixture* fixture);
	virtual float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction);
//																						___________________________
//																						I M P L E M E N T A T I O N
	
	PhysicsWorld *owner_world;
	QueryType query_type;
	int query_result_amount;
	int query_result_limit;

	const int max_queried_objects;
	std::vector<QueryResult> queried_objects;
};

}