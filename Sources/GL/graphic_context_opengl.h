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

#ifndef header_graphic_context_opengl
#define header_graphic_context_opengl

#if _MSC_VER > 1000
#pragma once
#endif

#include "Display/graphic_context_generic.h"
#include "API/signals.h"
#ifndef WIN32
#ifdef __APPLE__
#include <AGL/agl.h>
#else
#include <GL/glx.h>
#endif
#endif
#include "API/GL/opengl.h"

class CL_DisplayWindow_Generic;
class CL_Canvas_OpenGL;
class CL_GLStateChecker;
class CL_Surface_Target;

//: OpenGL graphic context
class CL_GraphicContext_OpenGL : public CL_GraphicContext_Generic
{
//! Construction:
public:
	CL_GraphicContext_OpenGL(CL_DisplayWindow_Generic *window);

	CL_GraphicContext_OpenGL(CL_Canvas_OpenGL *canvas_target);

	virtual ~CL_GraphicContext_OpenGL();

//! Attributes:
public:
	//: Returns the current width of the context.
	virtual int get_width() const;

	//: Returns the current height of the context.
	virtual int get_height() const;

	//: Returns a pixel value at specified coordinates.
	virtual CL_Color get_pixel(int x, int y) const;

	virtual CL_PixelBuffer get_pixeldata(const CL_Rect& rect = CL_Rect(0,0,0,0), int i = 0) const;

	bool is_canvas_target() const { return (canvas_target != 0); }

#ifdef WIN32
	HDC get_drawable();

	HGLRC get_context();
#else
#ifdef __APPLE__
	//AGLDrawable get_drawable();
	
	AGLContext get_context();
#else
	Display *get_display();

	GLXDrawable get_drawable();

	GLXContext get_context();
#endif
#endif

	//: Returns the maximum size of a surface this displaywindow supports.
	virtual CL_Size get_max_surface_size() const;

	//: Batched OpenGL 2D rendering.
	CL_GLStateChecker *state_checker;

	//: Signal emitted when OpenGL scissor needs to be updated.
	CL_Signal_v0 sig_update_scissor;

	//: Signal emitted when OpenGL modelview needs to be updated.
	CL_Signal_v0 sig_update_modelview;

	//: Signal emitted when OpenGL viewport needs to be updated.
	CL_Signal_v0 sig_update_viewport;

//! Operations:
public:
	//: Draw a pixel at (x, y) using the specified color.
	virtual void draw_pixel(int x, int y, const CL_Color &color);

	//: Draw a line from (x1, y1) to (x2, y2) using the specified color.
	virtual void draw_line(float x1, float y1, float x2, float y2, const CL_Color &color);

	//: Draw a chain of lines using the specified color.
	virtual void draw_lines(int count, double *vertices, const CL_Color &color);

	//: Draw a rectangle using the specified color.
	virtual void draw_rect(const CL_Rectf &rect, const CL_Color &color);

	//: Draw a gradient rectangle using the specified gradient.
	virtual void fill_rect(const CL_Rectf &rect, const CL_Gradient &gradient);

	//: Draw a filled rectangle using the specified color.
	virtual void fill_rect(const CL_Rectf &rect, const CL_Color &color);
	
	//: Draw a quad using the specified color.
	virtual void draw_quad(const CL_Quad &quad, const CL_Color &color);

	//: Draw a gradient quad using the specified gradient.
	virtual void fill_quad(const CL_Quad &quad, const CL_Gradient &gradient);

	//: Draw a filled quad using the specified color.
	virtual void fill_quad(const CL_Quad &quad, const CL_Color &color);

	//: Draw triangles (optionally colored and uv-mapped).
	virtual void draw_triangles( CL_TriangleParams *params );

	//: Clears the whole context using the specified color.
	virtual void clear(const CL_Color &color);

	//: Set the current clipping rectangle.
	virtual void set_cliprect(const CL_Rect &rect);

	//: Push current clipping rectangle to stack; then set clipping rectangle to the
	//: union of the current rectangle and the passed rectangle.
	virtual void push_cliprect(const CL_Rect &rect);

	//: Push current clipping rectangle to the stack.
	virtual void push_cliprect();

	//: Pop current clipping rectangle from the stack.
	virtual void pop_cliprect();

	//: Called every time the active modelview have changed.
	virtual void modelview_changed();

	//: Clears the stencil buffer contents.
	//param int value: stencil buffer contents will be cleared to this value. Default: 0
	virtual void clear_stencil(int value);

	//: Set stencil test function when writing to stencil, and operations to do when a test passes or fails
	//param CL_StencilOp pass : Modification to stencil buffer when a test passes.
	//param CL_StencilOp fail : Modification to stencil buffer when a test fails.
	//param CL_StencilFunc func : Type of test to do when writing to stencil.
	//param int ref : value compared to stencil contents. Default: 1
	virtual void set_stencil_operation(CL_StencilOp pass, CL_StencilOp fail, CL_StencilFunc func, int ref);

	//: Enables writing to the stencil buffer 
	//param bool enabled : Enables/disables stencil testing.
	//param bool visible : Enables/disables color buffer writing if stencil writing is enabled. Default: true.
	//param float alpha_limit : Only pixels with alpha >= alpha limit are drawn to the stencil buffer.
	virtual void enable_stencil_write(bool enabled, bool visible, float alpha_limit);

	//: Enables stencil buffer testing
	//- <p> Individual pixels are tested against the values in the stencil buffer, and
	//- drawn depending on the comparison result. This can be used for masking and
	//- boolean operations on bitmaps </p>
	//param bool enabled : Enables/disables stencil testing.
	//param int func : Type of comparison. Default: lequal (pass if ref <= stencil)
	//param int ref : Reference value that the stencil is compared with. Default: 1
	virtual void enable_stencil_test(bool enabled, CL_StencilFunc func, int ref);

	void sync_gc();

//! Implementation:
private:
	//: Called when window is resized.
	void on_resize(int width, int height);

	//: OpenGL window.
	//- Can be null if attached to a surface instead.
	CL_DisplayWindow_Generic *window;

	//: OpenGL surface.
	//- Can be null if attached to a window instead.
	CL_Canvas_OpenGL *canvas_target;

	//: Maximum allowed texture size for this graphic card.
	mutable CLint max_texture_size;

	//: Slots.
	CL_SlotContainer slots;
};

#endif
