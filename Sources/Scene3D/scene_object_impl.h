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

#include "API/Scene/scene_light.h"
#include "Culling/visible_object.h"
#include "Model/model_instance.h"
#include <list>

namespace clan
{

class SceneObject_Impl : public VisibleObject
{
public:
	SceneObject_Impl(Scene *scene);
	~SceneObject_Impl();

	void create_lights(Scene *scene);
	void update_lights();

	clan::Mat4f get_object_to_world() const
	{
		return clan::Mat4f::translate(position) * orientation.to_matrix() * clan::Mat4f::scale(scale);
	}

	AxisAlignedBoundingBox get_aabb() const;

	Scene *scene;
	OctTreeObject *tree_object;
	std::list<SceneObject_Impl *>::iterator it;

	clan::Vec3f position;
	clan::Quaternionf orientation;
	clan::Vec3f scale;

	ModelInstance instance;
	std::vector<SceneLight> lights;
};

}

