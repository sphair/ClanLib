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
#include "pixel_buffer_opengl_texture.h"
#include "Display/graphic_context_generic.h"
#include "API/GL/opengl_state.h"
#include "API/GL/opengl_wrap.h"
#include "gl_state_checker.h"

/////////////////////////////////////////////////////////////////////////////
// CL_PixelBuffer_OpenGL_Texture construction:

CL_PixelBuffer_OpenGL_Texture::CL_PixelBuffer_OpenGL_Texture(
	int offset_x,
	int offset_y,
	int width,
	int height,
	CLuint handle,
	CL_GraphicContext *gc)
: offset_x(offset_x), offset_y(offset_y), handle(handle), gc(gc), data(0)
{
	CL_PixelBuffer_Generic::width = width;
	CL_PixelBuffer_Generic::height = height;

	format.set_depth(32);
	format.set_red_mask  (0x000000ff);
	format.set_green_mask(0x0000ff00);
	format.set_blue_mask (0x00ff0000);
	format.set_alpha_mask(0xff000000);
	format.enable_colorkey(false);

	// todo: Use CL_TEXTURE_INTERNAL_FORMAT to use the native format of texture instead.

	CL_OpenGLState state(gc);
	state.set_active();
	clEnable(CL_TEXTURE_2D);
	clBindTexture(CL_TEXTURE_2D, handle);
	
	CLint internal_format = 0;

	clGetTexLevelParameteriv(CL_TEXTURE_2D, 0, CL_TEXTURE_WIDTH, &texture_width);
	clGetTexLevelParameteriv(CL_TEXTURE_2D, 0, CL_TEXTURE_HEIGHT, &texture_height);
	clGetTexLevelParameteriv(CL_TEXTURE_2D, 0, CL_TEXTURE_INTERNAL_FORMAT, &internal_format);
	
	CLint align;
	clGetIntegerv(CL_PACK_ALIGNMENT, &align);
	pitch = (texture_width*4+align-1)/align;
	pitch *= align;
}

CL_PixelBuffer_OpenGL_Texture::~CL_PixelBuffer_OpenGL_Texture()
{
	delete[] data;
}

/////////////////////////////////////////////////////////////////////////////
// CL_PixelBuffer_OpenGL_Texture operations:

void *CL_PixelBuffer_OpenGL_Texture::get_data()
{
	return data+offset_x*4+offset_y*pitch;
}

void CL_PixelBuffer_OpenGL_Texture::perform_lock()
{
	data = new char[pitch*texture_height];

	CL_OpenGLState state(gc);
	state.set_active();
	state.setup_2d();
	clEnable(CL_TEXTURE_2D);
	clBindTexture(CL_TEXTURE_2D, handle);
	clGetTexImage(CL_TEXTURE_2D, 0, CL_RGBA, CL_UNSIGNED_BYTE, data);
}

void CL_PixelBuffer_OpenGL_Texture::perform_unlock()
{
	CL_OpenGLState state(gc);
	state.set_active();
	state.setup_2d();
	clEnable(CL_TEXTURE_2D);
	clBindTexture(CL_TEXTURE_2D, handle);
	clTexSubImage2D(
		CL_TEXTURE_2D,          // target
		0,                      // level
		CL_RGBA,                // internalformat
		texture_width,          // width
		texture_height,         // height
		0,                      // border
		CL_RGBA,                // format
		CL_UNSIGNED_BYTE,       // type
		data);                  // texels

	delete[] data;
	data = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_PixelBuffer_OpenGL_Texture Implementation:
