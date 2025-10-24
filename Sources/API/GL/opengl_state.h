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

#ifndef header_opengl_state
#define header_opengl_state

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

#include "../Core/System/sharedptr.h"

class CL_GraphicContext;
class CL_GraphicContext_OpenGL;
class CL_OpenGLStateData;
class CL_OpenGLState_Generic;
class CL_GLStateChecker;

//: OpenGL state setup handler class.
//- !group=GL/System!
//- !header=gl.h!
class CL_API_GL CL_OpenGLState
{
//! Construction:
public:
	//: Constructs an opengl state set.
	CL_OpenGLState();

	CL_OpenGLState(CL_GraphicContext *gc);

	CL_OpenGLState(const CL_OpenGLState &copy);

	~CL_OpenGLState();

//! Attributes:
public:
	//: Returns true if opengl state is currently active.
	bool is_active() const;

//! Operations:
public:
	//: Activates this OpenGL state set.
	void set_active();

	//: Attaches a new data object responsible for saving and restoring states.
	void attach_data(CL_OpenGLStateData *data, bool delete_data = false);

	//: Detaches a data object.
	void detach_data(CL_OpenGLStateData *data);

	//: Setup window 2D rendering mode.
	//param upside_down: If set to true, (0,0) will be in the lower left corner.
	//- <p> This effectively loads a Orto 2D projection matrix and makes sure
	//- the device are using sane texture parameter values.</p>
	void setup_2d(bool upside_down = false);

//! Implementation:
private:
	CL_OpenGLState(CL_GraphicContext_OpenGL *gc_opengl);

	CL_SharedPtr<CL_OpenGLState_Generic> impl;

	friend class CL_GLStateChecker;
};

#endif
