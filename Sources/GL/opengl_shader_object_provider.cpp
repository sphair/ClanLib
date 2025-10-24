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
**    Harry Storbacka
**    Kenneth Gangstoe
*/

#include "GL/precomp.h"
#include "opengl_shader_object_provider.h"
#include "opengl_graphic_context_provider.h"
#include "API/Core/System/exception.h"
#include "API/Core/Text/string_help.h"
#include "API/Display/Render/shared_gc_data.h"

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLShaderObjectProvider Construction:

CL_OpenGLShaderObjectProvider::CL_OpenGLShaderObjectProvider(CL_OpenGLGraphicContextProvider *gc_provider)
: gc_provider(gc_provider), handle(0)
{
	CL_SharedGCData::add_disposable(this);
}

void CL_OpenGLShaderObjectProvider::create(
	CL_ShaderType shader_type,
	const CL_StringRef &source)
{
	CL_OpenGL::set_active(gc_provider);
	if (handle)
		clDeleteShader(handle);

	type = shader_type;
	handle = clCreateShader(shadertype_to_opengl(type));

	CL_String8 source8 = CL_StringHelp::text_to_local8(source);
	const CLchar *sources[1];
	CLint source_lengths[1];
	source_lengths[0] = source.length();
	sources[0] = source8.c_str();
	clShaderSource(handle, 1, sources, source_lengths);
}

void CL_OpenGLShaderObjectProvider::create(
	CL_ShaderType shader_type,
	const std::vector<CL_StringRef> &sources)
{
	CL_OpenGL::set_active(gc_provider);
	if (handle)
		clDeleteShader(handle);

	type = shader_type;
	handle = clCreateShader(shadertype_to_opengl(type));

	CLchar ** array_sources = 0;
	CLint *array_source_lengths = 0;
	try
	{
		array_sources = new CLchar*[sources.size()];
		array_source_lengths = new CLint[sources.size()];
	
		for (std::vector<CL_StringRef>::size_type i = 0; i < sources.size(); i++)
		{
			array_source_lengths[i] = sources[i].length();
			array_sources[i] = (CLchar*) sources[i].c_str();
		}
		clShaderSource(handle, sources.size(), (const CLchar**) array_sources, array_source_lengths);
	}
	catch (...)
	{
		delete[] array_source_lengths;
		delete[] array_sources;
		throw;
	}
}

CL_OpenGLShaderObjectProvider::~CL_OpenGLShaderObjectProvider()
{
	dispose();
	CL_SharedGCData::remove_disposable(this);
}

void CL_OpenGLShaderObjectProvider::on_dispose()
{
	if (handle)
	{
		if (CL_OpenGL::set_active())
		{
			clDeleteShader(handle);
		}
	}
}
void CL_OpenGLShaderObjectProvider::destroy()
{
	delete this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLShaderObjectProvider Attributes:

unsigned int CL_OpenGLShaderObjectProvider::get_handle() const
{
	return (unsigned int) handle;
}

bool CL_OpenGLShaderObjectProvider::get_compile_status() const
{
	CL_OpenGL::set_active(gc_provider);
	CLint status = 0;
	clGetShaderiv(handle, CL_COMPILE_STATUS, &status);
	return (status != CL_FALSE);
}

CL_ShaderType CL_OpenGLShaderObjectProvider::get_shader_type() const
{
	return type;
}

CL_String CL_OpenGLShaderObjectProvider::get_info_log() const
{
	CL_OpenGL::set_active(gc_provider);
	CL_String result;
	CLsizei buffer_size = 16*1024;
	while (buffer_size < 2*1024*1024)
	{
		CLchar *info_log = new CLchar[buffer_size];
		CLsizei length = 0;
		clGetShaderInfoLog(handle, buffer_size, &length, info_log);
		if (length < buffer_size-1)
			result = CL_StringHelp::local8_to_text(CL_StringRef8(info_log, length, false));
		delete[] info_log;
		if (length < buffer_size-1)
			break;
		buffer_size *= 2;
	}
	return result;
}

CL_String CL_OpenGLShaderObjectProvider::get_shader_source() const
{
	CL_OpenGL::set_active(gc_provider);
	CL_String result;
	CLsizei buffer_size = 16*1024;
	while (buffer_size < 2*1024*1024)
	{
		CLchar *shader_source = new CLchar[buffer_size];
		CLsizei length = 0;
		clGetShaderSource(handle, buffer_size, &length, shader_source);
		if (length < buffer_size-1)
			result = CL_StringHelp::local8_to_text(CL_StringRef8(shader_source, length, false));
		delete[] shader_source;
		if (length < buffer_size-1)
			break;
		buffer_size *= 2;
	}
	return result;
}

/////////////////////////////////////////////////////////////////////////////	
// CL_OpenGLShaderObjectProvider Operations:

void CL_OpenGLShaderObjectProvider::compile()
{
	CL_OpenGL::set_active(gc_provider);
	clCompileShader(handle);
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLShaderObjectProvider Implementation:

CLenum CL_OpenGLShaderObjectProvider::shadertype_to_opengl(CL_ShaderType type)
{
	switch (type)
	{
	case cl_shadertype_vertex:
		return CL_VERTEX_SHADER;
	case cl_shadertype_geometry:
		return CL_GEOMETRY_SHADER;
	case cl_shadertype_fragment:
		return CL_FRAGMENT_SHADER;
	default:
		throw CL_Exception("CL_OpenGLShaderObjectProvider: Unknown shader type: " + type);
	}
}
