/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
#include "d3d_shader_object_provider.h"
#include "API/D3D/d3d_target.h"
#include "API/Core/Text/string_format.h"

namespace clan
{
	class DXBC_Reader
	{
	public:
		DXBC_Reader(const unsigned char *read_ptr, int length);

		int get_int32();
		void skip_bytes(int length);
		const char *get_string(int offset);
		int get_remaining() const { return end_ptr - read_ptr; }

	private:
		const unsigned char *read_ptr;
		const unsigned char *end_ptr;
	};

	class DXBC_Reflect
	{
	public:
		DXBC_Reflect(const void *data, int size);

		std::vector<D3D11_SHADER_INPUT_BIND_DESC> binding;
	};

	D3DShaderObjectProvider::D3DShaderObjectProvider(const ComPtr<ID3D11Device> &device, D3D_FEATURE_LEVEL feature_level)
		: device(device), compile_status(false), feature_level(feature_level)
	{
	}

	D3DShaderObjectProvider::~D3DShaderObjectProvider()
	{
	}

	void D3DShaderObjectProvider::create(ShaderType new_type, const std::string &source)
	{
		shader_source = source;
		bytecode = DataBuffer();
		compile_status = false;
		type = new_type;
	}

	void D3DShaderObjectProvider::create(
		ShaderType new_type, const void *source, int source_size)
	{
		bytecode = DataBuffer(source, source_size);
		compile_status = false;
		type = new_type;

	}

	void D3DShaderObjectProvider::create(ShaderType type, const std::vector<std::string> &sources)
	{
		std::string source;
		for (size_t i = 0; i < sources.size(); i++)
			source += sources[i] + "\r\n";
		create(type, source);
	}

	unsigned int D3DShaderObjectProvider::get_handle() const
	{
		return 0;
	}

	bool D3DShaderObjectProvider::get_compile_status() const
	{
		return compile_status;
	}

	ShaderType D3DShaderObjectProvider::get_shader_type() const
	{
		return type;
	}

	std::string D3DShaderObjectProvider::get_info_log() const
	{
		return info_log;
	}

	std::string D3DShaderObjectProvider::get_shader_source() const
	{
		return shader_source;
	}

	void D3DShaderObjectProvider::compile()
	{
		shader.clear();
		info_log.clear();

		if (!bytecode.get_size())
		{
			load_compiler_dll();

			std::string entry_point = "main";
			std::string shader_model = get_shader_model();

			ComPtr<ID3DBlob> blob;
			ComPtr<ID3DBlob> log;
			HRESULT result = d3dcompile(
				shader_source.data(),
				shader_source.length(),
				0,
				0,
				0,
				entry_point.c_str(),
				shader_model.c_str(),
				D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_OPTIMIZATION_LEVEL3,
				0,
				blob.output_variable(),
				log.output_variable());

			if (log)
				info_log = std::string(reinterpret_cast<char*>(log->GetBufferPointer()), log->GetBufferSize());

			if (SUCCEEDED(result))
			{
				bytecode = DataBuffer(blob->GetBufferPointer(), blob->GetBufferSize());
			}
			else
				return;
		}

		try
		{
			create_shader();
			find_locations();
			compile_status = true;
		}
		catch (Exception &e)
		{
			if (!info_log.empty())
				info_log += "\r\n";
			info_log += e.message;
		}

	}

	std::recursive_mutex D3DShaderObjectProvider::d3dcompiler_mutex;
	HMODULE D3DShaderObjectProvider::d3dcompiler_dll = 0;
	D3DShaderObjectProvider::FuncD3DCompile D3DShaderObjectProvider::d3dcompile = 0;
	D3DShaderObjectProvider::FuncD3DReflect D3DShaderObjectProvider::d3dreflect = 0;

