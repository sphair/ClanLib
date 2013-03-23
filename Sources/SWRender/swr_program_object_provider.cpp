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

#include "SWRender/precomp.h"
#include "swr_program_object_provider.h"
#include "API/Display/Render/shader_object.h"
#include "API/Display/Render/program_attribute.h"
#include "API/Display/Render/program_uniform.h"
#include "API/SWRender/software_program.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// CL_SWRenderProgramObjectProvider Construction:

CL_SWRenderProgramObjectProvider::CL_SWRenderProgramObjectProvider() : current_program(NULL), sprite_program(false)
{
}

CL_SWRenderProgramObjectProvider::~CL_SWRenderProgramObjectProvider()
{
}

void CL_SWRenderProgramObjectProvider::destroy()
{
	delete this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SWRenderProgramObjectProvider Attributes:

CL_SoftwareProgram *CL_SWRenderProgramObjectProvider::get_program() const
{
	return current_program;
}

bool CL_SWRenderProgramObjectProvider::is_sprite_program() const
{
	return sprite_program;
}

unsigned int CL_SWRenderProgramObjectProvider::get_handle() const
{
	return 0;
}

bool CL_SWRenderProgramObjectProvider::get_link_status() const
{
	return false;
}

bool CL_SWRenderProgramObjectProvider::get_validate_status() const
{
	return false;
}

std::string CL_SWRenderProgramObjectProvider::get_info_log() const
{
	return std::string();
}

int CL_SWRenderProgramObjectProvider::get_attribute_count() const
{
	return 0;
}

std::vector<CL_ShaderObject> CL_SWRenderProgramObjectProvider::get_shaders() const
{
	return std::vector<CL_ShaderObject>();
}

std::vector<CL_ProgramAttribute> CL_SWRenderProgramObjectProvider::get_attributes() const
{
	return std::vector<CL_ProgramAttribute>();
}

int CL_SWRenderProgramObjectProvider::get_attribute_location(const std::string &name) const
{
	int attribute_index = current_program->get_attribute_index(name);
	if (attribute_index >= 0 && attribute_index < (int)bind_locations.size())
		return bind_locations[attribute_index];
	else
		return -1;
}

int CL_SWRenderProgramObjectProvider::get_uniform_count() const
{
	return 0;
}

std::vector<CL_ProgramUniform> CL_SWRenderProgramObjectProvider::get_uniforms() const
{
	return std::vector<CL_ProgramUniform>();
}

int CL_SWRenderProgramObjectProvider::get_uniform_location(const std::string &name) const
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SWRenderProgramObjectProvider Operations:

void CL_SWRenderProgramObjectProvider::set_program(CL_SoftwareProgram *program)
{
	current_program = program;
	bind_locations.clear();
	bind_locations.resize(current_program->get_attribute_count());
	attribute_defaults.resize(bind_locations.size());
	current_attribute_values.resize(bind_locations.size()*3);
	for (size_t i = 0; i < bind_locations.size(); i++)
	{
		bind_locations[i] = i;
		attribute_defaults[i] = current_program->get_attribute_default(i);
	}
}

void CL_SWRenderProgramObjectProvider::set_sprite_program(bool is_sprite_program_flag)
{
	sprite_program = is_sprite_program_flag;
}

void CL_SWRenderProgramObjectProvider::attach(const CL_ShaderObject &obj)
{
}

void CL_SWRenderProgramObjectProvider::detach(const CL_ShaderObject &obj)
{
}

void CL_SWRenderProgramObjectProvider::bind_attribute_location(int index, const std::string &name)
{
	int attribute_index = current_program->get_attribute_index(name);
	if (attribute_index >= 0 && attribute_index < (int)bind_locations.size())
		bind_locations[attribute_index] = index;
}

void CL_SWRenderProgramObjectProvider::bind_frag_data_location(int color_number, const std::string &name)
{
}

void CL_SWRenderProgramObjectProvider::link()
{
}

void CL_SWRenderProgramObjectProvider::validate()
{
}

void CL_SWRenderProgramObjectProvider::set_uniform1i(const std::string &name, int v1)
{
	current_program->set_uniform(name, CL_Vec4f(v1, 0.0f, 1.0f, 1.0f));
}

void CL_SWRenderProgramObjectProvider::set_uniform2i(const std::string &name, int v1, int v2)
{
	current_program->set_uniform(name, CL_Vec4f(v1, v2, 1.0f, 1.0f));
}

void CL_SWRenderProgramObjectProvider::set_uniform3i(const std::string &name, int v1, int v2, int v3)
{
	current_program->set_uniform(name, CL_Vec4f(v1, v2, v3, 1.0f));
}

void CL_SWRenderProgramObjectProvider::set_uniform4i(const std::string &name, int v1, int v2, int v3, int v4)
{
	current_program->set_uniform(name, CL_Vec4f(v1, v2, v3, v4));
}

void CL_SWRenderProgramObjectProvider::set_uniformiv(const std::string &name, int size, int count, int *data)
{
}

void CL_SWRenderProgramObjectProvider::set_uniform1f(const std::string &name, float v1)
{
	current_program->set_uniform(name, CL_Vec4f(v1, 0.0f, 1.0f, 1.0f));
}

void CL_SWRenderProgramObjectProvider::set_uniform2f(const std::string &name, float v1, float v2)
{
	current_program->set_uniform(name, CL_Vec4f(v1, v2, 1.0f, 1.0f));
}

void CL_SWRenderProgramObjectProvider::set_uniform3f(const std::string &name, float v1, float v2, float v3)
{
	current_program->set_uniform(name, CL_Vec4f(v1, v2, v3, 1.0f));
}

void CL_SWRenderProgramObjectProvider::set_uniform4f(const std::string &name, float v1, float v2, float v3, float v4)
{
	current_program->set_uniform(name, CL_Vec4f(v1, v2, v3, v4));
}

void CL_SWRenderProgramObjectProvider::set_uniformfv(const std::string &name, int size, int count, float *data)
{
}

void CL_SWRenderProgramObjectProvider::set_uniform_matrix(const std::string &name, int size, int count, bool transpose, float *data)
{
	if (!transpose)
		current_program->set_uniform_matrix(name, CL_Mat4f(data));
	else
		current_program->set_uniform_matrix(name, CL_Mat4f(data).transpose());
}

/////////////////////////////////////////////////////////////////////////////
// CL_SWRenderProgramObjectProvider Implementation:

}
