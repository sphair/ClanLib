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
*/

/// \addtogroup clanGL_System clanGL System
/// \{

#pragma once

#include "api_gl.h"
#include "opengl_defines.h"
#include <utility>
#include "../Core/Text/string_types.h"
#include "../Display/Image/pixel_format.h"
#include "../Display/Render/texture.h"

class CL_GLFunctions;
class CL_GraphicContext;
class CL_OpenGLGraphicContextProvider;

enum CL_TextureFormat;

/// \brief Extension procedure address typedef for OpenGL.
typedef void (CL_ProcAddress)();

/// \brief OpenGL utility class.
///
/// \xmlonly !group=GL/System! !header=gl.h! \endxmlonly
class CL_API_GL CL_OpenGL
{
/// \name Attributes
/// \{

public:
	/// \brief Get OpenGL extension specific function address.
	static CL_ProcAddress *get_proc_address(const CL_String8 &function_name);

	/// \brief Function table for OpenGL 2.0.
	static CL_GLFunctions *functions;

	/// \brief Returns the number of bits in a texture format
	static int get_textureformat_bits(CL_TextureFormat format);

	/// \brief Get a OpenGL format out of a pixel format.
	/** <p>The function returns false if pixelformat color depth is not convertible to
	    OpenGL pixel format, otherwise the format and type are returned with values in format and type.</p>*/
	static bool to_opengl_pixelformat(CL_TextureFormat texture_format, GLenum &format, GLenum &type);

	/// \brief To opengl textureformat
	///
	/// \param format = Texture Format
	/// \param gl_internal_format = GLint
	/// \param gl_pixel_format = GLenum
	static void to_opengl_textureformat(CL_TextureFormat format, GLint &gl_internal_format, GLenum &gl_pixel_format);

	/// \brief Sets the thread's OpenGL context to the one used by the graphic context.
	static void set_active(CL_GraphicContext &gc);

	/// \brief Sets the thread's OpenGL context to the one used by the graphic context.
	static void set_active(const CL_OpenGLGraphicContextProvider * const gc_provider);

	/// \brief Sets the thread's OpenGL context to the first valid allocated one
	///
	/// If a valid OpenGL context is not found, then "no render context" is set.
	///
	/// \return true = Success. false = No OpenGL contexts could be found
	static bool set_active();

	/// \brief Throw an exception if an OpenGL error was detected ( Using glGetError() )
	///
	/// Remember to call glGetError() to clear any previous errors
	static void check_error();

	/// \brief Get the opengl version major number
	static int get_opengl_version_major() {return opengl_version_major;}

	/// \brief Get the opengl version minor number
	static int get_opengl_version_minor() {return opengl_version_minor;}

	/// \brief Get the glsl version major number
	static int get_glsl_version_major() {return glsl_version_major;}

	/// \brief Get the glsl version minor number
	static int get_glsl_version_minor() {return glsl_version_minor;}

/// \}

private:
	/// \brief Remove the opengl bindings from the thread's OpenGL context
	/// 
	/// This should only be called by the CL_OpenGLGraphicContextProvider destructor
	static void remove_active(const CL_OpenGLGraphicContextProvider * const gc_provider);

	static int opengl_version_major;
	static int opengl_version_minor;
	static int glsl_version_major;
	static int glsl_version_minor;

	friend class CL_OpenGLGraphicContextProvider;

};

/// \}
