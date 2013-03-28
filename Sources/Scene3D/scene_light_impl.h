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

class SceneLight_Impl : public VisibleObject
{
public:
	SceneLight_Impl(Scene_Impl *scene);
	~SceneLight_Impl();

	AxisAlignedBoundingBox get_aabb();

	Scene_Impl *scene;
	OctTreeObject *tree_object;
	std::list<SceneLight_Impl *>::iterator it;

	SceneLight::Type type;
	Vec3f position;
	Quaternionf orientation;

	Vec3f color;
	float falloff;
	float hotspot;
	float ambient_illumination;
	float attenuation_start;
	float attenuation_end;
	bool rectangle_shape;
	float aspect_ratio;

	bool shadow_caster;
	bool light_caster;
	SceneLight shadow_source;

	std::unique_ptr<VSMShadowMapPassLightData> vsm_data;
};

class SceneLightVisitor
{
public:
	virtual void light(GraphicContext &gc, const Mat4f &world_to_eye, const Mat4f &eye_to_projection, SceneLight_Impl *light) = 0;
};

}

