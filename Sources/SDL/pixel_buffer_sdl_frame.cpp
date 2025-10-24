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
#include "pixel_buffer_sdl_frame.h"
#include "Display/graphic_context_generic.h"
#include "API/Core/System/error.h"

#include <SDL/SDL.h>

/////////////////////////////////////////////////////////////////////////////
// CL_PixelBuffer_SDL_Frame construction:

CL_PixelBuffer_SDL_Frame::CL_PixelBuffer_SDL_Frame(int frame, CL_GraphicContext &gc)
: frame(frame), gc(gc)
{
	SDL_Surface *screen = SDL_GetVideoSurface();
	format.set_depth(screen->format->BitsPerPixel);
	format.set_red_mask  (screen->format->Rmask);
	format.set_green_mask(screen->format->Gmask);
	format.set_blue_mask (screen->format->Bmask);
	format.set_alpha_mask(screen->format->Amask);
	format.enable_colorkey(false);

	width = screen->w;
	height = screen->h;
	pitch = screen->pitch;
}

CL_PixelBuffer_SDL_Frame::~CL_PixelBuffer_SDL_Frame()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_PixelBuffer_SDL_Frame operations:

void *CL_PixelBuffer_SDL_Frame::get_data()
{
	return SDL_GetVideoSurface();
}

void CL_PixelBuffer_SDL_Frame::perform_lock()
{
	if(SDL_MUSTLOCK(SDL_GetVideoSurface()))
		if(SDL_LockSurface(SDL_GetVideoSurface()) < 0)
			throw CL_Error(SDL_GetError());
}

void CL_PixelBuffer_SDL_Frame::perform_unlock()
{
	if(SDL_MUSTLOCK(SDL_GetVideoSurface())){
		SDL_UnlockSurface(SDL_GetVideoSurface());
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_PixelBuffer_SDL_Frame implementation:
