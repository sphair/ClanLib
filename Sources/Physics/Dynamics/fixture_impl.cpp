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
#include "fixture_description_impl.h"
#include "body_impl.h"
#include "../World/physic_world_impl.h"
#include "API/Physics/Dynamics/body.h"
#include "API/Physics/Dynamics/fixture_description.h"

namespace clan
{

Fixture_Impl::Fixture_Impl(Fixture &parent, PhysicWorld_Impl &pw_impl)
: fixture(NULL),
  fixture_occupied(false),
  owner(&parent),
  owner_world(&pw_impl)
{
}

void Fixture_Impl::create_fixture(Body &body, const FixtureDescription &description)
{
	if(fixture_occupied)
	{
		fixture->GetBody()->DestroyFixture(fixture);
	}
	else
	{
		fixture_occupied = true;
	}
		
	fixture = body.impl->body->CreateFixture(&description.impl->fixtureDef);
	fixture->SetUserData(this);
}

void Fixture_Impl::on_begin_collision(Fixture_Impl &fixture)
{
	sig_begin_collision.invoke(*fixture.owner); //Send the fixute that this fixture collided with.
}

void Fixture_Impl::on_end_collision(Fixture_Impl &fixture)
{
	sig_end_collision.invoke(*fixture.owner); //Send the fixture that this fixture collided with.
}

}