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
#include "fixture_impl.h"
#include "fixture_description_impl.h"
#include "body_impl.h"
#include "../World/physics_world_impl.h"
#include "API/Physics2D/Dynamics/body.h"
#include "API/Physics2D/Dynamics/fixture_description.h"

namespace clan
{

Fixture_Impl::Fixture_Impl(PhysicsWorld_Impl *owner)
: fixture(owner_world->get_dummy_fixture()),
  fixture_occupied(false),
  owner_world(owner),
  id(-1)
{

}

Fixture_Impl::~Fixture_Impl()
{
	if(fixture_occupied)
	{
		sig_fixture_deletion.invoke(); 
		owner_world->safe_destroy_fixture(fixture, id);
	}
}
void Fixture_Impl::create_fixture(Body &body, const FixtureDescription &description)
{
	std::shared_ptr<Fixture_Impl> impl = shared_from_this();
	if(fixture_occupied)
	{
		owner_world->destroy_fixture(impl);
		//fixture->GetBody()->DestroyFixture(fixture);
	}
	else
	{
		fixture_occupied = true;
	}
		
	owner_world->create_fixture(impl, body.impl->body, description.impl->fixtureDef);
	fixture->SetUserData(this);
}

void Fixture_Impl::remove_fixture()
{
	if(fixture_occupied)
	{
		sig_fixture_deletion.invoke();

		std::shared_ptr<Fixture_Impl> impl = shared_from_this();
		owner_world->destroy_fixture(impl);
		//fixture->GetBody()->DestroyFixture(fixture);
		fixture_occupied = false;

		//fixture = owner_world->get_dummy_fixture();
	}
}

void Fixture_Impl::remove_fixture_safetly()
{
	if(fixture_occupied)
	{
		sig_fixture_deletion.invoke(); //Might move this to the physics world when the bodies are deleted for real.

		owner_world->safe_destroy_fixture(fixture, id);

		fixture_occupied = false;
		
		fixture = owner_world->get_dummy_fixture();
	}
}
void Fixture_Impl::on_begin_collision(Fixture_Impl &fixture)
{
	Fixture collision_fixture;
	collision_fixture.impl = shared_from_this();

	sig_begin_collision.invoke(collision_fixture); //Send the fixute that this fixture collided with.
}

void Fixture_Impl::on_end_collision(Fixture_Impl &fixture)
{
	Fixture collision_fixture;
	collision_fixture.impl = shared_from_this();

	sig_end_collision.invoke(collision_fixture); //Send the fixture that this fixture collided with.
}

}