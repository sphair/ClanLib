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
**    Magnus Norddahl
**    Harry Storbacka
**    Kenneth Gangstoe
*/

/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

#pragma once

#include "../../Core/System/sharedptr.h"
#include "../../Core/IOData/virtual_directory.h"
#include "graphic_context.h"
#include "../api_display.h"
#include <vector>

class CL_GraphicContextProvider;
class CL_ResourceManager;
class CL_ShaderObject_Impl;

/// \brief Shader Type
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
enum CL_ShaderType
{
	cl_shadertype_vertex,
	cl_shadertype_geometry,
	cl_shadertype_fragment
};

/// \brief Shader Object
///
///    <p>The source code that makes up a program that gets executed by one of
///    the programmable stages is encapsulated in one or more shader
///    objects. Shader objects are attached to a program objects to form a
///    programmable setup. CL_ShaderObject is ClanLib's C++ interface to OpenGL
///    shader objects.</p> 
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_ShaderObject
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a null instance
	CL_ShaderObject();

	/// \brief Constructs an OpenGL shader.
	///
	/// \param type Shader type. Can be cl_shadertype_vertex or cl_shadertype_fragment.
	/// \param source Shader source code, in OpenGL Shader Language (GLSL).
	/// \param gc Graphics context in which to create the shader object
	CL_ShaderObject(CL_GraphicContext &gc, CL_ShaderType type, const CL_StringRef &source);

	/// \brief Constructs a ShaderObject
	///
	/// \param gc = Graphic Context
	/// \param type = Shader Type
	/// \param sources = sources
	CL_ShaderObject(CL_GraphicContext &gc, CL_ShaderType type, const std::vector<CL_StringRef> &sources);

	/// \brief Constructs a ShaderObject
	///
	/// \param gc_provider = Graphic Context Provider
	/// \param type = Shader Type
	/// \param source = String Ref
	CL_ShaderObject(CL_GraphicContextProvider *gc_provider, CL_ShaderType type, const CL_StringRef &source);

	/// \brief Constructs a ShaderObject
	///
	/// \param gc_provider = Graphic Context Provider
	/// \param type = Shader Type
	/// \param sources = sources
	CL_ShaderObject(CL_GraphicContextProvider *gc_provider, CL_ShaderType type, const std::vector<CL_StringRef> &sources);

	/// \brief Load
	///
	/// \param gc = Graphic Context
	/// \param resource_id = String Ref
	/// \param resources = Resource Manager
	///
	/// \return Shader Object
	static CL_ShaderObject load(CL_GraphicContext &gc, const CL_StringRef &resource_id, CL_ResourceManager *resources);

	/// \brief Load
	///
	/// \param gc = Graphic Context
	/// \param type = Shader Type
	/// \param filename = String Ref
	/// \param directory = Virtual Directory
	///
	/// \return Shader Object
	static CL_ShaderObject load(CL_GraphicContext &gc, CL_ShaderType type, const CL_StringRef &filename, const CL_VirtualDirectory &directory);

	/// \brief Load
	///
	/// \param gc = Graphic Context
	/// \param type = Shader Type
	/// \param fullname = String Ref
	///
	/// \return Shader Object
	static CL_ShaderObject load(CL_GraphicContext &gc, CL_ShaderType type, const CL_StringRef &fullname);

	/// \brief Load
	///
	/// \param gc = Graphic Context
	/// \param type = Shader Type
	/// \param file = IODevice
	///
	/// \return Shader Object
	static CL_ShaderObject load(CL_GraphicContext &gc, CL_ShaderType type, CL_IODevice &file);

	/// \brief Load and compile
	///
	/// \param gc = Graphic Context
	/// \param type = Shader Type
	/// \param filename = String Ref
	/// \param directory = Virtual Directory
	///
	/// \return Shader Object
	static CL_ShaderObject load_and_compile(CL_GraphicContext &gc, CL_ShaderType type, const CL_StringRef &filename, const CL_VirtualDirectory &directory);

	/// \brief Load and compile
	///
	/// \param gc = Graphic Context
	/// \param type = Shader Type
	/// \param filename = String Ref
	///
	/// \return Shader Object
	static CL_ShaderObject load_and_compile(CL_GraphicContext &gc, CL_ShaderType type, const CL_StringRef &filename);

	/// \brief Load and compile
	///
	/// \param gc = Graphic Context
	/// \param type = Shader Type
	/// \param file = IODevice
	///
	/// \return Shader Object
	static CL_ShaderObject load_and_compile(CL_GraphicContext &gc, CL_ShaderType type, CL_IODevice &file);

	virtual ~CL_ShaderObject();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the OpenGL shader handle.
	unsigned int get_handle() const;

	/// \brief Gets the shader type.
	CL_ShaderType get_shader_type() const;

	/// \brief Get shader object's info log.
	CL_String get_info_log() const;

	/// \brief Get shader source code.
	CL_String get_shader_source() const;

	/// \brief Returns true if this object is invalid.
	bool is_null() const { return impl.is_null(); }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Handle comparision operator.
	bool operator==(const CL_ShaderObject &other) const;

	/// \brief Compile program.
	/** <p>If the compiling fails, get_info_log() will return the compile log.</p>*/
	bool compile();

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_ShaderObject_Impl> impl;
/// \}
};

/// \}
