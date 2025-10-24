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

#ifndef header_program_object
#define header_program_object

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
class CL_ShaderObject;
class CL_ProgramAttribute;
class CL_ProgramUniform;
class CL_ProgramObject_Generic;

//: OpenGL Program Object
//- <p>The shader objects that are to be used by programmable stages of
//- OpenGL are collected together to form a program object. CL_ProgramObject
//- is ClanLib's C++ interface to OpenGL program objects.</p>
//- <p>To construct a program object programatically, the procedure is
//- as follows:</p>
//- <pre>
//- CL_ShaderObject vertex_shader(cl_shadertype_vertex, vertex_glsl_sourcecode);
//- CL_ShaderObject fragment_shader(cl_shadertype_fragment, fragment_glsl_sourcecode);
//- CL_ProgramObject program;
//- program.attach(vertex_shader);
//- program.attach(fragment_shader);
//- program.link();
//- </pre>
//- <p>For more information about program objects, see the OpenGL 2.0
//- specification at <a href="http://www.opengl.org">www.opengl.org</a>. Documentation
//- about the OpenGL Shader Language (GLSL) is also available from www.opengl.org.</p>
class CL_API_GL CL_ProgramObject
{
//! Construction:
public:
	//: Construct OpenGL program object.
	//param resource_id: Name of probject object resource.
	//param resources: Resource Manager providing resource.
	CL_ProgramObject();
	
	CL_ProgramObject(const std::string &resource_id, CL_ResourceManager *resources);
	
	virtual ~CL_ProgramObject();

//! Attributes:
public:
	//: Returns the OpenGL program object handle.
	unsigned int get_handle() const;

	//: Returns true if the link succeeded.
	bool get_link_status() const;
	
	//: Returns true if validation succeeded.
	bool get_validate_status() const;

	//: Returns the shaders attached to the program object.	
	std::vector<CL_ShaderObject> get_shaders() const;

	//: Returns the current info log for the program object.
	std::string get_info_log() const;
	
	//: Returns the count of active attributes.
	int get_attribute_count() const;
	
	//: Returns the active attributes.
	std::vector<CL_ProgramAttribute> get_attributes() const;
	
	//: Returns the location of a named active attribute.
	int get_attribute_location(const std::string &name) const;
	
	//: Returns the count of active uniform variables.
	int get_uniform_count() const;

	//: Returns true if the uniform variable is present in the program.
	//param name: uniform variable name
	//param type: CL_FLOAT, CL_FLOAT_VEC2, CL_FLOAT_VEC3, CL_FLOAT_VEC4, CL_INT, CL_INT_VEC2, CL_INT_VEC3, CL_INT_VEC4, CL_BOOL, CL_BOOL_VEC2, CL_BOOL_VEC3, CL_BOOL_VEC4, CL_FLOAT_MAT2, CL_FLOAT_MAT3, CL_FLOAT_MAT4, CL_SAMPLER_1D, CL_SAMPLER_2D, CL_SAMPLER_3D, CL_SAMPLER_CUBE, CL_SAMPLER_1D SHADOW or CL_SAMPLER_2D_SHADOW
	bool has_uniform(const std::string &name, int type) const;

	//: Returns a list of the active uniforms.
	std::vector<CL_ProgramUniform> get_uniforms() const;

	//: Returns the location of a named uniform variable.
	int get_uniform_location(const std::string &name) const;
	
//! Operations:
public:
	//: Add shader to program object.
	void attach(const CL_ShaderObject &obj);

	//: Remove shader from program object.
	void detach(const CL_ShaderObject &obj);

	//: Bind attribute to specific location.
	//- <p>This function must be called before linking.</p>
	void bind_attribute_location(int index, const std::string &name);

	//: Link program.
	//- <p>If the linking fails, get_link_status() will return false and
	//- get_info_log() will return the link log.</p>
	void link();

	//: Validate program.
	//- <p>If the validation fails, get_validate_status() will return
	//- false and get_info_log() will return the validation log.</p>
	void validate();

	//: Start using the shader.
	void use();

	//: Stop using the shader.
	void disable();

	//: Set uniform variable(s).
	void set_uniform(const std::string &name, int);
	void set_uniform(const std::string &name, int, int);
	void set_uniform(const std::string &name, int, int, int);
	void set_uniform(const std::string &name, int, int, int, int);
	void set_uniform(const std::string &name, int size, int count, int *data);
	void set_uniform(const std::string &name, float);
	void set_uniform(const std::string &name, float, float);
	void set_uniform(const std::string &name, float, float, float);
	void set_uniform(const std::string &name, float, float, float, float);
	void set_uniform(const std::string &name, int size, int count, float *data);

	//: Set uniform matrices.
	void set_uniform_matrix(const std::string &name, int size, int count, bool transpose, float *data);

//! Implementation:
private:
	CL_SharedPtr<CL_ProgramObject_Generic> impl;
};

#endif
