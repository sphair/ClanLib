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
**    Magnus Norddahl
*/

#include "Scene3D/precomp.h"
#include "model_instance.h"
#include "model.h"
#include "Scene3D/ModelData/model_data.h"
#include "Scene3D/Culling/aabb.h"

namespace clan
{


ModelInstance::ModelInstance()
: animation_name("default"), animation_index(-1), animation_time(0.0f)
{
}

void ModelInstance::set_renderer(std::shared_ptr<Model> new_renderer)
{
	renderer = new_renderer;
	update_animation_index();
}

void ModelInstance::play_animation(const std::string &name)
{
	animation_name = name;
	animation_time = 0.0f;
	update_animation_index();
}

void ModelInstance::update(float time_elapsed)
{
	if (animation_index != -1)
	{
		if (renderer->get_model_data()->animations[animation_index].length <= 0.0f)
		{
			animation_time = 0.0f;
		}
		else
		{
			animation_time += time_elapsed * renderer->get_model_data()->animations[animation_index].playback_speed;
			if (renderer->get_model_data()->animations[animation_index].loop)
			{
				while (animation_time > renderer->get_model_data()->animations[animation_index].length)
					animation_time -= renderer->get_model_data()->animations[animation_index].length;
			}
			else
			{
				animation_time = renderer->get_model_data()->animations[animation_index].length;
			}
		}
	}
}

void ModelInstance::moved(float units_moved)
{
	if (animation_index != -1)
	{
		if (renderer->get_model_data()->animations[animation_index].length <= 0.0f)
		{
			animation_time = 0.0f;
		}
		else
		{
			float local_distance_moved = units_moved;
			animation_time += local_distance_moved * renderer->get_model_data()->animations[animation_index].moving_speed;
			while (animation_time > renderer->get_model_data()->animations[animation_index].length)
				animation_time -= renderer->get_model_data()->animations[animation_index].length;
		}
	}
}

void ModelInstance::update_animation_index()
{
	animation_index = -1;
	if (renderer)
	{
		int playback_rarity = -1;
		for (size_t i = 0; i < renderer->get_model_data()->animations.size(); i++)
		{
			if (renderer->get_model_data()->animations[i].name == animation_name)
			{
				if (renderer->get_model_data()->animations[i].rarity != 0xffff && playback_rarity == -1)
					playback_rarity = rand() * 0xffff / RAND_MAX;

				if (playback_rarity <= renderer->get_model_data()->animations[i].rarity)
				{
					animation_index = i;
					break;
				}
				else
				{
					playback_rarity -= renderer->get_model_data()->animations[i].rarity;
				}
			}
		}
	}
}

}

