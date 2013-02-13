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
#include "particle_emitter_pass_data.h"
#include "Scene3D/scene_particle_emitter_impl.h"

namespace clan
{


ParticleEmitterPassData::ParticleEmitterPassData() : visible(), in_active_list(), time_to_next_emit(), last_emitted_index(), emitter()
{
}

bool ParticleEmitterPassData::update(float time_elapsed)
{
	if (visible)
	{
		size_t particles_required = (size_t)(emitter->life_span * emitter->particles_per_second) + 8;
		if (cpu_particles.size() < particles_required)
			cpu_particles.resize(particles_required);

		time_to_next_emit -= time_elapsed;
		if (time_to_next_emit <= 0.0f)
		{
			emit();
			time_to_next_emit = 1.0f / emitter->particles_per_second;
		}
	}

	bool particles_active = advance(time_elapsed);
	return visible || particles_active;
}

bool ParticleEmitterPassData::advance(float time_elapsed)
{
	bool active = false;
	for (size_t index = 0; index < cpu_particles.size(); index++)
	{
		if (cpu_particles[index].life > -1.0f)
		{
			cpu_particles[index].velocity += cpu_particles[index].acceleration * time_elapsed;
			cpu_particles[index].position += cpu_particles[index].velocity * time_elapsed;
			cpu_particles[index].life += cpu_particles[index].life_speed * time_elapsed;

			if (cpu_particles[index].life > 1.0f)
				cpu_particles[index].life = -1.0f;
			else
				active = true;
		}
	}
	return active;
}

void ParticleEmitterPassData::emit()
{
	Quaternionf particle_orientation;
	if (emitter->type == SceneParticleEmitter::type_spot)
		particle_orientation = emitter->orientation * Quaternionf(0.0f, random(-emitter->falloff, emitter->falloff), random(-180.0f, 180.0f), angle_degrees, order_ZYX);
	else
		particle_orientation = Quaternionf(random(-180.0f, 180.0f), random(-180.0f, 180.0f), random(-180.0f, 180.0f), angle_degrees, order_ZYX);

	last_emitted_index = (last_emitted_index + 1) % cpu_particles.size();
	cpu_particles[last_emitted_index].position = emitter->position;
	cpu_particles[last_emitted_index].velocity = particle_orientation.rotate_vector(Vec3f(0.0f,0.0f,1.0f)) * emitter->speed;
	cpu_particles[last_emitted_index].acceleration = emitter->acceleration;
	cpu_particles[last_emitted_index].life = 0.0f;
	cpu_particles[last_emitted_index].life_speed = 1.0f / emitter->life_span;
	cpu_particles[last_emitted_index].start_size = emitter->start_size;
	cpu_particles[last_emitted_index].end_size = emitter->end_size;
}

float ParticleEmitterPassData::random(float min_val, float max_val)
{
	float v = rand() / (float)RAND_MAX;
	return min_val + v * (max_val - min_val);
}

}

