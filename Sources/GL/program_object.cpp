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
**    Harry Storbacka
*/

#include "Display/display_precomp.h"
#include "API/GL/program_object.h"
#include "API/GL/program_attribute.h"
#include "API/GL/program_uniform.h"
#include "API/GL/shader_object.h"
#include "API/GL/opengl_wrap.h"
#include "API/Core/System/error.h"
#include "API/Core/System/clanstring.h"
#include "API/Core/Resources/resource.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/inputsource_provider.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ProgramObject_Generic:

class CL_ProgramObject_Generic
{
//! Construction:
public:
	CL_ProgramObject_Generic();
	
	~CL_ProgramObject_Generic();

//! Attributes:
public:
	CLuint handle;
	bool in_use;

	std::vector<CL_ShaderObject> shaders;
	
//! Operations:
public:
};

CL_ProgramObject_Generic::CL_ProgramObject_Generic() : handle(0), in_use(false)
{
	handle = clCreateProgram();
}

CL_ProgramObject_Generic::~CL_ProgramObject_Generic()
{
	clDeleteProgram(handle);
}

/////////////////////////////////////////////////////////////////////////////
// CL_ProgramObject Construction:

CL_ProgramObject::CL_ProgramObject() : impl(new CL_ProgramObject_Generic)
{
}

CL_ProgramObject::CL_ProgramObject(const std::string &resource_id, CL_ResourceManager *resources)
: impl(new CL_ProgramObject_Generic)
{
	CL_Resource resource = resources->get_resource(resource_id);
	CL_DomNode node = resource.get_element().get_first_child();
	while (!node.is_null())
	{
		if (node.is_element())
		{
			CL_DomElement element = node.to_element();
			if (element.get_tag_name() == "shader")
			{
				CL_ShaderObject shader(element.get_attribute("name"), resources);
				attach(shader);
			}
			else if (element.get_tag_name() == "bind-attribute")
			{
				bind_attribute_location(
					CL_String::to_int(element.get_attribute("index")),
					element.get_attribute("name"));
			}
		}
		node = node.get_next_sibling();
	}
	
	if (!resource.get_element().get_attribute("shader").empty())
	{
		CL_ShaderObject shader(resource.get_element().get_attribute("shader"), resources);
		attach(shader);
	}
	
	if (resource.get_element().get_attribute("link", "true") == "true")
	{
		link();
		if (get_link_status() == false)
			throw CL_Error(resource_id + " failed to link: " + get_info_log());
	}
}

CL_ProgramObject::~CL_ProgramObject()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ProgramObject Attributes:

unsigned int CL_ProgramObject::get_handle() const
{
	return (unsigned int) impl->handle;
}

bool CL_ProgramObject::get_link_status() const
{
	CLint status = 0;
	clGetProgramiv(impl->handle, CL_LINK_STATUS, &status);
	return (status != CL_FALSE);
}
	
bool CL_ProgramObject::get_validate_status() const
{
	CLint status = 0;
	clGetProgramiv(impl->handle, CL_VALIDATE_STATUS, &status);
	return (status != CL_FALSE);
}
	
std::vector<CL_ShaderObject> CL_ProgramObject::get_shaders() const
{
	return impl->shaders;
}
	
std::string CL_ProgramObject::get_info_log() const
{
	std::string result;
	CLsizei buffer_size = 16*1024;
	while (buffer_size < 2*1024*1024)
	{
		CLchar *info_log = new CLchar[buffer_size];
		CLsizei length = 0;
		clGetProgramInfoLog(impl->handle, buffer_size, &length, info_log);
		if (length < buffer_size-1)
			result = std::string(info_log, length);
		delete[] info_log;
		if (length < buffer_size-1)
			break;
		buffer_size *= 2;
	}
	return result;
}
	
int CL_ProgramObject::get_uniform_count() const
{
	CLint count = 0;
	clGetProgramiv(impl->handle, CL_ACTIVE_UNIFORMS, &count);
	return count;
}
	
std::vector<CL_ProgramUniform> CL_ProgramObject::get_uniforms() const
{
	std::vector<CL_ProgramUniform> uniforms;
	int count = get_uniform_count();
	CLint name_size = 0;
	clGetProgramiv(impl->handle, CL_ACTIVE_UNIFORM_MAX_LENGTH, &name_size);
	CLchar *name = new CLchar[name_size+1];
	name[name_size] = 0;
	for (int i=0; i<count; i++)
	{
		CLsizei length = 0;
		CLint size = 0;
		CLenum type = 0;
		name[0] = 0;
		clGetActiveUniform(impl->handle, i, name_size, &length, &size, &type, name);

		std::string uniform_name(name, length);
		CL_ProgramUniform uniform(uniform_name, size, type);
		uniforms.push_back(uniform);
	}
	return uniforms;
}
	
int CL_ProgramObject::get_uniform_location(const std::string &name) const
{
	int loc = clGetUniformLocation(impl->handle, name.c_str());

	if (loc == -1)
	{
		throw CL_Error(CL_String::format("Uniform variable '%1' not present in program object (maybe not active?).", name));
	}

	return loc;
}

bool CL_ProgramObject::has_uniform(const std::string &name, int type) const
{
	std::vector<CL_ProgramUniform> uniforms = get_uniforms();
	std::vector<CL_ProgramUniform>::iterator it;

	for(it = uniforms.begin(); it != uniforms.end(); ++it)
	{
		if( (*it).name == name && (*it).type == type )
		{
			return true;
		}
	}

	return false;
}

int CL_ProgramObject::get_attribute_count() const
{
	CLint count = 0;
	clGetProgramiv(impl->handle, CL_ACTIVE_ATTRIBUTES, &count);
	return count;
}
	
std::vector<CL_ProgramAttribute> CL_ProgramObject::get_attributes() const
{
	std::vector<CL_ProgramAttribute> attributes;
	int count = get_attribute_count();
	CLint name_size = 0;
	clGetProgramiv(impl->handle, CL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &name_size);
	CLchar *name = new CLchar[name_size+1];
	name[name_size] = 0;
	for (int i=0; i<count; i++)
	{
		CLsizei length = 0;
		CLint size = 0;
		CLenum type = 0;
		name[0] = 0;
		clGetActiveAttrib(impl->handle, i, name_size, &length, &size, &type, name);

		std::string attrib_name(name, length);
		CL_ProgramAttribute attribute(attrib_name, size, type);
		attributes.push_back(attribute);
	}
	return attributes;
}
	
int CL_ProgramObject::get_attribute_location(const std::string &name) const
{
	return clGetAttribLocation(impl->handle, name.c_str());
}
	
/////////////////////////////////////////////////////////////////////////////	
// CL_ProgramObject Operations:

void CL_ProgramObject::attach(const CL_ShaderObject &obj)
{
	impl->shaders.push_back(obj);
	clAttachShader(impl->handle, (CLuint) obj.get_handle());
}

void CL_ProgramObject::detach(const CL_ShaderObject &obj)
{
	for (std::vector<CL_ShaderObject>::size_type i = 0; i < impl->shaders.size(); i++)
	{
		if (impl->shaders[i] == obj)
		{
			impl->shaders.erase(impl->shaders.begin()+i);
			break;
		}
	}
	clDetachShader(impl->handle, (CLuint) obj.get_handle());
}

void CL_ProgramObject::bind_attribute_location(int index, const std::string &name)
{
	clBindAttribLocation(impl->handle, index, name.c_str());
}

void CL_ProgramObject::link()
{
	clLinkProgram(impl->handle);
}
	
void CL_ProgramObject::validate()
{
	clValidateProgram(impl->handle);
}

void CL_ProgramObject::use()
{
	if( impl->in_use == false )
	{
		impl->in_use = true;
		clUseProgram(impl->handle);
	}
}

void CL_ProgramObject::disable()
{
	if( impl->in_use )
	{
		impl->in_use = false;
		clUseProgram(0);
	}
	else
	{
		throw CL_Error("CL_ProgramObject::disable(): trying to disable shader program not currently in use.");
	}
}


void CL_ProgramObject::set_uniform(const std::string &name, int p1)
{
	int loc = get_uniform_location(name);
	clUniform1i(loc, p1);	
}

void CL_ProgramObject::set_uniform(const std::string &name, int p1, int p2)
{
	int loc = get_uniform_location(name);
	clUniform2i(loc, p1, p2);	
}

void CL_ProgramObject::set_uniform(const std::string &name, int p1, int p2, int p3)
{
	int loc = get_uniform_location(name);
	clUniform3i(loc, p1, p2, p3);	
}

void CL_ProgramObject::set_uniform(const std::string &name, int p1, int p2, int p3, int p4)
{
	int loc = get_uniform_location(name);
	clUniform4i(loc, p1, p2, p3, p4);	
}

void CL_ProgramObject::set_uniform(const std::string &name, int size, int count, int *data)
{
	int loc = get_uniform_location(name);
	if( size == 1 ) clUniform1iv(loc, count, (const CLint*)data);
	else if( size == 2 ) clUniform2iv(loc, count, (const CLint*)data);
	else if( size == 3 ) clUniform3iv(loc, count, (const CLint*)data);
	else if( size == 4 ) clUniform4iv(loc, count, (const CLint*)data);
	else throw CL_Error(CL_String::format("CL_ProgramObject::set_uniform: unsupported size given to uniform '%1'.", name.c_str()));
}


void CL_ProgramObject::set_uniform(const std::string &name, float p1)
{
	int loc = get_uniform_location(name);
	clUniform1f(loc, p1);	
}

void CL_ProgramObject::set_uniform(const std::string &name, float p1, float p2)
{
	int loc = get_uniform_location(name);
	clUniform2f(loc, p1, p2);	
}

void CL_ProgramObject::set_uniform(const std::string &name, float p1, float p2, float p3)
{
	int loc = get_uniform_location(name);
	clUniform3f(loc, p1, p2, p3);	
}

void CL_ProgramObject::set_uniform(const std::string &name, float p1, float p2, float p3, float p4)
{
	int loc = get_uniform_location(name);
	clUniform4f(loc, p1, p2, p3, p4);	
}

void CL_ProgramObject::set_uniform(const std::string &name, int size, int count, float *data)
{
	int loc = get_uniform_location(name);
	if( size == 1 ) clUniform1fv(loc, count, data);	
	else if( size == 2 ) clUniform2fv(loc, count, data);	
	else if( size == 3 ) clUniform3fv(loc, count, data);	
	else if( size == 4 ) clUniform4fv(loc, count, data);	
	else throw CL_Error(CL_String::format("CL_ProgramObject::set_uniform: unsupported size given to uniform '%1'.", name.c_str()));
}


void CL_ProgramObject::set_uniform_matrix(const std::string &name, int size, int count, bool transpose, float *data)
{
	int loc = get_uniform_location(name);
	if( size == 2 ) clUniformMatrix2fv(loc, count, transpose, data);	
	else if( size == 3 ) clUniformMatrix3fv(loc, count, transpose, data);	
	else if( size == 4 ) clUniformMatrix4fv(loc, count, transpose, data);
	else throw CL_Error(CL_String::format("CL_ProgramObject::set_uniform_matrix: unsupported size given to uniform '%1'.", name.c_str()));

}


/////////////////////////////////////////////////////////////////////////////
// CL_ProgramObject Implementation:
