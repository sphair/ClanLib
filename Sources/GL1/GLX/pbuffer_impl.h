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
**    Mark Page
*/

#pragma once

#include "../pbuffer.h"
#include "API/Core/System/disposable_object.h"
#include "API/GL1/opengl1_wrap.h"
#include "API/Core/System/disposable_object.h"
#include "API/Display/TargetProviders/render_window_provider.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

//#include <X11/extensions/XInput.h>

#include <GL/glx.h>

class CL_GL1GraphicContextProvider;
class CL_GL1WindowProvider_GLX;
class CL_GL1_GLXFunctions;
class CL_PBuffer_GL1_Impl;

class CL_RenderWindowProvider_GLX_PBuffer : public CL_RenderWindowProvider
{
public:
	CL_RenderWindowProvider_GLX_PBuffer(CL_PBuffer_GL1_Impl &pbuffer_impl, Display *display, GLXPbuffer pbuffer, GLXContext glx_context, CL_Size pb_size);
	virtual ~CL_RenderWindowProvider_GLX_PBuffer();
	virtual int get_viewport_width() const;
	virtual int get_viewport_height() const;
	virtual void flip_buffers(int interval) const;
	virtual void make_current() const;
	CL_GL1ProcAddress *get_proc_address(const CL_String8& function_name) const;

private:
	CL_PBuffer_GL1_Impl &pbuffer_impl;
	Display *display;
	GLXPbuffer pbuffer;
	GLXContext glx_context;
	CL_Size size;
};

class CL_PBuffer_GL1_Impl : public CL_DisposableObject
{
/// \name Construction
/// \{

public:
	CL_PBuffer_GL1_Impl(CL_GL1GraphicContextProvider *gc_provider);

	~CL_PBuffer_GL1_Impl();


/// \}
/// \name Attributes
/// \{

public:
	CL_GL1_GLXFunctions *glx;

/// \}
/// \name Operations
/// \{

public:
	void create(CL_GL1WindowProvider_GLX &window_provider, CL_Size &size);
	void set_active() const;

/// \}
/// \name Implementation
/// \{

private:
	void on_dispose();
	void reset();

	CL_GL1GraphicContextProvider *gc_provider;

	GLXPbuffer pbuffer;
	Display *disp;
	GLXContext pbuffer_context;

	CL_GL1GraphicContextProvider *pbuffer_gc_provider;
	CL_GraphicContext pbuffer_gc;


/// \}
};

