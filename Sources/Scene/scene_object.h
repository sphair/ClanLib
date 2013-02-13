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
#include "scene_light.h"
#include "Model/model_instance.h"
#include <list>
namespace clan
{

class Scene;
class SceneObject_Impl;
class OctTreeObject;
class ModelData;

class SceneObject
{
public:
	SceneObject();
	SceneObject(clan::GraphicContext &gc, Scene *scene, const std::string &model_name, const clan::Vec3f &position = clan::Vec3f(0.0f), const clan::Quaternionf &orientation = clan::Quaternionf(), const clan::Vec3f &scale = clan::Vec3f(1.0f));
	SceneObject(clan::GraphicContext &gc, Scene *scene, std::shared_ptr<ModelData> model_data, const clan::Vec3f &position = clan::Vec3f(0.0f), const clan::Quaternionf &orientation = clan::Quaternionf(), const clan::Vec3f &scale = clan::Vec3f(1.0f));
	bool is_null() const { return !impl; }

	clan::Vec3f get_position() const;
	clan::Quaternionf get_orientation() const;
	clan::Vec3f get_scale() const;
	void set_position(const clan::Vec3f &position);
	void set_orientation(const clan::Quaternionf &orientation);
	void set_scale(const clan::Vec3f &scale);

	// \brief Moves object relative to its current orientation
	SceneObject &move(clan::Vec3f offset);

	// \brief Rotate object relative to its current orientation
	SceneObject &rotate(float dir, float up, float tilt);

	void play_animation(const std::string &name);
	void update(float time_elapsed);
	void moved(float units_moved);

private:
	std::shared_ptr<SceneObject_Impl> impl;
};

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

