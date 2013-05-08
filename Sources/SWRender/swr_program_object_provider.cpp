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

#include "SWRender/precomp.h"
#include "swr_program_object_provider.h"
#include "API/Display/Render/shader_object.h"
#include "API/SWRender/software_program.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// SWRenderProgramObjectProvider Construction:

SWRenderProgramObjectProvider::SWRenderProgramObjectProvider() : current_program(NULL), sprite_program(false)
{
}

SWRenderProgramObjectProvider::~SWRenderProgramObjectProvider()
{
}

/////////////////////////////////////////////////////////////////////////////
// SWRenderProgramObjectProvider Attributes:

SoftwareProgram *SWRenderProgramObjectProvider::get_program() const
{
	return current_program;
}

bool SWRenderProgramObjectProvider::is_sprite_program() const
{
	return sprite_program;
}

unsigned int SWRenderProgramObjectProvider::get_handle() const
{
	return 0;
}

bool SWRenderProgramObjectProvider::get_link_status() const
{
	return false;
}

bool SWRenderProgramObjectProvider::get_validate_status() const
{
	return false;
}

std::string SWRenderProgramObjectProvider::get_info_log() const
{
	return std::string();
}

std::vector<ShaderObject> SWRenderProgramObjectProvider::get_shaders() const
{
	return std::vector<ShaderObject>();
}

int SWRenderProgramObjectProvider::get_attribute_location(const std::string &name) const
{
	int attribute_index = current_program->get_attribute_index(name);
	if (attribute_index >= 0 && attribute_index < (int)bind_locations.size())
		return bind_locations[attribute_index];
	else
		return -1;
}

int SWRenderProgramObjectProvider::get_uniform_location(const std::string &name) const
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// SWRenderProgramObjectProvider Operations:

void SWRenderProgramObjectProvider::set_program(SoftwareProgram *program)
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

void SWRenderProgramObjectProvider::set_sprite_program(bool is_sprite_program_flag)
{
	sprite_program = is_sprite_program_flag;
}

void SWRenderProgramObjectProvider::attach(const ShaderObject &obj)
{
}

void SWRenderProgramObjectProvider::detach(const ShaderObject &obj)
{
}

void SWRenderProgramObjectProvider::bind_attribute_location(int index, const std::string &name)
{
	int attribute_index = current_program->get_attribute_index(name);
	if (attribute_index >= 0 && attribute_index < (int)bind_locations.size())
		bind_locations[attribute_index] = index;
}

void SWRenderProgramObjectProvider::bind_frag_data_location(int color_number, const std::string &name)
{
}

void SWRenderProgramObjectProvider::link()
{
}

void SWRenderProgramObjectProvider::validate()
{
}

void SWRenderProgramObjectProvider::set_uniform1i(int location, int v1)
{
	current_program->set_uniform(location, Vec4f(v1, 0.0f, 1.0f, 1.0f));
}

void SWRenderProgramObjectProvider::set_uniform2i(int location, int v1, int v2)
{
	current_program->set_uniform(location, Vec4f(v1, v2, 1.0f, 1.0f));
}

void SWRenderProgramObjectProvider::set_uniform3i(int location, int v1, int v2, int v3)
{
	current_program->set_uniform(location, Vec4f(v1, v2, v3, 1.0f));
}

void SWRenderProgramObjectProvider::set_uniform4i(int location, int v1, int v2, int v3, int v4)
{
	current_program->set_uniform(location, Vec4f(v1, v2, v3, v4));
}

void SWRenderProgramObjectProvider::set_uniformiv(int location, int size, int count, int *data)
{
}

void SWRenderProgramObjectProvider::set_uniform1f(int location, float v1)
{
	current_program->set_uniform(location, Vec4f(v1, 0.0f, 1.0f, 1.0f));
}

void SWRenderProgramObjectProvider::set_uniform2f(int location, float v1, float v2)
{
	current_program->set_uniform(location, Vec4f(v1, v2, 1.0f, 1.0f));
}

void SWRenderProgramObjectProvider::set_uniform3f(int location, float v1, float v2, float v3)
{
	current_program->set_uniform(location, Vec4f(v1, v2, v3, 1.0f));
}

void SWRenderProgramObjectProvider::set_uniform4f(int location, float v1, float v2, float v3, float v4)
{
	current_program->set_uniform(location, Vec4f(v1, v2, v3, v4));
}

void SWRenderProgramObjectProvider::set_uniformfv(int location, int size, int count, float *data)
{
}

void SWRenderProgramObjectProvider::set_uniform_matrix(int location, int size, int count, bool transpose, float *data)
{
	if (!transpose)
		current_program->set_uniform_matrix(location, Mat4f(data));
	else
		current_program->set_uniform_matrix(location, Mat4f(data).transpose());
}

int SWRenderProgramObjectProvider::get_uniform_buffer_size(int block_index) const
{
	//FIXME: Stub
	return 0;
}
int SWRenderProgramObjectProvider::get_uniform_buffer_index(const std::string &block_name) const
{
	//FIXME: Stub
	return 0;
}
int SWRenderProgramObjectProvider::get_storage_buffer_index(const std::string &name) const
{
	//FIXME: Stub
	return 0;
}

void SWRenderProgramObjectProvider::set_uniform_buffer_index(int block_index, int bind_index)
{
	//FIXME: Stub
}
void SWRenderProgramObjectProvider::set_storage_buffer_index(int buffer_index, int bind_unit_index)
{
	//FIXME: Stub
}

/////////////////////////////////////////////////////////////////////////////
// SWRenderProgramObjectProvider Implementation:

}
