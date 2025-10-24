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

#ifndef header_gl_state_checker
#define header_gl_state_checker

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/GL/opengl_state_data.h"
#include "API/Display/stencil_func.h"

#ifdef WIN32
#include <windows.h>
#endif 

#include <stack>
#include "gl_state.h"

class CL_Quad;
class CL_Rect;
class CL_Rectf;
class CL_Pointf;
class CL_Color;
class CL_GraphicContext;
class CL_OpenGLState;
class CL_GraphicContext_OpenGL;
class CL_Surface_Target;
struct CL_TriangleParams;

class CL_GLStateChecker : private CL_OpenGLStateData
{
//! Construction:
public:
	CL_GLStateChecker(CL_GraphicContext_OpenGL *gc_opengl);

	~CL_GLStateChecker();

//! Attributes:
public:
	CL_OpenGLState *opengl_state;

	CL_GraphicContext_OpenGL *gc_opengl;

	CLint stencil_op_pass;
	CLint stencil_op_fail;
	CLint stencil_func_write;
	CLint stencil_write_ref;

//! Operations:
public:
	static CL_GLStateChecker *from_gc(CL_GraphicContext *gc);

	//: clear the screen. 
	void add_clear(
		const CL_Color &color);

	//: add a pixel
	void add_pixel(
		const CL_GLState &state,
		int x, int y);

	//: add a line
	void add_line(
		const CL_GLState &state,
		float x1, float y1, float x2, float y2);

	//: add lines
	void add_lines(
		const CL_GLState &state,
		unsigned int count,
		double *vertices);

	//: add a gradient filled rectangle.
	void add_gradient(
		const CL_GLState &state,
		const CL_Rectf &position,
		const CL_Gradient &gradient);
	
	//: add a gradient filled quad.
	void add_gradient(
		const CL_GLState &state,
		const CL_Quad &quad,
		const CL_Gradient &gradient);

	//: add a textured quad.
	void add_quad(
		const CL_GLState &state,
		double px1,
		double py1,
		double px2,
		double py2,
		int tx1,
		int ty1,
		int tx2,
		int ty2);

	//: add a textured quad.
	void add_quad(
		const CL_GLState &state,
		const double destX[4],
		const double destY[4],
		const int tx[4],
		const int ty[4]);

	//: add triangles (optionally colored and uv-mapped).
	void add_triangles( const CL_GLState &state, CL_TriangleParams *params);

	//: Renders state tree objects.
	void flush(bool rebind_texture=false) const;

	//: Reads a pixel from opengl context.
	CL_Color get_pixel(int x, int y) const;

	//: Clears the Stencil buffer
	void clear_stencil(int value);

	//: Enable writing to stencil buffer
	void enable_stencil_write(bool enabled, bool visible, double alpha_limit);

	//: Enable testing pixels to be drawn with stencil buffer
	void enable_stencil_test(bool enabled, CL_StencilFunc func, int ref);

	void reinitialize_asap(); //MacOS needed method after toggling windowed/fullscreen mode

//! CL_OpenGLStateData:
private:
	//: Save state information needed to restore this OpenGL state.
	virtual void on_save();

	//: Load state information and setup OpenGL to this state.
	virtual void on_load();

	//: Flush current rendering batch.
	virtual void on_flush();

//! Implementation:
private:
	inline void add_quad_vertices(const CL_GLState &state, double px1, double py1, double px2, double py2 );
	
	inline void add_quad_vertices(const CL_GLState &state, double px1, double py1, double px2, double py2, double px3, double py3, double px4, double py4 );

	void set_state(const CL_GLState &state, CLenum primitive);

	//: set_state helper.
	void set_blendfunc(const CL_GLState &state);

	void set_fillmode(const CL_GLState &state);

	mutable CL_GLState internal_state;

	mutable CLenum primitive;

	// Primitive texture.
	// Store pointer to surface used for texturing primitives (triangle)
	// Used for inc/dec of reference counting of the surface.
	// Makes sure the texture isn't free'ed before draw operations are completed.
	CL_Surface_Target *primitive_texture;

	// Number of quads that can be drawn at once.
	// If more are drawn, a flush() will take place.
	#define MAX_QUAD_COUNT 256

	// Need to distinguish gradients from plain quads.
	// This can be whatever except CL_QUADS, CL_LINES or CL_POINTS.
	#define CL_GRADIENT 0x1337

	//: keep track of how many primitives to draw.
	mutable int primitive_count;

	CLdouble vertex_array[MAX_QUAD_COUNT*3*4];
	CLint i_uv_array[MAX_QUAD_COUNT*2*4];
	CLdouble d_uv_array[MAX_QUAD_COUNT*2*4];
	CLdouble d_color_array[MAX_QUAD_COUNT*4*4];
	CLubyte b_color_array[MAX_QUAD_COUNT*4*4];

	bool tri_use_uv;
	bool tri_use_color;
	
	bool first_call;
	
	bool color_array_enabled;

	bool color_array_range_zero2one;
	bool uv_array_range_zero2one;
};

#endif
