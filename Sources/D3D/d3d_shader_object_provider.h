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

#include "API/Display/TargetProviders/shader_object_provider.h"
#include "API/Core/System/mutex.h"
#include <map>

// If this line errors, try commenting it out, it might work.
#include <D3Dcompiler.h>

#ifndef D3D_COMPILER_VERSION

typedef struct _D3D11_SHADER_INPUT_BIND_DESC
{
    LPCSTR                      Name;           // Name of the resource
    D3D_SHADER_INPUT_TYPE       Type;           // Type of resource (e.g. texture, cbuffer, etc.)
    UINT                        BindPoint;      // Starting bind point
    UINT                        BindCount;      // Number of contiguous bind points (for arrays)
    
    UINT                        uFlags;         // Input binding flags
    D3D_RESOURCE_RETURN_TYPE    ReturnType;     // Return type (if texture)
    D3D_SRV_DIMENSION           Dimension;      // Dimension (if texture)
    UINT                        NumSamples;     // Number of samples (0 if not MS texture)
} D3D11_SHADER_INPUT_BIND_DESC;
#define D3DCOMPILER_DLL_W L"d3dcompiler_46.dll"
#define D3DCOMPILER_DLL_A "d3dcompiler_46.dll"
#define D3DCOMPILER_DLL D3DCOMPILER_DLL_W 

typedef struct _D3D11_SHADER_DESC
{
    UINT                    Version;                     // Shader version
    LPCSTR                  Creator;                     // Creator string
    UINT                    Flags;                       // Shader compilation/parse flags
    
    UINT                    ConstantBuffers;             // Number of constant buffers
    UINT                    BoundResources;              // Number of bound resources
    UINT                    InputParameters;             // Number of parameters in the input signature
    UINT                    OutputParameters;            // Number of parameters in the output signature

    UINT                    InstructionCount;            // Number of emitted instructions
    UINT                    TempRegisterCount;           // Number of temporary registers used 
    UINT                    TempArrayCount;              // Number of temporary arrays used
    UINT                    DefCount;                    // Number of constant defines 
    UINT                    DclCount;                    // Number of declarations (input + output)
    UINT                    TextureNormalInstructions;   // Number of non-categorized texture instructions
    UINT                    TextureLoadInstructions;     // Number of texture load instructions
    UINT                    TextureCompInstructions;     // Number of texture comparison instructions
    UINT                    TextureBiasInstructions;     // Number of texture bias instructions
    UINT                    TextureGradientInstructions; // Number of texture gradient instructions
    UINT                    FloatInstructionCount;       // Number of floating point arithmetic instructions used
    UINT                    IntInstructionCount;         // Number of signed integer arithmetic instructions used
    UINT                    UintInstructionCount;        // Number of unsigned integer arithmetic instructions used
    UINT                    StaticFlowControlCount;      // Number of static flow control instructions used
    UINT                    DynamicFlowControlCount;     // Number of dynamic flow control instructions used
    UINT                    MacroInstructionCount;       // Number of macro instructions used
    UINT                    ArrayInstructionCount;       // Number of array instructions used
    UINT                    CutInstructionCount;         // Number of cut instructions used
    UINT                    EmitInstructionCount;        // Number of emit instructions used
    D3D_PRIMITIVE_TOPOLOGY   GSOutputTopology;           // Geometry shader output topology
    UINT                    GSMaxOutputVertexCount;      // Geometry shader maximum output vertex count
    D3D_PRIMITIVE           InputPrimitive;              // GS/HS input primitive
    UINT                    PatchConstantParameters;     // Number of parameters in the patch constant signature
    UINT                    cGSInstanceCount;            // Number of Geometry shader instances
    UINT                    cControlPoints;              // Number of control points in the HS->DS stage
    D3D_TESSELLATOR_OUTPUT_PRIMITIVE HSOutputPrimitive;  // Primitive output by the tessellator
    D3D_TESSELLATOR_PARTITIONING HSPartitioning;         // Partitioning mode of the tessellator
    D3D_TESSELLATOR_DOMAIN  TessellatorDomain;           // Domain of the tessellator (quad, tri, isoline)
    // instruction counts
    UINT cBarrierInstructions;                           // Number of barrier instructions in a compute shader
    UINT cInterlockedInstructions;                       // Number of interlocked instructions
    UINT cTextureStoreInstructions;                      // Number of texture writes
} D3D11_SHADER_DESC;

DEFINE_GUID(IID_ID3D11ShaderReflection, 0x8d536ca1, 0x0cca, 0x4956, 0xa8, 0x37, 0x78, 0x69, 0x63, 0x75, 0x55, 0x84);

#undef INTERFACE
#define INTERFACE ID3D11ShaderReflection

DECLARE_INTERFACE_(ID3D11ShaderReflection, IUnknown)
{
	STDMETHOD(QueryInterface)(THIS_ _In_ REFIID iid, _Out_ LPVOID *ppv) PURE;
	STDMETHOD(GetDesc)(THIS_ _Out_ D3D11_SHADER_DESC *pDesc) PURE;
	STDMETHOD(GetResourceBindingDesc)(THIS_ _In_ UINT ResourceIndex, _Out_ D3D11_SHADER_INPUT_BIND_DESC *pDesc) PURE;
};

#endif

namespace clan
{

class D3DShaderObjectProvider : public ShaderObjectProvider
{
/// \name Construction
/// \{
public:
	D3DShaderObjectProvider(const ComPtr<ID3D11Device> &device, D3D_FEATURE_LEVEL feature_level);
	~D3DShaderObjectProvider();
	void create(ShaderType type, const std::string &source);
	void create(ShaderType type, const void *source, int source_size);
	void create(ShaderType type, const std::vector<std::string> &sources);
/// \}

/// \name Attributes
/// \{
public:
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
/// \}

/// \name Operations
/// \{
public:
	void compile();
/// \}

/// \name Implementation
/// \{
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

	typedef HRESULT (WINAPI *FuncD3DCompile)(
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

	typedef HRESULT (WINAPI *FuncD3DReflect)(
		LPCVOID pSrcData,
		SIZE_T SrcDataSize,
		REFIID pInterface,
		void** ppReflector);

	static Mutex d3dcompiler_mutex;
	static HMODULE d3dcompiler_dll;
	static FuncD3DCompile d3dcompile;
	static FuncD3DReflect d3dreflect;
/// \}
};

}
