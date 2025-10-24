/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

#include "D3D9/precomp.h"
#include "d3d9_shader_object_provider.h"
#include "d3d9_display_window_provider.h"
#include "API/Core/Text/string_help.h"

/////////////////////////////////////////////////////////////////////////////
// CL_D3D9ShaderObjectProvider Construction:

CL_D3D9ShaderObjectProvider::CL_D3D9ShaderObjectProvider(CL_D3D9DisplayWindowProvider *window)
: window(window), type(cl_shadertype_vertex)
{
}

CL_D3D9ShaderObjectProvider::~CL_D3D9ShaderObjectProvider()
{
}

void CL_D3D9ShaderObjectProvider::create(CL_ShaderType new_type, const CL_StringRef &source)
{
	std::vector<CL_StringRef> sources;
	sources.push_back(source);
	create(new_type, sources);
}

void CL_D3D9ShaderObjectProvider::create(CL_ShaderType new_type, const std::vector<CL_StringRef> &sources)
{
	type = new_type;
	shader_source.clear();
	for (unsigned int i = 0; i < sources.size(); i++)
		shader_source.append(sources[i]);
}

void CL_D3D9ShaderObjectProvider::destroy()
{
	delete this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_D3D9ShaderObjectProvider Attributes:

unsigned int CL_D3D9ShaderObjectProvider::get_handle() const
{
	return 0;
}

bool CL_D3D9ShaderObjectProvider::get_compile_status() const
{
	return !shader_code.is_null();
}

CL_ShaderType CL_D3D9ShaderObjectProvider::get_shader_type() const
{
	return type;
}

CL_String CL_D3D9ShaderObjectProvider::get_info_log() const
{
	CL_StringRef8 s;
	if (!errors.is_null())
		s = CL_StringRef8((const char *) errors->GetBufferPointer(), errors->GetBufferSize(), false);
	return CL_StringHelp::local8_to_text(s);
}

CL_String CL_D3D9ShaderObjectProvider::get_shader_source() const
{
	return shader_source;
}

/////////////////////////////////////////////////////////////////////////////
// CL_D3D9ShaderObjectProvider Operations:

void CL_D3D9ShaderObjectProvider::compile()
{
	CL_String8 shader_source8 = CL_StringHelp::text_to_local8(shader_source);
	HRESULT result = D3DXCompileShader(shader_source8.data(), shader_source8.length(), 0, 0, "main", D3DXGetVertexShaderProfile(window->get_device()), 0, shader_code.output_variable(), errors.output_variable(), shader_constants.output_variable());
	if (FAILED(result))
		return;

	switch (type)
	{
	case cl_shadertype_vertex:
		result = window->get_device()->CreateVertexShader((const DWORD *) shader_code->GetBufferPointer(), vertex_shader.output_variable());
		if (FAILED(result))
			throw CL_Exception("Unable to create vertex shader object");
		break;
	case cl_shadertype_fragment:
		result = window->get_device()->CreatePixelShader((const DWORD *) shader_code->GetBufferPointer(), pixel_shader.output_variable());
		if (FAILED(result))
			throw CL_Exception("Unable to create pixel shader object");
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_D3D9ShaderObjectProvider Implementation:
