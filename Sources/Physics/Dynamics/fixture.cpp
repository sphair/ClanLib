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
#include "fixture_impl.h"
#include "API/Physics/Dynamics/body.h"
#include "API/Physics/Dynamics/fixture.h"
//#include "API/Physics/World/physic_world.h"
#include "../World/physic_world_impl.h"

namespace clan
{

//																											_______________________																											
//																											C O N S T R U C T O R S
Fixture::Fixture()
{
}

Fixture::Fixture(Body &body, const FixtureDescription &description)
: impl(new Fixture_Impl())
{
	if(!body.is_null())
	{
		impl->create_fixture(body,description);
		impl->fixture->SetUserData(this);
	}
	else
	throw Exception("Tried to create a fixture with a null Body object");
}

Fixture::~Fixture()
{
}

//																											___________________																											
//																											A T T R I B U T E S
void Fixture::throw_if_null() const
{
	if (!impl)
		throw Exception("Fixture is null");
}

bool Fixture::is_sensor() const
{
	return impl->fixture->IsSensor();
}

//																											___________________																											
//																											O P E R A T I O N S

void Fixture::set_as_sensor(const bool value)
{
	impl->fixture->SetSensor(value);
}

bool Fixture::test_point(const Vec2f &p) const
{
	float scale = impl->owner->physic_scale;
	return impl->fixture->TestPoint(b2Vec2(p.x/scale, p.y/scale));
}

//																											_____________																										
//																											S I G N A L S

Signal_v1<Fixture &> &Fixture::sig_collision()
{
	return impl->sig_collision;
}
Signal_v0 &Fixture::sig_fixture_deletion()
{
	return impl->sig_fixture_deletion;
}

}
