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
#include "graphic_context_sdl.h"
#include "API/Display/color.h"
#include "API/Core/System/error.h"
#include "API/Core/System/log.h"
#include "API/Display/gradient.h"
#include "API/Display/pixel_format.h"

#include <SDL/SDL.h>

#ifdef HAVE_LIBSDL_GFX
#  include <SDL_gfxPrimitives.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_GraphicContext_SDL construction:

CL_GraphicContext_SDL::CL_GraphicContext_SDL(CL_DisplayWindow_Generic *window)
: window(window)
{
        cliprects.pop_back();
        cliprects.push_back(CL_Rect(0,0, window->get_width(), window->get_height()));
}

CL_GraphicContext_SDL::~CL_GraphicContext_SDL()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GraphicContext_SDL attributes:

int CL_GraphicContext_SDL::get_width() const
{
	return window->get_width();
}

int CL_GraphicContext_SDL::get_height() const
{
	return window->get_height();
}

CL_Color CL_GraphicContext_SDL::get_pixel(int x, int y) const
{
	//Lawouach : shamelessly taken from the SDL doc
	SDL_Surface *screen = SDL_GetVideoSurface();

	if(SDL_MUSTLOCK(screen)){
		if(SDL_LockSurface(screen) < 0){
			throw CL_Error(SDL_GetError());
		}
	}

	x += static_cast<int>(get_modelview().get_origin_x());
	y += static_cast<int>(get_modelview().get_origin_y());

	int bpp = screen->format->BytesPerPixel;

	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;
	Uint32 pixel;

	switch(bpp) {
	case 1:
		pixel = *p;
		break;
	case 2:
		pixel = *(Uint16 *)p;
		break;
	case 3:
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
			pixel = p[0] << 16 | p[1] << 8 | p[2];
		else
			pixel = p[0] | p[1] << 8 | p[2] << 16;
		break;
	case 4:
		pixel = *(Uint32 *)p;
		break;
	default:
		pixel = 0;
		break;
	}
	
	if(SDL_MUSTLOCK(screen)){
		SDL_UnlockSurface(screen);
	}

	Uint8 r,g,b,a;

	SDL_GetRGBA(pixel,screen->format,&r,&b,&g,&a);

	return CL_Color(r,g,b,a);
}

CL_PixelBuffer CL_GraphicContext_SDL::get_pixeldata(const CL_Rect& rect, int frame) const
{
	SDL_Surface *screen = SDL_GetVideoSurface();

	int width;
	if (rect.get_width() == 0)
		width = screen->w;
	else
		width = rect.get_width();

	int height;
	if (rect.get_height() == 0)
		height = screen->h;
	else
		height = rect.get_height();

	int pitch = width * (screen->format->BitsPerPixel/8);
	CL_PixelBuffer buffer(width, height, pitch,
								 CL_PixelFormat(screen->format->BitsPerPixel,
													 screen->format->Rmask,
													 screen->format->Gmask,
													 screen->format->Bmask,
													 screen->format->Amask));

	if(SDL_MUSTLOCK(screen))
		if(SDL_LockSurface(screen) < 0)
			throw CL_Error(SDL_GetError());

	buffer.lock();
	if (rect.get_width() == 0 && rect.get_height() == 0)
	{
		memcpy(buffer.get_data(), screen->pixels, sizeof(unsigned char) * screen->pitch * screen->h);
	} 
	else
	{
		unsigned char* buf = static_cast<unsigned char*>(buffer.get_data());

		for(int y = 0; y < buffer.get_height(); ++y)
		{
			memcpy(buf + (pitch * y),
					 static_cast<unsigned char*>(screen->pixels)
					 + (screen->pitch * (y + rect.top))
					 + (rect.left * screen->format->BitsPerPixel/8), 
					 sizeof(unsigned char) * pitch);
		}
	}
	buffer.unlock();

	if(SDL_MUSTLOCK(screen))
		SDL_UnlockSurface(screen);

	return buffer;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GraphicContext_SDL operations:

void CL_GraphicContext_SDL::draw_pixel(int x, int y, const CL_Color &color)
{
	//Lawouach : shamelessly taken from the SDL doc
	SDL_Surface *screen = SDL_GetVideoSurface();

	Uint32 pixel = SDL_MapRGBA(screen->format,color.get_red(),color.get_green(),color.get_blue(),color.get_alpha());

	if(SDL_MUSTLOCK(screen))
		if(SDL_LockSurface(screen) < 0)
			throw CL_Error(SDL_GetError());

	x += static_cast<int>(get_modelview().get_origin_x());
	y += static_cast<int>(get_modelview().get_origin_y());

	int bpp = screen->format->BytesPerPixel;

	// Here p is the address to the pixel we want to set
	Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;

	switch(bpp) {
	case 1:
		*p = pixel;
		break;
	case 2:
		*(Uint16 *)p = pixel;
		break; 
	case 3:
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
		} else {
			p[0] = pixel & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = (pixel >> 16) & 0xff;
		}
		break;
	case 4:
		*(Uint32 *)p = pixel;
		break;
	}

	if(SDL_MUSTLOCK(screen)){
		SDL_UnlockSurface(screen);
	}
}

void CL_GraphicContext_SDL::draw_line(float x1, float y1, float x2, float y2, const CL_Color &color)
{
	x1 += get_modelview().get_origin_x();
	y1 += get_modelview().get_origin_y();
	
	x2 += get_modelview().get_origin_x();
	y2 += get_modelview().get_origin_y();
	
#ifdef HAVE_LIBSDL_GFX
	lineRGBA(SDL_GetVideoSurface(), 
                 static_cast<int>(x1), static_cast<int>(y1),
                 static_cast<int>(x2), static_cast<int>(y2), 
                 color.get_red(), color.get_green(), color.get_blue(), color.get_alpha());
#else
	int w,h;
	(x2-x1) == 0 ? w = 1 : w = (int)(x2-x1);
	(y2-y1) == 0 ? h = 1 : h = (int)(y2-y1);

	SDL_Rect dest = {(int)x1,(int)y1,w,h};

	SDL_FillRect(SDL_GetVideoSurface(),&dest,
					 SDL_MapRGBA(SDL_GetVideoSurface()->format, 
									 color.get_red(), color.get_green(), color.get_blue(), color.get_alpha()));
#endif
}

void CL_GraphicContext_SDL::draw_lines(int count, double *vertices, const CL_Color &color)
{
	if (count <= 1)
		return;

	double origin_x = get_modelview().get_origin_x();
	double origin_y = get_modelview().get_origin_x();

	for (int i=0; i<count-1; i++)
	{
		int x1 = static_cast<int>(origin_x + vertices[count*2]);
		int y1 = static_cast<int>(origin_y + vertices[count*2]);
		int x2 = static_cast<int>(origin_x + vertices[count*2+1]);
		int y2 = static_cast<int>(origin_y + vertices[count*2+1]);

#ifdef HAVE_LIBSDL_GFX
		lineRGBA(SDL_GetVideoSurface(), x1, y1, x2, y2, 
					color.get_red(), color.get_green(), color.get_blue(), color.get_alpha());
#else
		int w,h;
		(x2-x1) == 0 ? w = 1 : w = x2-x1;
		(y2-y1) == 0 ? h = 1 : h = y2-y1;

		SDL_Rect dest = {x1,y1,w,h};

		SDL_FillRect(SDL_GetVideoSurface(),&dest,
						SDL_MapRGBA(SDL_GetVideoSurface()->format, 
										color.get_red(), color.get_green(), color.get_blue(), color.get_alpha()));
#endif
	}
}

void CL_GraphicContext_SDL::draw_rect(const CL_Rectf &rect, const CL_Color &color)
{
	int x,y;
	x = static_cast<int>(rect.left + get_modelview().get_origin_x());
	y = static_cast<int>(rect.top  + get_modelview().get_origin_y());

#ifdef HAVE_LIBSDL_GFX
	rectangleRGBA(SDL_GetVideoSurface(), x, y,
                      static_cast<int>(x + rect.get_width()-1), 
                      static_cast<int>(y + rect.get_height()-1),
                      color.get_red(), color.get_green(), color.get_blue(), color.get_alpha());
#else
	draw_line(rect.left,rect.top,rect.right - 1,rect.top,color);
	draw_line(rect.left,rect.bottom - 1,rect.right,rect.bottom - 1,color);
	draw_line(rect.left,rect.top,rect.left,rect.bottom - 1,color);
	draw_line(rect.right - 1,rect.top,rect.right - 1,rect.bottom - 1,color);
#endif
}

void CL_GraphicContext_SDL::fill_rect(const CL_Rectf &rect, const CL_Color &color)
{
	int x,y;
	x = static_cast<int>(rect.left + get_modelview().get_origin_x());
	y = static_cast<int>(rect.top  + get_modelview().get_origin_y());

#ifdef HAVE_LIBSDL_GFX
	boxRGBA(SDL_GetVideoSurface(), x, y, 
                static_cast<int>(x + rect.get_width()-1),
                static_cast<int>(y + rect.get_height()-1), 
                color.get_red(), color.get_green(), color.get_blue(), color.get_alpha());
#else
	SDL_Rect dest = {x,y,(int)rect.get_width(),(int)rect.get_height()};
	SDL_FillRect(SDL_GetVideoSurface(),&dest,
                     SDL_MapRGBA(SDL_GetVideoSurface()->format, 
                                 color.get_red(), color.get_green(), color.get_blue(), color.get_alpha()));
#endif
}

void CL_GraphicContext_SDL::fill_rect(const CL_Rectf &rect, const CL_Gradient &gradient)
{
	// todo: fill gradient rect with sdl
	CL_Log::log("debug","fill rect method with CL_Gradient is not yet implemented");
	// right now we just fill the rectangle with the top_left color and set a log message
	fill_rect(rect, CL_Color((gradient.top_left.get_red() + gradient.top_right.get_red() + gradient.bottom_left.get_red() + gradient.bottom_right.get_red())/4,
                                 (gradient.top_left.get_green() + gradient.top_right.get_green() + gradient.bottom_left.get_green() + gradient.bottom_right.get_green())/4,
                                 (gradient.top_left.get_blue() + gradient.top_right.get_blue() + gradient.bottom_left.get_blue() + gradient.bottom_right.get_blue())/4,
                                 (gradient.top_left.get_alpha() + gradient.top_right.get_alpha() + gradient.bottom_left.get_alpha() + gradient.bottom_right.get_alpha())/4));
}

void CL_GraphicContext_SDL::draw_quad(const CL_Quad &quad, const CL_Color &color)
{
	// todo: draw quad with sdl
	CL_Log::log("debug","draw quad method is not yet implemented");
	// right now we just fill the rectangle with the top_left color and set a log message
	draw_rect(quad.get_bounds(),color);
}

void CL_GraphicContext_SDL::fill_quad(const CL_Quad &quad, const CL_Gradient &gradient)
{
	// todo: fill gradient quad with sdl
	CL_Log::log("debug","fill quad method with CL_Gradient is not yet implemented");
	
	fill_rect(quad.get_bounds(),gradient);
}

void CL_GraphicContext_SDL::draw_triangles(CL_TriangleParams *params)
{
	CL_Log::log("debug", "draw_triangles method is not yet implemented");
}

void CL_GraphicContext_SDL::fill_quad(const CL_Quad &quad, const CL_Color &color)
{
// todo: fill quad with sdl
	CL_Log::log("debug","fill quad method is not yet implemented");
	
	fill_rect(quad.get_bounds(),color);
}

void CL_GraphicContext_SDL::clear(const CL_Color &color)
{
	SDL_FillRect(SDL_GetVideoSurface(),0,SDL_MapRGBA(SDL_GetVideoSurface()->format,color.get_red(),color.get_green(),color.get_blue(),color.get_alpha()));
}

/////////////////////////////////////////////////////////////////////////////
// CL_GraphicContext_SDL implementation:

void CL_GraphicContext_SDL::setup_2d()
{
}

void CL_GraphicContext_SDL::setup_3d()
{
}

void CL_GraphicContext_SDL::set_cliprect(const CL_Rect &rect)
{
	CL_GraphicContext_Generic::set_cliprect(rect);
        update_cliprect();
}

void CL_GraphicContext_SDL::push_cliprect(const CL_Rect &rect)
{
	CL_GraphicContext_Generic::push_cliprect(rect);
        update_cliprect();
}

void CL_GraphicContext_SDL::push_cliprect()
{
	CL_GraphicContext_Generic::push_cliprect();
        update_cliprect();
}

void CL_GraphicContext_SDL::pop_cliprect()
{
	CL_GraphicContext_Generic::pop_cliprect();
        update_cliprect();
}

void CL_GraphicContext_SDL::update_cliprect()
{
        CL_Rect rect = CL_GraphicContext_Generic::get_cliprect();
        if (rect.top == 0 && rect.bottom == 0 && rect.left == 0 && rect.right == 0)
        {
                SDL_SetClipRect(SDL_GetVideoSurface(),0);
        }
        else
        {
                SDL_Rect crect = {rect.left,rect.top,rect.get_width(),rect.get_height()};
                SDL_SetClipRect(SDL_GetVideoSurface(),&crect);                
	}
}

void CL_GraphicContext_SDL::modelview_changed()
{
}

void CL_GraphicContext_SDL::clear_stencil(int /*value*/)
{
}

void CL_GraphicContext_SDL::set_stencil_operation(CL_StencilOp /*pass*/, CL_StencilOp /*fail*/, CL_StencilFunc /*func*/, int /*ref*/)
{
}

void CL_GraphicContext_SDL::enable_stencil_write(bool /*enabled*/, bool /*visible*/, float /*alpha_limit*/)
{
}

void CL_GraphicContext_SDL::enable_stencil_test(bool /*enabled*/, CL_StencilFunc /*func*/, int /*ref*/)
{
}
