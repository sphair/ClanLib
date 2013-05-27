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
**    Magnus Norddahl
*/

#include "Scene3D/precomp.h"
#include "API/Scene3D/scene_light_probe.h"
#include "API/Scene3D/scene.h"
#include "API/Core/Math/aabb.h"
#include "scene_light_probe_impl.h"
#include "scene_impl.h"

namespace clan
{

SceneLightProbe::SceneLightProbe()
{
}

SceneLightProbe::SceneLightProbe(Scene &scene)
	: impl(new SceneLightProbe_Impl(scene.impl.get()))
{
	impl->cull_proxy = impl->scene->cull_provider->create_proxy(impl.get(), impl->get_aabb());
}

Vec3f SceneLightProbe::get_position() const
{
	return impl->position;
}

float SceneLightProbe::get_radius() const
{
	return impl->radius;
}

Vec3f SceneLightProbe::get_color() const
{
	return impl->color;
}

void SceneLightProbe::set_position(const Vec3f &position)
{
	if (impl->position != position)
	{
		impl->position = position;
		if (impl->cull_proxy)
			impl->scene->cull_provider->set_aabb(impl->cull_proxy, impl->get_aabb());
	}
}

void SceneLightProbe::set_radius(float radius)
{
	if (impl->radius != radius)
	{
		impl->radius = radius;
		if (impl->cull_proxy)
			impl->scene->cull_provider->set_aabb(impl->cull_proxy, impl->get_aabb());
	}
}

void SceneLightProbe::set_color(const Vec3f &color)
{
	impl->color = color;
}

/////////////////////////////////////////////////////////////////////////////

SceneLightProbe_Impl::SceneLightProbe_Impl(Scene_Impl *scene)
: scene(scene), cull_proxy(0), radius(1.0f)
{
	it = scene->light_probes.insert(scene->light_probes.end(), this);
}

SceneLightProbe_Impl::~SceneLightProbe_Impl()
{
	if (cull_proxy)
		scene->cull_provider->delete_proxy(cull_proxy);
	scene->light_probes.erase(it);
}

AxisAlignedBoundingBox SceneLightProbe_Impl::get_aabb()
{
	AxisAlignedBoundingBox aabb;
	aabb.aabb_min = position - radius * 1.73205081f;
	aabb.aabb_max = position + radius * 1.73205081f;
	return aabb;
}

}

