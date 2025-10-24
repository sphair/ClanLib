/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
**    Harry Storbacka
*/

#include "GL/precomp.h"
#include "opengl_texture_provider.h"
#include "opengl_graphic_context_provider.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/Render/texture.h"
#include "API/Display/Render/shared_gc_data.h"
#include "API/GL/opengl_wrap.h"
#include "API/GL/opengl.h"
#include "API/Core/IOData/cl_endian.h"
#include "API/Core/System/databuffer.h"
#ifndef WIN32
#include "API/Core/Text/console.h"
#endif

#include "opengl_pixel_buffer_provider.h"

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLTextureProvider Construction:

CL_OpenGLTextureProvider::CL_OpenGLTextureProvider(CL_OpenGLGraphicContextProvider *gc_provider, CL_TextureDimensions texture_dimensions)
: width(0), height(0), depth(0), handle(0), texture_type(0)
{
	CL_SharedGCData::add_disposable(this);
	switch (texture_dimensions)
	{
	case cl_texture_1d:
		texture_type = CL_TEXTURE_1D;
		break;
	case cl_texture_2d:
	default:
		texture_type = CL_TEXTURE_2D;
		break;
	case cl_texture_3d:
		texture_type = CL_TEXTURE_3D;
		break;
	case cl_texture_cube_map:
		texture_type = CL_TEXTURE_CUBE_MAP;
		break;
	}

	CL_TextureStateTracker state_tracker(texture_type, 0, gc_provider);
	clGenTextures(1, &handle);
	clBindTexture(texture_type, handle);
	clTexParameteri(texture_type, CL_TEXTURE_MIN_FILTER, CL_LINEAR);
	clTexParameteri(texture_type, CL_TEXTURE_MAG_FILTER, CL_LINEAR);
	clTexParameteri(texture_type, CL_TEXTURE_WRAP_S, CL_CLAMP_TO_EDGE);
	if (texture_type != CL_TEXTURE_1D)
		clTexParameteri(texture_type, CL_TEXTURE_WRAP_T, CL_CLAMP_TO_EDGE);
	if (texture_type == CL_TEXTURE_3D)
		clTexParameteri(texture_type, CL_TEXTURE_WRAP_R, CL_CLAMP_TO_EDGE);
}

CL_OpenGLTextureProvider::~CL_OpenGLTextureProvider()
{
	dispose();
	CL_SharedGCData::remove_disposable(this);
}

void CL_OpenGLTextureProvider::on_dispose()
{
	if (handle)
	{
		if (CL_OpenGL::set_active())
		{
			clDeleteTextures(1, &handle);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLTextureProvider Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLTextureProvider Operations:

void CL_OpenGLTextureProvider::generate_mipmap()
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle, NULL);
	clGenerateMipmap(texture_type);
}

void CL_OpenGLTextureProvider::create(int new_width, int new_height, CL_TextureFormat internal_format, int new_depth)
{
	throw_if_disposed();

	CLint gl_internal_format;
	CLenum gl_pixel_format;
	CL_OpenGL::to_opengl_textureformat(internal_format, gl_internal_format, gl_pixel_format);

	width = new_width;
	height = new_height;
	depth = new_depth;
	CL_TextureStateTracker state_tracker(texture_type, handle, NULL);
	if (texture_type == CL_TEXTURE_1D)
	{
		clTexImage1D(
			CL_TEXTURE_1D,            // target
			0,                        // level
			gl_internal_format,       // internalformat
			width,                    // width
			0,                        // border
			gl_pixel_format,          // format 
			CL_UNSIGNED_BYTE,         // type (it really doesn't matter since nothing is uploaded)
			0);                       // texels (0 to avoid uploading)
	}
	else if (depth == 1)
	{
		clTexImage2D(
			CL_TEXTURE_2D,            // target
			0,                        // level
			gl_internal_format,       // internalformat
			width,                    // width
			height,                   // height
			0,                        // border
			gl_pixel_format,          // format 
			CL_UNSIGNED_BYTE,         // type (it really doesn't matter since nothing is uploaded)
			0);                       // texels (0 to avoid uploading)
	}
	else
	{
		clTexImage3D(
			CL_TEXTURE_3D,            // target
			0,                        // level
			gl_internal_format,       // internalformat
			width,                    // width
			height,                   // height
			depth,                    // depth
			0,                        // border
			gl_pixel_format,          // format
			CL_UNSIGNED_BYTE,         // type (it really doesn't matter since nothing is uploaded)
			0);                       // texels (0 to avoid uploading)
	}
}

void CL_OpenGLTextureProvider::destroy()
{
	delete this;
}

CL_PixelBuffer CL_OpenGLTextureProvider::get_pixeldata(CL_TextureFormat sized_format, int level) const 
{
	throw_if_disposed();

	// todo: be smart here and request the closest matching opengl format.

	CL_TextureStateTracker state_tracker(texture_type, handle, NULL);

	CL_PixelBuffer buffer(
		width, height, cl_abgr8);

	clGetTexImage(texture_type, level, CL_RGBA, CL_UNSIGNED_BYTE, buffer.get_data());

	return buffer.to_format(sized_format);
}

void CL_OpenGLTextureProvider::set_image(CL_PixelBuffer &image, int level)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle, NULL);
	if (texture_type == CL_TEXTURE_2D)
	{
		set_texture_image2d(CL_TEXTURE_2D, image, level);
	}
	else if (texture_type == CL_TEXTURE_3D)
	{
		set_texture_image3d(CL_TEXTURE_3D, image, image.get_height() / height, level);
	}
}

