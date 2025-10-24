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
#include "surface_target_opengl.h"
#include "gl_state_checker.h"
#include "graphic_context_opengl.h"
#include "API/Display/pixel_buffer.h"
#include "API/Display/graphic_context.h"
#include "API/Display/pixel_format.h"
#include "API/Display/display.h"
#include "API/Display/display_window.h"
#include "API/Core/System/error.h"
#include "API/Core/IOData/cl_endian.h"
#include "API/GL/opengl.h"
#include "API/GL/opengl_state.h"
#include "API/GL/opengl_wrap.h"
#include "canvas_opengl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Surface_Target_OpenGL construction:

CL_Surface_Target_OpenGL::CL_Surface_Target_OpenGL(
	CL_PixelBuffer provider,
	int flag)
{
	selected_canvas = 0;

	if(flag & CL_Surface::flag_keep_pixelbuffer)
	{
		original_pixelbuffer = provider;
	}

	CL_OpenGLState state(CL_Display::get_current_window()->get_gc());
	state.set_active();
	state.setup_2d();
	clEnable(CL_TEXTURE_2D);

	surface_size.width  = provider.get_width();
	surface_size.height = provider.get_height();
	
	texture_size.width = 1;
	texture_size.height = 1;
	while (texture_size.width < surface_size.width) texture_size.width *= 2;
	while (texture_size.height < surface_size.height) texture_size.height *= 2;

	// check out if the original texture needs or doesn't need an alpha channel
	bool needs_alpha = provider.get_format().get_alpha_mask() || provider.get_format().has_colorkey();

	CLint internal_format;
	CLenum format;
	CLenum type;

	// we must get the format and type correct here, since they must match
	// the ones passed to clTexSubImage2D() when the actual upload happens!
	bool conv_needed = !CL_OpenGL::to_opengl_pixelformat(provider.get_format(), format, type);

	// also check for the pitch (OpenGL can only skip pixels, not bytes)
	if (!conv_needed)
	{
		const int bytesPerPixel = (provider.get_format().get_depth() + 7) / 8;
		if (provider.get_pitch() % bytesPerPixel != 0)
			conv_needed = true;
	}

	// and determine the actual formats and type
	if (!conv_needed)
	{
		// type and format have been set by to_opengl_pixelformat()
		switch (format)
		{
			case CL_RED:
			case CL_GREEN:
			case CL_BLUE:
			case CL_ALPHA:
				internal_format = 1;
				break;
			default:
				internal_format = needs_alpha ? CL_RGBA : CL_RGB;
		}
	}
	else
	{
		internal_format = needs_alpha ? CL_RGBA : CL_RGB;
		format = needs_alpha ? CL_RGBA : CL_RGB;
		type = CL_UNSIGNED_BYTE;
	}

	// Upload to OpenGL:
	clGenTextures(1, &handle);
	clBindTexture(CL_TEXTURE_2D, handle);

	// create
	clTexImage2D(
		CL_TEXTURE_2D,            // target
		0,                        // level
		internal_format,          // internalformat
		texture_size.width,       // width
		texture_size.height,      // height
		0,                        // border
		format,                   // format
		type,                     // type
		0);                       // texels (0 to avoid uploading)

	set_pixeldata(CL_Point(0, 0), CL_Rect(0, 0, provider.get_width(), provider.get_height()), provider);

	clTexParameteri(CL_TEXTURE_2D, CL_TEXTURE_MIN_FILTER, CL_LINEAR);
	clTexParameteri(CL_TEXTURE_2D, CL_TEXTURE_MAG_FILTER, CL_LINEAR);
	clTexParameteri(CL_TEXTURE_2D, CL_TEXTURE_WRAP_S, CL_CLAMP_TO_EDGE);
	clTexParameteri(CL_TEXTURE_2D, CL_TEXTURE_WRAP_T, CL_CLAMP_TO_EDGE);
}

