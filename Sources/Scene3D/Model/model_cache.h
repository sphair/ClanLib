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

#include "Scene3D/Framework/work_queue.h"
#include <unordered_map>
#include "model.h"
namespace clan
{

class InstancesBuffer;

class ModelCache
{
public:
	ModelCache(WorkQueue &work_queue, ModelMaterialCache &texture_cache, ModelShaderCache &shader_cache, InstancesBuffer &instances_buffer);
	std::shared_ptr<Model> get_model(clan::GraphicContext &gc, const std::string &model_name);

private:
	std::unordered_map<std::string, std::shared_ptr<Model> > models;
	WorkQueue &work_queue;
	ModelMaterialCache &texture_cache;
	ModelShaderCache &shader_cache;
	InstancesBuffer &instances_buffer;
};

}

