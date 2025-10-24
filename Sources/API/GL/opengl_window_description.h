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

#ifndef header_opengl_window_description
#define header_opengl_window_description

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

#include "../Display/display_window_description.h"

class CL_OpenGLWindowDescription_Generic;

//: OpenGL specific display window description.
//- !group=GL/Display 2D!
//- !header=gl.h!
//- <p>Use this class to setup OpenGL specific settings for a top level
//- window:</p>
//- <pre>
//- CL_OpenGLWindowDescription desc;
//- dest.set_title("OpenGL window with stereo visual");
//- desc.set_stereo(true);
//- desc.set_stencil_size(4);
//- CL_DisplayWindow window(desc);
//- </pre>
class CL_API_GL CL_OpenGLWindowDescription : public CL_DisplayWindowDescription
{
//! Construction:
public:
	//: Constructs a window description with default values.
	CL_OpenGLWindowDescription();

	virtual ~CL_OpenGLWindowDescription();

//! Attributes:
public:
	//: Returns true if only TrueColor and DirectColor visuals are considered.
	//- <p>If not enabled, only PseudoColor and StaticColor visuals are considered.</p>
	bool get_rgba() const;

	//: Returns true if only double-buffered visuals are considered.
	//- <p>If not enabled, only single-buffered visuals are considered.</p>
	bool get_doublebuffer() const;

	//: Returns true if only stereo visuals are considered.
	//- <p>If not enabled, only monoscopic visuals are considered.</p>
	bool get_stereo() const;

	//: Returns the desired color index buffer size.
	//- <p>Ignored in GLX (X11) if the RGBA flag is set.</p>
	//- <p>In WGL (Windows) it specifies the size of the color buffer, excluding the alpha bitplanes.</p>
	int get_buffer_size() const;

	//: Returns the required buffer level.
	//- <p>Buffer level zero corresponds to the main frame buffer of the display. Buffer
	//- level one is the first overlay frame buffer, level two the second overlay frame
	//- buffer, and so on. Negative buffer levels correspond to underlay frame buffers.</p>
	int get_level() const;

	//: Returns the desired number of auxilary buffers.
	int get_aux_buffers() const;

	//: Returns the minimum required red buffer.
	int get_red_size() const;

	//: Returns the minimum required green buffer.
	int get_green_size() const;

	//: Returns the minimum required blue buffer.
	int get_blue_size() const;

	//: Returns the minimum required alpha buffer.
	int get_alpha_size() const;

	//: Returns the minimum required depth buffer.
	int get_depth_size() const;

	//: Returns the minimum required stencil buffer.
	int get_stencil_size() const;

	//: Returns the minimum required red accumulation buffer.
	int get_accum_red_size() const;

	//: Returns the minimum required green accumulation buffer.
	int get_accum_green_size() const;

	//: Returns the minimum required blue accumulation buffer.
	int get_accum_blue_size() const;

	//: Returns the minimum required alpha accumulation buffer.
	int get_accum_alpha_size() const;

//! Operations:
public:
	//: Sets if only TrueColor and DirectColor visuals are considered.
	//- <p>If not enabled, only PseudoColor and StaticColor visuals are considered.</p>
	void set_rgba(bool value);

	//: Sets if only double-buffered visuals are considered.
	//- <p>If not enabled, only single-buffered visuals are considered.</p>
	void set_doublebuffer(bool value);

	//: Sets if only stereo visuals are to be considered.
	//- <p>If not enabled, only monoscopic visuals are considered.</p>
	void set_stereo(bool value);

	//: Sets the desired color index buffer size.
	//- <p>Ignored in GLX (X11) if the RGBA flag is set.</p>
	//- <p>In WGL (Windows) it specifies the size of the color buffer, excluding the alpha bitplanes.</p>
	void set_buffer_size(int value);

	//: Sets the required buffer level.
	//- <p>Buffer level zero corresponds to the main frame buffer of the display. Buffer
	//- level one is the first overlay frame buffer, level two the second overlay frame
	//- buffer, and so on. Negative buffer levels correspond to underlay frame buffers.</p>
	void set_level(int value);

	//: Sets the desired number of auxilary buffers.
	//- <p>Visuals with the smallest number of auxiliary buffers that meets or exceeds
	//- the specified number are preferred.</p>
	void set_aux_buffers(int value);

	//: Sets the minimum required red buffer.
	//- <p>If this value is zero, the smallest available red buffer is preferred. Otherwise,
	//- the largest available red buffer of at least the minimum size is preferred.</p>
	void set_red_size(int value);

	//: Sets the minimum required green buffer.
	//- <p>If this value is zero, the smallest available green buffer is preferred. Otherwise,
	//- the largest available green buffer of at least the minimum size is preferred.</p>
	void set_green_size(int value);

	//: Sets the minimum required blue buffer.
	//- <p>If this value is zero, the smallest available blue buffer is preferred. Otherwise,
	//- the largest available blue buffer of at least the minimum size is preferred.</p>
	void set_blue_size(int value);

	//: Sets the minimum required alpha buffer.
	//- <p>If this value is zero, the smallest available alpha buffer is preferred. Otherwise,
	//- the largest available alpha buffer of at least the minimum size is preferred.</p>
	void set_alpha_size(int value);

	//: Sets the minimum required depth buffer.
	//- <p>If this value is zero, the smallest available depth buffer is preferred. Otherwise,
	//- the largest available depth buffer of at least the minimum size is preferred.</p>
	void set_depth_size(int value);

	//: Sets the minimum required stencil buffer.
	//- <p>If this value is zero, the smallest available stencil buffer is preferred. Otherwise,
	//- the largest available stencil buffer of at least the minimum size is preferred.</p>
	void set_stencil_size(int value);

	//: Sets the minimum required red accumulation buffer.
	//- <p>If this value is zero, the smallest available red accumulation buffer is preferred. Otherwise,
	//- the largest available red accumulation buffer of at least the minimum size is preferred.</p>
	void set_accum_red_size(int value);

	//: Sets the minimum required green accumulation buffer.
	//- <p>If this value is zero, the smallest available green accumulation buffer is preferred. Otherwise,
	//- the largest available green accumulation buffer of at least the minimum size is preferred.</p>
	void set_accum_green_size(int value);

	//: Sets the minimum required blue accumulation buffer.
	//- <p>If this value is zero, the smallest available blue accumulation buffer is preferred. Otherwise,
	//- the largest available blue accumulation buffer of at least the minimum size is preferred.</p>
	void set_accum_blue_size(int value);

	//: Sets the minimum required alpha accumulation buffer.
	//- <p>If this value is zero, the smallest available alpha accumulation buffer is preferred. Otherwise,
	//- the largest available alpha accumulation buffer of at least the minimum size is preferred.</p>
	void set_accum_alpha_size(int value);

//! Implementation:
private:
};

#endif
