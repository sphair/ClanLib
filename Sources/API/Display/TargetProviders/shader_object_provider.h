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
**    Harry Storbacka
**    Kenneth Gangstoe
*/

/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

#pragma once

#include "shader_object_provider.h"
#include "../Render/shader_object.h"

/// \brief Shader Object provider.
///
/// \xmlonly !group=Display/Display Target Interface! !header=display.h! \endxmlonly
class CL_ShaderObjectProvider
{
/// \name Construction
/// \{

public:
	virtual ~CL_ShaderObjectProvider() {};

	/// \brief Creates a shader object.
	virtual void create(CL_ShaderType type, const CL_StringRef &source)=0;

	/// \brief Create
	///
	/// \param type = Shader Type
	/// \param vector = std
	virtual void create(CL_ShaderType type, const std::vector<CL_StringRef> &sources)=0;

	/// \brief Destroys the shadoer object provider.
	virtual void destroy()=0;

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the OpenGL shader handle.
	virtual unsigned int get_handle() const=0;

	/// \brief Returns true if compile succeeded.
	virtual bool get_compile_status() const=0;

	/// \brief Gets the shader type.
	virtual CL_ShaderType get_shader_type() const=0;

	/// \brief Get shader object's info log.
	virtual CL_String get_info_log() const=0;

	/// \brief Get shader source code.
	virtual CL_String get_shader_source() const=0;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Compile program.
	virtual void compile()=0;

/// \}
/// \name Implementation
/// \{

private:
/// \}
};

/// \}
