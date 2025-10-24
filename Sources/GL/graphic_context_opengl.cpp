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

#include "Display/display_precomp.h"
#include "Display/display_window_generic.h"
#include "Display/triangle_params.h"
#include "graphic_context_opengl.h"
#include "canvas_opengl.h"
#include "surface_target_opengl.h"
#include "API/Display/color.h"
#include "API/Core/System/error.h"
#include "API/Core/System/log.h"
#include "API/GL/opengl_state.h"
#include "GL/gl_state_checker.h"
#include "API/GL/opengl.h"
#include "API/GL/opengl_wrap.h"
#ifdef WIN32
#include "WGL/display_window_opengl.h"
#else
#include "GLX/display_window_opengl.h"
#endif
#include <cstring>


/////////////////////////////////////////////////////////////////////////////
// CL_GraphicContext_OpenGL construction:

CL_GraphicContext_OpenGL::CL_GraphicContext_OpenGL(CL_DisplayWindow_Generic *window)
	: state_checker(0), window(window), canvas_target(0), max_texture_size(0)
{
	slots.connect(window->sig_resize, this, &CL_GraphicContext_OpenGL::on_resize);

	state_checker = new CL_GLStateChecker(this);
}

CL_GraphicContext_OpenGL::CL_GraphicContext_OpenGL(CL_Canvas_OpenGL *canvas_target)
	: state_checker(0), window(0), canvas_target(canvas_target), max_texture_size(0)
{
	state_checker = new CL_GLStateChecker(this);
}

CL_GraphicContext_OpenGL::~CL_GraphicContext_OpenGL()
{
	delete state_checker;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GraphicContext_OpenGL attributes:

int CL_GraphicContext_OpenGL::get_width() const
{
	if (canvas_target) return canvas_target->get_width();
	return window->get_width();
}

int CL_GraphicContext_OpenGL::get_height() const
{
	if (canvas_target) return canvas_target->get_height();
	return window->get_height();
}

CL_Color CL_GraphicContext_OpenGL::get_pixel(int x, int y) const
{
	return state_checker->get_pixel(static_cast<int>(x+get_modelview().get_origin_x()),
                                    static_cast<int>(y+get_modelview().get_origin_y()));
}

CL_PixelBuffer CL_GraphicContext_OpenGL::get_pixeldata(const CL_Rect& rect, int frame) const
{
	state_checker->opengl_state->set_active();
	state_checker->opengl_state->set_active();
	state_checker->opengl_state->setup_2d();

	int width;
	if (rect.get_width() == 0) 
		width = get_width();
	else
		width = rect.get_width();

	int height;
	if (rect.get_height() == 0) 
		height = get_height();
	else
		height = rect.get_height();

	CLint align;
	clGetIntegerv(CL_PACK_ALIGNMENT, &align);
	int pitch  = static_cast<int>((width*4+align-1)/align) * align;

	CL_PixelBuffer buffer(width, height, pitch, CL_PixelFormat::abgr8888);

	// Keep the current read buffer, so we can reset it after we are done
	CLint old_frame;
	clGetIntegerv(CL_READ_BUFFER, &old_frame);

	buffer.lock();
	clReadBuffer(frame);
	clReadPixels(rect.left, rect.top, width, height, CL_RGBA, CL_UNSIGNED_BYTE, buffer.get_data());

	// OpenGL gives us the image upside down, so we flip it
	unsigned char* buf = static_cast<unsigned char*>(buffer.get_data());
	unsigned char*  tmp = new unsigned char[pitch];
	for(int y = 0; y < height/2; ++y)
	{
      memcpy(tmp, buf + ((height-y-1) * pitch), sizeof(unsigned char) * pitch);
      memcpy(buf + ((height-y-1) * pitch), buf + (y * pitch), sizeof(unsigned char) * pitch);
      memcpy(buf + (y * pitch), tmp, sizeof(unsigned char) * pitch);
	}
	delete[] tmp;

	buffer.unlock();
	clReadBuffer(old_frame);

	return buffer;
}

CL_Size CL_GraphicContext_OpenGL::get_max_surface_size() const
{
	if (max_texture_size == 0)
	{
		state_checker->opengl_state->set_active();
		max_texture_size = 512;
		clGetIntegerv(CL_MAX_TEXTURE_SIZE, &max_texture_size);
	}
	return CL_Size(max_texture_size, max_texture_size);
}

#ifdef WIN32

HDC CL_GraphicContext_OpenGL::get_drawable()
{
	if (window) return ((CL_DisplayWindow_OpenGL *) window)->get_dc();
	return canvas_target->get_pbuffer_dc();
}

HGLRC CL_GraphicContext_OpenGL::get_context()
{
	if (window) return ((CL_DisplayWindow_OpenGL *) window)->get_context();
	return canvas_target->get_pbuffer_context();
}

#else
#ifdef __APPLE__

/*
AGLDrawable CL_GraphicContext_OpenGL::get_drawable()
{
	if (window) return ((CL_DisplayWindow_OpenGL *) window)->get_drawable();
	return canvas_target->get_pbuffer();
}
*/

AGLContext CL_GraphicContext_OpenGL::get_context()
{
	if (window) return ((CL_DisplayWindow_OpenGL *) window)->get_context();
	return canvas_target->get_pbuffer_context();
}

#else

Display *CL_GraphicContext_OpenGL::get_display()
{
	if (window) return ((CL_DisplayWindow_OpenGL *) window)->get_display();
	return CL_DisplayWindow_OpenGL::get_display();
}

GLXDrawable CL_GraphicContext_OpenGL::get_drawable()
{
	if (window) return ((CL_DisplayWindow_OpenGL *) window)->get_window();
	return canvas_target->get_pbuffer();
}

GLXContext CL_GraphicContext_OpenGL::get_context()
{
	if (window) return ((CL_DisplayWindow_OpenGL *) window)->get_context();
	return canvas_target->get_pbuffer_context();
}

#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_GraphicContext_OpenGL operations:

void CL_GraphicContext_OpenGL::draw_pixel(int x, int y, const CL_Color &color)
{
	CL_GLState state;
	state.texture_handle = -1; // disable texture
	state.color[0] = color.get_red()/255.0f;
	state.color[1] = color.get_green()/255.0f;
	state.color[2] = color.get_blue()/255.0f;
	state.color[3] = color.get_alpha()/255.0f;
	state.blendfunc[0] = blend_src_alpha;
	state.blendfunc[1] = blend_one_minus_src_alpha;
	state.blendfunc[2] = blend_src_alpha;
	state.blendfunc[3] = blend_one_minus_src_alpha;
	state.fillmode = fillmode_solid;
	
	state_checker->add_pixel(state, x, y);
}

void CL_GraphicContext_OpenGL::draw_line(float x1, float y1, float x2, float y2, const CL_Color &color)
{
	CL_GLState state;
	state.texture_handle = -1; // disable texture
	state.color[0] = color.get_red()/255.0f;
	state.color[1] = color.get_green()/255.0f;
	state.color[2] = color.get_blue()/255.0f;
	state.color[3] = color.get_alpha()/255.0f;
	state.blendfunc[0] = blend_src_alpha;
	state.blendfunc[1] = blend_one_minus_src_alpha;
	state.blendfunc[2] = blend_src_alpha;
	state.blendfunc[3] = blend_one_minus_src_alpha;
	state.fillmode = fillmode_solid;

	state_checker->add_line(state, x1, y1, x2, y2);
}

void CL_GraphicContext_OpenGL::draw_lines(int count, double *vertices, const CL_Color &color)
{
	CL_GLState state;
	state.texture_handle = -1; // disable texture
	state.color[0] = color.get_red()/255.0f;
	state.color[1] = color.get_green()/255.0f;
	state.color[2] = color.get_blue()/255.0f;
	state.color[3] = color.get_alpha()/255.0f;
	state.blendfunc[0] = blend_src_alpha;
	state.blendfunc[1] = blend_one_minus_src_alpha;
	state.blendfunc[2] = blend_src_alpha;
	state.blendfunc[3] = blend_one_minus_src_alpha;

	state.fillmode = fillmode_solid;

	state_checker->add_lines(state, count, vertices);
}

void CL_GraphicContext_OpenGL::draw_rect(const CL_Rectf &rect, const CL_Color &color)
{
	CL_GLState state;
	state.texture_handle = -1; // disable texture
	state.color[0] = color.get_red()/255.0f;
	state.color[1] = color.get_green()/255.0f;
	state.color[2] = color.get_blue()/255.0f;
	state.color[3] = color.get_alpha()/255.0f;
	state.blendfunc[0] = blend_src_alpha;
	state.blendfunc[1] = blend_one_minus_src_alpha;
	state.blendfunc[2] = blend_src_alpha;
	state.blendfunc[3] = blend_one_minus_src_alpha;

	state.fillmode = fillmode_solid;

	state_checker->add_line(state, rect.left, rect.top, rect.right, rect.top);
	state_checker->add_line(state, rect.right - 1, rect.top + 1, rect.right - 1, rect.bottom);
	state_checker->add_line(state, rect.left, rect.bottom - 1, rect.right - 1, rect.bottom - 1);
	state_checker->add_line(state, rect.left, rect.top + 1, rect.left, rect.bottom - 1);

	// draw rect without filling... just lines
/*	state.fillmode = fillmode_line;

	state_checker->add_quad(
		state,
		rect.left,
		rect.top,
//		rect.right,
//		rect.bottom,
		rect.right - 1,
		rect.bottom - 1,
		0,0,1,1);
*/
}

void CL_GraphicContext_OpenGL::fill_rect(const CL_Rectf &rect, const CL_Color &color)
{
	CL_GLState state;
	state.texture_handle = -1;
	state.color[0] = color.get_red()/255.0f;
	state.color[1] = color.get_green()/255.0f;
	state.color[2] = color.get_blue()/255.0f;
	state.color[3] = color.get_alpha()/255.0f;
	state.blendfunc[0] = blend_src_alpha;
	state.blendfunc[1] = blend_one_minus_src_alpha;
	state.blendfunc[2] = blend_src_alpha;
	state.blendfunc[3] = blend_one_minus_src_alpha;

	state.fillmode = fillmode_solid;

	state_checker->add_quad(
		state,
		rect.left,
		rect.top,
		rect.right,
		rect.bottom,
		0,
		0,
		1,
		1);
}

void CL_GraphicContext_OpenGL::fill_rect(const CL_Rectf &rect, const CL_Gradient &gradient)
{
	CL_GLState state;
	state.texture_handle = -1;
	state.color[0] = gradient.top_left.get_red()/255.0f;
	state.color[1] = gradient.top_left.get_green()/255.0f;
	state.color[2] = gradient.top_left.get_blue()/255.0f;
	state.color[3] = gradient.top_left.get_alpha()/255.0f;
	state.blendfunc[0] = blend_src_alpha;
	state.blendfunc[1] = blend_one_minus_src_alpha;
	state.blendfunc[2] = blend_src_alpha;
	state.blendfunc[3] = blend_one_minus_src_alpha;

	state.fillmode = fillmode_solid;

	CL_Rectf pos(rect.left, rect.top, rect.right, rect.bottom);
	state_checker->add_gradient(state, pos, gradient);
}

void CL_GraphicContext_OpenGL::draw_quad(const CL_Quad &quad, const CL_Color &color)
{
	CL_GLState state;
	state.texture_handle = -1; // disable texture
	state.color[0] = color.get_red()/255.0f;
	state.color[1] = color.get_green()/255.0f;
	state.color[2] = color.get_blue()/255.0f;
	state.color[3] = color.get_alpha()/255.0f;
	state.blendfunc[0] = blend_src_alpha;
	state.blendfunc[1] = blend_one_minus_src_alpha;
	state.blendfunc[2] = blend_src_alpha;
	state.blendfunc[3] = blend_one_minus_src_alpha;
	state.fillmode = fillmode_line;

	double qx[4] = { quad.x1, quad.x2, quad.x3, quad.x4 };
	double qy[4] = { quad.y1, quad.y2, quad.y3, quad.y4 };
	int tx[4] = { quad.x1, quad.x2, quad.x3, quad.x4 };
	int ty[4] = { quad.y1, quad.y2, quad.y3, quad.y4 };

	state_checker->add_quad(
		state,
		qx, qy,
		tx, ty);
}

void CL_GraphicContext_OpenGL::fill_quad(const CL_Quad &quad, const CL_Color &color)
{
	CL_GLState state;
	state.texture_handle = -1;
	state.color[0] = color.get_red()/255.0f;
	state.color[1] = color.get_green()/255.0f;
	state.color[2] = color.get_blue()/255.0f;
	state.color[3] = color.get_alpha()/255.0f;
	state.blendfunc[0] = blend_src_alpha;
	state.blendfunc[1] = blend_one_minus_src_alpha;
	state.blendfunc[2] = blend_src_alpha;
	state.blendfunc[3] = blend_one_minus_src_alpha;
	state.fillmode = fillmode_solid;

	double qx[4] = { quad.x1, quad.x2, quad.x3, quad.x4 };
	double qy[4] = { quad.y1, quad.y2, quad.y3, quad.y4 };
	int tx[4] = { quad.x1, quad.x2, quad.x3, quad.x4 };
	int ty[4] = { quad.y1, quad.y2, quad.y3, quad.y4 };

	state_checker->add_quad(
		state,
		qx, qy,
		tx, ty);
}

void CL_GraphicContext_OpenGL::fill_quad(const CL_Quad &quad, const CL_Gradient &gradient)
{
	CL_GLState state;
	state.texture_handle = -1;
	state.color[0] = gradient.top_left.get_red()/255.0f;
	state.color[1] = gradient.top_left.get_green()/255.0f;
	state.color[2] = gradient.top_left.get_blue()/255.0f;
	state.color[3] = gradient.top_left.get_alpha()/255.0f;
	state.blendfunc[0] = blend_src_alpha;
	state.blendfunc[1] = blend_one_minus_src_alpha;
	state.blendfunc[2] = blend_src_alpha;
	state.blendfunc[3] = blend_one_minus_src_alpha;
	state.fillmode = fillmode_solid;

	state_checker->add_gradient(state, quad, gradient);
}

void CL_GraphicContext_OpenGL::draw_triangles(
	CL_TriangleParams *params )
{
	CL_GLState state;

	state.fillmode = params->fill ? fillmode_solid : fillmode_line;

	if( params->texture )
	{
		CL_Surface_Target_OpenGL *ogl_tex = dynamic_cast<CL_Surface_Target_OpenGL*>(params->texture);

		if( ogl_tex )
		{
			state.texture_handle = ogl_tex->handle;
		}
		else
			state.texture_handle = -1;
	}
	else
		state.texture_handle = -1;

	if( params->single_color )
	{
		state.color[0] = params->single_color->get_red() / 255.0f;
		state.color[1] = params->single_color->get_green() / 255.0f;
		state.color[2] = params->single_color->get_blue() / 255.0f;
		state.color[3] = params->single_color->get_alpha() / 255.0f;
	}

	state.blendfunc[0] = blend_src_alpha;
	state.blendfunc[1] = blend_one_minus_src_alpha;
	state.blendfunc[2] = blend_src_alpha;
	state.blendfunc[3] = blend_one_minus_src_alpha;

	state.color_range_zero2one = (params->color != 0);
	state.uv_range_zero2one = (params->uv != 0);
	
	state_checker->add_triangles(state, params);
}


void CL_GraphicContext_OpenGL::clear(const CL_Color &color)
{
	state_checker->add_clear(color);
}

void CL_GraphicContext_OpenGL::sync_gc()
{
	if (canvas_target)
	{
		canvas_target->sync_pbuffer();
		canvas_target->set_pbuffer_modified();
	}
}


void CL_GraphicContext_OpenGL::clear_stencil(int value)
{
	state_checker->clear_stencil(value);	
}

void CL_GraphicContext_OpenGL::set_stencil_operation(CL_StencilOp pass, CL_StencilOp fail, CL_StencilFunc func, int ref)
{
	switch( pass )
	{
		case stencil_keep: state_checker->stencil_op_pass = CL_KEEP; break;
		case stencil_zero: state_checker->stencil_op_pass = CL_ZERO; break;
		case stencil_replace: state_checker->stencil_op_pass = CL_REPLACE; break;
		case stencil_incr: state_checker->stencil_op_pass = CL_INCR; break;
		case stencil_decr: state_checker->stencil_op_pass = CL_DECR; break;
		case stencil_invert: state_checker->stencil_op_pass = CL_INVERT; break;
		default: state_checker->stencil_op_pass = CL_KEEP;
	}		

	switch( fail )
	{
		case stencil_keep: state_checker->stencil_op_fail = CL_KEEP; break;
		case stencil_zero: state_checker->stencil_op_fail = CL_ZERO; break;
		case stencil_replace: state_checker->stencil_op_fail = CL_REPLACE; break;
		case stencil_incr: state_checker->stencil_op_fail = CL_INCR; break;
		case stencil_decr: state_checker->stencil_op_fail = CL_DECR; break;
		case stencil_invert: state_checker->stencil_op_fail = CL_INVERT; break;
		default: state_checker->stencil_op_fail = CL_KEEP;
	}

	switch( func )
	{
		case stencil_never: state_checker->stencil_func_write = CL_NEVER; break;
		case stencil_less: state_checker->stencil_func_write = CL_LESS; break;
		case stencil_lequal: state_checker->stencil_func_write = CL_LEQUAL; break;
		case stencil_greater: state_checker->stencil_func_write = CL_GREATER; break;
		case stencil_gequal: state_checker->stencil_func_write = CL_GEQUAL; break;
		case stencil_equal: state_checker->stencil_func_write = CL_EQUAL; break;
		case stencil_notequal: state_checker->stencil_func_write = CL_NOTEQUAL; break;
		case stencil_always: state_checker->stencil_func_write = CL_ALWAYS; break;
		default: state_checker->stencil_func_write = CL_ALWAYS;
	}
	
	state_checker->stencil_write_ref = ref;	
}

void CL_GraphicContext_OpenGL::enable_stencil_write(bool enabled, bool visible, float alpha_limit)
{
	state_checker->enable_stencil_write(enabled, visible, (double)alpha_limit);
}

void CL_GraphicContext_OpenGL::enable_stencil_test(bool enabled, CL_StencilFunc func, int ref)
{
	state_checker->enable_stencil_test(enabled, func, ref);
}


/////////////////////////////////////////////////////////////////////////////
// CL_GraphicContext_OpenGL implementation:

void CL_GraphicContext_OpenGL::set_cliprect(const CL_Rect &rect)
{
	CL_GraphicContext_Generic::set_cliprect(rect);
	sig_update_scissor();
}

void CL_GraphicContext_OpenGL::push_cliprect(const CL_Rect &rect)
{
	CL_GraphicContext_Generic::push_cliprect(rect);
	sig_update_scissor();
}

void CL_GraphicContext_OpenGL::push_cliprect()
{
	CL_GraphicContext_Generic::push_cliprect();
	sig_update_scissor();
}

void CL_GraphicContext_OpenGL::pop_cliprect()
{
	CL_GraphicContext_Generic::pop_cliprect();
	sig_update_scissor();
}

void CL_GraphicContext_OpenGL::modelview_changed()
{
	sig_update_modelview();
}

void CL_GraphicContext_OpenGL::on_resize(int width, int height)
{
	cliprects.pop_back();
	cliprects.push_back(CL_Rect(0, 0, width, height));
	sig_update_viewport();
}
