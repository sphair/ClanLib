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
#include "API/Scene3D/scene_camera.h"
#include "API/Scene3D/scene.h"
#include "scene_camera_impl.h"
#include "scene_impl.h"

namespace clan
{

SceneCamera::SceneCamera()
{
}

SceneCamera::SceneCamera(Scene &scene)
: impl(new SceneCamera_Impl())
{
	impl->scene = scene.impl.get();
}

bool SceneCamera::is_null() const
{
	return !impl;
}

Vec3f SceneCamera::get_position() const
{
	return impl->position;
}

Quaternionf SceneCamera::get_orientation() const
{
	return impl->orientation;
}

float SceneCamera::get_field_of_view() const
{
	return impl->fov;
}

void SceneCamera::set_position(const Vec3f &position)
{
	impl->position = position;
}

void SceneCamera::set_orientation(const Quaternionf &orientation)
{
	impl->orientation = orientation;
}

void SceneCamera::set_field_of_view(float fov)
{
	impl->fov = fov;
}

}
