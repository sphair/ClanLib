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
**    Harry Storbacka
*/

#pragma once

#include "API/Display/TargetProviders/program_object_provider.h"
#include "API/Display/Render/shader_object.h"
#include "API/Display/Render/uniform_buffer.h"
#include "API/Core/Math/vec4.h"
#include <memory>
#include <map>

namespace clan
{
	class D3DShaderObjectProvider;
	class D3DUniformBufferProvider;

	class D3DUniform
	{
	public:
		enum class Type { sampler, texture, image };

		D3DUniform() : type(Type::sampler), value(0) { for (int i = 0; i < static_cast<int>(ShaderType::num_types); i++) shader_index[i] = -1; }
		D3DUniform(Type type) : type(type), value(0) { for (int i = 0; i < static_cast<int>(ShaderType::num_types); i++) shader_index[i] = -1; }

		Type type;
		int shader_index[static_cast<int>(ShaderType::num_types)];
		int value;
	};

	class D3DUniformBlock
	{
	public:
		D3DUniformBlock() : value(0) { for (int i = 0; i < static_cast<int>(ShaderType::num_types); i++) shader_index[i] = -1; }

		int shader_index[static_cast<int>(ShaderType::num_types)];
		int value;
	};

	class D3DStorageBlock
	{
	public:
		D3DStorageBlock() : value(0) { for (int i = 0; i < static_cast<int>(ShaderType::num_types); i++) { shader_srv_index[i] = -1; shader_uav_index[i] = -1; } }

		int shader_uav_index[static_cast<int>(ShaderType::num_types)];
		int shader_srv_index[static_cast<int>(ShaderType::num_types)];
		int value;
	};

	class D3DProgramObjectProvider : public ProgramObjectProvider
	{
	public:
		D3DProgramObjectProvider(const ComPtr<ID3D11Device> &device, const ComPtr<ID3D11DeviceContext> &device_context);
		~D3DProgramObjectProvider();

		unsigned int get_handle() const;
		bool get_link_status() const;
		bool get_validate_status() const;
		std::string get_info_log() const;
		std::vector<ShaderObject> get_shaders() const;
		int get_attribute_location(const std::string &name) const;
		int get_uniform_location(const std::string &name) const;

		int get_uniform_buffer_size(int block_index) const;
		int get_uniform_buffer_index(const std::string &block_name) const;
		int get_storage_buffer_index(const std::string &name) const;

		DataBuffer &get_shader_bytecode(ShaderType shader_type);
		D3DShaderObjectProvider *get_shader_provider(ShaderType shader_type);

		struct AttributeBinding
		{
			AttributeBinding() : semantic_index(0) { }
			std::string semantic_name;
			int semantic_index;
		};
		std::map<int, AttributeBinding> attribute_bindings;

		std::vector<D3DUniform> uniforms;
		std::map<std::string, int> uniform_names;

		std::vector<D3DUniformBlock> uniform_blocks;
		std::map<std::string, int> uniform_block_names;

		std::vector<D3DStorageBlock> storage_blocks;
		std::map<std::string, int> storage_block_names;

		void attach(const ShaderObject &obj);
		void detach(const ShaderObject &obj);
		void bind_attribute_location(int index, const std::string &name);
		void bind_frag_data_location(int color_number, const std::string &name);
		void link();
		void validate();

		void set_uniform1i(int location, int);
		void set_uniform2i(int location, int, int);
		void set_uniform3i(int location, int, int, int);
		void set_uniform4i(int location, int, int, int, int);
		void set_uniformiv(int location, int size, int count, const int *data);
		void set_uniform1f(int location, float);
		void set_uniform2f(int location, float, float);
		void set_uniform3f(int location, float, float, float);
		void set_uniform4f(int location, float, float, float, float);
		void set_uniformfv(int location, int size, int count, const float *data);
		void set_uniform_matrix(int location, int size, int count, bool transpose, const float *data);
		void set_uniform_buffer_index(int block_index, int bind_index);
		void set_storage_buffer_index(int buffer_index, int bind_unit_index);

	private:
		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DeviceContext> device_context;
		ShaderObject shaders[static_cast<int>(ShaderType::num_types)];
	};
}
