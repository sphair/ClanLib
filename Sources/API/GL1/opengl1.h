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
*/

/// \addtogroup clanGL1_System clanGL1 System
/// \{

#pragma once

#include "api_gl1.h"
#include "opengl1_defines.h"
#include <utility>
#include "../Core/Text/string_types.h"
#include "../Display/Image/pixel_format.h"
#include "../Display/Render/texture.h"

class CL_GL1Functions;
class CL_GraphicContext;
class CL_GL1GraphicContextProvider;

enum CL_TextureFormat;

/// \brief Extension procedure address typedef for OpenGL.
typedef void (CL_GL1ProcAddress)();

/// \brief OpenGL utility class.
///
/// \xmlonly !group=GL1/System! !header=gl1.h! \endxmlonly
class CL_API_GL1 CL_GL1
{
/// \name Attributes
/// \{

public:
	/// \brief Get OpenGL extension specific function address.
	static CL_GL1ProcAddress *get_proc_address(const CL_String8 &function_name);

	/// \brief Function table for OpenGL 2.0.
	static CL_GL1Functions *functions;

	/// \brief Get a pixel format out of an OpenGL format.
	/** <p>The function returns false if it can't convert format, otherwise the
	    the formatare returned with values in pf.</p>*/
	static bool from_opengl_pixelformat(CLenum format, CLenum type, CL_PixelFormat &pf);

	/// \brief Get a OpenGL format out of a pixel format.
	/** <p>The function returns false if pixelformat color depth is not convertible to
	    OpenGL pixel format, otherwise the format and type are returned with values in format and type.</p>*/
	static bool to_opengl_pixelformat(const CL_PixelFormat &pf, CLenum &format, CLenum &type);

	/// \brief To opengl textureformat
	///
	/// \param format = Texture Format
	/// \param gl_internal_format = CLint
	/// \param gl_pixel_format = CLenum
	static void to_opengl_textureformat(CL_TextureFormat format, CLint &gl_internal_format, CLenum &gl_pixel_format);

	/// \brief Sets the thread's OpenGL context to the one used by the graphic context.
	static void set_active(CL_GraphicContext &gc);

	/// \brief Sets the thread's OpenGL context to the one used by the graphic context.
	static void set_active(const CL_GL1GraphicContextProvider * const gc_provider);
/// \}
};

/// \}
