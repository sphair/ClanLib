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

#include "model_mesh_buffers.h"
#include "Scene3D/Framework/texture.h"
#include "Scene3D/scene.h"
namespace clan
{

class ModelRenderCommand
{
public:
	virtual ~ModelRenderCommand() { }
	virtual void execute(clan::GraphicContext &gc, int num_instances) = 0;
};

class ModelRenderCommandList
{
public:
	~ModelRenderCommandList()
	{
		for (size_t i = 0; i < commands.size(); i++)
			delete commands[i];
	}

	void execute(clan::GraphicContext &gc, int num_instances)
	{
		for (size_t i = 0; i < commands.size(); i++)
			commands[i]->execute(gc, num_instances);
	}

	std::vector<ModelRenderCommand *> commands;
};

class ModelRenderCommand_BindMeshBuffers : public ModelRenderCommand
{
public:
	ModelRenderCommand_BindMeshBuffers(ModelMeshBuffers *buffers) : buffers(buffers)
	{
	}

	void execute(clan::GraphicContext &gc, int num_instances)
	{
		gc.set_primitives_array(buffers->primitives_array);
		gc.set_primitives_elements(buffers->elements);
	}

	ModelMeshBuffers *buffers;
};

class ModelRenderCommand_BindShader : public ModelRenderCommand
{
public:
	ModelRenderCommand_BindShader(clan::ProgramObject shader) : shader(shader)
	{
	}

	void execute(clan::GraphicContext &gc, int num_instances)
	{
		gc.set_program_object(shader);
	}

	clan::ProgramObject shader;
};

class ModelRenderCommand_BindTexture : public ModelRenderCommand
{
public:
	ModelRenderCommand_BindTexture(int bind_index, CachedTexture texture, clan::TextureWrapMode wrap_u, clan::TextureWrapMode wrap_v) : bind_index(bind_index), texture(texture), wrap_u(wrap_u), wrap_v(wrap_v)
	{
	}

	void execute(clan::GraphicContext &gc, int num_instances)
	{
		texture.get_texture().to_texture_2d().set_wrap_mode(wrap_u, wrap_v);
		gc.set_texture(bind_index, texture.get_texture());
	}

	int bind_index;
	CachedTexture texture;
	clan::TextureWrapMode wrap_u, wrap_v;
};

class ModelRenderCommand_SetRasterizerState : public ModelRenderCommand
{
public:
	ModelRenderCommand_SetRasterizerState(clan::RasterizerState state) : state(state)
	{
	}

	void execute(clan::GraphicContext &gc, int num_instances)
	{
		gc.set_rasterizer_state(state);
	}

	clan::RasterizerState state;
};

class ModelRenderCommand_DrawElements : public ModelRenderCommand
{
public:
	ModelRenderCommand_DrawElements(int start_element, int num_elements, clan::UniformVector<ModelMaterialUniforms> uniforms)
		: start_element(start_element), num_elements(num_elements), uniforms(uniforms)
	{
	}

	void execute(clan::GraphicContext &gc, int num_instances)
	{
		gc.set_uniform_buffer(0, uniforms);
		gc.draw_primitives_elements_instanced(clan::type_triangles, num_elements, clan::type_unsigned_int, start_element * sizeof(unsigned int), num_instances);

		Scene::draw_calls++;
		Scene::triangles_drawn += num_elements / 3;
	}

	int start_element;
	int num_elements;
	clan::UniformVector<ModelMaterialUniforms> uniforms;
};

}