	void D3DShaderObjectProvider::load_compiler_dll()
	{
		std::unique_lock<std::recursive_mutex> mutex_lock(d3dcompiler_mutex);
		if (d3dcompiler_dll == 0)
		{
			d3dcompiler_dll = LoadLibrary(D3DCOMPILER_DLL);
			if (d3dcompiler_dll == 0)
				throw Exception(string_format("Unable to load %1", D3DCOMPILER_DLL_A));

			try
			{
				d3dcompile = reinterpret_cast<FuncD3DCompile>(GetProcAddress(d3dcompiler_dll, "D3DCompile"));
				if (d3dcompile == 0)
					throw Exception("D3DCompile function not found!");

				d3dreflect = reinterpret_cast<FuncD3DReflect>(GetProcAddress(d3dcompiler_dll, "D3DReflect"));
				if (d3dreflect == 0)
					throw Exception("D3DReflect function not found!");
			}
			catch (...)
			{
				CloseHandle(d3dcompiler_dll);
				d3dcompiler_dll = 0;
				d3dcompile = 0;
				d3dreflect = 0;
				throw;
			}
		}
	}

	void D3DShaderObjectProvider::create_shader()
	{
		switch (type)
		{
		case shadertype_vertex:
		{
			ID3D11VertexShader *shader_obj = 0;
			HRESULT result = device->CreateVertexShader(bytecode.get_data(), bytecode.get_size(), 0, &shader_obj);
			D3DTarget::throw_if_failed("CreateVertexShader failed", result);
			shader = ComPtr<ID3D11DeviceChild>(shader_obj);
		}
		break;
		case shadertype_tess_evaluation:
		{
			ID3D11DomainShader *shader_obj = 0;
			HRESULT result = device->CreateDomainShader(bytecode.get_data(), bytecode.get_size(), 0, &shader_obj);
			D3DTarget::throw_if_failed("CreateDomainShader failed", result);
			shader = ComPtr<ID3D11DeviceChild>(shader_obj);
		}
		break;
		case shadertype_tess_control:
		{
			ID3D11HullShader *shader_obj = 0;
			HRESULT result = device->CreateHullShader(bytecode.get_data(), bytecode.get_size(), 0, &shader_obj);
			D3DTarget::throw_if_failed("CreateHullShader failed", result);
			shader = ComPtr<ID3D11DeviceChild>(shader_obj);
		}
		break;
		case shadertype_geometry:
		{
			ID3D11GeometryShader *shader_obj = 0;
			HRESULT result = device->CreateGeometryShader(bytecode.get_data(), bytecode.get_size(), 0, &shader_obj);
			D3DTarget::throw_if_failed("CreateGeometryShader failed", result);
			shader = ComPtr<ID3D11DeviceChild>(shader_obj);
		}
		break;
		case shadertype_fragment:
		{
			ID3D11PixelShader *shader_obj = 0;
			HRESULT result = device->CreatePixelShader(bytecode.get_data(), bytecode.get_size(), 0, &shader_obj);
			D3DTarget::throw_if_failed("CreatePixelShader failed", result);
			shader = ComPtr<ID3D11DeviceChild>(shader_obj);
		}
		break;
		case shadertype_compute:
		{
			ID3D11ComputeShader *shader_obj = 0;
			HRESULT result = device->CreateComputeShader(bytecode.get_data(), bytecode.get_size(), 0, &shader_obj);
			D3DTarget::throw_if_failed("CreateComputeShader failed", result);
			shader = ComPtr<ID3D11DeviceChild>(shader_obj);
		}
		break;
		default:
			throw Exception("Unknown shader type");
		}
	}

