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

#include "Physics2D/precomp.h"
#include "fixture_impl.h"
#include "body_impl.h"
#include "API/Physics2D/Dynamics/body.h"
#include "API/Physics2D/Dynamics/fixture.h"
#include "API/Physics2D/Dynamics/fixture_description.h"
#include "../World/physics_context_impl.h"
#include "../World/physics_world_impl.h"
#include "../Dynamics/fixture_description_impl.h"

namespace clan
{

//																											_______________________																											
//																											C O N S T R U C T O R S
Fixture::Fixture()
{
}

Fixture::Fixture(PhysicsContext &pc, Body &body, const FixtureDescription &description)
: impl(new Fixture_Impl(pc.impl->get_owner()))
{
	if(!body.is_null())
	{
		impl->create_fixture(body,description);
		body.impl->add_fixture(*this);
	}
	else
	throw Exception("Tried to create a fixture with a null Body object");
}

Fixture::~Fixture()
{
	if(impl)
	{
		if(impl->fixture_occupied)
		{
			impl->owner_world->check_fixture(impl->id);
		}
	}
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

int Fixture::get_id() const
{
	return impl->id;
}

Body &Fixture::get_owner_body() const
{
	return *static_cast<Body *> (impl->fixture->GetBody()->GetUserData());
}

bool Fixture::is_dummy() const
{
	return !impl->fixture_occupied;
}
//																											___________________																											
//																											O P E R A T I O N S

Fixture &Fixture::operator =(const Fixture &copy)
{
	if(impl)
	{
		if(impl->fixture_occupied)
		{
			impl->owner_world->check_fixture(impl->id);
		}
	}

	impl = copy.impl;
	return *this;
}

void Fixture::set_as_sensor(const bool value)
{
	impl->fixture->SetSensor(value);
}

bool Fixture::test_point(const Vec2f &p) const
{
	float scale = impl->owner_world->physic_scale;
	return impl->fixture->TestPoint(b2Vec2(p.x/scale, p.y/scale));
}

void Fixture::kill()
{
	impl->remove_fixture();
}
//																											_____________																										
//																											S I G N A L S

Signal_v1<Fixture> &Fixture::sig_begin_collision()
{
	return impl->sig_begin_collision;
}

Signal_v1<Fixture> &Fixture::sig_end_collision()
{
	return impl->sig_end_collision;
}

Signal_v0 &Fixture::sig_fixture_deletion()
{
	return impl->sig_fixture_deletion;
}

}
