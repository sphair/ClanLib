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
#include "model_cache.h"
#include "Scene3D/Framework/instances_buffer.h"
#include "API/Scene3D/ModelData/CModel/cmodel_format.h"

namespace clan
{


ModelCache::ModelCache(WorkQueue &work_queue, ModelMaterialCache &texture_cache, ModelShaderCache &shader_cache, InstancesBuffer &instances_buffer)
: work_queue(work_queue), texture_cache(texture_cache), shader_cache(shader_cache), instances_buffer(instances_buffer)
{
}

std::shared_ptr<Model> ModelCache::get_model(GraphicContext &gc, const std::string &model_name)
{
	std::shared_ptr<Model> renderer = models[model_name];
	if (!renderer)
	{
		renderer = std::shared_ptr<Model>(new Model(gc, texture_cache, shader_cache, CModelFormat::load(PathHelp::combine("Resources/Baleout/Scene", model_name)), instances_buffer.new_offset_index()));
		models[model_name]= renderer;
	}
	return renderer;
}

}

