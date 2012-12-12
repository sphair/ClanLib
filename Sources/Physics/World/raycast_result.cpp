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
#include "raycast_result_impl.h"
#include "API/Physics/World/raycast_result.h"

namespace clan
{


RaycastResult::RaycastResult()
{

}

RaycastResult::RaycastResult(Fixture &fixture, Pointf &point, Vec2f &normal, float fraction)
: impl(new RaycastResult_Impl)
{
	impl->fixture = fixture;
	impl->point = point;
	impl->normal = normal;
	impl->fraction = fraction;

}
RaycastResult::~RaycastResult()
{

}

void RaycastResult::throw_if_null() const
{
		if (!impl)
		throw Exception("RaycastResult is null");
}

Fixture &RaycastResult::get_fixture() const
{
	return impl->fixture;
}

Body &RaycastResult::get_body() const
{
	return impl->fixture.get_owner_body();
}

RaycastResult &RaycastResult::operator =(const RaycastResult &copy)
{
	impl = copy.impl;
	return *this;
}


}