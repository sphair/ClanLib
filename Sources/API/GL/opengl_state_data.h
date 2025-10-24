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

#ifndef header_opengl_state_data
#define header_opengl_state_data

#if _MSC_VER > 1000
#pragma once
#endif

class CL_OpenGLStateData_Generic;

//: OpenGL state worker; responsible for saving and restoring states.
//- !group=GL/System!
//- !header=gl.h!
class CL_OpenGLStateData
{
//! Construction:
public:
	CL_OpenGLStateData();

	virtual ~CL_OpenGLStateData();

//! Attributes:
public:

//! Operations:
public:
	//: Save state information needed to restore this OpenGL state.
	virtual void on_save()=0;

	//: Load state information and setup OpenGL to this state.
	virtual void on_load()=0;

	//: Flush current rendering batch.
	//- <p>This is a hint from clanDisplay that it needs to perform some state changes.
	//- Usually this happens if translate, viewport or scissor needs updating.</p>
	//- <p>Internally ClanLib uses this to end any open glBegin render batches.</p>
	virtual void on_flush()=0;

//! Implementation:
private:
	CL_OpenGLStateData_Generic *impl;
};

#endif