void CL_OpenGLTextureProvider::set_cube_map(
	CL_PixelBuffer &cube_map_positive_x,
	CL_PixelBuffer &cube_map_negative_x,
	CL_PixelBuffer &cube_map_positive_y,
	CL_PixelBuffer &cube_map_negative_y,
	CL_PixelBuffer &cube_map_positive_z,
	CL_PixelBuffer &cube_map_negative_z,
	int level)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle, NULL);

	set_texture_image2d(CL_TEXTURE_CUBE_MAP_POSITIVE_X, cube_map_positive_x, level);
	set_texture_image2d(CL_TEXTURE_CUBE_MAP_NEGATIVE_X, cube_map_negative_x, level);
	set_texture_image2d(CL_TEXTURE_CUBE_MAP_POSITIVE_Y, cube_map_positive_y, level);
	set_texture_image2d(CL_TEXTURE_CUBE_MAP_NEGATIVE_Y, cube_map_negative_y, level);
	set_texture_image2d(CL_TEXTURE_CUBE_MAP_POSITIVE_Z, cube_map_positive_z, level);
	set_texture_image2d(CL_TEXTURE_CUBE_MAP_NEGATIVE_Z, cube_map_negative_z, level);
}

void CL_OpenGLTextureProvider::set_compressed_image(
	int level,
	CL_TextureFormat internal_format,
	int width,
	int height,
	CL_DataBuffer &image)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle, NULL);

	CLint gl_internal_format;
	CLenum gl_pixel_format;
	CL_OpenGL::to_opengl_textureformat(internal_format, gl_internal_format, gl_pixel_format);

	clCompressedTexImage2D(texture_type, level, gl_internal_format, width, height, 0, image.get_size(), image.get_data());
}

