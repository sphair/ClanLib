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

#pragma once

#include "Culling/visible_object.h"
#include <list>

namespace clan
{

class Scene_Impl;

class SceneParticleEmitter_Impl : public VisibleObject
{
public:
	SceneParticleEmitter_Impl(Scene_Impl *scene);
	~SceneParticleEmitter_Impl();

	AxisAlignedBoundingBox get_aabb();

	Scene_Impl *scene;
	OctTreeObject *tree_object;
	std::list<SceneParticleEmitter_Impl *>::iterator it;

	SceneParticleEmitter::Type type;
	Vec3f position;
	Quaternionf orientation;

	float particles_per_second;
	float falloff;
	float life_span;
	float start_size;
	float end_size;
	float speed;
	Vec3f acceleration;
	std::string particle_texture;
	std::string gradient_texture;

	std::shared_ptr<ParticleEmitterPassData> pass_data;
};

class SceneParticleEmitterVisitor
{
public:
	virtual void emitter(GraphicContext &gc, const Mat4f &world_to_eye, const Mat4f &eye_to_projection, SceneParticleEmitter_Impl *emitter) = 0;
};

}

