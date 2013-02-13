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

#include "Scene/precomp.h"
#include "scene_particle_emitter.h"
#include "scene.h"
#include "Passes/ParticleEmitter/particle_emitter_pass_data.h"

namespace clan
{

SceneParticleEmitter::SceneParticleEmitter()
{
}

SceneParticleEmitter::SceneParticleEmitter(Scene *scene)
: impl(new SceneParticleEmitter_Impl(scene))
{
	impl->tree_object = scene->tree.add_object(impl.get(), impl->get_aabb());
}

SceneParticleEmitter::Type SceneParticleEmitter::get_type() const
{
	return impl->type;
}

Vec3f SceneParticleEmitter::get_position() const
{
	return impl->position;
}

Quaternionf SceneParticleEmitter::get_orientation() const
{
	return impl->orientation;
}

float SceneParticleEmitter::get_particles_per_second() const
{
	return impl->particles_per_second;
}

float SceneParticleEmitter::get_falloff() const
{
	return impl->falloff;
}

float SceneParticleEmitter::get_life_span() const
{
	return impl->life_span;
}

float SceneParticleEmitter::get_start_size() const
{
	return impl->start_size;
}

float SceneParticleEmitter::get_end_size() const
{
	return impl->end_size;
}

float SceneParticleEmitter::get_speed() const
{
	return impl->speed;
}

clan::Vec3f SceneParticleEmitter::get_acceleration() const
{
	return impl->acceleration;
}

std::string SceneParticleEmitter::get_particle_texture() const
{
	return impl->particle_texture;
}

std::string SceneParticleEmitter::get_gradient_texture() const
{
	return impl->gradient_texture;
}

void SceneParticleEmitter::set_type(Type type)
{
	impl->type = type;
}

void SceneParticleEmitter::set_position(const Vec3f &position)
{
	if (impl->position != position)
	{
		impl->position = position;
		if (impl->tree_object)
			impl->scene->tree.move_object(impl->tree_object, impl->get_aabb());
	}
}

void SceneParticleEmitter::set_orientation(const Quaternionf &orientation)
{
	impl->orientation = orientation;
}

void SceneParticleEmitter::set_particles_per_second(float pps)
{
	impl->particles_per_second = pps;
}

void SceneParticleEmitter::set_falloff(float falloff)
{
	impl->falloff = falloff;
}

void SceneParticleEmitter::set_life_span(float life_span)
{
	impl->life_span = life_span;
}

void SceneParticleEmitter::set_start_size(float size)
{
	impl->start_size = size;
}

void SceneParticleEmitter::set_end_size(float size)
{
	impl->end_size = size;
}

void SceneParticleEmitter::set_speed(float speed)
{
	impl->speed = speed;
}

void SceneParticleEmitter::set_acceleration(const clan::Vec3f &acceleration)
{
	impl->acceleration = acceleration;
}

void SceneParticleEmitter::set_particle_texture(const std::string &texture)
{
	impl->particle_texture = texture;
}

void SceneParticleEmitter::set_gradient_texture(const std::string &texture)
{
	impl->gradient_texture = texture;
}

/////////////////////////////////////////////////////////////////////////////

SceneParticleEmitter_Impl::SceneParticleEmitter_Impl(Scene *scene)
: scene(scene), tree_object(0), type(SceneParticleEmitter::type_omni), particles_per_second(10), falloff(90.0f), life_span(5.0f), start_size(1.0f), end_size(2.0f), speed(10.0f)
{
	it = scene->emitters.insert(scene->emitters.end(), this);
}

SceneParticleEmitter_Impl::~SceneParticleEmitter_Impl()
{
	if (tree_object)
		scene->tree.remove_object(tree_object);
	scene->emitters.erase(it);

	if (pass_data)
	{
		pass_data->visible = false;
		pass_data->emitter = 0;
	}
}

AxisAlignedBoundingBox SceneParticleEmitter_Impl::get_aabb()
{
	float attenuation_end = 30.0f;
	AxisAlignedBoundingBox aabb;
	aabb.aabb_min = position - attenuation_end * 1.41421356f;
	aabb.aabb_max = position + attenuation_end * 1.41421356f;
	return aabb;
}

}