void CL_OpenGLTextureProvider::set_subimage(
	int x,
	int y,
	const CL_PixelBuffer &image,
	const CL_Rect &src_rect,
	int level)
{
	if (src_rect.left < 0 || src_rect.top < 0 || src_rect.right > image.get_width(), src_rect.bottom > image.get_height())
		throw CL_Exception("Rectangle out of bounds");

	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle, NULL);

	// check out if the original texture needs or doesn't need an alpha channel
	bool needs_alpha = image.get_alpha_mask() || image.has_colorkey();

	CLenum format;
	CLenum type;
	bool conv_needed = image.has_colorkey();
	if (!conv_needed) conv_needed = !CL_OpenGL::to_opengl_pixelformat(image.get_format(), format, type);

	// also check for the pitch (OpenGL can only skip pixels, not bytes)
	if (!conv_needed)
	{
		const int bytesPerPixel = image.get_bytes_per_pixel();
		if (image.get_pitch() % bytesPerPixel != 0)
			conv_needed = true;
	}

	// no conversion needed
	if (!conv_needed)
	{
		// Upload to OpenGL:

		// change alignment
		clPixelStorei(CL_UNPACK_ALIGNMENT, 1);
		const int bytesPerPixel = image.get_bytes_per_pixel();
		clPixelStorei(CL_UNPACK_ROW_LENGTH, image.get_pitch() / bytesPerPixel);
		clPixelStorei(CL_UNPACK_SKIP_PIXELS, src_rect.left);
		clPixelStorei(CL_UNPACK_SKIP_ROWS, src_rect.top);

		CL_OpenGLPixelBufferProvider *buffer_provider = NULL;
		if (image.get_provider())
			buffer_provider = dynamic_cast<CL_OpenGLPixelBufferProvider*>(image.get_provider());

		if (buffer_provider)		// Pixel Buffer Object texture
		{
			if (buffer_provider->get_target() != CL_PIXEL_UNPACK_BUFFER)
				throw CL_Exception("PixelBuffer direction is set to cl_data_from_gpu");
			CLint last_buffer = 0;
			clGetIntegerv(buffer_provider->get_binding(), &last_buffer);
			clBindBuffer(buffer_provider->get_target(), buffer_provider->get_handle());

			// upload
			clTexSubImage2D(
			CL_TEXTURE_2D,            // target
			level,                    // level
			x, y,                     // xoffset, yoffset
			src_rect.get_width(),        // width
			src_rect.get_height(),       // height
			format,					  // format
			type,					  // type
			NULL);                    // texels

			clBindBuffer(buffer_provider->get_target(), last_buffer);

		}
		else					// CPU Texture
		{
			char *data = (char *) image.get_data();
			// upload
			clTexSubImage2D(
			CL_TEXTURE_2D,            // target
			level,                    // level
			x, y,                     // xoffset, yoffset
			src_rect.get_width(),        // width
			src_rect.get_height(),       // height
			format,					  // format
			type,					  // type
			data);                    // texels
		}
		// Restore these unpack values to the default
		clPixelStorei(CL_UNPACK_SKIP_PIXELS, 0);
		clPixelStorei(CL_UNPACK_SKIP_ROWS, 0);

	}
	// conversion needed
	else
	{
		bool big_endian = CL_Endian::is_system_big();

		CL_PixelBuffer buffer;
		if (!big_endian)
			buffer = CL_PixelBuffer(
				src_rect.get_width(), src_rect.get_height(),
				needs_alpha ? cl_abgr8 : cl_bgr8); // OpenGL RGB/RGBA is always big endian
		else
			buffer = CL_PixelBuffer(
				src_rect.get_width(), src_rect.get_height(),
				needs_alpha ? cl_rgba8 : cl_rgb8);
	
		image.convert(buffer, CL_Rect(0, 0, buffer.get_size()), src_rect);

		format = needs_alpha ? CL_RGBA : CL_RGB;

		// Upload to OpenGL:
		clBindTexture(CL_TEXTURE_2D, handle);

		// change alignment
		clPixelStorei(CL_UNPACK_ALIGNMENT, 1);
		const int bytesPerPixel = buffer.get_bytes_per_pixel();
		clPixelStorei(CL_UNPACK_ROW_LENGTH, buffer.get_pitch() / bytesPerPixel);
		clPixelStorei(CL_UNPACK_SKIP_PIXELS, 0);
		clPixelStorei(CL_UNPACK_SKIP_ROWS, 0);
		// upload
		clTexSubImage2D(
			CL_TEXTURE_2D,            // target
			level,                    // level
			x, y,                     // xoffset, yoffset
			buffer.get_width(),        // width
			buffer.get_height(),       // height
			format,                   // format
			CL_UNSIGNED_BYTE,         // type
			buffer.get_data());       // texels
	}
}

void CL_OpenGLTextureProvider::copy_image_from(
	int x,
	int y,
	int width,
	int height,
	int level,
	CL_TextureFormat internal_format,
	CL_GraphicContextProvider *gc)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle, static_cast<CL_OpenGLGraphicContextProvider*>(gc));

	CLint gl_internal_format;
	CLenum gl_pixel_format;
	CL_OpenGL::to_opengl_textureformat(internal_format, gl_internal_format, gl_pixel_format);


	clCopyTexImage2D(
		CL_TEXTURE_2D,
		level,
		gl_internal_format,
		x, y,
		width, height,
		0);
}

void CL_OpenGLTextureProvider::copy_subimage_from(
	int offset_x,
	int offset_y,
	int x,
	int y,
	int width,
	int height,
	int level,
	CL_GraphicContextProvider *gc)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle, static_cast<CL_OpenGLGraphicContextProvider*>(gc));

	clCopyTexSubImage2D( 
		CL_TEXTURE_2D,
		level, 
		offset_x, 
		offset_y, 
		x, y,
		width, height );
}

void CL_OpenGLTextureProvider::set_min_lod(double min_lod)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle, NULL);
	clTexParameterf(texture_type, CL_TEXTURE_MIN_LOD, (CLfloat)min_lod);
}

void CL_OpenGLTextureProvider::set_max_lod(double max_lod)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle, NULL);
	clTexParameterf(texture_type, CL_TEXTURE_MAX_LOD, (CLfloat)max_lod);
}

void CL_OpenGLTextureProvider::set_lod_bias(double lod_bias)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle, NULL);
	clTexParameterf(texture_type, CL_TEXTURE_LOD_BIAS, (CLfloat)lod_bias);
}

void CL_OpenGLTextureProvider::set_base_level(int base_level)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle, NULL);
	clTexParameteri(texture_type, CL_TEXTURE_BASE_LEVEL, base_level);
}

void CL_OpenGLTextureProvider::set_max_level(int max_level)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle, NULL);
	clTexParameteri(texture_type, CL_TEXTURE_MAX_LEVEL, max_level);
}

void CL_OpenGLTextureProvider::set_wrap_mode(
	CL_TextureWrapMode wrap_s,
	CL_TextureWrapMode wrap_t,
	CL_TextureWrapMode wrap_r)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle, NULL);
	clTexParameteri(texture_type, CL_TEXTURE_WRAP_S, to_enum(wrap_s));
	clTexParameteri(texture_type, CL_TEXTURE_WRAP_T, to_enum(wrap_t));
	clTexParameteri(texture_type, CL_TEXTURE_WRAP_R, to_enum(wrap_r));
}

void CL_OpenGLTextureProvider::set_wrap_mode(
	CL_TextureWrapMode wrap_s,
	CL_TextureWrapMode wrap_t)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle, NULL);
	clTexParameteri(texture_type, CL_TEXTURE_WRAP_S, to_enum(wrap_s));
	clTexParameteri(texture_type, CL_TEXTURE_WRAP_T, to_enum(wrap_t));
}

void CL_OpenGLTextureProvider::set_wrap_mode(
	CL_TextureWrapMode wrap_s)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle, NULL);
	clTexParameteri(texture_type, CL_TEXTURE_WRAP_S, to_enum(wrap_s));
}

void CL_OpenGLTextureProvider::set_min_filter(CL_TextureFilter filter)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle, NULL);
	clTexParameteri(texture_type, CL_TEXTURE_MIN_FILTER, to_enum(filter));
}

void CL_OpenGLTextureProvider::set_mag_filter(CL_TextureFilter filter)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle, NULL);

	// Validation (see opengl spec)
	if ( ! ((filter == cl_filter_nearest) || (filter == cl_filter_linear)) )
		throw CL_Exception("cl_filter_nearest, cl_filter_linear are only valid options for the mag filter");

	clTexParameteri(texture_type, CL_TEXTURE_MAG_FILTER, to_enum(filter));
}

void CL_OpenGLTextureProvider::set_max_anisotropy(float v)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle, NULL);
	clTexParameterf(texture_type, CL_TEXTURE_MAX_ANISOTROPY_EXT, v);
}

void CL_OpenGLTextureProvider::set_texture_compare(CL_TextureCompareMode mode, CL_CompareFunction func)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle, NULL);
	clTexParameteri(texture_type, CL_TEXTURE_COMPARE_MODE, to_enum(mode));	
	clTexParameteri(texture_type, CL_TEXTURE_COMPARE_FUNC, to_enum(func));	
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLTextureProvider Implementation:


void CL_OpenGLTextureProvider::set_texture_image2d(
	CLuint target,
	CL_PixelBuffer &image,
	int level)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle, NULL);

	CLint gl_internal_format;
	CLenum gl_pixel_format;
	CL_OpenGL::to_opengl_textureformat(image.get_format(), gl_internal_format, gl_pixel_format);

