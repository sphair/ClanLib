/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
#include "gl3_program_object_provider.h"
#include "API/Display/Render/shader_object.h"
#include "API/GL/opengl_wrap.h"
#include "API/Core/System/exception.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/Display/Render/shared_gc_data.h"
#include "gl3_graphic_context_provider.h"
#include "gl3_uniform_buffer_provider.h"

namespace clan
{
	GL3ProgramObjectProvider::GL3ProgramObjectProvider()
		: handle(0)
	{
		SharedGCData::add_disposable(this);
		OpenGL::set_active();
		handle = glCreateProgram();
	}

	GL3ProgramObjectProvider::~GL3ProgramObjectProvider()
	{
		dispose();
		SharedGCData::remove_disposable(this);
	}

	void GL3ProgramObjectProvider::on_dispose()
	{
		if (handle)
		{
			if (OpenGL::set_active())
			{
				glDeleteProgram(handle);
			}
		}
	}

	unsigned int GL3ProgramObjectProvider::get_handle() const
	{
		throw_if_disposed();
		return handle;
	}

	bool GL3ProgramObjectProvider::get_link_status() const
	{
		throw_if_disposed();
		OpenGL::set_active();
		GLint status = 0;
		glGetProgramiv(handle, GL_LINK_STATUS, &status);
		return (status != GL_FALSE);
	}

	bool GL3ProgramObjectProvider::get_validate_status() const
	{
		throw_if_disposed();
		OpenGL::set_active();
		GLint status = 0;
		glGetProgramiv(handle, GL_VALIDATE_STATUS, &status);
		return (status != GL_FALSE);
	}

	std::vector<ShaderObject> GL3ProgramObjectProvider::get_shaders() const
	{
		throw_if_disposed();
		return shaders;
	}

	std::string GL3ProgramObjectProvider::get_info_log() const
	{
		throw_if_disposed();
		OpenGL::set_active();
		std::string result;
		GLsizei buffer_size = 16 * 1024;
		while (buffer_size < 2 * 1024 * 1024)
		{
			auto info_log = new GLchar[buffer_size];
			GLsizei length = 0;
			glGetProgramInfoLog(handle, buffer_size, &length, info_log);
			if (length < buffer_size - 1)
				result = std::string(info_log, length);
			delete[] info_log;
			if (length < buffer_size - 1)
				break;
			buffer_size *= 2;
		}
		return result;
	}

	int GL3ProgramObjectProvider::get_uniform_location(const std::string &name) const
	{
		throw_if_disposed();
		OpenGL::set_active();
		return glGetUniformLocation(handle, name.c_str());
	}

	int GL3ProgramObjectProvider::get_attribute_location(const std::string &name) const
	{
		throw_if_disposed();
		OpenGL::set_active();
		return glGetAttribLocation(handle, name.c_str());
	}

	int GL3ProgramObjectProvider::get_uniform_buffer_size(int block_index) const
	{
		throw_if_disposed();
		OpenGL::set_active();

		if (!glGetActiveUniformBlockiv)
			throw Exception("incorrect OpenGL version");

		GLsizei uniformBlockSize = 0;
		glGetActiveUniformBlockiv(handle, block_index, GL_UNIFORM_BLOCK_DATA_SIZE, &uniformBlockSize);

		return uniformBlockSize;
	}

	int GL3ProgramObjectProvider::get_uniform_buffer_index(const std::string &block_name) const
	{
		throw_if_disposed();
		OpenGL::set_active();

		if (!glGetUniformBlockIndex)
			throw Exception("incorrect OpenGL version");

		return glGetUniformBlockIndex(handle, block_name.c_str());
	}

	int GL3ProgramObjectProvider::get_storage_buffer_index(const std::string &name) const
	{
		throw_if_disposed();
		OpenGL::set_active();

		if (!glGetProgramResourceIndex)
			throw Exception("incorrect OpenGL version");

		return glGetProgramResourceIndex(handle, GL_SHADER_STORAGE_BLOCK, name.c_str());
	}

	void GL3ProgramObjectProvider::attach(const ShaderObject &obj)
	{
		throw_if_disposed();
		shaders.push_back(obj);
		OpenGL::set_active();
		glAttachShader(handle, (GLuint)obj.get_handle());
	}

	void GL3ProgramObjectProvider::detach(const ShaderObject &obj)
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
		OpenGL::set_active();
		glDetachShader(handle, (GLuint)obj.get_handle());
	}

	void GL3ProgramObjectProvider::bind_attribute_location(int index, const std::string &name)
	{
		throw_if_disposed();
		OpenGL::set_active();
		glBindAttribLocation(handle, index, name.c_str());
	}

	void GL3ProgramObjectProvider::bind_frag_data_location(int color_number, const std::string &name)
	{
		throw_if_disposed();
		OpenGL::set_active();
		glBindFragDataLocation(handle, color_number, name.c_str());
	}

	void GL3ProgramObjectProvider::link()
	{
		throw_if_disposed();
		OpenGL::set_active();
		glLinkProgram(handle);
	}

	void GL3ProgramObjectProvider::validate()
	{
		throw_if_disposed();
		OpenGL::set_active();
		glValidateProgram(handle);
	}

	void GL3ProgramObjectProvider::set_uniform1i(int location, int p1)
	{
		throw_if_disposed();
		if (location >= 0)
		{
			ProgramObjectStateTracker state_tracker(handle);
			glUniform1i(location, p1);
		}
	}

	void GL3ProgramObjectProvider::set_uniform2i(int location, int v1, int v2)
	{
		throw_if_disposed();
		if (location >= 0)
		{
			ProgramObjectStateTracker state_tracker(handle);
			glUniform2i(location, v1, v2);
		}
	}

	void GL3ProgramObjectProvider::set_uniform3i(int location, int v1, int v2, int v3)
	{
		throw_if_disposed();
		if (location >= 0)
		{
			ProgramObjectStateTracker state_tracker(handle);
			glUniform3i(location, v1, v2, v3);
		}
	}

	void GL3ProgramObjectProvider::set_uniform4i(int location, int v1, int v2, int v3, int v4)
	{
		throw_if_disposed();
		if (location >= 0)
		{
			ProgramObjectStateTracker state_tracker(handle);
			glUniform4i(location, v1, v2, v3, v4);
		}
	}

	void GL3ProgramObjectProvider::set_uniformiv(int location, int size, int count, const int *data)
	{
		throw_if_disposed();
		if (location >= 0)
		{
			ProgramObjectStateTracker state_tracker(handle);
			if (size == 1) glUniform1iv(location, count, data);
			else if (size == 2) glUniform2iv(location, count, data);
			else if (size == 3) glUniform3iv(location, count, data);
			else if (size == 4) glUniform4iv(location, count, data);
		}
	}

	void GL3ProgramObjectProvider::set_uniform1f(int location, float v1)
	{
		throw_if_disposed();
		if (location >= 0)
		{
			ProgramObjectStateTracker state_tracker(handle);
			glUniform1f(location, v1);
		}
	}

	void GL3ProgramObjectProvider::set_uniform2f(int location, float v1, float v2)
	{
		throw_if_disposed();
		if (location >= 0)
		{
			ProgramObjectStateTracker state_tracker(handle);
			glUniform2f(location, v1, v2);
		}
	}

	void GL3ProgramObjectProvider::set_uniform3f(int location, float v1, float v2, float v3)
	{
		throw_if_disposed();
		if (location >= 0)
		{
			ProgramObjectStateTracker state_tracker(handle);
			glUniform3f(location, v1, v2, v3);
		}
	}

	void GL3ProgramObjectProvider::set_uniform4f(int location, float v1, float v2, float v3, float v4)
	{
		throw_if_disposed();
		if (location >= 0)
		{
			ProgramObjectStateTracker state_tracker(handle);
			glUniform4f(location, v1, v2, v3, v4);
		}
	}

	void GL3ProgramObjectProvider::set_uniformfv(int location, int size, int count, const float *data)
	{
		throw_if_disposed();
		if (location >= 0)
		{
			ProgramObjectStateTracker state_tracker(handle);
			if (size == 1) glUniform1fv(location, count, data);
			else if (size == 2) glUniform2fv(location, count, data);
			else if (size == 3) glUniform3fv(location, count, data);
			else if (size == 4) glUniform4fv(location, count, data);
		}
	}

	void GL3ProgramObjectProvider::set_uniform_matrix(int location, int size, int count, bool transpose, const float *data)
	{
		throw_if_disposed();
		if (location >= 0)
		{
			ProgramObjectStateTracker state_tracker(handle);
			if (size == 2) glUniformMatrix2fv(location, count, transpose, data);
			else if (size == 3) glUniformMatrix3fv(location, count, transpose, data);
			else if (size == 4) glUniformMatrix4fv(location, count, transpose, data);
		}
	}

	void GL3ProgramObjectProvider::set_uniform_buffer_index(int block_index, int bind_index)
	{
		throw_if_disposed();
		if (block_index == -1)
			return;
		glUniformBlockBinding(handle, block_index, bind_index);
	}

	void GL3ProgramObjectProvider::set_storage_buffer_index(int buffer_index, int bind_unit_index)
	{
		throw_if_disposed();
		if (buffer_index == -1)
			return;
		glShaderStorageBlockBinding(handle, buffer_index, bind_unit_index);
	}

	/////////////////////////////////////////////////////////////////////////////

	ProgramObjectStateTracker::ProgramObjectStateTracker(GLuint handle) : program_set(false)
	{
		OpenGL::set_active();

		glGetIntegerv(GL_CURRENT_PROGRAM, (GLint *)&last_program_object);
		if (handle != last_program_object)
		{
			program_set = true;
			glUseProgram(handle);
		}
	}

	ProgramObjectStateTracker::~ProgramObjectStateTracker()
	{
		if (program_set)
			glUseProgram(last_program_object);
	}
}
