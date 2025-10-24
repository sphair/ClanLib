/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "Display/display_precomp.h"
#include "resourcedata_sprite.h"
#include "API/Display/sprite_description.h"
#include "API/Display/pixel_buffer.h"
#include "API/Core/System/clanstring.h"
#include "API/Core/System/error.h"
#include "API/Core/Resources/resource_manager.h"

//////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Sprite construction:

CL_ResourceData_Sprite::CL_ResourceData_Sprite(CL_Resource &resource)
: CL_ResourceData_SpriteDescription(resource, "sprite")
{
}

CL_ResourceData_Sprite::~CL_ResourceData_Sprite()
{
}

//////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Sprite attributes:


//////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Sprite implementation:

void CL_ResourceData_Sprite::on_load()
{
	// <sprite name="string" id="integer" base_angle="integer" pack_texture="[yes,no] description="walking_man">

	CL_Resource resource = get_resource();
	CL_SpriteDescription desc = load_description();

	bool pack_texture = resource.get_element().get_attribute("pack_texture", "yes") == "yes";

	// Create sprite from spritedescription
	sprite = CL_Sprite(desc, pack_texture);

	// Load base angle
	float base_angle = (float)atof(resource.get_element().get_attribute("base_angle", "0").c_str());
	sprite.set_base_angle(base_angle);

	// Load id
	int id = atoi(resource.get_element().get_attribute("id", "0").c_str());
	sprite.set_id(id);

	// Load play options	
	CL_DomNode cur_node;
	for (
		cur_node = resource.get_element().get_first_child();
		!cur_node.is_null();
		cur_node = cur_node.get_next_sibling())
	{
		if (!cur_node.is_element()) continue;
		CL_DomElement cur_element = cur_node.to_element();

		// <color red="float" green="float" blue="float" alpha="float" />
		std::string tag_name = cur_element.get_tag_name();
		if (tag_name == "color")
		{
			float r = (float)atof(cur_element.get_attribute("red", "1.0").c_str());
			float g = (float)atof(cur_element.get_attribute("green","1.0").c_str());
			float b = (float)atof(cur_element.get_attribute("blue", "1.0").c_str());
			float a = (float)atof(cur_element.get_attribute("alpha", "1.0").c_str());
			sprite.set_color(r,g,b,a);
		}
		// <animation speed="integer" loop="[yes,no]" pingpong="[yes,no]" direction="[backward,forward]" on_finish="[blank,last_frame,first_frame]"/>
		else if (tag_name == "animation")
		{
			float delay = (float)atof(cur_element.get_attribute("speed", "60").c_str()) / 1000.0f;
			int frame_count = sprite.get_frame_count();
			for(int i=0; i<frame_count; ++i)
				sprite.set_frame_delay(i, delay);

			sprite.set_play_loop((cur_element.get_attribute("loop", "yes")) == "yes");
			sprite.set_play_pingpong((cur_element.get_attribute("pingpong", "no")) == "yes");
			sprite.set_play_backward((cur_element.get_attribute("direction", "forward")) == "backward");

			std::string on_finish = cur_element.get_attribute("on_finish", "blank");
			if(on_finish == "first_frame")
				sprite.set_show_on_finish(CL_Sprite::show_first_frame);
			else if(on_finish == "last_frame")
				sprite.set_show_on_finish(CL_Sprite::show_last_frame);
			else
				sprite.set_show_on_finish(CL_Sprite::show_blank);
		}
		// <scale x="float" y="float />
		else if (tag_name == "scale")
		{
			float x = (float)atof(cur_element.get_attribute("x", "1.0").c_str());
			float y = (float)atof(cur_element.get_attribute("y", "1.0").c_str());
			sprite.set_scale(x, y);
		}
		// <translation origin="string" x="integer" y="integer" />
		else if (tag_name == "translation")
		{
			std::string hotspot = cur_element.get_attribute("origin", "top_left");
			CL_Origin origin;

			if(hotspot == "center")
				origin = origin_center;
			else if(hotspot == "top_center")
				origin = origin_top_center;
			else if(hotspot == "top_right")
				origin = origin_top_right;
			else if(hotspot == "center_left")
				origin = origin_center_left;
			else if(hotspot == "center_right")
				origin = origin_center_right;
			else if(hotspot == "bottom_left")
				origin = origin_bottom_left;
			else if(hotspot == "bottom_center")
				origin = origin_bottom_center;
			else if(hotspot == "bottom_right")
				origin = origin_bottom_right;
			else
				origin = origin_top_left;

			int xoffset = atoi(cur_element.get_attribute("x", "0").c_str());
			int yoffset = atoi(cur_element.get_attribute("y", "0").c_str());

			sprite.set_alignment(origin, xoffset, yoffset);
		}
		// <rotation origin="string" x="integer" y="integer" />
		else if (tag_name == "rotation")
		{
			std::string hotspot = cur_element.get_attribute("origin", "center");
			CL_Origin origin;

			if(hotspot == "top_left")
				origin = origin_top_left;
			else if(hotspot == "top_center")
				origin = origin_top_center;
			else if(hotspot == "top_right")
				origin = origin_top_right;
			else if(hotspot == "center_left")
				origin = origin_center_left;
			else if(hotspot == "center_right")
				origin = origin_center_right;
			else if(hotspot == "bottom_left")
				origin = origin_bottom_left;
			else if(hotspot == "bottom_center")
				origin = origin_bottom_center;
			else if(hotspot == "bottom_right")
				origin = origin_bottom_right;
			else
				origin = origin_center;

			int xoffset = atoi(cur_element.get_attribute("x", "0").c_str());
			int yoffset = atoi(cur_element.get_attribute("y", "0").c_str());

			sprite.set_rotation_hotspot(origin, xoffset, yoffset);
		}
		// <frame nr="integer" speed="integer" x="integer" y="integer" />
		else if (tag_name == "frame")
		{
			int nr = atoi(cur_element.get_attribute("nr", "0").c_str());
			float delay = (float)atof(cur_element.get_attribute("speed", "60").c_str()) / 1000.0f;
			int yoffset = atoi(cur_element.get_attribute("y", "0").c_str());
			int xoffset = atoi(cur_element.get_attribute("x", "0").c_str());

			sprite.set_frame_delay(nr, delay);
			sprite.set_frame_offset(nr, CL_Point(xoffset, yoffset));
		}
	}
}
	
void CL_ResourceData_Sprite::on_unload()
{
	sprite = CL_Sprite();
}
