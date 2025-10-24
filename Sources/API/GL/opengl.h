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

//! clanGL="System"
//! header=gl.h

#ifndef header_opengl
#define header_opengl

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

#include <string>
#include <vector>
#include <utility>
#include "../Display/pixel_format.h"

class CL_GLFunctions;

//: Extension procedure address typedef for OpenGL.
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
typedef __int64 CLintptr;
typedef __int64 CLsizeiptr
#elif defined(__ia64__) || defined(__x86_64__)
typedef long int CLintptr;
typedef long int CLsizeiptr;
#else
typedef int CLsizeiptr;
typedef int CLintptr;
#endif
#endif

//: OpenGL utility class.
//- !group=GL/System!
//- !header=gl.h!
class CL_API_GL CL_OpenGL
{
//! Attributes:
public:
	//: Get OpenGL extension specific function address.
	static CL_ProcAddress *get_proc_address(const std::string &function_name);

	//: Function table for OpenGL 2.0.
	static CL_GLFunctions *functions;

	//: Get a pixel format out of an OpenGL format.
	//- <p>The function returns false if it can't convert format, otherwise the
	//- the formatare returned with values in pf.</p>
	static bool from_opengl_pixelformat(CLenum format, CLenum type, CL_PixelFormat &pf);

	//: Get a OpenGL format out of a pixel format.
	//- <p>The function returns false if pixelformat color depth is not convertible to 
	//- OpenGL pixel format, otherwise the format and type are returned with values in format and type.</p>
	static bool to_opengl_pixelformat(const CL_PixelFormat &pf, CLenum &format, CLenum &type);

	//: Ignore OpenGL extensions by name, causing ClanLib to not attempt to use them
	//- <p>Due to buggy OpenGL drivers it may be useful to pretend that certain extensions don't exist, causes
	//- ClanLib to work around them.  The only extension this would apply to right now is GL_EXT_abgr.  extension_exists()
	//- will always return false when an extension is specified here.</p> Note: You must call this BEFORE initting openGL.
	static void ignore_extension( const std::string &extension_name);

	//: Test to see if a certain OGL Extension is present or not.  It's slow, don't call it in a main loop.
	static bool extension_exists( const char * extension_name);

private:

	static std::vector<std::string> m_ignored_gl_extension;
};

#endif
