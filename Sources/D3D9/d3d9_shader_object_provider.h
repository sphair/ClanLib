/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "API/Core/System/comptr.h"
#include <d3d9.h>
#include <d3dx9tex.h>

class CL_D3D9DisplayWindowProvider;

class CL_D3D9ShaderObjectProvider : public CL_ShaderObjectProvider
{
/// \name Construction
/// \{

public:
	CL_D3D9ShaderObjectProvider(CL_D3D9DisplayWindowProvider *window);

	~CL_D3D9ShaderObjectProvider();

	void create(CL_ShaderType type, const CL_StringRef &source);

	void create(CL_ShaderType type, const std::vector<CL_StringRef> &sources);

	void destroy();


/// \}
/// \name Attributes
/// \{

public:
	unsigned int get_handle() const;

	bool get_compile_status() const;

	CL_ShaderType get_shader_type() const;

	CL_String get_info_log() const;

	CL_String get_shader_source() const;


/// \}
/// \name Operations
/// \{

public:
	void compile();


/// \}
/// \name Implementation
/// \{

private:
	CL_D3D9DisplayWindowProvider *window;
	CL_ShaderType type;
	CL_ComPtr<IDirect3DVertexShader9> vertex_shader;
	CL_ComPtr<IDirect3DPixelShader9> pixel_shader;
	mutable CL_ComPtr<ID3DXBuffer> errors;
	CL_ComPtr<ID3DXBuffer> shader_code;
	CL_ComPtr<ID3DXConstantTable> shader_constants;
	CL_String shader_source;
/// \}
};


