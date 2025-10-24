/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
**    Kenneth Gangstoe
*/

/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{


#pragma once


#include "../api_display.h"
#include "../../Core/System/sharedptr.h"
#include "../../Core/Text/string_types.h"
#include "../../Core/IOData/virtual_directory.h"
#include "graphic_context.h"

class CL_ShaderObject;
class CL_ProgramAttribute;
class CL_ProgramUniform;
class CL_ProgramObject_Impl;
class CL_GraphicContext;
class CL_GraphicContextProvider;
class CL_ResourceManager;

/// \brief OpenGL Program Object
///
///    <p>The shader objects that are to be used by programmable stages of
///    OpenGL are collected together to form a program object. CL_ProgramObject
///    is ClanLib's C++ interface to OpenGL program objects.</p>
///    <p>To construct a program object programatically, the procedure is
///    as follows:</p>
///    <pre>
///    CL_ShaderObject vertex_shader(cl_shadertype_vertex, vertex_glsl_sourcecode);
///    CL_ShaderObject fragment_shader(cl_shadertype_fragment, fragment_glsl_sourcecode);
///    CL_ProgramObject program;
///    program.attach(vertex_shader);
///    program.attach(fragment_shader);
///    program.link();
///    </pre>
///    <p>For more information about program objects, see the OpenGL 2.0
///    specification at <a href="http://www.opengl.org">www.opengl.org</a>. Documentation
///    about the OpenGL Shader Language (GLSL) is also available from www.opengl.org.</p>
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_ProgramObject
{
/// \name Construction
/// \{

public:
	/// \brief Construct OpenGL program object.
	///
	/// \param resource_id Name of program object resource.
	/// \param vdir Virtual directory holding the file to load.
	CL_ProgramObject();

	CL_ProgramObject(CL_GraphicContext &gc);

	CL_ProgramObject(CL_GraphicContextProvider *gc_provider);

	static CL_ProgramObject load(CL_GraphicContext &gc, const CL_StringRef &resource_id, CL_ResourceManager *resman);

	static CL_ProgramObject load(CL_GraphicContext &gc, const CL_StringRef &vertex_filename, const CL_StringRef &fragment_filename) {CL_VirtualDirectory directory; return CL_ProgramObject::load(gc, vertex_filename, fragment_filename, directory);}

	static CL_ProgramObject load(CL_GraphicContext &gc, const CL_StringRef &vertex_filename, const CL_StringRef &fragment_filename, CL_VirtualDirectory &directory);

	static CL_ProgramObject load(CL_GraphicContext &gc, const CL_StringRef &fragment_filename) {CL_VirtualDirectory directory; return CL_ProgramObject::load(gc, fragment_filename, directory);}

	static CL_ProgramObject load(CL_GraphicContext &gc, const CL_StringRef &fragment_filename, CL_VirtualDirectory &directory);

	static CL_ProgramObject load_and_link(CL_GraphicContext &gc, const CL_StringRef &vertex_filename, const CL_StringRef &fragment_filename) {CL_VirtualDirectory directory; return CL_ProgramObject::load_and_link(gc, vertex_filename, fragment_filename, directory);}

	static CL_ProgramObject load_and_link(CL_GraphicContext &gc, const CL_StringRef &vertex_filename, const CL_StringRef &fragment_filename, CL_VirtualDirectory &directory);

	static CL_ProgramObject load_and_link(CL_GraphicContext &gc, const CL_StringRef &fragment_filename) {CL_VirtualDirectory directory; return CL_ProgramObject::load_and_link(gc, fragment_filename, directory);}

	static CL_ProgramObject load_and_link(CL_GraphicContext &gc, const CL_StringRef &fragment_filename, CL_VirtualDirectory &directory);

	virtual ~CL_ProgramObject();


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns true if the Program object is a dummy
	bool is_null() const;

	/// \brief Returns the OpenGL program object handle.
	unsigned int get_handle() const;

	/// \brief Returns the shaders attached to the program object.
	std::vector<CL_ShaderObject> get_shaders() const;

	/// \brief Returns the current info log for the program object.
	CL_String get_info_log() const;

	/// \brief Returns the count of active attributes.
	int get_attribute_count() const;

	/// \brief Returns the active attributes.
	std::vector<CL_ProgramAttribute> get_attributes() const;

	/// \brief Returns the location of a named active attribute.
	int get_attribute_location(const CL_StringRef &name) const;

	/// \brief Returns the count of active uniform variables.
	int get_uniform_count() const;

	/// \brief Returns the active uniforms.
	std::vector<CL_ProgramUniform> get_uniforms() const;

	/// \brief Returns the location of a named uniform variable.
	int get_uniform_location(const CL_StringRef &name) const;


/// \}
/// \name Operations
/// \{

public:
	/// \brief Add shader to program object.
	void attach(const CL_ShaderObject &obj);

	/// \brief Remove shader from program object.
	void detach(const CL_ShaderObject &obj);

	/// \brief Bind attribute to specific location.
	/** <p>This function must be called before linking.</p>*/
	void bind_attribute_location(int index, const CL_StringRef &name);

	/// \brief Link program.
	/** <p>If the linking fails, get_info_log() will return the link log.</p>*/
	bool link();

	/// \brief Validate program.
	/** <p>If the validation fails, get_info_log() will return the validation log.</p>*/
	bool validate();

	/// \brief Set uniform variable(s).
	void set_uniform1i(const CL_StringRef &name, int);
	void set_uniform2i(const CL_StringRef &name, int, int);
	void set_uniform3i(const CL_StringRef &name, int, int, int);
	void set_uniform4i(const CL_StringRef &name, int, int, int, int);
	void set_uniformiv(const CL_StringRef &name, int size, int count, int *data);
	void set_uniform2i(const CL_StringRef &name, CL_Vec2i vec) {set_uniform2i(name, vec.x, vec.y);}
	void set_uniform3i(const CL_StringRef &name, CL_Vec3i vec) {set_uniform3i(name, vec.x, vec.y, vec.z);}
	void set_uniform4i(const CL_StringRef &name, CL_Vec4i vec) {set_uniform4i(name, vec.x, vec.y, vec.z, vec.w);}
	void set_uniform2s(const CL_StringRef &name, CL_Vec2s vec) {set_uniform2i(name, vec.x, vec.y);}
	void set_uniform3s(const CL_StringRef &name, CL_Vec3s vec) {set_uniform3i(name, vec.x, vec.y, vec.z);}
	void set_uniform4s(const CL_StringRef &name, CL_Vec4s vec) {set_uniform4i(name, vec.x, vec.y, vec.z, vec.w);}
	void set_uniform2b(const CL_StringRef &name, CL_Vec2b vec) {set_uniform2i(name, vec.x, vec.y);}
	void set_uniform3b(const CL_StringRef &name, CL_Vec3b vec) {set_uniform3i(name, vec.x, vec.y, vec.z);}
	void set_uniform4b(const CL_StringRef &name, CL_Vec4b vec) {set_uniform4i(name, vec.x, vec.y, vec.z, vec.w);}
	void set_uniformiv(const CL_StringRef &name, int count, CL_Vec2i *data) {set_uniformiv(name, 2, count, *data);}
	void set_uniformiv(const CL_StringRef &name, int count, CL_Vec3i *data) {set_uniformiv(name, 3, count, *data);}
	void set_uniformiv(const CL_StringRef &name, int count, CL_Vec4i *data) {set_uniformiv(name, 4, count, *data);}

	void set_uniform1f(const CL_StringRef &name, float);
	void set_uniform2f(const CL_StringRef &name, float, float);
	void set_uniform3f(const CL_StringRef &name, float, float, float);
	void set_uniform4f(const CL_StringRef &name, float, float, float, float);
	void set_uniformfv(const CL_StringRef &name, int size, int count, float *data);
	void set_uniform2f(const CL_StringRef &name, CL_Vec2f vec) {set_uniform2f(name, vec.x, vec.y);}
	void set_uniform3f(const CL_StringRef &name, CL_Vec3f vec) {set_uniform3f(name, vec.x, vec.y, vec.z);}
	void set_uniform4f(const CL_StringRef &name, CL_Vec4f vec) {set_uniform4f(name, vec.x, vec.y, vec.z, vec.w);}
	void set_uniformfv(const CL_StringRef &name, int count, CL_Vec2f *data) {set_uniformfv(name, 2, count, *data);}
	void set_uniformfv(const CL_StringRef &name, int count, CL_Vec3f *data) {set_uniformfv(name, 3, count, *data);}
	void set_uniformfv(const CL_StringRef &name, int count, CL_Vec4f *data) {set_uniformfv(name, 4, count, *data);}

	/// \brief Set uniform matrices.
	void set_uniform_matrix(const CL_StringRef &name, int size, int count, bool transpose, float *data);
	void set_uniform_matrix(const CL_StringRef &name, CL_Mat2f matrix) {set_uniform_matrix(name, 2, 1, false, matrix.matrix);}
	void set_uniform_matrix(const CL_StringRef &name, CL_Mat3f matrix) {set_uniform_matrix(name, 3, 1, false, matrix.matrix);}
	void set_uniform_matrix(const CL_StringRef &name, CL_Mat4f matrix) {set_uniform_matrix(name, 4, 1, false, matrix.matrix);}
	void set_uniform_matrix(const CL_StringRef &name, int count, CL_Mat2f *matrix) {set_uniform_matrix(name, 2, count, false, matrix->matrix);}
	void set_uniform_matrix(const CL_StringRef &name, int count, CL_Mat3f *matrix) {set_uniform_matrix(name, 3, count, false, matrix->matrix);}
	void set_uniform_matrix(const CL_StringRef &name, int count, CL_Mat4f *matrix) {set_uniform_matrix(name, 4, count, false, matrix->matrix);}


/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_ProgramObject_Impl> impl;
/// \}
};



/// \}