	void D3DShaderObjectProvider::find_locations()
	{
		sampler_locations.clear();
		texture_locations.clear();
		uniform_buffer_locations.clear();

		if (d3dcompiler_dll)	// If the compiler is available, we must use it! This ensures compatility with the blob
		{
			ComPtr<ID3D11ShaderReflection> reflect;
			HRESULT result = d3dreflect(bytecode.get_data(), bytecode.get_size(), IID_ID3D11ShaderReflection, (void**)reflect.output_variable());
			D3DTarget::throw_if_failed("D3DReflect failed", result);

			D3D11_SHADER_DESC desc;
			result = reflect->GetDesc(&desc);
			D3DTarget::throw_if_failed("D3DReflect.GetDesc failed", result);

			for (UINT i = 0; i < desc.BoundResources; i++)
			{
				D3D11_SHADER_INPUT_BIND_DESC binding;
				result = reflect->GetResourceBindingDesc(i, &binding);
				D3DTarget::throw_if_failed("D3DReflect.GetResourceBindingDesc failed", result);

				set_binding(binding);
			}
		}
		else
		{
			DXBC_Reflect reflect(bytecode.get_data(), bytecode.get_size());
			for (size_t cnt = 0; cnt < reflect.binding.size(); cnt++)
			{
				set_binding(reflect.binding[cnt]);
			}
		}

	}

	void D3DShaderObjectProvider::set_binding(D3D11_SHADER_INPUT_BIND_DESC &binding)
	{
		switch (binding.Type)
		{
		case D3D_SIT_CBUFFER: // constant buffer
			uniform_buffer_locations[binding.Name] = binding.BindPoint;
			break;

		case D3D_SIT_TBUFFER: // texture buffer
			// What does this map to in OpenGL? glTexBuffer?
			break;

		case D3D_SIT_TEXTURE: // texture
			texture_locations[binding.Name] = binding.BindPoint;
			break;

		case D3D_SIT_SAMPLER: // sampler
			sampler_locations[binding.Name] = binding.BindPoint;
			break;

		case D3D_SIT_UAV_RWTYPED: // read-and-write buffer
			image_locations[binding.Name] = binding.BindPoint;
			break;

		case D3D_SIT_UAV_RWSTRUCTURED: // read-and-write structured buffer
		case D3D_SIT_UAV_RWBYTEADDRESS: // read-and-write byte-address buffer
		case D3D_SIT_UAV_APPEND_STRUCTURED: // append-structured buffer
		case D3D_SIT_UAV_CONSUME_STRUCTURED: // consume-structured buffer
		case D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER: // read-and-write structured buffer that uses the built-in counter to append or consume
			storage_buffer_uav_locations[binding.Name] = binding.BindPoint;
			break;

		case D3D_SIT_STRUCTURED: // structured buffer
		case D3D_SIT_BYTEADDRESS: // byte-address buffer
			storage_buffer_srv_locations[binding.Name] = binding.BindPoint;
			break;
		}
	}

	std::string D3DShaderObjectProvider::get_shader_model() const
	{
		int major = 5;
		int minor = 0;
		switch (feature_level)
		{
		case D3D_FEATURE_LEVEL_11_0: major = 5; minor = 0; break;
		case D3D_FEATURE_LEVEL_10_1: major = 4; minor = 1; break;
		case D3D_FEATURE_LEVEL_10_0: major = 4; minor = 0; break;
		case D3D_FEATURE_LEVEL_9_1: major = 3; minor = 1; break;
		case D3D_FEATURE_LEVEL_9_2: major = 3; minor = 2; break;
		case D3D_FEATURE_LEVEL_9_3: major = 3; minor = 3; break;
		}

		switch (type)
		{
		case shadertype_vertex:
			return string_format("vs_%1_%2", major, minor);
		case shadertype_tess_control:
			return string_format("hs_%1_%2", major, minor);
		case shadertype_tess_evaluation:
			return string_format("ds_%1_%2", major, minor);
		case shadertype_geometry:
			return string_format("gs_%1_%2", major, minor);
		case shadertype_fragment:
			return string_format("ps_%1_%2", major, minor);
		case shadertype_compute:
			return string_format("cs_%1_%2", major, minor);
		default:
			throw Exception("Unknown shader type");
		}
	}

	#define MAKE_TAG(a,b,c,d) ((a) | (b) << 8 | (c) << 16 | (d) << 24)
	#define TAG_DXBC MAKE_TAG('D', 'X', 'B','C')
	#define TAG_RDEF MAKE_TAG('R', 'D', 'E','F')

	DXBC_Reader::DXBC_Reader(const unsigned char *new_read_ptr, int new_length)
	{
		read_ptr = new_read_ptr;
		end_ptr = read_ptr + new_length;
	}

	const char * DXBC_Reader::get_string(int offset)
	{
		if ((offset < 0) || (offset > get_remaining()))
			throw Exception("Unknown DXBC format");

		const char *str_ptr = (const char *)read_ptr;
		str_ptr += offset;

		// Validate string .. check for end NUL
		const unsigned char *check_ptr = (const unsigned char *)str_ptr;
		while (*check_ptr)
		{
			check_ptr++;
			if (check_ptr >= end_ptr)
				throw Exception("Unknown DXBC format");
		}

		return str_ptr;

	}

	int DXBC_Reader::get_int32()
	{
		if (get_remaining() < 4)
			throw Exception("Unknown DXBC format");

		int value = *(read_ptr++);
		value |= *(read_ptr++) << 8;
		value |= *(read_ptr++) << 16;
		value |= *(read_ptr++) << 24;
		return value;
	}

	void DXBC_Reader::skip_bytes(int size)
	{
		if (get_remaining() < size)
			throw Exception("Unknown DXBC format");
		read_ptr += size;
	}

	DXBC_Reflect::DXBC_Reflect(const void *new_data, int new_size)
	{
		DXBC_Reader base_reader((const unsigned char *)new_data, new_size);

		DXBC_Reader dxbc_reader(base_reader);

		if (dxbc_reader.get_int32() != TAG_DXBC)
			throw Exception("Not DXBC format");

		dxbc_reader.skip_bytes(4 * 4);	// GUID
		int file_format_version = dxbc_reader.get_int32();	// Version

		int internal_total_size = dxbc_reader.get_int32();
		if (internal_total_size != new_size)
			throw Exception("Invalid DXBC size");

		int chunk_count = dxbc_reader.get_int32();
		for (int cnt = 0; cnt < chunk_count; cnt++)
		{
			int chunk_offset = dxbc_reader.get_int32();
			DXBC_Reader chunk_reader(base_reader);
			chunk_reader.skip_bytes(chunk_offset);

			int chunk_tag = chunk_reader.get_int32();
			int chunk_size = chunk_reader.get_int32();	// We ignore this

			if (chunk_tag == TAG_RDEF)
			{
				DXBC_Reader header_reader(chunk_reader);
				int constant_buffer_count = header_reader.get_int32();
				int constant_buffer_offset = header_reader.get_int32();
				int bound_resource_count = header_reader.get_int32();
				int bound_resource_offset = header_reader.get_int32();
				int target_shader_model = header_reader.get_int32();	// Shader model in low word
				int flags = header_reader.get_int32();

				if (bound_resource_count)
				{
					DXBC_Reader bind_reader(chunk_reader);
					bind_reader.skip_bytes(bound_resource_offset);

					binding.reserve(bound_resource_count);
					for (int i = 0; i < bound_resource_count; i++)
					{
						D3D11_SHADER_INPUT_BIND_DESC desc = { 0 };
						desc.Name = chunk_reader.get_string(bind_reader.get_int32());
						desc.Type = (D3D_SHADER_INPUT_TYPE)bind_reader.get_int32();
						desc.ReturnType = (D3D_RESOURCE_RETURN_TYPE)bind_reader.get_int32();
						desc.Dimension = (D3D_SRV_DIMENSION)bind_reader.get_int32();
						desc.NumSamples = bind_reader.get_int32();
						desc.BindPoint = bind_reader.get_int32();
						desc.BindCount = bind_reader.get_int32();
						desc.uFlags = bind_reader.get_int32();

						binding.push_back(desc);
					}
				}
			}
		}
	}
}
