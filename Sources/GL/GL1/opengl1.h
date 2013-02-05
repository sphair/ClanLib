/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
**    Mark Page
*/

/// \addtogroup clanLegacyGL_System clanLegacyGL System
/// \{

#pragma once

#include "API/LegacyGL/api_legacy_gl.h"
#include "opengl1_defines.h"
#include <utility>
#include "API/Display/Render/texture.h"
#include "API/Core/System/thread_local_storage.h"

namespace clan
{

class GL1Functions;
class GraphicContext;
class GL1GraphicContextProvider;

enum TextureFormat;

/// \brief Extension procedure address typedef for OpenGL.
typedef void (GL1ProcAddress)();

/// \brief OpenGL utility class.
///
/// \xmlonly !group=GL1/System! !header=gl1.h! \endxmlonly
class CL_API_LegacyGL GL1
{
/// \name Attributes
/// \{

public:
	/// \brief Get OpenGL extension specific function address.
	static GL1ProcAddress *get_proc_address(const std::string &function_name);

	/// \brief Function table for OpenGL 2.0.
	static cl_tls_variable GL1Functions *functions;

	/// \brief Get a OpenGL format out of a pixel format.
	/** <p>The function returns false if pixelformat color depth is not convertible to
	    OpenGL pixel format, otherwise the format and type are returned with values in format and type.</p>*/
	static bool to_opengl_pixelformat(const PixelBuffer &pbuffer, GLenum &format, GLenum &type);

		/// \brief Get a OpenGL format out of a pixel format.
	/** <p>The function returns false if pixelformat color depth is not convertible to
	    OpenGL pixel format, otherwise the format and type are returned with values in format and type.</p>*/
	static bool to_opengl_pixelformat(TextureFormat texture_format, GLenum &format, GLenum &type);

	/// \brief To opengl textureformat
	///
	/// \param format = Texture Format
	/// \param gl_internal_format = GLint
	/// \param gl_pixel_format = GLenum
	static void to_opengl_textureformat(TextureFormat format, GLint &gl_internal_format, GLenum &gl_pixel_format);

	/// \brief Sets the thread's OpenGL context to the one used by the graphic context.
	static void set_active(GraphicContext &gc);

	/// \brief Sets the thread's OpenGL context to the one used by the graphic context.
	static void set_active(const GL1GraphicContextProvider * const gc_provider);

	/// \brief Sets the thread's OpenGL context to the first valid allocated one
	///
	/// If a valid OpenGL context is not found, then "no render context" is set.
	///
	/// \return true = Success. false = No OpenGL contexts could be found
	static bool set_active();

/// \}
private:
	/// \brief Remove the opengl bindings from the thread's OpenGL context
	/// 
	/// This should only be called by the GL1GraphicContextProvider destructor
	static void remove_active(const GL1GraphicContextProvider * const gc_provider);

	friend class GL1GraphicContextProvider;

};

}

/// \}
