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
#include "API/Scene3D/scene_model.h"
#include "API/Scene3D/scene.h"
#include "Scene3D/Model/model_cache.h"
#include "scene_impl.h"
#include "scene_model_impl.h"

namespace clan
{

SceneModel::SceneModel()
{
}

SceneModel::SceneModel(GraphicContext &gc, Scene &scene, const std::string &model_name)
: impl(new SceneModel_Impl())
{
	impl->scene = scene.impl.get();
	impl->model = impl->scene->model_cache.get_model(gc, model_name);
}

SceneModel::SceneModel(GraphicContext &gc, Scene &scene, std::shared_ptr<ModelData> model_data)
: impl(new SceneModel_Impl())
{
	impl->model = std::shared_ptr<Model>(new Model(gc, impl->scene->material_cache, impl->scene->model_shader_cache, model_data, impl->scene->instances_buffer.new_offset_index()));
}

bool SceneModel::is_null() const
{
	return !impl;
}

}
