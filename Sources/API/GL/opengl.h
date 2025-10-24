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

/// \addtogroup clanGL_System clanGL System
/// \{


#pragma once


#if _MSC_VER > 1000
#pragma once
#endif

#include "api_gl.h"
#include <utility>
#include "../Core/Text/string_types.h"
#include "../Display/Image/pixel_format.h"

class CL_GLFunctions;
class CL_GraphicContext;
class CL_OpenGLGraphicContextProvider;

/// \brief Extension procedure address typedef for OpenGL.
typedef void (CL_ProcAddress)();

#ifndef __APPLE__
typedef unsigned int CLenum;
typedef unsigned char CLboolean;
typedef unsigned int CLbitfield;
typedef signed char CLbyte;
typedef short CLshort;
typedef int CLint;
typedef int CLsizei;
typedef unsigned char CLubyte;
typedef unsigned short CLushort;
typedef unsigned int CLuint;
typedef float CLfloat;
typedef float CLclampf;
typedef double CLdouble;
typedef double CLclampd;
typedef void CLvoid;
typedef char CLchar;
typedef long int CLsizeiptr;
typedef long int CLintptr;
#else
typedef unsigned long CLenum;
typedef unsigned char CLboolean;
typedef unsigned long CLbitfield;
typedef signed char CLbyte;
typedef short CLshort;
typedef long CLint;
typedef long CLsizei;
typedef unsigned char CLubyte;
typedef unsigned short CLushort;
typedef unsigned long CLuint;
typedef float CLfloat;
typedef float CLclampf;
typedef double CLdouble;
typedef double CLclampd;
typedef void CLvoid;
typedef char CLchar;
#if defined(_WIN64)
typedef __int64 GLintptr;
typedef __int64 GLsizeiptr
#elif defined(__ia64__) || defined(__x86_64__)
typedef long int GLintptr;
typedef long int GLsizeiptr;
#else
typedef int CLsizeiptr;
typedef int CLintptr;
#endif
#endif

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

	/// \brief Get a pixel format out of an OpenGL format.
	/** <p>The function returns false if it can't convert format, otherwise the
	    the formatare returned with values in pf.</p>*/
	static bool from_opengl_pixelformat(CLenum format, CLenum type, CL_PixelFormat &pf);

	/// \brief Get a OpenGL format out of a pixel format.
	/** <p>The function returns false if pixelformat color depth is not convertible to
	    OpenGL pixel format, otherwise the format and type are returned with values in format and type.</p>*/
	static bool to_opengl_pixelformat(const CL_PixelFormat &pf, CLenum &format, CLenum &type);

	/// \brief Sets the thread's OpenGL context to the one used by the graphic context.
	static void set_active(const CL_GraphicContext &gc);

	/// \brief Sets the thread's OpenGL context to the one used by the graphic context.
	static void set_active(const CL_OpenGLGraphicContextProvider * const gc_provider);
/// \}
};


/// \}
