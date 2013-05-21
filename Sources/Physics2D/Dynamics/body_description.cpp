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
#include "../World/physics_world_impl.h"
#include "../World/physics_context_impl.h"
#include "API/Physics2D/Dynamics/body_description.h"
#include "API/Physics2D/World/physics_world.h"
#include "API/Core/Math/point.h"
#include "API/Core/Math/angle.h"
#include "API/Core/XML/dom_node.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/Resources/xml_resource_node.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"

namespace clan
{
//																											_______________________																											
//																											C O N S T R U C T O R S

BodyDescription::BodyDescription()
{

}
BodyDescription::BodyDescription(const PhysicsWorld &pw)
: impl(new BodyDescription_Impl(pw.impl.get()))
{

}

BodyDescription::BodyDescription(const PhysicsContext &pc)
: impl(new BodyDescription_Impl(pc.impl->get_owner()))
{

}

BodyDescription::BodyDescription(const PhysicsContext &pc, const std::string &resource_id, XMLResourceDocument *resources)
: impl(new BodyDescription_Impl(pc.impl->get_owner()))
{
	/* example resource entry with all parameters:
	
	<body2d name="TestBody" type="static">
        <position x="0" y="0"/>
        <rotation angle="180"/>
        <velocity x="0" y="0" angular="0"/>
        <damping linear="0" angular="0"/>
		<parameters awake="true" can_sleep="true" bullet="false" active="true" />
    </body2d>

	*/
	XMLResourceNode resource = resources->get_resource(resource_id);
	if (resource.get_type() != "body2d" && resource.get_type() != "body2d_description")
		throw Exception(string_format("Resource '%1' is not of type 'body2d' or 'body2d_description'", resource_id));

	DomNode cur_node = resource.get_element().get_first_child();

	//Body type
	std::string body_type = resource.get_element().get_attribute("type","static");
	if(body_type == "kinematic") set_type(body_kinematic);
	else if(body_type == "dynamic") set_type(body_dynamic);
	else set_type(body_static);

	while(!cur_node.is_null())
	{
		if (!cur_node.is_element()) 
			continue;
		
		DomElement cur_element = cur_node.to_element();
		std::string tag_name = cur_element.get_tag_name();

		//<position x="0" y="0"/>
		if(tag_name == "position")
		{
			float pos_x = 0.0f;
			float pos_y = 0.0f;

			if(cur_element.has_attribute("x"))
			{
				pos_x = StringHelp::text_to_float(cur_element.get_attribute("x"));
			}

			if(cur_element.has_attribute("y"))
			{
				pos_y = StringHelp::text_to_float(cur_element.get_attribute("y"));
			}

			set_position(pos_x, pos_y);
		}

		//<rotation angle="180"/>
		else if(tag_name == "rotation")
		{
			Angle angle(0.0f, angle_degrees);

			if(cur_element.has_attribute("angle"))
			{
				angle = Angle(StringHelp::text_to_float(cur_element.get_attribute("angle")), angle_degrees);
			}

			set_angle(angle);
		}

		//<velocity x="0" y="0" angular="0"/>
		else if(tag_name == "velocity")
		{
			Vec2f velocity(0.0f, 0.0f);
			Angle angular_velocity(0.0f, angle_degrees);

			if(cur_element.has_attribute("x"))
			{
				velocity.x = StringHelp::text_to_float(cur_element.get_attribute("x"));
			}

			if(cur_element.has_attribute("y"))
			{
				velocity.y = StringHelp::text_to_float(cur_element.get_attribute("y"));
			}

			if(cur_element.has_attribute("angular"))
			{
				angular_velocity = Angle(StringHelp::text_to_float(cur_element.get_attribute("angular")), angle_degrees);
			}

			set_linear_velocity(velocity);
			set_angular_velocity(angular_velocity);
		}

		//<damping linear="0" angular="0"/>
		else if(tag_name == "damping")
		{
			float linear;
			float angular;

			if(cur_element.has_attribute("linear"))
			{
				linear = StringHelp::text_to_float(cur_element.get_attribute("linear"));
			}

			if(cur_element.has_attribute("angular"))
			{
				angular = StringHelp::text_to_float(cur_element.get_attribute("angular"));
			}

			set_linear_damping(linear);
			set_angular_damping(angular);
		}

		//<parameters awake="true" can_sleep="true" bullet="false" active="true" />
		else if(tag_name == "parameters")
		{
			bool value;
		
			if(cur_element.has_attribute("awake"))
			{
				value = true;
				value = StringHelp::text_to_bool(cur_element.get_attribute("awake"));
				set_awake(value);
			}
			if(cur_element.has_attribute("active"))
			{
				value = true;
				value = StringHelp::text_to_bool(cur_element.get_attribute("active"));
				set_active(value);
			}
			if(cur_element.has_attribute("bullet"))
			{
				value = false;
				value = StringHelp::text_to_bool(cur_element.get_attribute("bullet"));
				set_as_bullet(value);
			}
			if(cur_element.has_attribute("can_sleep"))
			{
				value = true;
				value = StringHelp::text_to_bool(cur_element.get_attribute("can_sleep"));
				allow_sleep(value);
			}
		}

		cur_node = cur_node.get_next_sibling();
	}
}

BodyDescription::BodyDescription(const BodyDescription &copy)
: impl(copy.impl)
{
}

BodyDescription::~BodyDescription()
{
}

//																											___________________																											
//																											A T T R I B U T E S

//																											___________________																											
//																											O P E R A T I O N S
BodyDescription &BodyDescription::operator =(const BodyDescription &copy)
{
	impl = copy.impl;
	return *this;
}

void BodyDescription::throw_if_null() const
{
	if (!impl)
	throw Exception("BodyDescription is null");
}

void BodyDescription::set_type(const BodyType type)
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

	impl->bodyDef.type = body_type;
		
}

void BodyDescription::set_position(const Vec2f &position)
{
	float scale = impl->owner_world->physic_scale;
	impl->bodyDef.position = b2Vec2(position.x/scale, position.y/scale);
}
void BodyDescription::set_position(const Pointf &position)
{
	float scale = impl->owner_world->physic_scale;
	impl->bodyDef.position = b2Vec2(position.x/scale, position.y/scale);
}

void BodyDescription::set_position(const int x, const int y)
{
	float scale = impl->owner_world->physic_scale;
	impl->bodyDef.position = b2Vec2(x/scale, y/scale);
}
void BodyDescription::set_position(const float x, const float y)
{
	float scale = impl->owner_world->physic_scale;
	impl->bodyDef.position = b2Vec2(x/scale, y/scale);
}

void BodyDescription::set_angle(const Angle &angle)
{
	impl->bodyDef.angle = angle.to_radians();
}

void BodyDescription::set_linear_velocity(const Vec2f &velocity)
{
	float scale = impl->owner_world->physic_scale;
	impl->bodyDef.linearVelocity = b2Vec2(velocity.x/scale, velocity.y/scale);
}

void BodyDescription::set_angular_velocity(const Angle &velocity)
{
	impl->bodyDef.angularVelocity = velocity.to_radians();
}

void BodyDescription::set_linear_damping(const float damping)
{
	impl->bodyDef.linearDamping = damping;
}
void BodyDescription::set_angular_damping(const float damping)
{
	impl->bodyDef.angularDamping = damping;
}
void BodyDescription::allow_sleep(const bool value)
{
	impl->bodyDef.allowSleep = value;
}
	
void BodyDescription::set_awake(const bool value)
{
	impl->bodyDef.awake = value;
}

void BodyDescription::set_fixed_rotation(const bool value)
{
	impl->bodyDef.fixedRotation = value;
}

void BodyDescription::set_as_bullet(const bool value)
{
	impl->bodyDef.bullet = value;
}

void BodyDescription::set_active(const bool value)
{
	impl->bodyDef.active = value;
}

void BodyDescription::set_gravity_scale(const float scale)
{
	impl->bodyDef.gravityScale = scale;
}

}