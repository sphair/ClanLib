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

#pragma once

namespace clan
{

class Model;
class ModelInstanceUniforms;
class ModelMeshVisitor;
class AxisAlignedBoundingBox;

class ModelReplacedMaterial
{
public:
	size_t texture_index;
	Resource<Texture> replaced_texture;
};

class ModelInstance
{
public:
	ModelInstance();
	std::shared_ptr<Model> get_renderer() const { return renderer; }
	void set_renderer(std::shared_ptr<Model> renderer);
	void play_animation(const std::string &name);
	void update(float time_elapsed);
	void moved(float units_moved);

	int get_animation_index() const { return animation_index; }
	float get_animation_time() const { return animation_time; }

private:
	void update_animation_index();

	std::string animation_name;
	int animation_index;
	float animation_time;
	std::vector<ModelReplacedMaterial> replaced_materials;
	std::shared_ptr<Model> renderer;

	// To do: add selection id

	friend class Model;
};

}

