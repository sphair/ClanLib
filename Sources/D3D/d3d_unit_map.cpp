/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

#include "D3D/precomp.h"
#include "d3d_unit_map.h"
#include "d3d_graphic_context_provider.h"
#include "d3d_program_object_provider.h"
#include "d3d_uniform_buffer_provider.h"
#include "d3d_storage_buffer_provider.h"
#include "d3d_display_window_provider.h"
#include "d3d_texture_provider.h"

namespace clan
{
	void D3DUnitMap::bind_program(D3DGraphicContextProvider *gc, D3DProgramObjectProvider *program)
	{
		for (size_t i = 0; i < program->uniforms.size(); i++)
		{
			switch (program->uniforms[i].type)
			{
			case D3DUniform::Type::sampler:
				if (sampler_units.size() < program->uniforms[i].value + 1)
					sampler_units.resize(program->uniforms[i].value + 1);
				for (int j = 0; j < static_cast<int>(ShaderType::num_types); j++)
					sampler_units[program->uniforms[i].value].shader_index[j] = program->uniforms[i].shader_index[j];
				bind_sampler(gc, program->uniforms[i].value);
				break;
			case D3DUniform::Type::texture:
				if (texture_units.size() < program->uniforms[i].value + 1)
					texture_units.resize(program->uniforms[i].value + 1);
				for (int j = 0; j < static_cast<int>(ShaderType::num_types); j++)
					texture_units[program->uniforms[i].value].shader_index[j] = program->uniforms[i].shader_index[j];
				bind_texture(gc, program->uniforms[i].value);
				break;
			case D3DUniform::Type::image:
				if (image_units.size() < program->uniforms[i].value + 1)
					image_units.resize(program->uniforms[i].value + 1);
				for (int j = 0; j < static_cast<int>(ShaderType::num_types); j++)
					image_units[program->uniforms[i].value].shader_index[j] = program->uniforms[i].shader_index[j];
				bind_image(gc, program->uniforms[i].value);
				break;
			}
		}

		for (size_t i = 0; i < program->uniform_blocks.size(); i++)
		{
			if (uniform_units.size() < program->uniform_blocks[i].value + 1)
				uniform_units.resize(program->uniform_blocks[i].value + 1);
			for (int j = 0; j < static_cast<int>(ShaderType::num_types); j++)
				uniform_units[program->uniform_blocks[i].value].shader_index[j] = program->uniform_blocks[i].shader_index[j];
			bind_uniform_buffer(gc, program->uniform_blocks[i].value);
		}

		for (size_t i = 0; i < program->storage_blocks.size(); i++)
		{
			if (storage_units.size() < program->storage_blocks[i].value + 1)
				storage_units.resize(program->storage_blocks[i].value + 1);
			for (int j = 0; j < static_cast<int>(ShaderType::num_types); j++)
			{
				storage_units[program->storage_blocks[i].value].shader_srv_index[j] = program->storage_blocks[i].shader_srv_index[j];
				storage_units[program->storage_blocks[i].value].shader_uav_index[j] = program->storage_blocks[i].shader_uav_index[j];
			}
			bind_storage_buffer(gc, program->storage_blocks[i].value);
		}
	}

	void D3DUnitMap::unbind_program(D3DGraphicContextProvider *gc, D3DProgramObjectProvider *program)
	{
		for (size_t i = 0; i < program->uniforms.size(); i++)
		{
			switch (program->uniforms[i].type)
			{
			case D3DUniform::Type::sampler:
				unbind_sampler(gc, program->uniforms[i].value);
				for (int j = 0; j < static_cast<int>(ShaderType::num_types); j++)
					sampler_units[program->uniforms[i].value].shader_index[j] = -1;
				break;
			case D3DUniform::Type::texture:
				unbind_texture(gc, program->uniforms[i].value);
				for (int j = 0; j < static_cast<int>(ShaderType::num_types); j++)
					texture_units[program->uniforms[i].value].shader_index[j] = -1;
				break;
			case D3DUniform::Type::image:
				unbind_image(gc, program->uniforms[i].value);
				for (int j = 0; j < static_cast<int>(ShaderType::num_types); j++)
					image_units[program->uniforms[i].value].shader_index[j] = -1;
				break;
			}
		}

		for (size_t i = 0; i < program->uniform_blocks.size(); i++)
		{
			unbind_uniform_buffer(gc, program->uniform_blocks[i].value);
			for (int j = 0; j < static_cast<int>(ShaderType::num_types); j++)
				uniform_units[program->uniform_blocks[i].value].shader_index[j] = -1;
		}

		for (size_t i = 0; i < program->storage_blocks.size(); i++)
		{
			unbind_storage_buffer(gc, program->storage_blocks[i].value);
			for (int j = 0; j < static_cast<int>(ShaderType::num_types); j++)
			{
				storage_units[program->storage_blocks[i].value].shader_srv_index[j] = -1;
				storage_units[program->storage_blocks[i].value].shader_uav_index[j] = -1;
			}
		}
	}

	void D3DUnitMap::set_sampler(D3DGraphicContextProvider *gc, int index, const Texture &texture)
	{
		if (sampler_units.size() < index + 1)
			sampler_units.resize(index + 1);
		sampler_units[index].object = texture;
		bind_sampler(gc, index);
	}

	void D3DUnitMap::set_texture(D3DGraphicContextProvider *gc, int index, const Texture &texture)
	{
		if (texture_units.size() < index + 1)
			texture_units.resize(index + 1);
		texture_units[index].object = texture;
		bind_texture(gc, index);

		set_sampler(gc, index, texture);
	}

	void D3DUnitMap::set_image(D3DGraphicContextProvider *gc, int index, const Texture &texture)
	{
		if (image_units.size() < index + 1)
			image_units.resize(index + 1);
		image_units[index].object = texture;
		bind_image(gc, index);
	}

	void D3DUnitMap::set_uniform_buffer(D3DGraphicContextProvider *gc, int index, const UniformBuffer &buffer)
	{
		if (uniform_units.size() < index + 1)
			uniform_units.resize(index + 1);
		uniform_units[index].object = buffer;
		bind_uniform_buffer(gc, index);
	}

	void D3DUnitMap::set_storage_buffer(D3DGraphicContextProvider *gc, int index, const StorageBuffer &buffer)
	{
		if (storage_units.size() < index + 1)
			storage_units.resize(index + 1);
		storage_units[index].object = buffer;
		bind_storage_buffer(gc, index);
	}

	void D3DUnitMap::bind_sampler(D3DGraphicContextProvider *gc, int index)
	{
		if (sampler_units.size() > index)
		{
			for (int j = 0; j < static_cast<int>(ShaderType::num_types); j++)
			{
				if (sampler_units[index].shader_index[j] != -1)
				{
					ID3D11SamplerState *sampler_state = 0;
					if (!sampler_units[index].object.is_null())
						sampler_state = static_cast<D3DTextureProvider*>(sampler_units[index].object.get_provider())->get_sampler_state(gc->get_window()->get_device());
					switch (static_cast<ShaderType>(j))
					{
					case ShaderType::vertex:
						gc->get_window()->get_device_context()->VSSetSamplers(sampler_units[index].shader_index[j], 1, &sampler_state);
						break;
					case ShaderType::tess_control:
						gc->get_window()->get_device_context()->HSSetSamplers(sampler_units[index].shader_index[j], 1, &sampler_state);
						break;
					case ShaderType::tess_evaluation:
						gc->get_window()->get_device_context()->DSSetSamplers(sampler_units[index].shader_index[j], 1, &sampler_state);
						break;
					case ShaderType::geometry:
						gc->get_window()->get_device_context()->GSSetSamplers(sampler_units[index].shader_index[j], 1, &sampler_state);
						break;
					case ShaderType::fragment:
						gc->get_window()->get_device_context()->PSSetSamplers(sampler_units[index].shader_index[j], 1, &sampler_state);
						break;
					case ShaderType::compute:
						gc->get_window()->get_device_context()->CSSetSamplers(sampler_units[index].shader_index[j], 1, &sampler_state);
						break;
					}
				}
			}
		}
	}

	void D3DUnitMap::bind_texture(D3DGraphicContextProvider *gc, int index)
	{
		if (texture_units.size() > index)
		{
			for (int j = 0; j < static_cast<int>(ShaderType::num_types); j++)
			{
				if (texture_units[index].shader_index[j] != -1)
				{
					ID3D11ShaderResourceView *srv = 0;
					if (!texture_units[index].object.is_null())
						srv = static_cast<D3DTextureProvider*>(texture_units[index].object.get_provider())->get_srv(gc->get_window()->get_device());
					switch (static_cast<ShaderType>(j))
					{
					case ShaderType::vertex:
						gc->get_window()->get_device_context()->VSSetShaderResources(texture_units[index].shader_index[j], 1, &srv);
						break;
					case ShaderType::tess_control:
						gc->get_window()->get_device_context()->HSSetShaderResources(texture_units[index].shader_index[j], 1, &srv);
						break;
					case ShaderType::tess_evaluation:
						gc->get_window()->get_device_context()->DSSetShaderResources(texture_units[index].shader_index[j], 1, &srv);
						break;
					case ShaderType::geometry:
						gc->get_window()->get_device_context()->GSSetShaderResources(texture_units[index].shader_index[j], 1, &srv);
						break;
					case ShaderType::fragment:
						gc->get_window()->get_device_context()->PSSetShaderResources(texture_units[index].shader_index[j], 1, &srv);
						break;
					case ShaderType::compute:
						gc->get_window()->get_device_context()->CSSetShaderResources(texture_units[index].shader_index[j], 1, &srv);
						break;
					}
				}
			}
		}
	}

	void D3DUnitMap::bind_image(D3DGraphicContextProvider *gc, int index)
	{
		if (image_units.size() > index)
		{
			// Seems Direct3D only supports UAVs in compute shaders?
			if (image_units[index].shader_index[static_cast<int>(ShaderType::compute)] != -1)
			{
				ID3D11UnorderedAccessView *uav = 0;
				UINT uav_initial_count_value = 0;
				if (!image_units[index].object.is_null())
					uav = static_cast<D3DTextureProvider*>(image_units[index].object.get_provider())->get_uav(gc->get_window()->get_device());
				gc->get_window()->get_device_context()->CSSetUnorderedAccessViews(image_units[index].shader_index[static_cast<int>(ShaderType::compute)], 1, &uav, &uav_initial_count_value);
			}
		}
	}

	void D3DUnitMap::bind_uniform_buffer(D3DGraphicContextProvider *gc, int index)
	{
		if (uniform_units.size() > index)
		{
			for (int j = 0; j < static_cast<int>(ShaderType::num_types); j++)
			{
				if (uniform_units[index].shader_index[j] != -1)
				{
					ID3D11Buffer *d3d_buffer = 0;
					if (!uniform_units[index].object.is_null())
						d3d_buffer = static_cast<D3DUniformBufferProvider*>(uniform_units[index].object.get_provider())->get_buffer(gc->get_window()->get_device());
					switch (static_cast<ShaderType>(j))
					{
					case ShaderType::vertex:
						gc->get_window()->get_device_context()->VSSetConstantBuffers(uniform_units[index].shader_index[j], 1, &d3d_buffer);
						break;
					case ShaderType::tess_control:
						gc->get_window()->get_device_context()->HSSetConstantBuffers(uniform_units[index].shader_index[j], 1, &d3d_buffer);
						break;
					case ShaderType::tess_evaluation:
						gc->get_window()->get_device_context()->DSSetConstantBuffers(uniform_units[index].shader_index[j], 1, &d3d_buffer);
						break;
					case ShaderType::geometry:
						gc->get_window()->get_device_context()->GSSetConstantBuffers(uniform_units[index].shader_index[j], 1, &d3d_buffer);
						break;
					case ShaderType::fragment:
						gc->get_window()->get_device_context()->PSSetConstantBuffers(uniform_units[index].shader_index[j], 1, &d3d_buffer);
						break;
					case ShaderType::compute:
						gc->get_window()->get_device_context()->CSSetConstantBuffers(uniform_units[index].shader_index[j], 1, &d3d_buffer);
						break;
					}
				}
			}
		}
	}

	void D3DUnitMap::bind_storage_buffer(D3DGraphicContextProvider *gc, int index)
	{
		if (storage_units.size() > index)
		{
			for (int j = 0; j < static_cast<int>(ShaderType::num_types); j++)
			{
				if (storage_units[index].shader_srv_index[j] != -1)
				{
					ID3D11ShaderResourceView *srv = 0;
					if (!storage_units[index].object.is_null())
						srv = static_cast<D3DStorageBufferProvider*>(storage_units[index].object.get_provider())->get_srv(gc->get_window()->get_device());
					switch (static_cast<ShaderType>(j))
					{
					case ShaderType::vertex:
						gc->get_window()->get_device_context()->VSSetShaderResources(storage_units[index].shader_srv_index[j], 1, &srv);
						break;
					case ShaderType::tess_control:
						gc->get_window()->get_device_context()->HSSetShaderResources(storage_units[index].shader_srv_index[j], 1, &srv);
						break;
					case ShaderType::tess_evaluation:
						gc->get_window()->get_device_context()->DSSetShaderResources(storage_units[index].shader_srv_index[j], 1, &srv);
						break;
					case ShaderType::geometry:
						gc->get_window()->get_device_context()->GSSetShaderResources(storage_units[index].shader_srv_index[j], 1, &srv);
						break;
					case ShaderType::fragment:
						gc->get_window()->get_device_context()->PSSetShaderResources(storage_units[index].shader_srv_index[j], 1, &srv);
						break;
					case ShaderType::compute:
						gc->get_window()->get_device_context()->CSSetShaderResources(storage_units[index].shader_srv_index[j], 1, &srv);
						break;
					}
				}
			}

			if (storage_units[index].shader_uav_index[static_cast<int>(ShaderType::compute)] != -1)
			{
				ID3D11UnorderedAccessView *uav = 0;
				UINT uav_initial_count = 0;
				if (!storage_units[index].object.is_null())
					uav = static_cast<D3DStorageBufferProvider*>(storage_units[index].object.get_provider())->get_uav(gc->get_window()->get_device());
				gc->get_window()->get_device_context()->CSSetUnorderedAccessViews(storage_units[index].shader_uav_index[static_cast<int>(ShaderType::compute)], 1, &uav, &uav_initial_count);
			}
		}
	}

	void D3DUnitMap::unbind_sampler(D3DGraphicContextProvider *gc, int index)
	{
		if (sampler_units.size() > index)
		{
			for (int j = 0; j < static_cast<int>(ShaderType::num_types); j++)
			{
				if (sampler_units[index].shader_index[j] != -1)
				{
					ID3D11SamplerState *sampler_state = 0;

					switch (static_cast<ShaderType>(j))
					{
					case ShaderType::vertex:
						gc->get_window()->get_device_context()->VSSetSamplers(sampler_units[index].shader_index[j], 1, &sampler_state);
						break;
					case ShaderType::tess_control:
						gc->get_window()->get_device_context()->HSSetSamplers(sampler_units[index].shader_index[j], 1, &sampler_state);
						break;
					case ShaderType::tess_evaluation:
						gc->get_window()->get_device_context()->DSSetSamplers(sampler_units[index].shader_index[j], 1, &sampler_state);
						break;
					case ShaderType::geometry:
						gc->get_window()->get_device_context()->GSSetSamplers(sampler_units[index].shader_index[j], 1, &sampler_state);
						break;
					case ShaderType::fragment:
						gc->get_window()->get_device_context()->PSSetSamplers(sampler_units[index].shader_index[j], 1, &sampler_state);
						break;
					case ShaderType::compute:
						gc->get_window()->get_device_context()->CSSetSamplers(sampler_units[index].shader_index[j], 1, &sampler_state);
						break;
					}
				}
			}
		}
	}

	void D3DUnitMap::unbind_texture(D3DGraphicContextProvider *gc, int index)
	{
		if (texture_units.size() > index)
		{
			for (int j = 0; j < static_cast<int>(ShaderType::num_types); j++)
			{
				if (texture_units[index].shader_index[j] != -1)
				{
					ID3D11ShaderResourceView *srv = 0;

					switch (static_cast<ShaderType>(j))
					{
					case ShaderType::vertex:
						gc->get_window()->get_device_context()->VSSetShaderResources(texture_units[index].shader_index[j], 1, &srv);
						break;
					case ShaderType::tess_control:
						gc->get_window()->get_device_context()->HSSetShaderResources(texture_units[index].shader_index[j], 1, &srv);
						break;
					case ShaderType::tess_evaluation:
						gc->get_window()->get_device_context()->DSSetShaderResources(texture_units[index].shader_index[j], 1, &srv);
						break;
					case ShaderType::geometry:
						gc->get_window()->get_device_context()->GSSetShaderResources(texture_units[index].shader_index[j], 1, &srv);
						break;
					case ShaderType::fragment:
						gc->get_window()->get_device_context()->PSSetShaderResources(texture_units[index].shader_index[j], 1, &srv);
						break;
					case ShaderType::compute:
						gc->get_window()->get_device_context()->CSSetShaderResources(texture_units[index].shader_index[j], 1, &srv);
						break;
					}
				}
			}
		}
	}

	void D3DUnitMap::unbind_image(D3DGraphicContextProvider *gc, int index)
	{
		if (image_units.size() > index)
		{
			ID3D11UnorderedAccessView *uav = 0;
			UINT uav_initial_count = 0;
			if (texture_units[index].shader_index[static_cast<int>(ShaderType::compute)] != -1)
				gc->get_window()->get_device_context()->CSSetUnorderedAccessViews(texture_units[index].shader_index[static_cast<int>(ShaderType::compute)], 1, &uav, &uav_initial_count);
		}
	}

	void D3DUnitMap::unbind_uniform_buffer(D3DGraphicContextProvider *gc, int index)
	{
		if (uniform_units.size() > index)
		{
			for (int j = 0; j < static_cast<int>(ShaderType::num_types); j++)
			{
				if (uniform_units[index].shader_index[j] != -1)
				{
					ID3D11Buffer *d3d_buffer = 0;
					switch (static_cast<ShaderType>(j))
					{
					case ShaderType::vertex:
						gc->get_window()->get_device_context()->VSSetConstantBuffers(uniform_units[index].shader_index[j], 1, &d3d_buffer);
						break;
					case ShaderType::tess_control:
						gc->get_window()->get_device_context()->HSSetConstantBuffers(uniform_units[index].shader_index[j], 1, &d3d_buffer);
						break;
					case ShaderType::tess_evaluation:
						gc->get_window()->get_device_context()->DSSetConstantBuffers(uniform_units[index].shader_index[j], 1, &d3d_buffer);
						break;
					case ShaderType::geometry:
						gc->get_window()->get_device_context()->GSSetConstantBuffers(uniform_units[index].shader_index[j], 1, &d3d_buffer);
						break;
					case ShaderType::fragment:
						gc->get_window()->get_device_context()->PSSetConstantBuffers(uniform_units[index].shader_index[j], 1, &d3d_buffer);
						break;
					case ShaderType::compute:
						gc->get_window()->get_device_context()->CSSetConstantBuffers(uniform_units[index].shader_index[j], 1, &d3d_buffer);
						break;
					}
				}
			}
		}
	}

	void D3DUnitMap::unbind_storage_buffer(D3DGraphicContextProvider *gc, int index)
	{
		if (storage_units.size() > index)
		{
			for (int j = 0; j < static_cast<int>(ShaderType::num_types); j++)
			{
				if (storage_units[index].shader_srv_index[j] != -1)
				{
					ID3D11ShaderResourceView *srv = 0;
					switch (static_cast<ShaderType>(j))
					{
					case ShaderType::vertex:
						gc->get_window()->get_device_context()->VSSetShaderResources(storage_units[index].shader_srv_index[j], 1, &srv);
						break;
					case ShaderType::tess_control:
						gc->get_window()->get_device_context()->HSSetShaderResources(storage_units[index].shader_srv_index[j], 1, &srv);
						break;
					case ShaderType::tess_evaluation:
						gc->get_window()->get_device_context()->DSSetShaderResources(storage_units[index].shader_srv_index[j], 1, &srv);
						break;
					case ShaderType::geometry:
						gc->get_window()->get_device_context()->GSSetShaderResources(storage_units[index].shader_srv_index[j], 1, &srv);
						break;
					case ShaderType::fragment:
						gc->get_window()->get_device_context()->PSSetShaderResources(storage_units[index].shader_srv_index[j], 1, &srv);
						break;
					case ShaderType::compute:
						gc->get_window()->get_device_context()->CSSetShaderResources(storage_units[index].shader_srv_index[j], 1, &srv);
						break;
					}
				}
			}

			if (storage_units[index].shader_uav_index[static_cast<int>(ShaderType::compute)] != -1)
			{
				ID3D11UnorderedAccessView *uav = 0;
				UINT uav_initial_count = 0;
				gc->get_window()->get_device_context()->CSSetUnorderedAccessViews(storage_units[index].shader_uav_index[static_cast<int>(ShaderType::compute)], 1, &uav, &uav_initial_count);
			}
		}
	}
}
