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

#include "D3D/precomp.h"
#include "d3d_shader_object_provider.h"
#include "API/D3D/d3d_target.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/System/mutex.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// D3DShaderObjectProvider Construction:

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
/////////////////////////////////////////////////////////////////////////////
// D3DShaderObjectProvider Attributes:

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

/////////////////////////////////////////////////////////////////////////////
// D3DShaderObjectProvider Operations:

void D3DShaderObjectProvider::compile()
{
	shader.clear();
	bytecode.clear();
	info_log.clear();

	load_compiler_dll();

	std::string entry_point = "main";
	std::string shader_model = get_shader_model();

	ComPtr<ID3DBlob> log;
	HRESULT result = d3dcompile(
		shader_source.data(),
		shader_source.length(),
		0,
		0,
		0,
		entry_point.c_str(),
		shader_model.c_str(),
		D3D10_SHADER_ENABLE_STRICTNESS|D3D10_SHADER_OPTIMIZATION_LEVEL3,
		0,
		bytecode.output_variable(),
		log.output_variable());

	if (log)
		info_log = std::string(reinterpret_cast<char*>(log->GetBufferPointer()), log->GetBufferSize());

	if (SUCCEEDED(result))
	{
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
}

/////////////////////////////////////////////////////////////////////////////
// D3DShaderObjectProvider Implementation:

Mutex D3DShaderObjectProvider::d3dcompiler_mutex;
HMODULE D3DShaderObjectProvider::d3dcompiler_dll = 0;
D3DShaderObjectProvider::FuncD3DCompile D3DShaderObjectProvider::d3dcompile = 0;
D3DShaderObjectProvider::FuncD3DReflect D3DShaderObjectProvider::d3dreflect = 0;

void D3DShaderObjectProvider::load_compiler_dll()
{
	MutexSection mutex_lock(&d3dcompiler_mutex);
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
			HRESULT result = device->CreateVertexShader(bytecode->GetBufferPointer(), bytecode->GetBufferSize(), 0, &shader_obj);
			D3DTarget::throw_if_failed("CreateVertexShader failed", result);
			shader = ComPtr<ID3D11DeviceChild>(shader_obj);
		}
		break;
	case shadertype_tess_evaluation:
		{
			ID3D11DomainShader *shader_obj = 0;
			HRESULT result = device->CreateDomainShader(bytecode->GetBufferPointer(), bytecode->GetBufferSize(), 0, &shader_obj);
			D3DTarget::throw_if_failed("CreateDomainShader failed", result);
			shader = ComPtr<ID3D11DeviceChild>(shader_obj);
		}
		break;
	case shadertype_tess_control:
		{
			ID3D11HullShader *shader_obj = 0;
			HRESULT result = device->CreateHullShader(bytecode->GetBufferPointer(), bytecode->GetBufferSize(), 0, &shader_obj);
			D3DTarget::throw_if_failed("CreateHullShader failed", result);
			shader = ComPtr<ID3D11DeviceChild>(shader_obj);
		}
		break;
	case shadertype_geometry:
		{
			ID3D11GeometryShader *shader_obj = 0;
			HRESULT result = device->CreateGeometryShader(bytecode->GetBufferPointer(), bytecode->GetBufferSize(), 0, &shader_obj);
			D3DTarget::throw_if_failed("CreateGeometryShader failed", result);
			shader = ComPtr<ID3D11DeviceChild>(shader_obj);
		}
		break;
	case shadertype_fragment:
		{
			ID3D11PixelShader *shader_obj = 0;
			HRESULT result = device->CreatePixelShader(bytecode->GetBufferPointer(), bytecode->GetBufferSize(), 0, &shader_obj);
			D3DTarget::throw_if_failed("CreatePixelShader failed", result);
			shader = ComPtr<ID3D11DeviceChild>(shader_obj);
		}
		break;
	case shadertype_compute:
		{
			ID3D11ComputeShader *shader_obj = 0;
			HRESULT result = device->CreateComputeShader(bytecode->GetBufferPointer(), bytecode->GetBufferSize(), 0, &shader_obj);
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

	load_compiler_dll();

	ComPtr<ID3D11ShaderReflection> reflect;
	HRESULT result = d3dreflect(bytecode->GetBufferPointer(), bytecode->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)reflect.output_variable());
	D3DTarget::throw_if_failed("D3DReflect failed", result);

	D3D11_SHADER_DESC desc;
	result = reflect->GetDesc(&desc);
	D3DTarget::throw_if_failed("D3DReflect.GetDesc failed", result);

	for (UINT i = 0; i < desc.BoundResources; i++)
	{
		D3D11_SHADER_INPUT_BIND_DESC binding;
		result = reflect->GetResourceBindingDesc(i, &binding);
		D3DTarget::throw_if_failed("D3DReflect.GetResourceBindingDesc failed", result);
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

}
