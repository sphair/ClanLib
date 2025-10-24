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
class CL_ProgramObjectProvider;

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
	/// \brief Construct a null instance
	CL_ProgramObject();

	/// \brief Constructs a ProgramObject
	///
	/// \param gc = Graphic Context
	CL_ProgramObject(CL_GraphicContext &gc);

	/// \brief Constructs a ProgramObject
	///
	/// \param gc_provider = Graphic Context Provider
	CL_ProgramObject(CL_GraphicContextProvider *gc_provider);

	/// \brief Constructs a ProgramObject
	///
	/// \param provider = Font Provider
	CL_ProgramObject(CL_ProgramObjectProvider *provider);

	/// \brief Load
	///
	/// \param gc = Graphic Context
	/// \param resource_id = String Ref
	/// \param resman = Resource Manager
	///
	/// \return Program Object
	static CL_ProgramObject load(CL_GraphicContext &gc, const CL_StringRef &resource_id, CL_ResourceManager *resman);

	/// \brief Load
	///
	/// \param gc = Graphic Context
	/// \param vertex_fullname = String Ref
	/// \param fragment_fullname = String Ref
	///
	/// \return Program Object
	static CL_ProgramObject load(CL_GraphicContext &gc, const CL_StringRef &vertex_fullname, const CL_StringRef &fragment_fullname);

	/// \brief Load
	///
	/// \param gc = Graphic Context
	/// \param vertex_fullname = String Ref
	/// \param geometry_fullname = String Ref
	/// \param fragment_fullname = String Ref
	///
	/// \return Program Object
	static CL_ProgramObject load(CL_GraphicContext &gc, const CL_StringRef &vertex_fullname, const CL_StringRef &geometry_fullname, const CL_StringRef &fragment_fullname);

	/// \brief Load
	///
	/// \param gc = Graphic Context
	/// \param vertex_filename = String Ref
	/// \param fragment_filename = String Ref
	/// \param directory = Virtual Directory
	///
	/// \return Program Object
	static CL_ProgramObject load(CL_GraphicContext &gc, const CL_StringRef &vertex_filename, const CL_StringRef &fragment_filename, const CL_VirtualDirectory &directory);

	/// \brief Load
	///
	/// \param gc = Graphic Context
	/// \param vertex_filename = String Ref
	/// \param geometry_filename = String Ref
	/// \param fragment_filename = String Ref
	/// \param directory = Virtual Directory
	///
	/// \return Program Object
	static CL_ProgramObject load(CL_GraphicContext &gc, const CL_StringRef &vertex_filename, const CL_StringRef &geometry_filename, const CL_StringRef &fragment_filename, const CL_VirtualDirectory &directory);

	/// \brief Load
	///
	/// \param gc = Graphic Context
	/// \param vertex_file = IODevice
	/// \param fragment_file = IODevice
	///
	/// \return Program Object
	static CL_ProgramObject load(CL_GraphicContext &gc, CL_IODevice &vertex_file, CL_IODevice &fragment_file);

	/// \brief Load
	///
	/// \param gc = Graphic Context
	/// \param vertex_file = IODevice
	/// \param geometry_file = IODevice
	/// \param fragment_file = IODevice
	///
	/// \return Program Object
	static CL_ProgramObject load(CL_GraphicContext &gc, CL_IODevice &vertex_file, CL_IODevice &geometry_file, CL_IODevice &fragment_file);

	/// \brief Load
	///
	/// \param gc = Graphic Context
	/// \param fragment_fullname = String Ref
	///
	/// \return Program Object
	static CL_ProgramObject load(CL_GraphicContext &gc, const CL_StringRef &fragment_fullname);

	/// \brief Load
	///
	/// \param gc = Graphic Context
	/// \param fragment_filename = String Ref
	/// \param directory = Virtual Directory
	///
	/// \return Program Object
	static CL_ProgramObject load(CL_GraphicContext &gc, const CL_StringRef &fragment_filename, const CL_VirtualDirectory &directory);

	/// \brief Load
	///
	/// \param gc = Graphic Context
	/// \param fragment_file = IODevice
	///
	/// \return Program Object
	static CL_ProgramObject load(CL_GraphicContext &gc, CL_IODevice &fragment_file);

	/// \brief Load and link
	///
	/// \param gc = Graphic Context
	/// \param vertex_fullname = String Ref
	/// \param fragment_fullname = String Ref
	///
	/// \return Program Object
	static CL_ProgramObject load_and_link(CL_GraphicContext &gc, const CL_StringRef &vertex_fullname, const CL_StringRef &fragment_fullname);

	/// \brief Load and link
	///
	/// \param gc = Graphic Context
	/// \param vertex_fullname = String Ref
	/// \param geometry_fullname = String Ref
	/// \param fragment_fullname = String Ref
	///
	/// \return Program Object
	static CL_ProgramObject load_and_link(CL_GraphicContext &gc, const CL_StringRef &vertex_fullname, const CL_StringRef &geometry_fullname, const CL_StringRef &fragment_fullname);

	/// \brief Load and link
	///
	/// \param gc = Graphic Context
	/// \param vertex_filename = String Ref
	/// \param fragment_filename = String Ref
	/// \param directory = Virtual Directory
	///
	/// \return Program Object
	static CL_ProgramObject load_and_link(CL_GraphicContext &gc, const CL_StringRef &vertex_filename, const CL_StringRef &fragment_filename, const CL_VirtualDirectory &directory);

	/// \brief Load and link
	///
	/// \param gc = Graphic Context
	/// \param vertex_filename = String Ref
	/// \param geometry_filename = String Ref
	/// \param fragment_filename = String Ref
	/// \param directory = Virtual Directory
	///
	/// \return Program Object
	static CL_ProgramObject load_and_link(CL_GraphicContext &gc, const CL_StringRef &vertex_filename, const CL_StringRef &geometry_filename, const CL_StringRef &fragment_filename, const CL_VirtualDirectory &directory);

	/// \brief Load and link
	///
	/// \param gc = Graphic Context
	/// \param vertex_file = IODevice
	/// \param fragment_file = IODevice
	///
	/// \return Program Object
	static CL_ProgramObject load_and_link(CL_GraphicContext &gc, CL_IODevice &vertex_file, CL_IODevice &fragment_file);

	/// \brief Load and link
	///
	/// \param gc = Graphic Context
	/// \param vertex_file = IODevice
	/// \param geometry_file = IODevice
	/// \param fragment_file = IODevice
	///
	/// \return Program Object
	static CL_ProgramObject load_and_link(CL_GraphicContext &gc, CL_IODevice &vertex_file, CL_IODevice &geometry_file, CL_IODevice &fragment_file);

	/// \brief Load and link
	///
	/// \param gc = Graphic Context
	/// \param fragment_fullname = String Ref
	///
	/// \return Program Object
	static CL_ProgramObject load_and_link(CL_GraphicContext &gc, const CL_StringRef &fragment_fullname);

	/// \brief Load and link
	///
	/// \param gc = Graphic Context
	/// \param fragment_filename = String Ref
	/// \param directory = Virtual Directory
	///
	/// \return Program Object
	static CL_ProgramObject load_and_link(CL_GraphicContext &gc, const CL_StringRef &fragment_filename, const CL_VirtualDirectory &directory);

	/// \brief Load and link
	///
	/// \param gc = Graphic Context
	/// \param fragment_file = IODevice
	///
	/// \return Program Object
	static CL_ProgramObject load_and_link(CL_GraphicContext &gc, CL_IODevice &fragment_file);

	virtual ~CL_ProgramObject();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Retrieves the provider.
	CL_ProgramObjectProvider *get_provider() const;

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
	/// \brief Equality operator
	bool operator==(const CL_ProgramObject &other) const;

	/// \brief Add shader to program object.
	void attach(const CL_ShaderObject &obj);

	/// \brief Remove shader from program object.
	void detach(const CL_ShaderObject &obj);

	/// \brief Bind attribute to specific location.
	/** <p>This function must be called before linking.</p>*/
	void bind_attribute_location(int index, const CL_StringRef &name);

	/// \brief Bind shader out variable a specific color buffer location.
	/** <p>This function must be called before linking.</p>*/
	void bind_frag_data_location(int color_number, const CL_StringRef &name);

	/// \brief Link program.
	/** <p>If the linking fails, get_info_log() will return the link log.</p>*/
	bool link();

	/// \brief Validate program.
	/** <p>If the validation fails, get_info_log() will return the validation log.</p>*/
	bool validate();

	/// \brief Set uniform variable(s).
	///
	/// \param name = String Ref
	/// \param value_a = value
	void set_uniform1i(const CL_StringRef &name, int value_a);

	/// \brief Set uniform2i
	///
	/// \param name = String Ref
	/// \param value_a = value
	/// \param value_b = value
	void set_uniform2i(const CL_StringRef &name, int value_a, int value_b);

	/// \brief Set uniform2i
	///
	/// \param name = String Ref
	/// \param value_a = value
	/// \param value_b = value
	/// \param value_c = value
	void set_uniform3i(const CL_StringRef &name, int value_a, int value_b, int value_c);

	/// \brief Set uniform4i
	///
	/// \param name = String Ref
	/// \param value_a = value
	/// \param value_b = value
	/// \param value_c = value
	/// \param value_d = value
	void set_uniform4i(const CL_StringRef &name, int value_a, int value_b, int value_c, int value_d);

	/// \brief Set uniformiv
	///
	/// \param name = String Ref
	/// \param size = value
	/// \param count = value
	/// \param data = value
	void set_uniformiv(const CL_StringRef &name, int size, int count, int *data);

	/// \brief Set uniform2i
	///
	/// \param name = String Ref
	/// \param vec = Vec2i
	void set_uniform2i(const CL_StringRef &name, CL_Vec2i vec) {set_uniform2i(name, vec.x, vec.y);}

	/// \brief Set uniform3i
	///
	/// \param name = String Ref
	/// \param vec = Vec3i
	void set_uniform3i(const CL_StringRef &name, CL_Vec3i vec) {set_uniform3i(name, vec.x, vec.y, vec.z);}

	/// \brief Set uniform4i
	///
	/// \param name = String Ref
	/// \param vec = Vec4i
	void set_uniform4i(const CL_StringRef &name, CL_Vec4i vec) {set_uniform4i(name, vec.x, vec.y, vec.z, vec.w);}

	/// \brief Set uniform2s
	///
	/// \param name = String Ref
	/// \param vec = Vec2s
	void set_uniform2s(const CL_StringRef &name, CL_Vec2s vec) {set_uniform2i(name, vec.x, vec.y);}

	/// \brief Set uniform3s
	///
	/// \param name = String Ref
	/// \param vec = Vec3s
	void set_uniform3s(const CL_StringRef &name, CL_Vec3s vec) {set_uniform3i(name, vec.x, vec.y, vec.z);}

	/// \brief Set uniform4s
	///
	/// \param name = String Ref
	/// \param vec = Vec4s
	void set_uniform4s(const CL_StringRef &name, CL_Vec4s vec) {set_uniform4i(name, vec.x, vec.y, vec.z, vec.w);}

	/// \brief Set uniform2b
	///
	/// \param name = String Ref
	/// \param vec = Vec2b
	void set_uniform2b(const CL_StringRef &name, CL_Vec2b vec) {set_uniform2i(name, vec.x, vec.y);}

	/// \brief Set uniform3b
	///
	/// \param name = String Ref
	/// \param vec = Vec3b
	void set_uniform3b(const CL_StringRef &name, CL_Vec3b vec) {set_uniform3i(name, vec.x, vec.y, vec.z);}

	/// \brief Set uniform4b
	///
	/// \param name = String Ref
	/// \param vec = Vec4b
	void set_uniform4b(const CL_StringRef &name, CL_Vec4b vec) {set_uniform4i(name, vec.x, vec.y, vec.z, vec.w);}

	/// \brief Set uniformiv
	///
	/// \param name = String Ref
	/// \param count = value
	/// \param data = Vec2i
	void set_uniformiv(const CL_StringRef &name, int count, CL_Vec2i *data) {set_uniformiv(name, 2, count, *data);}

	/// \brief Set uniformiv
	///
	/// \param name = String Ref
	/// \param count = value
	/// \param data = Vec3i
	void set_uniformiv(const CL_StringRef &name, int count, CL_Vec3i *data) {set_uniformiv(name, 3, count, *data);}

	/// \brief Set uniformiv
	///
	/// \param name = String Ref
	/// \param count = value
	/// \param data = Vec4i
	void set_uniformiv(const CL_StringRef &name, int count, CL_Vec4i *data) {set_uniformiv(name, 4, count, *data);}

	/// \brief Set uniform1f
	///
	/// \param name = String Ref
	/// \param value_a = value
	void set_uniform1f(const CL_StringRef &name, float value_a);

	/// \brief Set uniform2f
	///
	/// \param name = String Ref
	/// \param value_a = value
	/// \param value_b = value
	void set_uniform2f(const CL_StringRef &name, float value_a, float value_b);

	/// \brief Set uniform2f
	///
	/// \param name = String Ref
	/// \param value_a = value
	/// \param value_b = value
	/// \param value_c = value
	void set_uniform3f(const CL_StringRef &name, float value_a, float value_b, float value_c);

	/// \brief Set uniform4f
	///
	/// \param name = String Ref
	/// \param value_a = value
	/// \param value_b = value
	/// \param value_c = value
	/// \param value_d = value
	void set_uniform4f(const CL_StringRef &name, float value_a, float value_b, float value_c, float value_d);

	/// \brief Set uniformfv
	///
	/// \param name = String Ref
	/// \param size = value
	/// \param count = value
	/// \param data = value
	void set_uniformfv(const CL_StringRef &name, int size, int count, float *data);

	/// \brief Set uniform2f
	///
	/// \param name = String Ref
	/// \param vec = Vec2f
	void set_uniform2f(const CL_StringRef &name, CL_Vec2f vec) {set_uniform2f(name, vec.x, vec.y);}

	/// \brief Set uniform3f
	///
	/// \param name = String Ref
	/// \param vec = Vec3f
	void set_uniform3f(const CL_StringRef &name, CL_Vec3f vec) {set_uniform3f(name, vec.x, vec.y, vec.z);}

	/// \brief Set uniform4f
	///
	/// \param name = String Ref
	/// \param vec = Vec4f
	void set_uniform4f(const CL_StringRef &name, CL_Vec4f vec) {set_uniform4f(name, vec.x, vec.y, vec.z, vec.w);}

	/// \brief Set uniformfv
	///
	/// \param name = String Ref
	/// \param count = value
	/// \param data = Vec2f
	void set_uniformfv(const CL_StringRef &name, int count, CL_Vec2f *data) {set_uniformfv(name, 2, count, *data);}

	/// \brief Set uniformfv
	///
	/// \param name = String Ref
	/// \param count = value
	/// \param data = Vec3f
	void set_uniformfv(const CL_StringRef &name, int count, CL_Vec3f *data) {set_uniformfv(name, 3, count, *data);}

	/// \brief Set uniformfv
	///
	/// \param name = String Ref
	/// \param count = value
	/// \param data = Vec4f
	void set_uniformfv(const CL_StringRef &name, int count, CL_Vec4f *data) {set_uniformfv(name, 4, count, *data);}

	/// \brief Set uniform matrices.
	void set_uniform_matrix(const CL_StringRef &name, int size, int count, bool transpose, float *data);

	/// \brief Set uniform matrix
	///
	/// \param name = String Ref
	/// \param matrix = Mat2f
	void set_uniform_matrix(const CL_StringRef &name, CL_Mat2f matrix) {set_uniform_matrix(name, 2, 1, false, matrix.matrix);}

	/// \brief Set uniform matrix
	///
	/// \param name = String Ref
	/// \param matrix = Mat3f
	void set_uniform_matrix(const CL_StringRef &name, CL_Mat3f matrix) {set_uniform_matrix(name, 3, 1, false, matrix.matrix);}

	/// \brief Set uniform matrix
	///
	/// \param name = String Ref
	/// \param matrix = Mat4f
	void set_uniform_matrix(const CL_StringRef &name, CL_Mat4f matrix) {set_uniform_matrix(name, 4, 1, false, matrix.matrix);}

	/// \brief Set uniform matrix
	///
	/// \param name = String Ref
	/// \param count = value
	/// \param matrix = Mat2f
	void set_uniform_matrix(const CL_StringRef &name, int count, CL_Mat2f *matrix) {set_uniform_matrix(name, 2, count, false, matrix->matrix);}

	/// \brief Set uniform matrix
	///
	/// \param name = String Ref
	/// \param count = value
	/// \param matrix = Mat3f
	void set_uniform_matrix(const CL_StringRef &name, int count, CL_Mat3f *matrix) {set_uniform_matrix(name, 3, count, false, matrix->matrix);}

	/// \brief Set uniform matrix
	///
	/// \param name = String Ref
	/// \param count = value
	/// \param matrix = Mat4f
	void set_uniform_matrix(const CL_StringRef &name, int count, CL_Mat4f *matrix) {set_uniform_matrix(name, 4, count, false, matrix->matrix);}

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_ProgramObject_Impl> impl;
/// \}
};

/// \}
