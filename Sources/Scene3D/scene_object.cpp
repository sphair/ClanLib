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
#include "API/Scene3D/scene_object.h"
#include "API/Scene3D/scene.h"
#include <algorithm>
#include "Scene3D/scene_object_impl.h"

namespace clan
{

SceneObject::SceneObject()
{
}

SceneObject::SceneObject(GraphicContext &gc, Scene *scene, const std::string &model_name, const Vec3f &position, const Quaternionf &orientation, const Vec3f &scale)
: impl(new SceneObject_Impl(scene))
{
	impl->position = position;
	impl->orientation = orientation;
	impl->scale = scale;
	impl->instance.set_renderer(scene->model_cache.get_model(gc, model_name));
	impl->tree_object = scene->tree.add_object(impl.get(), impl->get_aabb());

	impl->create_lights(scene);
}

SceneObject::SceneObject(GraphicContext &gc, Scene *scene, std::shared_ptr<ModelData> model_data, const Vec3f &position, const Quaternionf &orientation, const Vec3f &scale)
: impl(new SceneObject_Impl(scene))
{
	std::shared_ptr<Model> model(new Model(gc, scene->material_cache, scene->model_shader_cache, model_data, scene->instances_buffer.new_offset_index()));

	impl->position = position;
	impl->orientation = orientation;
	impl->scale = scale;
	impl->instance.set_renderer(model);
	impl->tree_object = scene->tree.add_object(impl.get(), impl->get_aabb());

	impl->create_lights(scene);
}

Vec3f SceneObject::get_position() const
{
	return impl->position;
}

Quaternionf SceneObject::get_orientation() const
{
	return impl->orientation;
}

Vec3f SceneObject::get_scale() const
{
	return impl->scale;
}

void SceneObject::set_position(const Vec3f &position)
{
	if (impl->position != position)
	{
		impl->position = position;
		if (impl->tree_object)
			impl->scene->tree.move_object(impl->tree_object, impl->get_aabb());
		impl->update_lights();
	}
}

void SceneObject::set_orientation(const Quaternionf &orientation)
{
	impl->orientation = orientation;
	impl->update_lights();
}

void SceneObject::set_scale(const Vec3f &scale)
{
	if (impl->scale != scale)
	{
		impl->scale = scale;
		if (impl->tree_object)
			impl->scene->tree.move_object(impl->tree_object, impl->get_aabb());
		impl->update_lights();
	}
}

void SceneObject::play_animation(const std::string &name)
{
	impl->instance.play_animation(name);
	impl->update_lights();
}

void SceneObject::update(float time_elapsed)
{
	impl->instance.update(time_elapsed);
	impl->update_lights();
}

void SceneObject::moved(float units_moved)
{
	impl->instance.moved(units_moved / impl->scale.y);
	impl->update_lights();
}

SceneObject &SceneObject::move(Vec3f offset)
{
	set_position(get_position() + get_orientation().rotate_vector(offset));
	return *this;
}

SceneObject &SceneObject::rotate(float dir, float up, float tilt)
{
	set_orientation(get_orientation() * Quaternionf(up, dir, tilt, angle_degrees, order_YXZ));
	return *this;
}

/////////////////////////////////////////////////////////////////////////////

SceneObject_Impl::SceneObject_Impl(Scene *scene)
: scene(scene), tree_object(0), scale(1.0f)
{
	it = scene->objects.insert(scene->objects.end(), this);
}

SceneObject_Impl::~SceneObject_Impl()
{
	if (tree_object)
		scene->tree.remove_object(tree_object);
	scene->objects.erase(it);
}

void SceneObject_Impl::create_lights(Scene *scene)
{
	std::vector<ModelDataLight> &model_lights = instance.get_renderer()->get_model_data()->lights;
	for (size_t i = 0; i < model_lights.size(); i++)
		lights.push_back(SceneLight(scene));
}

void SceneObject_Impl::update_lights()
{
	Mat4f object_to_world = get_object_to_world();

	int animation_index = instance.get_animation_index();
	float animation_time = instance.get_animation_time();
	if (animation_index != -1)
	{
		std::vector<ModelDataLight> &model_lights = instance.get_renderer()->get_model_data()->lights;
		for (size_t i = 0; i < model_lights.size(); i++)
		{
			// To do: apply bone_selector
			lights[i].set_type(model_lights[i].falloff.get_value(animation_index, animation_time) == 0.0f ? SceneLight::type_omni : SceneLight::type_spot);
			lights[i].set_position(Vec3f(object_to_world * Vec4f(model_lights[i].position.get_value(animation_index, animation_time), 1.0f)));
			lights[i].set_orientation(orientation * model_lights[i].orientation.get_value(animation_index, animation_time));
			lights[i].set_attenuation_start(model_lights[i].attenuation_start.get_value(animation_index, animation_time) * scale.y);
			lights[i].set_attenuation_end(model_lights[i].attenuation_end.get_value(animation_index, animation_time) * scale.y);
			lights[i].set_color(model_lights[i].color.get_value(animation_index, animation_time));
			lights[i].set_aspect_ratio(model_lights[i].aspect.get_value(animation_index, animation_time));
			lights[i].set_falloff(model_lights[i].falloff.get_value(animation_index, animation_time));
			lights[i].set_hotspot(model_lights[i].hotspot.get_value(animation_index, animation_time));
			lights[i].set_shadow_caster(model_lights[i].casts_shadows);
			lights[i].set_light_caster(true);
			lights[i].set_rectangle_shape(model_lights[i].rectangle);
		}
	}
}

AxisAlignedBoundingBox SceneObject_Impl::get_aabb() const
{
	if (instance.get_renderer())
		return AxisAlignedBoundingBox(instance.get_renderer()->get_model_data()->aabb_min * scale + position, instance.get_renderer()->get_model_data()->aabb_max * scale + position);
	else
		return AxisAlignedBoundingBox();
}

}