CL_Surface_Target_OpenGL::CL_Surface_Target_OpenGL(
	const CL_PixelFormat &pf,
	const CL_Size &size)
{
	selected_canvas = 0;

	CL_OpenGLState state(CL_Display::get_current_window()->get_gc());
	state.set_active();
	state.setup_2d();
	clEnable(CL_TEXTURE_2D);

	surface_size.width = size.width;
	surface_size.height = size.height;
	
	texture_size.width = 1;
	texture_size.height = 1;
	while (texture_size.width < surface_size.width) texture_size.width *= 2;
	while (texture_size.height < surface_size.height) texture_size.height *= 2;

	// check out if the original texture needs or doesn't need an alpha channel
	bool needs_alpha = pf.get_alpha_mask() || pf.has_colorkey();

	CLint internal_format;
	CLenum format;
	CLenum type;

	// we must get the format and type correct here, since they must match
	// the ones passed to clTexSubImage2D() when the actual upload happens!
	bool conv_needed = !CL_OpenGL::to_opengl_pixelformat(pf, format, type);

	// and determine the actual formats and type
	if (!conv_needed)
	{
		// type and format have been set by to_opengl_pixelformat()
		switch (format)
		{
			case CL_RED:
			case CL_GREEN:
			case CL_BLUE:
			case CL_ALPHA:
				internal_format = 1;
				break;
			default:
				internal_format = needs_alpha ? CL_RGBA : CL_RGB;
		}
	}
	else
	{
		internal_format = needs_alpha ? CL_RGBA : CL_RGB;
		format = needs_alpha ? CL_RGBA : CL_RGB;
		type = CL_UNSIGNED_BYTE;
	}

	// Upload to OpenGL:
	clGenTextures(1, &handle);
	clBindTexture(CL_TEXTURE_2D, handle);

	// create
	clTexImage2D(
		CL_TEXTURE_2D,            // target
		0,                        // level
		internal_format,          // internalformat
		texture_size.width,       // width
		texture_size.height,      // height
		0,                        // border
		format,                   // format
		type,                     // type
		0);                       // texels (0 to avoid uploading)

	clTexParameteri(CL_TEXTURE_2D, CL_TEXTURE_MIN_FILTER, CL_LINEAR);
	clTexParameteri(CL_TEXTURE_2D, CL_TEXTURE_MAG_FILTER, CL_LINEAR);
	clTexParameteri(CL_TEXTURE_2D, CL_TEXTURE_WRAP_S, CL_CLAMP_TO_EDGE);
	clTexParameteri(CL_TEXTURE_2D, CL_TEXTURE_WRAP_T, CL_CLAMP_TO_EDGE);
}

CL_Surface_Target_OpenGL::~CL_Surface_Target_OpenGL()
{
	// Make sure context exists
	if (CL_OpenGL::functions == 0) 
		return;
	CL_OpenGLState state(CL_Display::get_current_window()->get_gc());
	state.set_active();

	clDeleteTextures(1, &handle);
}

/////////////////////////////////////////////////////////////////////////////
// CL_Surface_Target_OpenGL attributes:

int CL_Surface_Target_OpenGL::get_width() const
{
	return surface_size.width;
}
	
int CL_Surface_Target_OpenGL::get_height() const
{
	return surface_size.height;
}

CL_PixelBuffer CL_Surface_Target_OpenGL::get_pixeldata()
{
	if (original_pixelbuffer && !selected_canvas)
		return original_pixelbuffer;

	if (selected_canvas) selected_canvas->sync_texture();

	CL_OpenGLState state(CL_Display::get_current_window()->get_gc());
	state.set_active();
	state.setup_2d();
	clEnable(CL_TEXTURE_2D);
	clBindTexture(CL_TEXTURE_2D, handle);

	CL_PixelFormat abgr_format;
	abgr_format.set_depth(32);
	abgr_format.enable_colorkey(false);
	abgr_format.set_alpha_mask(0xFF000000);
	abgr_format.set_blue_mask (0x00FF0000);
	abgr_format.set_green_mask(0x0000FF00);
	abgr_format.set_red_mask  (0x000000FF);

	CL_PixelBuffer buffer(
		texture_size.width, texture_size.height,
		texture_size.width * 4,
		abgr_format);

	buffer.lock();
	clGetTexImage(CL_TEXTURE_2D, 0, CL_RGBA, CL_UNSIGNED_BYTE, buffer.get_data());
	buffer.unlock();

	return buffer;
}
	
CL_GraphicContext *CL_Surface_Target_OpenGL::get_gc()
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Surface_Target_OpenGL operations:

void CL_Surface_Target_OpenGL::set_pixeldata(
	const CL_Point &pos,
	const CL_Rect &src_rect,
	CL_PixelBuffer &pixeldata)
{
	// check the coordinates
	{
		CL_Point p1(pos.x + src_rect.left, pos.y + src_rect.top);
		CL_Point p2(p1.x + src_rect.get_width() - 1, p1.y + src_rect.get_height() - 1);
		CL_Rect r(CL_Point(0, 0), texture_size);
		if (!r.is_inside(p1) || !r.is_inside(p2))
			throw CL_Error("The destination rectangle cannot be outside the texture rectangle");
	}

	if (selected_canvas) selected_canvas->sync_texture();

	CL_OpenGLState state(CL_Display::get_current_window()->get_gc());
	state.set_active();
	state.setup_2d();
	clEnable(CL_TEXTURE_2D);

	// check out if the original texture needs or doesn't need an alpha channel
	bool needs_alpha = pixeldata.get_format().get_alpha_mask() || pixeldata.get_format().has_colorkey();

	CLenum format;
	CLenum type;
	bool conv_needed = !CL_OpenGL::to_opengl_pixelformat(pixeldata.get_format(), format, type);

	// also check for the pitch (OpenGL can only skip pixels, not bytes)
	if (!conv_needed)
	{
		const int bytesPerPixel = (pixeldata.get_format().get_depth() + 7) / 8;
		if (pixeldata.get_pitch() % bytesPerPixel != 0)
			conv_needed = true;
	}

	// no conversion needed
	if (!conv_needed)
	{
		pixeldata.lock();

		// Upload to OpenGL:
		clBindTexture(CL_TEXTURE_2D, handle);

		// change alignment
		clPixelStorei(CL_UNPACK_ALIGNMENT, 1);
		const int bytesPerPixel = (pixeldata.get_format().get_depth() + 7) / 8;
		clPixelStorei(CL_UNPACK_ROW_LENGTH, pixeldata.get_pitch() / bytesPerPixel);

		char *data = (char *)pixeldata.get_data() + (src_rect.top * pixeldata.get_pitch()) + (src_rect.left * bytesPerPixel);

		// upload
		clTexSubImage2D(
			CL_TEXTURE_2D,            // target
			0,                        // level
			0, 0,                     // xoffset, yoffset
			src_rect.get_width(),     // width
			src_rect.get_height(),    // height
			format,					  // format
			type,					  // type
			data);                    // texels

		pixeldata.unlock();
	}
	// conversion needed
	else
	{
		bool big_endian = CL_Endian::is_system_big();

		CL_PixelBuffer buffer;
		if (!big_endian)
			buffer = CL_PixelBuffer(
				src_rect.get_width(), src_rect.get_height(),
				src_rect.get_width() * (needs_alpha ? 4 : 3),
				needs_alpha ? CL_PixelFormat::abgr8888 : CL_PixelFormat::bgr888); // OpenGL RGB/RGBA is always big endian
		else
			buffer = CL_PixelBuffer(
				src_rect.get_width(), src_rect.get_height(),
				src_rect.get_width() * (needs_alpha ? 4 : 3),
				needs_alpha ? CL_PixelFormat::rgba8888 : CL_PixelFormat::rgb888);
	
		pixeldata.convert(buffer);

		format = needs_alpha ? CL_RGBA : CL_RGB;

		buffer.lock();	

		// Upload to OpenGL:
		clBindTexture(CL_TEXTURE_2D, handle);

		// change alignment
		clPixelStorei(CL_UNPACK_ALIGNMENT, 1);
		const int bytesPerPixel = (buffer.get_format().get_depth() + 7) / 8;
		clPixelStorei(CL_UNPACK_ROW_LENGTH, buffer.get_pitch() / bytesPerPixel);

		// upload
		clTexSubImage2D(
			CL_TEXTURE_2D,            // target
			0,                        // level
			0, 0,                     // xoffset, yoffset
			src_rect.get_width(),     // width
			src_rect.get_height(),    // height
			format,                   // format
			CL_UNSIGNED_BYTE,         // type
			buffer.get_data());      // texels

		buffer.unlock();
	}

#ifdef __APPLE__
        // seems there's either a bug or some kind of strange quirk on OSX
        // where the textures can become corrupted at times - this flush call
        // fixes it.  it seems very wrong.  -- SMH sep 24 2005
        clFlush();
#endif

#ifdef __APPLE__
        // seems there's either a bug or some kind of strange quirk on OSX
        // where the textures can become corrupted at times - this flush call
        // fixes it.  it seems very wrong.  -- SMH sep 27 2005
        clFlush();
#endif

	// Is this really needed according to OpenGL standard? (already set in constructor)
	// Think its a Linux OpenGL driver bug.. -- MBN 25. jan 2004
	clTexParameteri(CL_TEXTURE_2D, CL_TEXTURE_MIN_FILTER, CL_LINEAR);
	clTexParameteri(CL_TEXTURE_2D, CL_TEXTURE_MAG_FILTER, CL_LINEAR);
	clTexParameteri(CL_TEXTURE_2D, CL_TEXTURE_WRAP_S, CL_CLAMP_TO_EDGE);
	clTexParameteri(CL_TEXTURE_2D, CL_TEXTURE_WRAP_T, CL_CLAMP_TO_EDGE);

	if (selected_canvas) selected_canvas->sync_pbuffer();
}

void CL_Surface_Target_OpenGL::draw(
	CL_Surface_Generic *attributes,
	const CL_Surface_DrawParams1& params1,
	CL_GraphicContext *gc)
{
	if (selected_canvas) selected_canvas->sync_texture();

	state.blendfunc[0] = params1.blend_src;
	state.blendfunc[1] = params1.blend_dest;
	state.blendfunc[2] = params1.blendfunc_src_alpha;
	state.blendfunc[3] = params1.blendfunc_dest_alpha;
	state.color[0] = float(params1.red[0]);
	state.color[1] = float(params1.green[0]);
	state.color[2] = float(params1.blue[0]);
	state.color[3] = float(params1.alpha[0]);
	state.texture_handle = handle;
	state.fillmode = fillmode_solid;
	state.sub_pixel_accuracy = params1.sub_pixel_accuracy;

	CL_GLStateChecker::from_gc(gc)->add_quad(
		state,
		params1.destX,
		params1.destY,
		params1.srcX,
		params1.srcY);
}

int CL_Surface_Target_OpenGL::get_handle()
{
	return (int) handle;
}


/////////////////////////////////////////////////////////////////////////////
// CL_Surface_Target_OpenGL implementation:
