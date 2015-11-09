/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
#include "d3d_program_object_provider.h"
#include "d3d_shader_object_provider.h"
#include "d3d_uniform_buffer_provider.h"
#include "API/Display/Render/shader_object.h"
#include "API/Core/Text/string_help.h"

namespace clan
{
	D3DProgramObjectProvider::D3DProgramObjectProvider(const ComPtr<ID3D11Device> &device, const ComPtr<ID3D11DeviceContext> &device_context)
		: device(device), device_context(device_context)
	{
	}

	D3DProgramObjectProvider::~D3DProgramObjectProvider()
	{
	}

	unsigned int D3DProgramObjectProvider::get_handle() const
	{
		return 0;
	}

	bool D3DProgramObjectProvider::get_link_status() const
	{
		return true;
	}

	bool D3DProgramObjectProvider::get_validate_status() const
	{
		return true;
	}

	std::string D3DProgramObjectProvider::get_info_log() const
	{
		return std::string();
	}

	std::vector<ShaderObject> D3DProgramObjectProvider::get_shaders() const
	{
		return std::vector<ShaderObject>();
	}

	int D3DProgramObjectProvider::get_attribute_location(const std::string &name) const
	{
		return -1;
	}

	int D3DProgramObjectProvider::get_uniform_location(const std::string &name) const
	{
		std::map<std::string, int>::const_iterator it = uniform_names.find(name);
		if (it != uniform_names.end())
			return it->second;
		else
			return -1;
	}

	int D3DProgramObjectProvider::get_uniform_buffer_size(int block_index) const
	{
		return 0;
	}

	int D3DProgramObjectProvider::get_uniform_buffer_index(const std::string &block_name) const
	{
		std::map<std::string, int>::const_iterator it = uniform_block_names.find(block_name);
		if (it != uniform_block_names.end())
			return it->second;
		else
			return -1;
	}

	int D3DProgramObjectProvider::get_storage_buffer_index(const std::string &block_name) const
	{
		std::map<std::string, int>::const_iterator it = storage_block_names.find(block_name);
		if (it != storage_block_names.end())
			return it->second;
		else
			return -1;
	}

	DataBuffer &D3DProgramObjectProvider::get_shader_bytecode(ShaderType shader_type)
	{
		return get_shader_provider(shader_type)->bytecode;
	}

	D3DShaderObjectProvider *D3DProgramObjectProvider::get_shader_provider(ShaderType shader_type)
	{
		if (!shaders[shader_type].is_null())
			return static_cast<D3DShaderObjectProvider*>(shaders[shader_type].get_provider());
		else
			return 0;
	}

	void D3DProgramObjectProvider::attach(const ShaderObject &obj)
	{
		D3DShaderObjectProvider *shader_provider = static_cast<D3DShaderObjectProvider*>(obj.get_provider());
		shaders[shader_provider->get_shader_type()] = obj;
	}

	void D3DProgramObjectProvider::detach(const ShaderObject &obj)
	{
		D3DShaderObjectProvider *shader_provider = static_cast<D3DShaderObjectProvider*>(obj.get_provider());
		shaders[shader_provider->get_shader_type()] = ShaderObject();
	}

	void D3DProgramObjectProvider::bind_attribute_location(int index, const std::string &name)
	{
		// Numbers at the end of a semantic name in HLSL maps to the semantic index.
		if (!name.empty() && name[name.length() - 1] >= '0' && name[name.length() - 1] <= '9')
		{
			std::string::size_type number_start = name.length() - 1;
			while (number_start > 0 && name[number_start - 1] >= '0' && name[number_start - 1] <= '9')
				number_start--;

			attribute_bindings[index].semantic_name = name.substr(0, number_start);
			attribute_bindings[index].semantic_index = StringHelp::text_to_int(name.substr(number_start));
		}
		else
		{
			attribute_bindings[index].semantic_name = name;
			attribute_bindings[index].semantic_index = 0;
		}
	}

	void D3DProgramObjectProvider::bind_frag_data_location(int color_number, const std::string &name)
	{
		// This isn't relevant for Direct3D.  The output semantic names (SV_TargetN) have hardcoded locations in HLSL.
	}

