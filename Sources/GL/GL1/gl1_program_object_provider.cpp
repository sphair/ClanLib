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
**    Harry Storbacka
**    Mark Page
*/

#include "GL/precomp.h"
#include "gl1_program_object_provider.h"
#include "API/Display/Render/shader_object.h"
#include "API/GL/opengl_wrap.h"
#include "API/Core/System/exception.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/Display/Render/shared_gc_data.h"
#include "gl1_graphic_context_provider.h"

namespace clan
{
	GL1ProgramObjectProvider::GL1ProgramObjectProvider(GL1GraphicContextProvider *gc_provider) : gc_provider(gc_provider)
	{
	}

	GL1ProgramObjectProvider::~GL1ProgramObjectProvider()
	{
	}

	void GL1ProgramObjectProvider::on_dispose()
	{

	}

	unsigned int GL1ProgramObjectProvider::get_handle() const
	{
		return 0;
	}

	bool GL1ProgramObjectProvider::get_link_status() const
	{
		return false;
	}

	bool GL1ProgramObjectProvider::get_validate_status() const
	{
		return false;

	}

	std::vector<ShaderObject> GL1ProgramObjectProvider::get_shaders() const
	{
		return shaders;
	}

	std::string GL1ProgramObjectProvider::get_info_log() const
	{
		return std::string("Not implemented");
	}

	int GL1ProgramObjectProvider::get_uniform_location(const std::string &name) const
	{
		return -1;
	}

	int GL1ProgramObjectProvider::get_attribute_location(const std::string &name) const
	{
		return -1;
	}

	int GL1ProgramObjectProvider::get_uniform_buffer_size(int block_index) const
	{
		return 0;
	}

	int GL1ProgramObjectProvider::get_uniform_buffer_index(const std::string &block_name) const
	{
		return -1;
	}

	int GL1ProgramObjectProvider::get_storage_buffer_index(const std::string &name) const
	{
		return -1;
	}

	void GL1ProgramObjectProvider::attach(const ShaderObject &obj)
	{
		throw_if_disposed();
		shaders.push_back(obj);
	}

	void GL1ProgramObjectProvider::detach(const ShaderObject &obj)
	{
		throw_if_disposed();
		for (std::vector<ShaderObject>::size_type i = 0; i < shaders.size(); i++)
		{
			if (shaders[i] == obj)
			{
				shaders.erase(shaders.begin() + i);
				break;
			}
		}
	}

	void GL1ProgramObjectProvider::bind_attribute_location(int index, const std::string &name)
	{
	}

	void GL1ProgramObjectProvider::bind_frag_data_location(int color_number, const std::string &name)
	{
	}

	void GL1ProgramObjectProvider::link()
	{
	}

	void GL1ProgramObjectProvider::validate()
	{
	}

	void GL1ProgramObjectProvider::set_uniform1i(int location, int p1)
	{
	}


	void GL1ProgramObjectProvider::set_uniform2i(int location, int v1, int v2)
	{
	}

	void GL1ProgramObjectProvider::set_uniform3i(int location, int v1, int v2, int v3)
	{
	}

	void GL1ProgramObjectProvider::set_uniform4i(int location, int v1, int v2, int v3, int v4)
	{
	}

	void GL1ProgramObjectProvider::set_uniformiv(int location, int size, int count, const int *data)
	{
	}

	void GL1ProgramObjectProvider::set_uniform1f(int location, float v1)
	{
	}

	void GL1ProgramObjectProvider::set_uniform2f(int location, float v1, float v2)
	{
	}

	void GL1ProgramObjectProvider::set_uniform3f(int location, float v1, float v2, float v3)
	{
	}

	void GL1ProgramObjectProvider::set_uniform4f(int location, float v1, float v2, float v3, float v4)
	{
	}

	void GL1ProgramObjectProvider::set_uniformfv(int location, int size, int count, const float *data)
	{
	}

	void GL1ProgramObjectProvider::set_uniform_matrix(int location, int size, int count, bool transpose, const float *data)
	{
	}

	void GL1ProgramObjectProvider::set_uniform_buffer_index(int block_index, int bind_index)
	{
	}

	void GL1ProgramObjectProvider::set_storage_buffer_index(int buffer_index, int bind_unit_index)
	{
	}
}
