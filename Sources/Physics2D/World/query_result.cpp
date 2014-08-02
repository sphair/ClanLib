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
**    Arkadiusz Kalinowski
*/

#include "Physics2D/precomp.h"
#include "query_result_impl.h"
#include "API/Physics2D/World/query_result.h"

namespace clan
{


QueryResult::QueryResult()
{

}

QueryResult::QueryResult(Fixture &fixture)
: impl(std::make_shared<QueryResult_Impl>())
{
	impl->fixture = fixture;
	impl->point = Pointf(0.0f, 0.0f);
	impl->normal = Vec2f(0.0f, 0.0f);
	impl->fraction = 0.0f;
	impl->type = query_aabb;
}

QueryResult::QueryResult(Fixture &fixture, Pointf &point, Vec2f &normal, float fraction)
: impl(std::make_shared<QueryResult_Impl>())
{
	impl->fixture = fixture;
	impl->point = point;
	impl->normal = normal;
	impl->fraction = fraction;
	impl->type = query_raycast;

}
QueryResult::~QueryResult()
{

}

void QueryResult::throw_if_null() const
{
		if (!impl)
		throw Exception("QueryResult is null");
}

Fixture &QueryResult::get_fixture() const
{
	return impl->fixture;
}

Body &QueryResult::get_body() const
{
	return impl->fixture.get_owner_body();
}

QueryResultType QueryResult::get_query_type() const
{
	return impl->type;
}
QueryResult &QueryResult::operator =(const QueryResult &copy)
{
	impl = copy.impl;
	return *this;
}


}