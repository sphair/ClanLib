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

#pragma once


#include "API/Display/TargetProviders/shader_object_provider.h"
#include "API/GL/opengl_wrap.h"
#include "API/GL/opengl.h"
#include "API/Core/System/disposable_object.h"

class CL_OpenGLGraphicContextProvider;

class CL_OpenGLShaderObjectProvider : public CL_ShaderObjectProvider, CL_DisposableObject
{
/// \name Construction
/// \{

public:
	CL_OpenGLShaderObjectProvider(CL_OpenGLGraphicContextProvider *provider);

	virtual ~CL_OpenGLShaderObjectProvider();

	virtual void create(CL_ShaderType type, const CL_StringRef &source);

	virtual void create(CL_ShaderType type, const std::vector<CL_StringRef> &sources);

	virtual void destroy();


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the OpenGL shader handle.
	virtual unsigned int get_handle() const;

	/// \brief Returns true if compile succeeded.
	virtual bool get_compile_status() const;

	/// \brief Gets the shader type.
	virtual CL_ShaderType get_shader_type() const;

	/// \brief Get shader object's info log.
	virtual CL_String get_info_log() const;

	/// \brief Get shader source code.
	virtual CL_String get_shader_source() const;


/// \}
/// \name Operations
/// \{

public:
	/// \brief Compile program.
	/** <p>If the compilation fails, get_compile_status() will return false and
	    get_info_log() will return the compile log.</p>*/
	virtual void compile();


/// \}
/// \name Implementation
/// \{

private:
	void on_dispose();
	CLenum shadertype_to_opengl(CL_ShaderType type);

	CL_OpenGLGraphicContextProvider *gc_provider;

	CLuint handle;

	CL_ShaderType type;
/// \}
};


