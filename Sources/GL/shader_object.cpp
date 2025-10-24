/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "Display/display_precomp.h"
#include "API/GL/shader_object.h"
#include "API/GL/opengl_wrap.h"
#include "API/Core/System/error.h"
#include "API/Core/Resources/resource.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/inputsource_provider.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ShaderObject_Generic:

class CL_ShaderObject_Generic
{
//! Construction:
public:
	CL_ShaderObject_Generic(CL_ShaderType type);
	
	~CL_ShaderObject_Generic();

//! Attributes:
public:
	CLuint handle;
	
	CL_ShaderType type;

//! Operations:
private:
	static CLenum shadertype_to_opengl(CL_ShaderType type);
};

CL_ShaderObject_Generic::CL_ShaderObject_Generic(CL_ShaderType type) : handle(0), type(type)
{
	handle = clCreateShader(shadertype_to_opengl(type));
}

CL_ShaderObject_Generic::~CL_ShaderObject_Generic()
{
	clDeleteShader(handle);
}

CLenum CL_ShaderObject_Generic::shadertype_to_opengl(CL_ShaderType type)
{
	switch (type)
	{
	case cl_shadertype_vertex:
		return CL_VERTEX_SHADER;
	case cl_shadertype_fragment:
		return CL_FRAGMENT_SHADER;
	default:
		throw CL_Error("Unknown shader type!");
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_ShaderObject Construction:

CL_ShaderObject::CL_ShaderObject(CL_ShaderType type) : impl(new CL_ShaderObject_Generic(type))
{
}

CL_ShaderObject::CL_ShaderObject(CL_ShaderType type, const std::string &source) : impl(new CL_ShaderObject_Generic(type))
{
	const CLchar *sources[1];
	CLint source_lengths[1];
	source_lengths[0] = source.length();
	sources[0] = source.c_str();
	clShaderSource(impl->handle, 1, sources, source_lengths);
	clCompileShader(impl->handle);
}

CL_ShaderObject::CL_ShaderObject(CL_ShaderType type, const std::vector<std::string> &sources) : impl(new CL_ShaderObject_Generic(type))
{
	CLchar ** array_sources = 0;
	CLint *array_source_lengths = 0;
	try
	{
		array_sources = new CLchar*[sources.size()];
		array_source_lengths = new CLint[sources.size()];
	
		for (std::vector<std::string>::size_type i = 0; i < sources.size(); i++)
		{
			array_source_lengths[i] = sources[i].length();
			array_sources[i] = (CLchar*) sources[i].c_str();
		}
		clShaderSource(impl->handle, sources.size(), (const CLchar**) array_sources, array_source_lengths);
	}
	catch (...)
	{
		delete[] array_source_lengths;
		delete[] array_sources;
		throw;
	}
	clCompileShader(impl->handle);
}

CL_ShaderObject::CL_ShaderObject(const std::string &resource_id, CL_ResourceManager *resources)
{
	CL_Resource resource = resources->get_resource(resource_id);
	std::string filename = resource.get_element().get_attribute("file");
	std::string type = resource.get_element().get_tag_name();
	
	CL_ShaderType shader_type;
	if (type == "fragment-shader")
		shader_type = cl_shadertype_fragment;
	else if (type == "vertex-shader")
		shader_type = cl_shadertype_vertex;
	else
		throw CL_Error("Unknown shader type: " + type);

	CL_InputSource *file = resources->get_resource_provider()->open_source(filename);
	CLint size = (CLint) file->size();
	std::string source(size, 0);
	file->read(&source[0], size);
	delete file;
	file = 0;

	const CLchar *str = (const CLchar *) source.c_str();
	impl = CL_SharedPtr<CL_ShaderObject_Generic>(new CL_ShaderObject_Generic(shader_type));
	clShaderSource(impl->handle, 1, &str, &size);
	clCompileShader(impl->handle);

	if (get_compile_status() == false)
		throw CL_Error(resource_id + " compile failed: " + get_info_log());
}

CL_ShaderObject::~CL_ShaderObject()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ShaderObject Attributes:

unsigned int CL_ShaderObject::get_handle() const
{
	return (unsigned int) impl->handle;
}

bool CL_ShaderObject::get_compile_status() const
{
	CLint status = 0;
	clGetShaderiv(impl->handle, CL_COMPILE_STATUS, &status);
	return (status != CL_FALSE);
}

CL_ShaderType CL_ShaderObject::get_shader_type() const
{
	return impl->type;
}

std::string CL_ShaderObject::get_info_log() const
{
	std::string result;
	CLsizei buffer_size = 16*1024;
	while (buffer_size < 2*1024*1024)
	{
		CLchar *info_log = new CLchar[buffer_size];
		CLsizei length = 0;
		clGetShaderInfoLog(impl->handle, buffer_size, &length, info_log);
		if (length < buffer_size-1)
			result = std::string(info_log, length);
		delete[] info_log;
		if (length < buffer_size-1)
			break;
		buffer_size *= 2;
	}
	return result;
}

std::string CL_ShaderObject::get_shader_source() const
{
	std::string result;
	CLsizei buffer_size = 16*1024;
	while (buffer_size < 2*1024*1024)
	{
		CLchar *shader_source = new CLchar[buffer_size];
		CLsizei length = 0;
		clGetShaderSource(impl->handle, buffer_size, &length, shader_source);
		if (length < buffer_size-1)
			result = std::string(shader_source, length);
		delete[] shader_source;
		if (length < buffer_size-1)
			break;
		buffer_size *= 2;
	}
	return result;
}

/////////////////////////////////////////////////////////////////////////////	
// CL_ShaderObject Operations:

bool CL_ShaderObject::operator ==(const CL_ShaderObject &other) const
{
	return impl == other.impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ShaderObject Implementation:

