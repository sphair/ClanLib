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
*/

#include "GL/precomp.h"
#include "opengl_program_object_provider.h"
#include "API/Display/Render/program_attribute.h"
#include "API/Display/Render/program_uniform.h"
#include "API/Display/Render/shader_object.h"
#include "API/GL/opengl_wrap.h"
#include "API/Core/System/exception.h"
#include "API/Core/Text/string_types.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/Display/Render/shared_gc_data.h"
#include "opengl_graphic_context_provider.h"

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLProgramObjectProvider Construction:

CL_OpenGLProgramObjectProvider::CL_OpenGLProgramObjectProvider(CL_OpenGLGraphicContextProvider *gc_provider)
: handle(0), gc_provider(gc_provider)
{
	CL_SharedGCData::add_disposable(this);
	CL_OpenGL::set_active(gc_provider);
	handle = clCreateProgram();
}

CL_OpenGLProgramObjectProvider::~CL_OpenGLProgramObjectProvider()
{
	dispose();
	CL_SharedGCData::remove_disposable(this);
}

void CL_OpenGLProgramObjectProvider::on_dispose()
{
	if (handle)
	{
		if (CL_OpenGL::set_active())
		{
			clDeleteProgram(handle);
		}
	}
}
void CL_OpenGLProgramObjectProvider::destroy()
{
	delete this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLProgramObjectProvider Attributes:

unsigned int CL_OpenGLProgramObjectProvider::get_handle() const
{
	throw_if_disposed();
	return handle;
}

bool CL_OpenGLProgramObjectProvider::get_link_status() const
{
	throw_if_disposed();
	CL_ProgramObjectStateTracker state_tracker(handle, 0);
	CLint status = 0;
	clGetProgramiv(handle, CL_LINK_STATUS, &status);
	return (status != CL_FALSE);
}
	
bool CL_OpenGLProgramObjectProvider::get_validate_status() const
{
	throw_if_disposed();
	CL_ProgramObjectStateTracker state_tracker(handle, 0);
	CLint status = 0;
	clGetProgramiv(handle, CL_VALIDATE_STATUS, &status);
	return (status != CL_FALSE);
}
	
std::vector<CL_ShaderObject> CL_OpenGLProgramObjectProvider::get_shaders() const
{
	throw_if_disposed();
	return shaders;
}
	
CL_String CL_OpenGLProgramObjectProvider::get_info_log() const
{
	throw_if_disposed();
	CL_ProgramObjectStateTracker state_tracker(handle, 0);
	CL_String result;
	CLsizei buffer_size = 16*1024;
	while (buffer_size < 2*1024*1024)
	{
		CLchar *info_log = new CLchar[buffer_size];
		CLsizei length = 0;
		clGetProgramInfoLog(handle, buffer_size, &length, info_log);
		if (length < buffer_size-1)
			result = CL_StringHelp::local8_to_text(CL_StringRef8(info_log, length, false));
		delete[] info_log;
		if (length < buffer_size-1)
			break;
		buffer_size *= 2;
	}
	return result;
}
	
int CL_OpenGLProgramObjectProvider::get_uniform_count() const
{
	throw_if_disposed();
	CL_ProgramObjectStateTracker state_tracker(handle, 0);
	CLint count = 0;
	clGetProgramiv(handle, CL_ACTIVE_UNIFORMS, &count);
	return count;
}
	
std::vector<CL_ProgramUniform> CL_OpenGLProgramObjectProvider::get_uniforms() const
{
	throw_if_disposed();
	if (cached_uniforms.empty())
		fetch_uniforms();

	return cached_uniforms;
}
	
int CL_OpenGLProgramObjectProvider::get_uniform_location(const CL_StringRef &name) const
{
	throw_if_disposed();
	if (cached_uniforms.empty())
		fetch_uniforms();

	std::vector<CL_ProgramUniform>::size_type i;
	for (i = 0; i < cached_uniforms.size(); i++)
	{
		if (cached_uniforms[i].get_name() == name)
			return cached_uniforms[i].get_location();
	}

	return -1;
}

int CL_OpenGLProgramObjectProvider::get_attribute_count() const
{
	throw_if_disposed();
	CL_ProgramObjectStateTracker state_tracker(handle, 0);
	if (cached_attribs.empty())
		fetch_attributes();

	return (int)cached_attribs.size();
}
	
std::vector<CL_ProgramAttribute> CL_OpenGLProgramObjectProvider::get_attributes() const
{
	throw_if_disposed();
	if (cached_attribs.empty())
		fetch_attributes();

	return cached_attribs;
}
	
int CL_OpenGLProgramObjectProvider::get_attribute_location(const CL_StringRef &name) const
{
	throw_if_disposed();
	CL_ProgramObjectStateTracker state_tracker(handle, 0);
	return clGetAttribLocation(handle, CL_StringHelp::text_to_local8(name).c_str());
}
	
/////////////////////////////////////////////////////////////////////////////	
// CL_OpenGLProgramObjectProvider Operations:

void CL_OpenGLProgramObjectProvider::attach(const CL_ShaderObject &obj)
{
	throw_if_disposed();
	shaders.push_back(obj);
	CL_ProgramObjectStateTracker state_tracker(handle, 0);
	clAttachShader(handle, (CLuint) obj.get_handle());
}

void CL_OpenGLProgramObjectProvider::detach(const CL_ShaderObject &obj)
{
	throw_if_disposed();
	for (std::vector<CL_ShaderObject>::size_type i = 0; i < shaders.size(); i++)
	{
		if (shaders[i] == obj)
		{
			shaders.erase(shaders.begin()+i);
			break;
		}
	}
	CL_ProgramObjectStateTracker state_tracker(handle, 0);
	clDetachShader(handle, (CLuint) obj.get_handle());
}

void CL_OpenGLProgramObjectProvider::bind_attribute_location(int index, const CL_StringRef &name)
{
	throw_if_disposed();
	CL_ProgramObjectStateTracker state_tracker(handle, 0);
	clBindAttribLocation(handle, index, CL_StringHelp::text_to_local8(name).c_str());
}

void CL_OpenGLProgramObjectProvider::link()
{
	throw_if_disposed();
	CL_ProgramObjectStateTracker state_tracker(handle, 0);
	clLinkProgram(handle);
}
	
void CL_OpenGLProgramObjectProvider::validate()
{
	throw_if_disposed();
	CL_ProgramObjectStateTracker state_tracker(handle, 0);
	clValidateProgram(handle);
}

void CL_OpenGLProgramObjectProvider::set_uniform1i(const CL_StringRef &name, int p1)
{
	throw_if_disposed();
	CL_ProgramObjectStateTracker state_tracker(handle, 0);
	int loc = get_uniform_location(name);
	if (loc == -1)
		return;
	clUniform1i(loc, p1);	
}

void CL_OpenGLProgramObjectProvider::set_uniform2i(const CL_StringRef &name, int p1, int p2)
{
	throw_if_disposed();
	CL_ProgramObjectStateTracker state_tracker(handle, 0);
	int loc = get_uniform_location(name);
	if (loc == -1)
		return;

	clUniform2i(loc, p1, p2);	
}

void CL_OpenGLProgramObjectProvider::set_uniform3i(const CL_StringRef &name, int p1, int p2, int p3)
{
	throw_if_disposed();
	CL_ProgramObjectStateTracker state_tracker(handle, 0);
	int loc = get_uniform_location(name);
	if (loc == -1)
		return;

	clUniform3i(loc, p1, p2, p3);	
}

void CL_OpenGLProgramObjectProvider::set_uniform4i(const CL_StringRef &name, int p1, int p2, int p3, int p4)
{
	throw_if_disposed();
	CL_ProgramObjectStateTracker state_tracker(handle, 0);
	int loc = get_uniform_location(name);
	if (loc == -1)
		return;

	clUniform4i(loc, p1, p2, p3, p4);	
}

void CL_OpenGLProgramObjectProvider::set_uniformiv(const CL_StringRef &name, int size, int count, int *data)
{
	throw_if_disposed();

	CL_ProgramObjectStateTracker state_tracker(handle, 0);
	int loc = get_uniform_location(name);
	if (loc == -1)
		return;

	if( size == 1 ) clUniform1iv(loc, count, data);	
	else if( size == 2 ) clUniform2iv(loc, count, data);	
	else if( size == 3 ) clUniform3iv(loc, count, data);	
	else if( size == 4 ) clUniform4iv(loc, count, data);	
	else throw CL_Exception(cl_format("Unsupported size given to uniform '%1'.", name));
}

void CL_OpenGLProgramObjectProvider::set_uniform1f(const CL_StringRef &name, float p1)
{
	throw_if_disposed();

	CL_ProgramObjectStateTracker state_tracker(handle, 0);
	int loc = get_uniform_location(name);
	if (loc == -1)
		return;

	clUniform1f(loc, p1);	
}

void CL_OpenGLProgramObjectProvider::set_uniform2f(const CL_StringRef &name, float p1, float p2)
{
	throw_if_disposed();

	CL_ProgramObjectStateTracker state_tracker(handle, 0);
	int loc = get_uniform_location(name);
	if (loc == -1)
		return;

	clUniform2f(loc, p1, p2);	
}

void CL_OpenGLProgramObjectProvider::set_uniform3f(const CL_StringRef &name, float p1, float p2, float p3)
{
	throw_if_disposed();

		CL_ProgramObjectStateTracker state_tracker(handle, 0);
	int loc = get_uniform_location(name);
	if (loc == -1)
		return;

	clUniform3f(loc, p1, p2, p3);	
}

void CL_OpenGLProgramObjectProvider::set_uniform4f(const CL_StringRef &name, float p1, float p2, float p3, float p4)
{
	throw_if_disposed();

	CL_ProgramObjectStateTracker state_tracker(handle, 0);
	int loc = get_uniform_location(name);
	if (loc == -1)
		return;

	clUniform4f(loc, p1, p2, p3, p4);	
}

void CL_OpenGLProgramObjectProvider::set_uniformfv(const CL_StringRef &name, int size, int count, float *data)
{
	throw_if_disposed();

	CL_ProgramObjectStateTracker state_tracker(handle, 0);
	int loc = get_uniform_location(name);
	if (loc == -1)
		return;

	if( size == 1 ) clUniform1fv(loc, count, data);	
	else if( size == 2 ) clUniform2fv(loc, count, data);	
	else if( size == 3 ) clUniform3fv(loc, count, data);	
	else if( size == 4 ) clUniform4fv(loc, count, data);	
	else throw CL_Exception(cl_format("Unsupported size given to uniform '%1'.", name));
}

void CL_OpenGLProgramObjectProvider::set_uniform_matrix(const CL_StringRef &name, int size, int count, bool transpose, float *data)
{
	throw_if_disposed();

	CL_ProgramObjectStateTracker state_tracker(handle, 0);
	int loc = get_uniform_location(name);
	if (loc == -1)
		return;

	if( size == 2 ) clUniformMatrix2fv(loc, count, transpose, data);	
	else if( size == 3 ) clUniformMatrix3fv(loc, count, transpose, data);	
	else if( size == 4 ) clUniformMatrix4fv(loc, count, transpose, data);
	else throw CL_Exception(cl_format("Unsupported size given to uniform '%1'.", name));
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLProgramObjectProvider Implementation:

CL_ProgramObjectStateTracker::CL_ProgramObjectStateTracker(CLuint handle, CL_OpenGLGraphicContextProvider *gc_provider)
{
	// If the gc_provider is unknown, we need to use the first active provider
	if (!gc_provider)
	{
		CL_OpenGL::set_active();
	}
	else
	{
		CL_OpenGL::set_active(gc_provider);
	}

	clGetIntegerv(CL_CURRENT_PROGRAM, (CLint *) &last_program_object);
	clUseProgram(handle);
}

CL_ProgramObjectStateTracker::~CL_ProgramObjectStateTracker()
{
	clUseProgram(last_program_object);
}

void CL_OpenGLProgramObjectProvider::fetch_attributes() const
{
	if (!cached_attribs.empty())
		return;

	CL_ProgramObjectStateTracker state_tracker(handle, 0);

	int count = get_attribute_count();
	CLint name_size = 0;
	clGetProgramiv(handle, CL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &name_size);
	CLchar *name = new CLchar[name_size+1];
	name[name_size] = 0;
	for (int i=0; i<count; i++)
	{
		CLsizei length = 0;
		CLint size = 0;
		CLenum type = 0;
		name[0] = 0;
		clGetActiveAttrib(handle, i, name_size, &length, &size, &type, name);
		CL_String attrib_name = CL_StringHelp::local8_to_text(CL_StringRef8(name, length, false));

		int loc = clGetAttribLocation(handle, CL_StringHelp::text_to_local8(name).c_str());
		CL_ProgramAttribute attribute(attrib_name, size, type);
		cached_attribs.push_back(attribute);
	}
	delete [] name;
}

void CL_OpenGLProgramObjectProvider::fetch_uniforms() const
{
	if (!cached_uniforms.empty())
		return;

	CL_ProgramObjectStateTracker state_tracker(handle, 0);

	int count = get_uniform_count();
	CLint name_size = 0;
	clGetProgramiv(handle, CL_ACTIVE_UNIFORM_MAX_LENGTH, &name_size);
	CLchar *name = new CLchar[name_size+1];
	name[name_size] = 0;
	for (int i=0; i<count; i++)
	{
		CLsizei length = 0;
		CLint size = 0;
		CLenum type = 0;
		name[0] = 0;
		clGetActiveUniform(handle, i, name_size, &length, &size, &type, name);

		CL_String uniform_name = CL_StringHelp::local8_to_text(CL_StringRef8(name, length, false));
		int loc = clGetUniformLocation(handle, CL_StringHelp::text_to_local8(name).c_str());

		CL_ProgramUniform uniform(uniform_name, size, type, loc);
		cached_uniforms.push_back(uniform);
	}
	delete[] name;
};

