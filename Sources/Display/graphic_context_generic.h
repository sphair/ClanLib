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

#ifndef header_graphic_context_generic
#define header_graphic_context_generic

#ifdef CL_API_DLL
#ifdef CL_DISPLAY_EXPORT
#define CL_API_DISPLAY __declspec(dllexport)
#else
#define CL_API_DISPLAY __declspec(dllimport)
#endif
#else
#define CL_API_DISPLAY
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/signals.h"
#include "API/Core/Math/point.h"
#include "API/Core/Math/rect.h"
#include "API/Core/Math/quad.h"
#include "API/Core/Math/matrix4x4.h"
#include "API/Display/pixel_buffer.h"
#include "API/Display/stencil_func.h"

class CL_Color;
class CL_Gradient;
class CL_Surface_Target;
struct CL_TriangleParams;

//: Implementation interface for the graphic context.
class CL_API_DISPLAY CL_GraphicContext_Generic
{
//! Construction:
public:
	CL_GraphicContext_Generic();

	virtual ~CL_GraphicContext_Generic();

//! Attributes:
public:
	//: Returns the current width of the context.
	virtual int get_width() const=0;

	//: Returns the current height of the context.
	virtual int get_height() const=0;

	//: Returns the current clipping rectangle used on the graphic context.
	virtual const CL_Rect &get_cliprect() const { return cliprects.front(); }

	//: Returns the current effective modelview matrix.
	virtual const CL_Matrix4x4 &get_modelview() const { return modelviews.front(); }

	//: Returns the color at position (x,y).
	virtual CL_Color get_pixel(int x, int y) const=0;

	virtual CL_PixelBuffer get_pixeldata(const CL_Rect& rect, int i = 0) const=0;

	//: Returns the maximum size of a surface this displaywindow supports.
	virtual CL_Size get_max_surface_size() const { return CL_Size(0,0); }

// Operations:
public:
	void add_ref();

	void release_ref();

	//: Flushes current rendering batch.
	//- <p>With the OpenGL target, this causes the graphic context to end its current
	//- active glBegin()/glEnd() pair, making it possible to call other OpenGL calls.</p>
	virtual void flush() { return; }

	//: Draw a pixel at (x, y) using the specified color.
	virtual void draw_pixel(int x, int y, const CL_Color &color)=0;

	//: Draw a line from (x1, y1) to (x2, y2) using the specified color.
	virtual void draw_line(float x1, float y1, float x2, float y2, const CL_Color &color)=0;

	//: Draw a chain of lines using the specified color.
	virtual void draw_lines(int count, double *vertices, const CL_Color &color)=0;

	//: Draw a rectangle using the specified color.
	virtual void draw_rect(const CL_Rectf &rect, const CL_Color &color)=0;

	//: Draw a gradient rectangle using the specified gradient.
	virtual void fill_rect(const CL_Rectf &rect, const CL_Gradient &gradient)=0;

	//: Draw a filled rectangle using the specified color.
	virtual void fill_rect(const CL_Rectf &rect, const CL_Color &color)=0;
	
	//: Draw a quad using the specified color.
	virtual void draw_quad(const CL_Quad &quad, const CL_Color &color)=0;

	//: Draw a gradient quad using the specified gradient.
	virtual void fill_quad(const CL_Quad &quad, const CL_Gradient &gradient)=0;

	//: Draw a filled quad using the specified color.
	virtual void fill_quad(const CL_Quad &quad, const CL_Color &color)=0;

	//: Draw triangles (optionally colored and uv-mapped).
	virtual void draw_triangles(CL_TriangleParams *params)=0;

	//: Clears the whole context using the specified color.
	virtual void clear(const CL_Color &color)=0;

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
	virtual void modelview_changed()=0;

	//: Sets the model view matrix to a new matrix.
	void set_modelview(const CL_Matrix4x4 &matrix);

	//: Multiplies the passed matrix onto the model view matrix.
	void add_modelview(const CL_Matrix4x4 &matrix);

	//: Pushes current model view matrix onto the model view stack.
	void push_modelview();

	//: Sets a translate offset matrix, ignoring any earlier modelview settings.
	//- <p> This offset will affect any subsequent display operations on the current
	//- displaycard, by translating the position of the display operation with the offset.</p>
	void set_translate(double x, double y, double z = 0.0);

	//: Adds the translate offset.
	//- <p> This offset will affect any subsequent display operations on the current
	//- displaycard, by translating the position of the display operation with the offset.
	//- The offset will be offset by any previous offsets pushed onto the stack,
	//- eg. it inherits the previous offset. </p>
	void add_translate(double x, double y, double z = 0.0);
	
	//: Push translation offset onto model view stack.
	//- <p>This function is a convenience function for calling push_modelview, then add_translate.</p>
	//- <p> This offset will affect any subsequent display operations on the current
	//- displaycard, by translating the position of the display operation with the offset.
	//- The offset will be offset by any previous offsets pushed onto the stack,
	//- eg. it inherits the previous offset. </p>
	void push_translate(double x, double y, double z = 0.0);

	//: Sets a rotation matrix, ignoring any earlier model view settings.
	void set_rotate(double angle, double x = 0.0, double y = 0.0, double z = 1.0);

	//: Adds a rotation matrix to existing model view.
	void add_rotate(double angle, double x = 0.0, double y = 0.0, double z = 1.0);

	//: Pushes a rotation matrix onto model view stack.
	void push_rotate(double angle, double x = 0.0, double y = 0.0, double z = 1.0);

	//: Sets a scale matrix, ignoring any earlier model view settings.
	void set_scale(double x, double y, double z = 1.0);

	//: Adds a scale matrix to existing model view.
	void add_scale(double x, double y, double z = 1.0);

	//: Pushes a scale matrix onto model view stack.
	void push_scale(double x, double y, double z = 1.0);

	//: Pops last pushed model view matrix off the stack and makes it the active one.
	void pop_modelview();

	//: Clear the stencil buffer
	//param int value : value to clear to. Default: 0
	virtual void clear_stencil(int value)=0;

	//: Set stencil test function when writing to stencil, and operations to do when a test passes or fails
	//param CL_StencilOp pass : Modification to stencil buffer when a test passes.
	//param CL_StencilOp fail : Modification to stencil buffer when a test fails.
	//param CL_StencilFunc func : Type of test to do when writing to stencil.
	//param int ref : value compared to stencil contents. Default: 1
	virtual void set_stencil_operation(CL_StencilOp pass, CL_StencilOp fail, CL_StencilFunc func, int ref)=0;

	//: Enables writing to the stencil buffer 
	//param bool enabled : Enables/disables stencil testing.
	//param bool visible : Enables/disables color buffer writing if stencil writing is enabled. Default: true.
	//param float alpha_limit : Only pixels with alpha >= alpha limit are drawn to the stencil buffer.
	virtual void enable_stencil_write(bool enabled, bool visible, float alpha_limit)=0;

	//: Enables stencil buffer testing
	//- <p> Individual pixels are tested against the values in the stencil buffer, and
	//- drawn depending on the comparison result. This can be used for masking and
	//- boolean operations on bitmaps </p>
	//param bool enabled : Enables/disables stencil testing.
	//param int func : Type of comparison. Default: lequal (pass if ref <= stencil)
	//param int ref : Reference value that the stencil is compared with. Default: 1
	virtual void enable_stencil_test(bool enabled, CL_StencilFunc func, int ref)=0;

//! Implementation:
protected:
	std::list<CL_Rect> cliprects;

	std::list<CL_Matrix4x4> modelviews;

private:
	int ref_count;
};

#endif