	void D3DProgramObjectProvider::link()
	{
		for (int j = 0; j < shadertype_num_types; j++)
		{
			if (!shaders[j].is_null())
			{
				D3DShaderObjectProvider *shader_provider = static_cast<D3DShaderObjectProvider*>(shaders[j].get_provider());

				std::map<std::string, int>::iterator it, it2;
				for (it = shader_provider->sampler_locations.begin(); it != shader_provider->sampler_locations.end(); ++it)
				{
					it2 = uniform_names.find(it->first);
					if (it2 == uniform_names.end())
					{
						uniform_names[it->first] = uniforms.size();
						uniforms.push_back(D3DUniform(D3DUniform::type_sampler));
					}
					uniforms[uniform_names[it->first]].shader_index[j] = it->second;
				}

				for (it = shader_provider->texture_locations.begin(); it != shader_provider->texture_locations.end(); ++it)
				{
					it2 = uniform_names.find(it->first);
					if (it2 == uniform_names.end())
					{
						uniform_names[it->first] = uniforms.size();
						uniforms.push_back(D3DUniform(D3DUniform::type_texture));
					}
					uniforms[uniform_names[it->first]].shader_index[j] = it->second;
				}

				for (it = shader_provider->image_locations.begin(); it != shader_provider->image_locations.end(); ++it)
				{
					it2 = uniform_names.find(it->first);
					if (it2 == uniform_names.end())
					{
						uniform_names[it->first] = uniforms.size();
						uniforms.push_back(D3DUniform(D3DUniform::type_image));
					}
					uniforms[uniform_names[it->first]].shader_index[j] = it->second;
				}

				for (it = shader_provider->uniform_buffer_locations.begin(); it != shader_provider->uniform_buffer_locations.end(); ++it)
				{
					it2 = uniform_block_names.find(it->first);
					if (it2 == uniform_block_names.end())
					{
						uniform_block_names[it->first] = uniform_blocks.size();
						uniform_blocks.push_back(D3DUniformBlock());
					}
					uniform_blocks[uniform_block_names[it->first]].shader_index[j] = it->second;
				}

				for (it = shader_provider->storage_buffer_uav_locations.begin(); it != shader_provider->storage_buffer_uav_locations.end(); ++it)
				{
					it2 = storage_block_names.find(it->first);
					if (it2 == storage_block_names.end())
					{
						storage_block_names[it->first] = storage_blocks.size();
						storage_blocks.push_back(D3DStorageBlock());
					}
					storage_blocks[storage_block_names[it->first]].shader_uav_index[j] = it->second;
				}

				for (it = shader_provider->storage_buffer_srv_locations.begin(); it != shader_provider->storage_buffer_srv_locations.end(); ++it)
				{
					it2 = storage_block_names.find(it->first);
					if (it2 == storage_block_names.end())
					{
						storage_block_names[it->first] = storage_blocks.size();
						storage_blocks.push_back(D3DStorageBlock());
					}
					storage_blocks[storage_block_names[it->first]].shader_srv_index[j] = it->second;
				}
			}
		}
	}

	void D3DProgramObjectProvider::validate()
	{
	}

	void D3DProgramObjectProvider::set_uniform1i(int location, int v1)
	{
		if (location >= 0 && location <= (int)uniforms.size())
		{
			uniforms[location].value = v1;
			// To do: update D3DUnitMap if the program object is currently bound
		}
	}


	void D3DProgramObjectProvider::set_uniform2i(int location, int v1, int v2)
	{
	}

	void D3DProgramObjectProvider::set_uniform3i(int location, int v1, int v2, int v3)
	{
	}

	void D3DProgramObjectProvider::set_uniform4i(int location, int v1, int v2, int v3, int v4)
	{
	}

	void D3DProgramObjectProvider::set_uniformiv(int location, int size, int count, const int *data)
	{
	}

	void D3DProgramObjectProvider::set_uniform1f(int location, float v1)
	{
	}

	void D3DProgramObjectProvider::set_uniform2f(int location, float v1, float v2)
	{
	}

	void D3DProgramObjectProvider::set_uniform3f(int location, float v1, float v2, float v3)
	{
	}

	void D3DProgramObjectProvider::set_uniform4f(int location, float v1, float v2, float v3, float v4)
	{
	}

	void D3DProgramObjectProvider::set_uniformfv(int location, int size, int count, const float *data)
	{
	}

	void D3DProgramObjectProvider::set_uniform_matrix(int location, int size, int count, bool transpose, const float *data)
	{
	}

	void D3DProgramObjectProvider::set_uniform_buffer_index(int block_index, int bind_index)
	{
		if (block_index >= 0 && block_index <= (int)uniform_blocks.size())
		{
			uniform_blocks[block_index].value = bind_index;
			// To do: update D3DUnitMap if the program object is currently bound
		}
	}

	void D3DProgramObjectProvider::set_storage_buffer_index(int buffer_index, int bind_unit_index)
	{
		if (buffer_index >= 0 && buffer_index <= (int)storage_blocks.size())
		{
			storage_blocks[buffer_index].value = bind_unit_index;
			// To do: update D3DUnitMap if the program object is currently bound
		}
	}
}