/*
	CL_UNPACK_SWAP_BYTES
	CL_UNPACK_LSB_FIRST
	CL_UNPACK_SKIP_ROWS
	CL_UNPACK_SKIP_PIXELS

	CL_UNPACK_ROW_LENGTH
	CL_UNPACK_ALIGNMENT
	CL_UNPACK_IMAGE_HEIGHT
	CL_UNPACK_SKIP_IMAGES
*/

	// check out if the original texture needs or doesn't need an alpha channel
	bool needs_alpha = image.get_alpha_mask() || image.has_colorkey();

	CLenum format;
	CLenum type;
	bool conv_needed = image.has_colorkey();
	if (!conv_needed) conv_needed = !CL_OpenGL::to_opengl_pixelformat(image.get_format(), format, type);

	// also check for the pitch (OpenGL can only skip pixels, not bytes)
	if (!conv_needed)
	{
		const int bytesPerPixel = image.get_bytes_per_pixel();

		if (image.get_pitch() % bytesPerPixel != 0)
			conv_needed = true;
	}

	// no conversion needed
	if (!conv_needed)
	{
		// Upload to OpenGL:

		// change alignment
		clPixelStorei(CL_UNPACK_ALIGNMENT, 1);
		const int bytesPerPixel = image.get_bytes_per_pixel();
		clPixelStorei(CL_UNPACK_ROW_LENGTH, image.get_pitch() / bytesPerPixel);
		clPixelStorei(CL_UNPACK_SKIP_PIXELS, 0);
		clPixelStorei(CL_UNPACK_SKIP_ROWS, 0);

		char *data = (char *) image.get_data();
		int image_width = image.get_width();
		int image_height = image.get_height();
/*
		int image_width = 1;
		int image_height = 1;
		while (image_width < image.get_width()) image_width *= 2;
		while (image_height < image.get_height()) image_height *= 2;
*/
		clTexImage2D(
			target,                   // target
			level,                    // level
			gl_internal_format,           // internalformat
			image_width,              // width
			image_height,             // height
			0,                        // border
			format,                   // format
			type,                     // type
			data);                    // texels
	}
	// conversion needed
	else
	{
		bool big_endian = CL_Endian::is_system_big();

		CL_PixelBuffer buffer;
		if (!big_endian)
			buffer = CL_PixelBuffer(
				image.get_width(), image.get_height(),
				needs_alpha ? cl_abgr8 : cl_bgr8); // OpenGL RGB/RGBA is always big endian
		else
			buffer = CL_PixelBuffer(
				image.get_width(), image.get_height(),
				needs_alpha ? cl_rgba8 : cl_rgb8);
	
		image.convert(buffer);

		format = needs_alpha ? CL_RGBA : CL_RGB;

		// Upload to OpenGL:

		// change alignment
		clPixelStorei(CL_UNPACK_ALIGNMENT, 1);
		const int bytesPerPixel = buffer.get_bytes_per_pixel();
		clPixelStorei(CL_UNPACK_ROW_LENGTH, buffer.get_pitch() / bytesPerPixel);
		clPixelStorei(CL_UNPACK_SKIP_PIXELS, 0);
		clPixelStorei(CL_UNPACK_SKIP_ROWS, 0);

		// upload
		clTexImage2D(
			target,                   // target
			level,                    // level
			gl_internal_format,           // internalformat
			image.get_width(),        // width
			image.get_height(),       // height
			0,                        // border
			format,                   // format
			CL_UNSIGNED_BYTE,         // type
			buffer.get_data());       // texels
/*

		clTexParameteri(CL_TEXTURE_2D, CL_TEXTURE_MIN_FILTER, CL_LINEAR);
		clTexParameteri(CL_TEXTURE_2D, CL_TEXTURE_MAG_FILTER, CL_LINEAR);
		clTexParameteri(CL_TEXTURE_2D, CL_TEXTURE_WRAP_S, CL_CLAMP_TO_EDGE);
		clTexParameteri(CL_TEXTURE_2D, CL_TEXTURE_WRAP_T, CL_CLAMP_TO_EDGE);
*/
	}
}

void CL_OpenGLTextureProvider::set_texture_image3d(
	CLuint target,
	CL_PixelBuffer &image,
	int image_depth,
	int level)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle, NULL);

	CLint gl_internal_format;
	CLenum gl_pixel_format;
	CL_OpenGL::to_opengl_textureformat(image.get_format(), gl_internal_format, gl_pixel_format);

	// check out if the original texture needs or doesn't need an alpha channel
	bool needs_alpha = image.get_alpha_mask() || image.has_colorkey();

	CLenum format;
	CLenum type;
	bool conv_needed = image.has_colorkey();
	if (!conv_needed) conv_needed = !CL_OpenGL::to_opengl_pixelformat(image.get_format(), format, type);

	// also check for the pitch (OpenGL can only skip pixels, not bytes)
	if (!conv_needed)
	{
		const int bytesPerPixel = image.get_bytes_per_pixel();
		if (image.get_pitch() % bytesPerPixel != 0)
			conv_needed = true;
	}

	// no conversion needed
	if (!conv_needed)
	{
		// Upload to OpenGL:

		// change alignment
		clPixelStorei(CL_UNPACK_ALIGNMENT, 1);
		const int bytesPerPixel = image.get_bytes_per_pixel();
		clPixelStorei(CL_UNPACK_ROW_LENGTH, image.get_pitch() / bytesPerPixel);
		clPixelStorei(CL_UNPACK_SKIP_PIXELS, 0);
		clPixelStorei(CL_UNPACK_SKIP_ROWS, 0);

		char *data = (char *) image.get_data();
		int image_width = image.get_width();
		int image_height = image.get_height() / image_depth;

		clTexImage3D(
			target,                   // target
			level,                    // level
			gl_internal_format,           // internalformat
			image_width,              // width
			image_height,             // height
			image_depth,             // depth
			0,                        // border
			format,                   // format
			type,                     // type
			data);                    // texels
	}
	// conversion needed
	else
	{
		bool big_endian = CL_Endian::is_system_big();

		CL_PixelBuffer buffer;
		if (!big_endian)
			buffer = CL_PixelBuffer(
				image.get_width(), image.get_height(),
				needs_alpha ? cl_abgr8 : cl_bgr8); // OpenGL RGB/RGBA is always big endian
		else
			buffer = CL_PixelBuffer(
				image.get_width(), image.get_height(),
				needs_alpha ? cl_rgba8 : cl_rgb8);
	
		image.convert(buffer);

		format = needs_alpha ? CL_RGBA : CL_RGB;

		// Upload to OpenGL:

		// change alignment
		clPixelStorei(CL_UNPACK_ALIGNMENT, 1);
		const int bytesPerPixel = buffer.get_bytes_per_pixel();
		clPixelStorei(CL_UNPACK_ROW_LENGTH, buffer.get_pitch() / bytesPerPixel);
		clPixelStorei(CL_UNPACK_SKIP_PIXELS, 0);
		clPixelStorei(CL_UNPACK_SKIP_ROWS, 0);

		int image_width = image.get_width();
		int image_height = image.get_height() / image_depth;

		// upload
		clTexImage3D(
			target,                   // target
			level,                    // level
			gl_internal_format,           // internalformat
			image_width,        // width
			image_height,       // height
			image_depth,       // depth
			0,                        // border
			format,                   // format
			CL_UNSIGNED_BYTE,         // type
			buffer.get_data());       // texels
	}
}

CLenum CL_OpenGLTextureProvider::to_enum(CL_TextureFilter filter)
{
	switch(filter)
	{
	case cl_filter_nearest: return CL_NEAREST;
	case cl_filter_linear: return CL_LINEAR;
	case cl_filter_nearest_mipmap_nearest: return CL_NEAREST_MIPMAP_NEAREST;
	case cl_filter_nearest_mipmap_linear: return CL_NEAREST_MIPMAP_LINEAR;
	case cl_filter_linear_mipmap_nearest: return CL_LINEAR_MIPMAP_NEAREST;
	case cl_filter_linear_mipmap_linear: return CL_LINEAR_MIPMAP_LINEAR;
	default: return CL_NEAREST;
	}
}

CLenum CL_OpenGLTextureProvider::to_enum(CL_TextureWrapMode mode)
{
 	switch(mode)
	{
	case cl_wrap_clamp_to_edge: return CL_CLAMP_TO_EDGE;
	case cl_wrap_repeat: return CL_REPEAT;
	case cl_wrap_mirrored_repeat: return CL_MIRRORED_REPEAT;
	default: return CL_CLAMP_TO_EDGE;
	}
}

CLenum CL_OpenGLTextureProvider::to_enum(CL_TextureCompareMode mode)
{
 	switch(mode)
	{
	case cl_comparemode_none: return CL_NONE;
	case cl_comparemode_compare_r_to_texture: return CL_COMPARE_REF_TO_TEXTURE;		
	default: return CL_NONE;
	}
}

CLenum CL_OpenGLTextureProvider::to_enum(CL_CompareFunction func)
{
	switch( func )
	{
	case cl_comparefunc_never: return CL_NEVER;
	case cl_comparefunc_less: return CL_LESS;
	case cl_comparefunc_lequal: return CL_LEQUAL; 
	case cl_comparefunc_greater: return CL_GREATER; 
	case cl_comparefunc_gequal: return CL_GEQUAL; 
	case cl_comparefunc_equal: return CL_EQUAL; 
	case cl_comparefunc_notequal: return CL_NOTEQUAL; 
	case cl_comparefunc_always: return CL_ALWAYS; 
	default: return CL_LEQUAL;
	}
}

/////////////////////////////////////////////////////////////////////////////

CL_TextureStateTracker::CL_TextureStateTracker(CLuint texture_type, CLuint handle, CL_OpenGLGraphicContextProvider *gc_provider)
{
	// If the gc_provider is unknown, we need to use the first active provider
	if (!gc_provider)
	{
		CL_OpenGL::set_active();
	}
	else
	{
		CL_OpenGL::set_active(gc_provider);
	}

	last_is_enabled_texture1d = clIsEnabled(CL_TEXTURE_1D);
	last_is_enabled_texture2d = clIsEnabled(CL_TEXTURE_2D);
	last_is_enabled_texture3d = clIsEnabled(CL_TEXTURE_3D);
	last_is_enabled_texture_cube_map = clIsEnabled(CL_TEXTURE_CUBE_MAP);
	clGetIntegerv(CL_TEXTURE_BINDING_1D, (CLint *) &last_bound_texture1d);
	clGetIntegerv(CL_TEXTURE_BINDING_2D, (CLint *) &last_bound_texture2d);
	clGetIntegerv(CL_TEXTURE_BINDING_3D, (CLint *) &last_bound_texture3d);
	clGetIntegerv(CL_TEXTURE_BINDING_CUBE_MAP, (CLint *) &last_bound_texture_cube_map);

	if (texture_type == CL_TEXTURE_1D)
	{
		clDisable(CL_TEXTURE_2D);
		clDisable(CL_TEXTURE_3D);
		clDisable(CL_TEXTURE_CUBE_MAP);
		clEnable(CL_TEXTURE_1D);
		clBindTexture(CL_TEXTURE_1D, handle);
	}

	if (texture_type == CL_TEXTURE_2D)
	{
		clDisable(CL_TEXTURE_1D);
		clDisable(CL_TEXTURE_3D);
		clDisable(CL_TEXTURE_CUBE_MAP);
		clEnable(CL_TEXTURE_2D);
		clBindTexture(CL_TEXTURE_2D, handle);
	}

	if (texture_type == CL_TEXTURE_3D)
	{
		clDisable(CL_TEXTURE_1D);
		clDisable(CL_TEXTURE_2D);
		clDisable(CL_TEXTURE_CUBE_MAP);
		clEnable(CL_TEXTURE_3D);
		clBindTexture(CL_TEXTURE_3D, handle);
	}

	if (texture_type == CL_TEXTURE_CUBE_MAP)
	{
		clDisable(CL_TEXTURE_1D);
		clDisable(CL_TEXTURE_2D);
		clDisable(CL_TEXTURE_3D);
		clEnable(CL_TEXTURE_CUBE_MAP);
		clBindTexture(CL_TEXTURE_CUBE_MAP, handle);
	}
}

CL_TextureStateTracker::~CL_TextureStateTracker()
{
	if (last_is_enabled_texture1d) clEnable(CL_TEXTURE_1D); else clDisable(CL_TEXTURE_1D);
	if (last_is_enabled_texture2d) clEnable(CL_TEXTURE_2D); else clDisable(CL_TEXTURE_2D);
	if (last_is_enabled_texture3d) clEnable(CL_TEXTURE_3D); else clDisable(CL_TEXTURE_3D);
	if (last_is_enabled_texture_cube_map) clEnable(CL_TEXTURE_CUBE_MAP); else clDisable(CL_TEXTURE_CUBE_MAP);

	if (last_is_enabled_texture1d) clBindTexture(CL_TEXTURE_1D, last_bound_texture1d);
	if (last_is_enabled_texture2d) clBindTexture(CL_TEXTURE_2D, last_bound_texture2d);
	if (last_is_enabled_texture3d) clBindTexture(CL_TEXTURE_3D, last_bound_texture3d);
	if (last_is_enabled_texture_cube_map) clBindTexture(CL_TEXTURE_CUBE_MAP, last_bound_texture_cube_map);
}
