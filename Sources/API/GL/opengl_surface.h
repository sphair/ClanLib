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

//! clanGL="Display 2D"
//! header=gl.h

#ifndef header_opengl_surface
#define header_opengl_surface

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

#include "../Display/surface.h"

//: OpenGL surface.
//- !group=GL/Display 2D!
//- !header=gl.h!
class CL_API_GL CL_OpenGLSurface : public CL_Surface
{
//! Construction:
public:
	//: Constructs an opengl surface.
	CL_OpenGLSurface();
	
	CL_OpenGLSurface(const CL_Surface &copy);
	
	CL_OpenGLSurface(
		CL_PixelBuffer provider,
		int flag = flag_none);

	CL_OpenGLSurface(
		const std::string &resource_id,
		CL_ResourceManager *manager);

	CL_OpenGLSurface(const std::string &filename, int flag = flag_none);

	virtual ~CL_OpenGLSurface();

//! Attributes:
public:

//! Operations:
public:
	//: Bind surface texture in OpenGL.
	void bind();

//! Implementation:
private:
};

#endif
