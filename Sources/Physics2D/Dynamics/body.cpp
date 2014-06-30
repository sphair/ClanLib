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
#include "../Box2D/Box2D.h"
#include "body_description_impl.h"
#include "body_impl.h"
#include "fixture_description_impl.h"
#include "../World/physics_world_impl.h"
#include "../World/physics_context_impl.h"
#include "API/Physics2D/World/physics_world.h"
#include "API/Physics2D/World/physics_context.h"
#include "API/Physics2D/Dynamics/fixture_description.h"
#include "API/Physics2D/Dynamics/body.h"
#include "API/Core/Math/angle.h"
#include "API/Core/Resources/xml_resource_node.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/XML/dom_element.h"

namespace clan
{
//																											_______________________																											
//																											C O N S T R U C T O R S
Body::Body()
{
}

Body::Body(PhysicsContext &pc, const BodyDescription &description)
: impl(new Body_Impl(pc.impl->get_owner()))
{
	if(impl->owner_world)
	{
		impl->create_body(description);
	}
	else
	throw Exception("Tried to create a body with a null PhysicsWorld object");
}

Body::Body(PhysicsContext &pc, const std::string &resource_id, const XMLResourceDocument &resources)
: impl(new Body_Impl(pc.impl->get_owner()))
{
	if(impl->owner_world)
	{
		XMLResourceNode resource = resources.get_resource(resource_id);
		std::string type = resource.get_element().get_tag_name();
	
		if (type != "body2d")
		throw Exception(string_format("Resource '%1' is not of type 'body2d'", resource_id));

		impl->init(pc, resource_id, resources);
	}
	else
	throw Exception("Tried to create a body with a null PhysicsWorld object");
}

Body::~Body()
{
	if(impl)
	{
		if(impl->body_occupied)
		{
			impl->owner_world->check_body(impl->id);
		}
	}
}

//																											___________________																											
//																											A T T R I B U T E S
void Body::throw_if_null() const
{
	if (!impl)
		throw Exception("Body is null");
}

Vec2f Body::get_position() const
{
	float scale = impl->owner_world->physic_scale;
	b2Vec2 vec = impl->body->GetPosition();

	return Vec2f(vec.x*scale, vec.y*scale);
}

Angle Body::get_angle() const
{
	float angle = impl->body->GetAngle();
	impl->body->GetAngle();

	return Angle(angle,angle_radians);
}

Vec2f Body::get_linear_velocity() const
{
	float scale = impl->owner_world->physic_scale;
	b2Vec2 vec = impl->body->GetLinearVelocity();

	return Vec2f(vec.x*scale, vec.y*scale);
}

Angle Body::get_angular_velocity() const
{
	float velocity = impl->body->GetAngularVelocity();

	return Angle(velocity,angle_radians);
}

float Body::get_gravity_scale()
{
	return impl->body->GetGravityScale();
}

BodyType Body::get_type() const
{
	BodyType body_type;
	switch(impl->body->GetType())
	{
		case b2_dynamicBody:
			body_type = body_dynamic;
		break;
		case b2_kinematicBody:
			body_type = body_kinematic;
		break;
		case b2_staticBody:
			body_type = body_static;
		break;
	}

	return body_type;
}

bool Body::has_fixed_rotation() const
{
	return impl->body->IsFixedRotation();
}

float Body::get_linear_damping() const
{
	return impl->body->GetLinearDamping();
}

float Body::get_angular_damping() const
{
	return impl->body->GetAngularDamping();
}

int Body::get_id() const
{
	return impl->id;
}

bool Body::is_dummy() const
{
	return !impl->body_occupied;
}

bool Body::is_awake() const
{
	return impl->body->IsAwake();
}

bool Body::is_active() const
{
	return impl->body->IsActive();
}

bool Body::is_bullet() const
{
	return impl->body->IsBullet();
}

PhysicsObject *Body::get_data()
{
	return impl->data;
}
//																											___________________																											
//																											O P E R A T I O N S

/*
Fixture Body::create_fixture(const FixtureDescription &description)
{

	Fixture fixture(*this,description);
 	
	return fixture;
}
*/

Body &Body::operator =(const Body &copy)
{
	if(impl)
	{
		if(impl->body_occupied)
		{
			impl->owner_world->check_body(impl->id);
		}
	}

	impl = copy.impl;
	return *this;
}

void Body::set_position(const Vec2f &pos)
{
	float scale = impl->owner_world->physic_scale;
	impl->body->SetTransform(b2Vec2(pos.x/scale, pos.y/scale), impl->body->GetAngle());
}

void Body::set_angle(const Angle &angle)
{
	impl->body->SetTransform(impl->body->GetPosition(), angle.to_radians());
}

void Body::set_linear_velocity(const Vec2f &velocity)
{
	float scale = impl->owner_world->physic_scale;
	impl->body->SetLinearVelocity(b2Vec2(velocity.x/scale, velocity.y/scale));
}

void Body::set_angular_velocity(const Angle &velocity)
{
	impl->body->SetAngularVelocity(velocity.to_radians());
}

void Body::apply_force (const Vec2f &force, const Vec2f &point)
{
	float scale = impl->owner_world->physic_scale;

	b2Vec2 b2force(force.x, force.y);
	b2Vec2 b2point(point.x/scale, point.y/scale);

	impl->body->ApplyForce(b2force, b2point);
}

void Body::apply_force_to_center (const Vec2f &force)
{
	b2Vec2 b2force(force.x, force.y);

	impl->body->ApplyForceToCenter(b2force);
}

void Body::apply_torque (const float torque)
{
	impl->body->ApplyTorque(torque);
}

void Body::apply_linear_impulse (const Vec2f &impulse, const Vec2f &point)
{
	float scale = impl->owner_world->physic_scale;

	b2Vec2 b2impulse(impulse.x, impulse.y);
	b2Vec2 b2point(point.x/scale, point.y/scale);

	impl->body->ApplyLinearImpulse(b2impulse, b2point);
}

void Body::apply_angular_impulse (const float impulse)
{
	impl->body->ApplyAngularImpulse(impulse);
}

void Body::set_gravity_scale(const float scale)
{
	impl->body->SetGravityScale(scale);
}

void Body::set_type(const BodyType type)
{
	b2BodyType body_type;
	switch(type)
	{
		case body_dynamic:
			body_type = b2_dynamicBody;
		break;
		case body_kinematic:
			body_type = b2_kinematicBody;
		break;
		case body_static:
			body_type = b2_staticBody;
		break;
	}

	impl->body->SetType(body_type);
}

void Body::set_as_bullet(const bool value)
{
	impl->body->SetBullet(value);
}

void Body::set_awake(const bool value)
{
	impl->body->SetAwake(value);
}

void Body::set_active(const bool value)
{
	impl->body->SetActive(value);
}

void Body::set_fixed_rotation(const bool value)
{
	impl->body->SetFixedRotation(value);
}

void Body::set_linear_damping(const float damping)
{
	impl->body->SetLinearDamping(damping);
}

void Body::set_angular_damping(const float damping)
{
	impl->body->SetAngularDamping(damping);
}

void Body::reset_mass_data()
{
	impl->body->ResetMassData();
}

void Body::kill()
{
	impl->remove_body();
}

void Body::set_data(PhysicsObject *data)
{
	impl->data = data;
}

//																											_____________																										
//																											S I G N A L S

Signal<void(Body)> &Body::sig_begin_collision()
{
	return impl->sig_begin_collision;
}

Signal<void(Body)> &Body::sig_end_collision()
{
	return impl->sig_end_collision;
}

Signal<void()> &Body::sig_body_deletion()
{
	return impl->sig_body_deletion;
}

}