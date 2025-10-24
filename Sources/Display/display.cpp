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
#include "API/Display/display.h"
#include "API/Display/pixel_buffer.h"
#include "API/Display/display_mode.h"
#include "API/Display/input_context.h"
#include "API/Display/graphic_context.h"
#include "display_window_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Display attributes:

CL_DisplayWindow *CL_Display::get_current_window()
{
	return CL_DisplayWindow_Generic::current;
}

int CL_Display::get_width()
{
	return CL_DisplayWindow_Generic::current->get_width();
}

int CL_Display::get_height()
{
	return CL_DisplayWindow_Generic::current->get_height();
}

bool CL_Display::is_fullscreen()
{
	return CL_DisplayWindow_Generic::current->is_fullscreen();
}

bool CL_Display::has_focus()
{
	return CL_DisplayWindow_Generic::current->has_focus();
}

CL_PixelBuffer CL_Display::get_buffer(int i)
{
	return CL_DisplayWindow_Generic::current->get_buffer(i);
}

int CL_Display::get_buffer_count()
{
	return CL_DisplayWindow_Generic::current->get_buffer_count();
}

CL_PixelBuffer CL_Display::get_front_buffer()
{
	return CL_DisplayWindow_Generic::current->get_front_buffer();
}

CL_PixelBuffer CL_Display::get_back_buffer()
{
	return CL_DisplayWindow_Generic::current->get_back_buffer();
}

const CL_Rect &CL_Display::get_cliprect()
{
	return CL_DisplayWindow_Generic::current->get_gc()->get_cliprect();
}

const CL_Matrix4x4 &CL_Display::get_modelview()
{
	return CL_DisplayWindow_Generic::current->get_gc()->get_modelview();
}
	
/////////////////////////////////////////////////////////////////////////////
// CL_Display operations:

void CL_Display::set_current_window(CL_DisplayWindow *window)
{
	CL_DisplayWindow_Generic::current = window;
}

void CL_Display::set_fullscreen(int width, int height, int bpp)
{
	CL_DisplayWindow_Generic::current->set_fullscreen(width, height, bpp, 0);
}

void CL_Display::set_fullscreen(const CL_DisplayMode &display_mode)
{
	CL_DisplayWindow_Generic::current->set_fullscreen(display_mode);
}

void CL_Display::set_windowed()
{
	CL_DisplayWindow_Generic::current->set_windowed();
}

void CL_Display::set_title(const std::string &title)
{
	CL_DisplayWindow_Generic::current->set_title(title);
}

void CL_Display::set_position(const CL_Rect &pos)
{
	CL_DisplayWindow_Generic::current->set_position(pos);
}

void CL_Display::set_position(int x, int y)
{
	CL_DisplayWindow_Generic::current->set_position(x, y);
}

void CL_Display::set_size(int width, int height)
{
	CL_DisplayWindow_Generic::current->set_size(width, height);
}

void CL_Display::set_buffer_count(int flipping_buffers)
{
	CL_DisplayWindow_Generic::current->set_buffer_count(flipping_buffers);
}

void CL_Display::update(const CL_Rect &rect)
{
	CL_DisplayWindow_Generic::current->update(rect);
}

void CL_Display::flip(int interval)
{
	CL_DisplayWindow_Generic::current->flip(interval);
}

void CL_Display::flush()
{
	CL_DisplayWindow_Generic::current->get_gc()->flush();
}

void CL_Display::draw_pixel(int x, int y, const CL_Color &color)
{
	CL_DisplayWindow_Generic::current->get_gc()->draw_pixel(x, y, color);
}

void CL_Display::draw_line(float x1, float y1, float x2, float y2, const CL_Color &color)
{
	CL_DisplayWindow_Generic::current->get_gc()->draw_line(x1, y1, x2, y2, color);
}

void CL_Display::draw_line(const CL_Pointf &a, const CL_Pointf &b, const CL_Color &color)
{
	CL_DisplayWindow_Generic::current->get_gc()->draw_line(a.x, a.y, b.x, b.y, color);
}

void CL_Display::draw_lines(int count, double *vertices, const CL_Color &color)
{
	CL_DisplayWindow_Generic::current->get_gc()->draw_lines(count, vertices, color);
}

void CL_Display::draw_rect(const CL_Rectf &rect, const CL_Color &color)
{
	CL_DisplayWindow_Generic::current->get_gc()->draw_rect(rect, color);
}

void CL_Display::fill_rect(const CL_Rectf &rect, const CL_Gradient &gradient)
{
	CL_DisplayWindow_Generic::current->get_gc()->fill_rect(rect, gradient);
}

void CL_Display::fill_rect(const CL_Rectf &rect, const CL_Color &color)
{
	CL_DisplayWindow_Generic::current->get_gc()->fill_rect(rect, color);
}

void CL_Display::draw_quad(const CL_Quad &quad, const CL_Color &color)
{
	CL_DisplayWindow_Generic::current->get_gc()->draw_quad(quad, color);
}

void CL_Display::fill_quad(const CL_Quad &quad, const CL_Gradient &gradient)
{
	CL_DisplayWindow_Generic::current->get_gc()->fill_quad(quad, gradient);
}

void CL_Display::fill_quad(const CL_Quad &quad, const CL_Color &color)
{
	CL_DisplayWindow_Generic::current->get_gc()->fill_quad(quad, color);
}

void CL_Display::draw_triangle(
		double x1, double y1,
		double x2, double y2,
		double x3, double y3,
		const CL_Color &color)
{
	CL_GraphicContext *gc = CL_DisplayWindow_Generic::current->get_gc();

	gc->draw_triangle(x1,y1,x2,y2,x3,y3,color);
}

void CL_Display::fill_triangle(
		double x1, double y1,
		double x2, double y2,
		double x3, double y3,
		const CL_Gradient &gradient)
{
	CL_GraphicContext *gc = CL_DisplayWindow_Generic::current->get_gc();

	gc->fill_triangle(x1,y1,x2,y2,x3,y3,gradient);
}

void CL_Display::draw_triangles(
		unsigned int count,
		bool fill,
		double *vertices,
		unsigned char *color,
		int *uv,
		CL_Surface *texture)
{
	CL_DisplayWindow_Generic::current->get_gc()->draw_triangles(count, fill, vertices, color, uv, texture);
}

void CL_Display::draw_triangles(
		unsigned int count,
		bool fill,
		double *vertices,
		const CL_Color &color,
		int *uv,
		CL_Surface *texture )
{
	CL_DisplayWindow_Generic::current->get_gc()->draw_triangles(count, fill, vertices, color, uv, texture);
}

void CL_Display::draw_trianglesd(
		unsigned int count,
		bool fill,
		double *vertices,
		double *color,
		double *uv,
		CL_Surface *texture )
{
	CL_DisplayWindow_Generic::current->get_gc()->draw_trianglesd(count, fill, vertices, color, uv, texture);
}

void CL_Display::draw_trianglesd(
		unsigned int count,
		bool fill,
		double *vertices,
		const CL_Color &color,
		double *uv,
		CL_Surface *texture )
{
	CL_DisplayWindow_Generic::current->get_gc()->draw_trianglesd(count, fill, vertices, color, uv, texture);
}

void CL_Display::clear(const CL_Color &color)
{
	CL_DisplayWindow_Generic::current->get_gc()->clear(color);
}

void CL_Display::set_cliprect(const CL_Rect &rect)
{
	CL_DisplayWindow_Generic::current->get_gc()->set_cliprect(rect);
}

void CL_Display::push_cliprect(const CL_Rect &rect)
{
	CL_DisplayWindow_Generic::current->get_gc()->push_cliprect(rect);
}

void CL_Display::push_cliprect()
{
	CL_DisplayWindow_Generic::current->get_gc()->push_cliprect();
}

void CL_Display::pop_cliprect()
{
	CL_DisplayWindow_Generic::current->get_gc()->pop_cliprect();
}

void CL_Display::set_modelview(const CL_Matrix4x4 &matrix)
{
	CL_DisplayWindow_Generic::current->get_gc()->set_modelview(matrix);
}

void CL_Display::add_modelview(const CL_Matrix4x4 &matrix)
{
	CL_DisplayWindow_Generic::current->get_gc()->add_modelview(matrix);
}

void CL_Display::push_modelview()
{
	CL_DisplayWindow_Generic::current->get_gc()->push_modelview();
}

void CL_Display::set_translate(double x, double y, double z)
{
	CL_DisplayWindow_Generic::current->get_gc()->set_translate(x, y, z);
}

void CL_Display::add_translate(double x, double y, double z)
{
	CL_DisplayWindow_Generic::current->get_gc()->add_translate(x, y, z);
}

void CL_Display::push_translate(double x, double y, double z)
{
	CL_DisplayWindow_Generic::current->get_gc()->push_translate(x, y, z);
}

void CL_Display::set_rotate(double angle, double x, double y, double z)
{
	CL_DisplayWindow_Generic::current->get_gc()->set_rotate(angle, x, y, z);
}

void CL_Display::add_rotate(double angle, double x, double y, double z)
{
	CL_DisplayWindow_Generic::current->get_gc()->add_rotate(angle, x, y, z);
}

void CL_Display::push_rotate(double angle, double x, double y, double z)
{
	CL_DisplayWindow_Generic::current->get_gc()->push_rotate(angle, x, y, z);
}

void CL_Display::set_scale(double x, double y, double z)
{
	CL_DisplayWindow_Generic::current->get_gc()->set_scale(x, y, z);
}

void CL_Display::add_scale(double x, double y, double z)
{
	CL_DisplayWindow_Generic::current->get_gc()->add_scale(x, y, z);
}

void CL_Display::push_scale(double x, double y, double z)
{
	CL_DisplayWindow_Generic::current->get_gc()->push_scale(x, y, z);
}

void CL_Display::pop_modelview()
{
	CL_DisplayWindow_Generic::current->get_gc()->pop_modelview();
}

/////////////////////////////////////////////////////////////////////////////
// CL_Display signals:

CL_Signal_v2<int, int> &CL_Display::sig_resize()
{
	return CL_DisplayWindow_Generic::current->sig_resize();
}

CL_Signal_v1<const CL_Rect &> &CL_Display::sig_paint()
{
	return CL_DisplayWindow_Generic::current->sig_paint();
}

CL_Signal_v0 &CL_Display::sig_lost_focus()
{
	return CL_DisplayWindow_Generic::current->sig_lost_focus();
}

CL_Signal_v0 &CL_Display::sig_got_focus()
{
	return CL_DisplayWindow_Generic::current->sig_got_focus();
}

CL_Signal_v0 &CL_Display::sig_window_close()
{
	return CL_DisplayWindow_Generic::current->sig_window_close();
}
