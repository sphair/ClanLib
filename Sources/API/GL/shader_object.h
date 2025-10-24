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

//! clanGL="Display 3D"
//! header=gl.h

#ifndef header_shader_object
#define header_shader_object

#ifdef CL_API_DLL
#ifdef CL_GL_EXPORT
#define CL_API_GL __declspec(dllexport)
#else
#define CL_API_GL __declspec(dllimport)
#endif
#else
#define CL_API_GL
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "../Core/System/sharedptr.h"
#include <string>
#include <vector>

class CL_ResourceManager;
class CL_ShaderObject_Generic;

//: OpenGL Shader Type
enum CL_ShaderType
{
	cl_shadertype_vertex,
	cl_shadertype_fragment
};

//: OpenGL Shader Object
//- <p>The source code that makes up a program that gets executed by one of
//- the programmable stages is encapsulated in one or more shader
//- objects. Shader objects are attached to a program objects to form a
//- programmable setup. CL_ShaderObject is ClanLib's C++ interface to OpenGL
//- shader objects.</p>
class CL_API_GL CL_ShaderObject
{
//! Construction:
public:
	//: Constructs an OpenGL shader.
	//param type: Shader type. Can be cl_shadertype_vertex or cl_shadertype_fragment.
	//param source: Shader source code, in OpenGL Shader Language (GLSL).
	//param sources: Shader source code as list of strings.
	//param resource_id: Name of shader object resource.
	//param resources: Resource Manager providing resource.
	CL_ShaderObject(CL_ShaderType type);

	CL_ShaderObject(CL_ShaderType type, const std::string &source);
	
	CL_ShaderObject(CL_ShaderType type, const std::vector<std::string> &sources);
	
	CL_ShaderObject(const std::string &resource_id, CL_ResourceManager *resources);
	
	virtual ~CL_ShaderObject();

//! Attributes:
public:
	//: Returns the OpenGL shader handle.
	unsigned int get_handle() const;
	
	//: Returns true if compile succeeded.
	bool get_compile_status() const;

	//: Gets the shader type.
	CL_ShaderType get_shader_type() const;

	//: Get shader object's info log.
	std::string get_info_log() const;

	//: Get shader source code.
	std::string get_shader_source() const;
	
//! Operations:
public:
	//: Handle assignment operator.
	bool operator==(const CL_ShaderObject &other) const;

//! Implementation:
private:
	CL_SharedPtr<CL_ShaderObject_Generic> impl;
};

#endif
