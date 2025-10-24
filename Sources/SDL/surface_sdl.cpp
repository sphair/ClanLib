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
#include "surface_sdl.h"
#include "API/Display/pixel_buffer.h"
#include "API/Display/palette.h"
#include "API/Display/graphic_context.h"
#include "API/Display/pixel_format.h"
#include "API/Core/System/error.h"
#include "API/Core/System/log.h"
#include "API/core.h"

#ifdef HAVE_LIBSDL_GFX
#  include <SDL_rotozoom.h>
#else
#  include "API/Core/System/log.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_Surface_SDL construction:

CL_Surface_SDL::CL_Surface_SDL(
	CL_PixelBuffer provider,
	int flag)
{
	// TODO: Use flag
	surface = 0;
	SDL_Surface *tmp = 0;

	provider.lock();

	tmp = SDL_CreateRGBSurfaceFrom(
		provider.get_data(),
		provider.get_width(),
		provider.get_height(),
		provider.get_format().get_depth(),
		provider.get_pitch(),
		provider.get_format().get_red_mask(),
		provider.get_format().get_green_mask(),
		provider.get_format().get_blue_mask(),
		provider.get_format().get_alpha_mask());

	if(tmp == 0)
		throw CL_Error(SDL_GetError());

	if(tmp->format->BitsPerPixel == 8)
	{
		const CL_Color *palette_colors = provider.get_palette().colors;
		tmp->format->palette->ncolors = 256;
		int i;
		for(i = 0; i < 256; i++)
		{
			tmp->format->palette->colors[i].r = palette_colors[i].get_red();
			tmp->format->palette->colors[i].g = palette_colors[i].get_green();
			tmp->format->palette->colors[i].b = palette_colors[i].get_blue();
			tmp->format->palette->colors[i].unused = 0;
		}

		surface = SDL_CreateRGBSurface(
			SDL_SWSURFACE,
			provider.get_width(),
			provider.get_height(),
			provider.get_format().get_depth(),
			provider.get_format().get_red_mask(),
			provider.get_format().get_green_mask(),
			provider.get_format().get_blue_mask(),
			provider.get_format().get_alpha_mask());
		
		surface->format->palette->ncolors = 256;
		for(i = 0; i < 256; i++)
		{
			surface->format->palette->colors[i].r = palette_colors[i].get_red();
			surface->format->palette->colors[i].g = palette_colors[i].get_green();
			surface->format->palette->colors[i].b = palette_colors[i].get_blue();
			surface->format->palette->colors[i].unused = 0;
		}
		
		SDL_BlitSurface(tmp,0,surface,0);
		SDL_FreeSurface(tmp);
		/* Uncomment this if you want to convert 8bits surfaces to screen format display
		   The conversion itself is slow though once it is done it increases the blitting performance
		tmp = SDL_DisplayFormat(surface);
		SDL_FreeSurface(surface);
		surface = tmp;
		*/
	}
	else
	{
/*		if(tmp->format->BitsPerPixel == 32)
		{
			// Detect what level of alpha we require if application didn't tell us:
			if (hint == CL_Surface::hint_automatic)
			{
				bool found_colorkey = false;
				bool found_alpha = false;
				
				unsigned char *data = (unsigned char *) provider.get_data();
				for (int y=0; y<provider.get_height(); y++)
				{
					for (int x=0; x<provider.get_width(); x++)
					{
						unsigned char alpha = data[x*4+3];
						if (alpha == 0) found_colorkey = true;
						else if (alpha != 255) found_alpha = true;
					}
					data += provider.get_pitch();
				}
				
				if (found_alpha) hint = CL_Surface::hint_alpha;
				else if (found_colorkey) hint = CL_Surface::hint_colorkey;
				else hint = CL_Surface::hint_opaque;
			}
			switch (hint)
			{
			case CL_Surface::hint_alpha:
				mode = SDL_SRCALPHA|SDL_HWSURFACE;
				break;
			case CL_Surface::hint_colorkey:
				mode = SDL_SRCCOLORKEY|SDL_HWSURFACE;
				break;
			case CL_Surface::hint_opaque:
				mode = SDL_SRCALPHA|SDL_HWSURFACE;
				break;
			case CL_Surface::hint_automatic:
				break;
			}
		}
		SDL_ConvertSurface(tmp,tmp->format,mode);
*/
		surface = SDL_DisplayFormatAlpha(tmp);
		SDL_FreeSurface(tmp);
	}

	if(provider.get_format().has_colorkey())
		SDL_SetColorKey(surface,SDL_RLEACCEL|SDL_SRCCOLORKEY,provider.get_format().get_colorkey());
}

CL_Surface_SDL::~CL_Surface_SDL()
{
	if(surface != 0)
		SDL_FreeSurface(surface);
}

/////////////////////////////////////////////////////////////////////////////
// CL_Surface_SDL attributes:

int CL_Surface_SDL::get_width() const
{
	return surface->w;
}
	
int CL_Surface_SDL::get_height() const
{
	return surface->h;
}

CL_PixelBuffer CL_Surface_SDL::get_pixeldata()
{
	// FIXME: No locking here and we might either need to copy the data
	// or write a CL_SDLPixelBuffor that takes care of the locking
	if (surface->format->BitsPerPixel != 8)
	{ 
		CL_PixelFormat format(surface->format->BitsPerPixel, 
									 surface->format->Rmask,
									 surface->format->Gmask,
									 surface->format->Bmask,
									 surface->format->Amask,
									 surface->flags & SDL_SRCCOLORKEY,
									 surface->format->colorkey,
									 pixelformat_rgba);
		
		CL_PixelBuffer buffer(surface->w, surface->h, surface->pitch, format);
		SDL_LockSurface(surface);
		buffer.lock();
		memcpy(buffer.get_data(), surface->pixels, surface->h * surface->pitch);
		buffer.unlock();
		SDL_UnlockSurface(surface);
		return buffer;
	}
	else
	{
		CL_Palette palette;
		SDL_Palette* sdl_pal = surface->format->palette;
		
		for(int i = 0; i < sdl_pal->ncolors; ++i)
		{
			palette.colors[i].set_red(sdl_pal->colors[i].r);
			palette.colors[i].set_green(sdl_pal->colors[i].g);
			palette.colors[i].set_blue(sdl_pal->colors[i].b);
		}		

		CL_PixelFormat format(surface->format->BitsPerPixel, 
									 surface->format->Rmask,
									 surface->format->Gmask,
									 surface->format->Bmask,
									 surface->format->Amask,
									 surface->flags & SDL_SRCCOLORKEY,
									 surface->format->colorkey,
									 pixelformat_index);
		
		CL_PixelBuffer buffer(surface->w, surface->h, surface->w*4, format, palette);
		SDL_LockSurface(surface);
		buffer.lock();
		memcpy(buffer.get_data(), surface->pixels, surface->h * surface->pitch);
		buffer.unlock();
		SDL_UnlockSurface(surface);
		return buffer;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_Surface_SDL operations:

void CL_Surface_SDL::set_pixeldata(
	const CL_Point &pos,
	const CL_Rect &src_rect,
	CL_PixelBuffer &pixeldata)
{
}

void CL_Surface_SDL::draw(
	CL_Surface_Generic *attributes,
	const CL_Surface_DrawParams1& params1,
	CL_GraphicContext *gc)
{
	// todo: move SDL code from other draws to this one.
}

void CL_Surface_SDL::draw(
	CL_Surface_Generic *attributes,
	const CL_Surface_DrawParams2& params2,
	CL_Surface_TargetDrawParams1 *t_params1,
	CL_GraphicContext *context)
{
	SDL_Rect source = {params2.srcX, params2.srcY, params2.srcWidth, params2.srcHeight};

	SDL_Rect dst = {
		static_cast<Sint16>(t_params1->pixDestX + context->get_modelview().get_origin_x()), 
		static_cast<Sint16>(t_params1->pixDestY + context->get_modelview().get_origin_y()), 
		static_cast<Sint16>(t_params1->destWidth), 
		static_cast<Sint16>(t_params1->destHeight) 
	};

	if (t_params1->destWidth == params2.srcWidth && t_params1->destHeight == params2.srcHeight)
	{
		if(surface->format->BitsPerPixel > 8)
		{
			Uint8 a = (Uint8)(attributes->alpha * 255);
			SDL_SetAlpha(surface,SDL_RLEACCEL|SDL_SRCALPHA,a);
		}
		SDL_BlitSurface(surface,&source,SDL_GetVideoSurface(),&dst);
	}
	else
	{
#ifdef HAVE_LIBSDL_GFX
		SDL_Surface *zoom = zoomSurface (surface, t_params1->destWidth/params2.srcWidth, t_params1->destHeight/params2.srcHeight, SMOOTHING_ON);

		if(zoom->format->BitsPerPixel > 8)
		{
			Uint8 a = (Uint8)(attributes->alpha * 255);
			SDL_SetAlpha(zoom ,SDL_RLEACCEL|SDL_SRCALPHA,a);
		}
		SDL_BlitSurface(zoom,&source,SDL_GetVideoSurface(),&dst);

		SDL_FreeSurface(zoom);
#else
		CL_Log::log("debug", 
						"Error: clanSDL not compiled with SDL_gfx library, zooming not possible.\n"
						"Error: see: http://www.ferzkopp.net/~aschiffler/Software/SDL_gfx-2.0/\n");
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_Surface_SDL implementation:
