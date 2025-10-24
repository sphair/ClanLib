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

//: <p>ClanLib OpenGL support. Contains OpenGL binding code and 3D helper
//: frameworks.</p>
//! Global=GL

#ifndef header_gl
#define header_gl

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _MSC_VER
	#pragma warning (disable:4786)
#endif

#ifdef WIN32
	#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "GL/setupgl.h"
#include "GL/opengl.h"
#include "GL/opengl_window.h"
#include "GL/opengl_window_description.h"
#include "GL/opengl_surface.h"
#include "GL/opengl_state.h"
#include "GL/opengl_state_data.h"
#include "GL/opengl_wrap.h"
#include "GL/viewpoint.h"
#include "GL/light_source.h"
#include "GL/shader_object.h"
#include "GL/program_object.h"
#include "GL/program_uniform.h"
#include "GL/program_attribute.h"
#include "GL/texture.h"

#if defined (_MSC_VER)
#if !defined (_DEBUG)
#pragma comment(lib, "clanGL-static-mt.lib")
#else
#pragma comment(lib, "clanGL-static-mt-debug.lib")
#endif
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLU32.lib")
#endif

#endif
