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
#include "API/Display/graphic_context.h"
#include "API/Display/surface.h"
#include "graphic_context_generic.h"
#include "surface_generic.h"
#include "surface_target.h"

#include "triangle_params.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GraphicContext construction:

CL_GraphicContext::CL_GraphicContext()
: impl(0)
{
}

CL_GraphicContext::CL_GraphicContext(const CL_GraphicContext &copy)
: impl(copy.impl)
{
	if (impl) impl->add_ref();
}

CL_GraphicContext::CL_GraphicContext(CL_GraphicContext_Generic *impl)
: impl(impl)
{
	if (impl) impl->add_ref();
}

CL_GraphicContext::~CL_GraphicContext()
{
	if (impl) impl->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_GraphicContext attributes:

int CL_GraphicContext::get_width() const
{
	return impl->get_width();
}

int CL_GraphicContext::get_height() const
{
	return impl->get_height();
}

const CL_Rect &CL_GraphicContext::get_cliprect() const
{
	return impl->get_cliprect();
}

const CL_Matrix4x4 &CL_GraphicContext::get_modelview() const
{
	return impl->get_modelview();
}

CL_Color CL_GraphicContext::get_pixel(int x, int y) const
{
	return impl->get_pixel(x, y);
}

CL_PixelBuffer CL_GraphicContext::get_pixeldata(const CL_Rect& rect, int i) const
{
	return impl->get_pixeldata(rect, i);
}

CL_Size CL_GraphicContext::get_max_surface_size() const
{
	return impl->get_max_surface_size();
}

/////////////////////////////////////////////////////////////////////////////
// CL_GraphicContext operations:

CL_GraphicContext &CL_GraphicContext::operator =(const CL_GraphicContext &copy)
{
	if (impl) impl->release_ref();
	impl = copy.impl;
	if (impl) impl->add_ref();
	return *this;
}

void CL_GraphicContext::flush()
{
	impl->flush();
}

void CL_GraphicContext::set_cliprect(const CL_Rect &rect)
{
	if (rect.right < rect.left || rect.bottom < rect.top)
	{
		static CL_Rect null_rect(0,0,0,0);
		impl->set_cliprect(null_rect);
	}
	impl->set_cliprect(rect);
}

void CL_GraphicContext::push_cliprect(const CL_Rect &rect)
{
	if (rect.right < rect.left || rect.bottom < rect.top)
	{
		static CL_Rect null_rect(0,0,0,0);
		impl->push_cliprect(null_rect);
	}
	impl->push_cliprect(rect);
}

void CL_GraphicContext::push_cliprect()
{
	impl->push_cliprect();
}

void CL_GraphicContext::pop_cliprect()
{
	impl->pop_cliprect();
}

void CL_GraphicContext::draw_pixel(int x, int y, const CL_Color &color)
{
	impl->draw_pixel(x, y, color);
}

void CL_GraphicContext::draw_line(float x1, float y1, float x2, float y2, const CL_Color &color)
{
	impl->draw_line(x1, y1, x2, y2, color);
}

void CL_GraphicContext::draw_lines(int count, double *vertices, const CL_Color &color)
{
	impl->draw_lines(count, vertices, color);
}

void CL_GraphicContext::draw_rect(const CL_Rectf &rect, const CL_Color &color)
{
	impl->draw_rect(rect, color);
}

void CL_GraphicContext::fill_rect(const CL_Rectf &rect, const CL_Gradient &gradient)
{
	impl->fill_rect(rect, gradient);
}

void CL_GraphicContext::fill_rect(const CL_Rectf &rect, const CL_Color &color)
{
	impl->fill_rect(rect, color);
}

void CL_GraphicContext::draw_quad(const CL_Quad &quad, const CL_Color &color)
{
	impl->draw_quad(quad, color);
}

void CL_GraphicContext::fill_quad(const CL_Quad &quad, const
	CL_Gradient &gradient)
{
	impl->fill_quad(quad, gradient);
}

void CL_GraphicContext::fill_quad(const CL_Quad &quad, const CL_Color
&color)
{
	impl->fill_quad(quad, color);
}

//: Draw a triangle using the specified color.
void CL_GraphicContext::draw_triangle(
		double x1, double y1,
		double x2, double y2,
		double x3, double y3,
		const CL_Color &color)
{
	CL_TriangleParams params;

	double vertice_array[] = {x1, y1, 0, x2, y2, 0, x3, y3, 0};

	params.count = 1;
	params.vertices = vertice_array;
	params.single_color = &color;
	params.fill = false;

	impl->draw_triangles( &params );
}

//: Draw a filled triangle using the specified color gradient.
void CL_GraphicContext::fill_triangle(
		double x1, double y1,
		double x2, double y2,
		double x3, double y3,
		const CL_Gradient &grad)
{
	CL_TriangleParams params;

	double vertice_array[] = {x1, y1, 0, x2, y2, 0, x3, y3, 0};
	
/*	double color_array[] = {
		grad.top_left.get_red()/256.0,
		grad.top_left.get_green()/256.0,
		grad.top_left.get_blue()/256.0,
		grad.top_left.get_alpha()/256.0,
		grad.top_right.get_red()/256.0,
		grad.top_right.get_green()/256.0,
		grad.top_right.get_blue()/256.0,
		grad.top_right.get_alpha()/256.0,
		grad.bottom_left.get_red()/256.0,
		grad.bottom_left.get_green()/256.0,
		grad.bottom_left.get_blue()/256.0,
		grad.bottom_left.get_alpha()/256.0};
*/
	unsigned char color_array[] = {
		grad.top_left.get_red(),
		grad.top_left.get_green(),
		grad.top_left.get_blue(),
		grad.top_left.get_alpha(),
		grad.top_right.get_red(),
		grad.top_right.get_green(),
		grad.top_right.get_blue(),
		grad.top_right.get_alpha(),
		grad.bottom_left.get_red(),
		grad.bottom_left.get_green(),
		grad.bottom_left.get_blue(),
		grad.bottom_left.get_alpha()};

	params.count = 1;
	params.vertices = vertice_array;
	params.uchar_color = color_array;
 	params.fill = true;

	impl->draw_triangles( &params );	
}

//: Draw triangles.
//- double color array, range: 0-1
//- double (0-1) texture coordinates
void CL_GraphicContext::draw_trianglesd(
		unsigned int count,
		bool fill,
		double *vertices,
		double *color,
		double *uv,
		CL_Surface *surface )
{
	CL_TriangleParams params;

	params.count = count;
	params.vertices = vertices;
	params.color = color;
	params.uv = uv;
	params.fill = fill;

	if( surface != 0 )
		params.texture = surface->impl->target;

	impl->draw_triangles( &params );	
}

//: Draw triangles (optionally colored and uv-mapped).
//- single color
//- double (0-1) texture coordinates
void CL_GraphicContext::draw_trianglesd(
		unsigned int count,
		bool fill,
		double *vertices,
		const CL_Color &color,
		double *uv,
		CL_Surface *surface)
{
	CL_TriangleParams params;

	params.count = count;
	params.vertices = vertices;
	params.single_color = &color;
	params.uv = uv;
	params.fill = fill;

	if( surface != 0 )
		params.texture = surface->impl->target;

	impl->draw_triangles( &params );	
}

//: Draw triangles (optionally colored and uv-mapped).
//- uchar color array, range: 0-255
//- integer (pixel) texture coordinates
void CL_GraphicContext::draw_triangles(
		unsigned int count,
		bool fill,
		double *vertices,
		unsigned char *color,
		int *uv,
		CL_Surface *surface )
{
	CL_TriangleParams params;

	params.count = count;
	params.vertices = vertices;
	params.uchar_color = color;
	params.int_uv = uv;
	params.fill = fill;

	if( surface != 0 )
		params.texture = surface->impl->target;

	impl->draw_triangles( &params );	
}

//: Draw triangles (optionally colored and uv-mapped).
//- single color
//- integer (pixel) texture coordinates
void CL_GraphicContext::draw_triangles(
		unsigned int count,
		bool fill,
		double *vertices,
		const CL_Color &color,
		int *uv,
		CL_Surface *surface )
{
	CL_TriangleParams params;

	params.count = count;
	params.vertices = vertices;
	params.single_color = &color;
	params.int_uv = uv;
	params.fill = fill;

	if( surface != 0 )
		params.texture = surface->impl->target;

	impl->draw_triangles( &params );	
}

void CL_GraphicContext::clear(const CL_Color &color)
{
	impl->clear(color);
}

void CL_GraphicContext::set_modelview(const CL_Matrix4x4 &matrix)
{
	impl->set_modelview(matrix);
}

void CL_GraphicContext::add_modelview(const CL_Matrix4x4 &matrix)
{
	impl->add_modelview(matrix);
}

void CL_GraphicContext::push_modelview()
{
	impl->push_modelview();
}

void CL_GraphicContext::set_translate(double x, double y, double z)
{
	impl->set_translate(x, y, z);
}

void CL_GraphicContext::add_translate(double x, double y, double z)
{
	impl->add_translate(x, y, z);
}

void CL_GraphicContext::push_translate(double x, double y, double z)
{
	impl->push_translate(x, y, z);
}

void CL_GraphicContext::set_rotate(double angle, double x, double y, double z)
{
	impl->set_rotate(angle, x, y, z);
}

void CL_GraphicContext::add_rotate(double angle, double x, double y, double z)
{
	impl->add_rotate(angle, x, y, z);
}

void CL_GraphicContext::push_rotate(double angle, double x, double y, double z)
{
	impl->push_rotate(angle, x, y, z);
}

void CL_GraphicContext::set_scale(double x, double y, double z)
{
	impl->set_scale(x, y, z);
}

void CL_GraphicContext::add_scale(double x, double y, double z)
{
	impl->add_scale(x, y, z);
}

void CL_GraphicContext::push_scale(double x, double y, double z)
{
	impl->push_scale(x, y, z);
}

void CL_GraphicContext::pop_modelview()
{
	impl->pop_modelview();
}

void CL_GraphicContext::clear_stencil(int value)
{
	impl->clear_stencil(value);
}

void CL_GraphicContext::enable_stencil_write(bool enabled, bool visible, float alpha_limit)
{
	impl->enable_stencil_write(enabled, visible, alpha_limit);
}

void CL_GraphicContext::enable_stencil_test(bool enabled, CL_StencilFunc func, int ref)
{
	impl->enable_stencil_test(enabled, func, ref);
}

void CL_GraphicContext::set_stencil_operation(CL_StencilOp pass, CL_StencilOp fail, CL_StencilFunc func, int ref)
{
	impl->set_stencil_operation(pass, fail, func, ref);
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:
