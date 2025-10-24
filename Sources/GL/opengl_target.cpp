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
#include "opengl_target.h"
#ifdef WIN32
// #include "Display/Win32/font_win32.h"
#include "WGL/display_window_opengl.h"
#include "API/Display/Providers/dc_buffer.h"
#include "API/Display/sprite_description.h"
#include "API/Display/pixel_format.h"
#else
#ifdef __APPLE__
#include "AGL/display_window_opengl.h"
#else
// #include "GLX/font_x11.h"
#include "GLX/display_window_opengl.h"
#endif
#endif
#include "surface_target_opengl.h"
#include "canvas_opengl.h"
#include "Display/font_target_sprite.h"

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLTarget construction:


/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLTarget attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLTarget operations:

CL_DisplayWindow_Generic *CL_OpenGLTarget::create_window()
{
	return new CL_DisplayWindow_OpenGL;
}

CL_Surface_Target *CL_OpenGLTarget::create_surface(
	CL_PixelBuffer provider,
	int flag)
{
	return new CL_Surface_Target_OpenGL(provider, flag);
}

CL_Canvas_Generic *CL_OpenGLTarget::create_canvas()
{
	return new CL_Canvas_OpenGL;
}

CL_Font_Target* CL_OpenGLTarget::create_system_font_target(
	CL_Font_Generic *attributes,
	const std::string &font_name,
	const std::string &letters,
	int font_height,
	int font_width,
	bool bold,
	bool italic,
	bool underline,
	bool strikeout)
{
#ifdef WIN32
	// Create the font we need:
	HFONT font = CreateFont(
		font_height,
		font_width,
		0,
		0,
		bold ? FW_BOLD : FW_NORMAL,
		italic ? TRUE : FALSE,
		underline ? TRUE : FALSE,
		strikeout ? TRUE : FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		FF_DONTCARE,
		font_name.c_str());

	const int num_chars = letters.length();
	int i;
	int total_width = 0;
	int pos = 0;
	HDC screen_dc;
	HFONT old_font;
	INT widths[256];

	// Get the glyph widths:
	screen_dc = GetDC(0);
	old_font = (HFONT) SelectObject(screen_dc, font);
	GetCharWidth32(screen_dc, 0, 255, widths);
	SelectObject(screen_dc, old_font);

	// Figure out how big our device context has to be:
	for (i=0; i<num_chars; i++)
	{
		total_width += widths[static_cast<unsigned char>(letters[i])];
	}

	// Create device context, set it up for glyph drawing:
	CL_DeviceContextBuffer dc_buffer(screen_dc, total_width, font_height, true);
	RECT rc = { 0, 0, total_width, font_height };
	SetBkColor(dc_buffer.get_dc(), RGB(0,0,0));
	FillRect(dc_buffer.get_dc(), &rc, (HBRUSH) GetStockObject(BLACK_BRUSH));
	SetTextColor(dc_buffer.get_dc(), RGB(255,255,255));
	SetBkMode(dc_buffer.get_dc(), OPAQUE);
	old_font = (HFONT) SelectObject(dc_buffer.get_dc(), font);

	// Draw each glyph:
	for (i=0; i<num_chars; i++)
	{
		const unsigned char cur = static_cast<unsigned char>(letters[i]);
		TextOut(dc_buffer.get_dc(), pos, 0, (char *)(&cur), 1);
		pos += widths[cur];
	}
	SelectObject(dc_buffer.get_dc(), old_font);
	DeleteObject(font);

	// Add each glyph as a frame:
	CL_SpriteDescription desc;
	pos = 0;
	for (i=0; i<num_chars; i++)
	{
		const int character = static_cast<unsigned char>(letters[i]);
		desc.add_gridclipped_frames(dc_buffer, pos, 0, widths[character], font_height, 1, 1);
		pos += widths[character];
	}

	// Set up the actual font data:
	return new CL_Font_Target_Sprite(
		attributes,
		CL_Sprite(desc),
		letters);
#else
	return 0;
/*		new CL_Font_X11(
		attributes,
		font_name,
		letters,
		height,
		width,
		bold,
		italic,
		underline,
		strikeout);
*/
#endif
}

bool CL_OpenGLTarget::enable_packer()
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLTarget implementation:
