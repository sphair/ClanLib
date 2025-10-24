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
#include "display_window_sdl.h"
#include "sdl_target.h"
#include "surface_sdl.h"
#include "API/Core/System/error.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SDLTarget construction:


/////////////////////////////////////////////////////////////////////////////
// CL_SDLTarget attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_SDLTarget operations:

CL_DisplayWindow_Generic *CL_SDLTarget::create_window()
{
	return new CL_DisplayWindow_SDL;
}

CL_Surface_Target *CL_SDLTarget::create_surface(
	CL_PixelBuffer provider,
	int flag)
{
	return new CL_Surface_SDL(provider, flag);
}

CL_Canvas_Generic *CL_SDLTarget::create_canvas()
{
	throw CL_Error("Congratulations! You just got assigned the task of implementing CL_Canvas for clanSDL :-)");
	return 0;
}

CL_Font_Target *CL_SDLTarget::create_system_font_target(
	CL_Font_Generic *attributes,
	const std::string &font_name,
	const std::string &letters,
	int height,
	int width,
	bool bold,
	bool italic,
	bool underline,
	bool strikeout)
{
	throw CL_Error("Congratulations! You just got assigned the task of implementing system fonts for clanSDL :-)");
	return 0;
}

bool CL_SDLTarget::enable_packer()
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLTarget implementation:
