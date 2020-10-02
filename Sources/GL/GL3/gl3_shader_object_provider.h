/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
**    Harry Storbacka
**    Kenneth Gangstoe
*/

#pragma once


#include "API/Display/TargetProviders/shader_object_provider.h"
#include "API/GL/opengl_wrap.h"
#include "API/GL/opengl.h"
#include "API/Core/System/disposable_object.h"

namespace clan
{
	class GL3GraphicContextProvider;

	class GL3ShaderObjectProvider : public ShaderObjectProvider, DisposableObject
	{
	public:
		GL3ShaderObjectProvider();
		virtual ~GL3ShaderObjectProvider();

		void create(ShaderType type, const std::string &source) override;
		void create(ShaderType type, const void *source, int source_size) override;
		void create(ShaderType type, const std::vector<std::string> &sources) override;

		/// \brief Returns the OpenGL shader handle.
		unsigned int get_handle() const override;

		/// \brief Returns true if compile succeeded.
		bool get_compile_status() const override;

		/// \brief Gets the shader type.
		ShaderType get_shader_type() const override;

		/// \brief Get shader object's info log.
		std::string get_info_log() const override;

		/// \brief Get shader source code.
		std::string get_shader_source() const override;

		/// \brief Compile program.
		/** <p>If the compilation fails, get_compile_status() will return false and
			get_info_log() will return the compile log.</p>*/
		void compile() override;

	private:
		void on_dispose() override;
		GLenum shadertype_to_opengl(ShaderType type);

		GLuint handle;
		ShaderType type;
	};
}
