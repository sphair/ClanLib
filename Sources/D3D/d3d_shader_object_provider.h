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

#pragma once

#include "API/Display/TargetProviders/shader_object_provider.h"
#include <map>
#include <D3Dcompiler.h>
#include <mutex>

namespace clan
{
	class D3DShaderObjectProvider : public ShaderObjectProvider
	{
	public:
		D3DShaderObjectProvider(const ComPtr<ID3D11Device> &device, D3D_FEATURE_LEVEL feature_level);
		~D3DShaderObjectProvider();
		void create(ShaderType type, const std::string &source);
		void create(ShaderType type, const void *source, int source_size);
		void create(ShaderType type, const std::vector<std::string> &sources);

		unsigned int get_handle() const;
		bool get_compile_status() const;
		ShaderType get_shader_type() const;
		std::string get_info_log() const;
		std::string get_shader_source() const;

		ID3D11VertexShader *get_vertex() { return static_cast<ID3D11VertexShader*>(shader.get()); }
		ID3D11PixelShader *get_pixel() { return static_cast<ID3D11PixelShader*>(shader.get()); }
		ID3D11GeometryShader *get_geometry() { return static_cast<ID3D11GeometryShader*>(shader.get()); }
		ID3D11DomainShader *get_domain() { return static_cast<ID3D11DomainShader*>(shader.get()); }
		ID3D11HullShader *get_hull() { return static_cast<ID3D11HullShader*>(shader.get()); }
		ID3D11ComputeShader *get_compute() { return static_cast<ID3D11ComputeShader*>(shader.get()); }

		ComPtr<ID3D11Device> device;
		DataBuffer bytecode;
		ComPtr<ID3D11DeviceChild> shader;

		std::map<std::string, int> sampler_locations;
		std::map<std::string, int> texture_locations;
		std::map<std::string, int> image_locations;
		std::map<std::string, int> uniform_buffer_locations;
		std::map<std::string, int> storage_buffer_srv_locations;
		std::map<std::string, int> storage_buffer_uav_locations;

		void compile();

	private:
		void set_binding(D3D11_SHADER_INPUT_BIND_DESC &binding);
		void create_shader();
		void find_locations();
		std::string get_shader_model() const;
		void load_compiler_dll();

		std::string info_log;
		bool compile_status;
		std::string shader_source;
		ShaderType type;
		D3D_FEATURE_LEVEL feature_level;

		typedef HRESULT(WINAPI *FuncD3DCompile)(
			LPCVOID pSrcData,
			SIZE_T SrcDataSize,
			LPCSTR pSourceName,
			CONST D3D_SHADER_MACRO* pDefines,
			ID3DInclude* pInclude,
			LPCSTR pEntrypoint,
			LPCSTR pTarget,
			UINT Flags1,
			UINT Flags2,
			ID3DBlob** ppCode,
			ID3DBlob** ppErrorMsgs);

		typedef HRESULT(WINAPI *FuncD3DReflect)(
			LPCVOID pSrcData,
			SIZE_T SrcDataSize,
			REFIID pInterface,
			void** ppReflector);

		static std::recursive_mutex d3dcompiler_mutex;
		static HMODULE d3dcompiler_dll;
		static FuncD3DCompile d3dcompile;
		static FuncD3DReflect d3dreflect;
	};
}
