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

#include "model_render_command.h"
#include "model_mesh_buffers.h"
namespace clan
{

class ModelData;

class ModelLOD
{
public:
	ModelLOD(GraphicContext &gc, int model_index, std::shared_ptr<ModelData> model_data, int level);

	std::vector<ModelMeshBuffers> mesh_buffers;

	ModelRenderCommandList gbuffer_commands;
	ModelRenderCommandList transparency_commands;
	ModelRenderCommandList shadow_commands;
	ModelRenderCommandList early_z_commands;

private:
	template<typename Type>
	VertexArrayVector<Type> upload_vector(GraphicContext &gc, PrimitivesArray &primitives_array, int index, const std::vector<Type> &vec);

	template<typename Type>
	VertexArrayVector<Type> upload_vector(GraphicContext &gc, PrimitivesArray &primitives_array, int index, const std::vector<Type> &vec, bool normalize);
};

}

