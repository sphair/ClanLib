/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

CL_OpenGLTextureProvider::CL_OpenGLTextureProvider(CL_TextureDimensions texture_dimensions)
: width(0), height(0), depth(0), handle(0), texture_type(0)
{
	CL_SharedGCData::add_disposable(this);
	switch (texture_dimensions)
	{
	case cl_texture_1d:
		texture_type = GL_TEXTURE_1D;
		break;
	case cl_texture_2d:
	default:
		texture_type = GL_TEXTURE_2D;
		break;
	case cl_texture_3d:
		texture_type = GL_TEXTURE_3D;
		break;
	case cl_texture_cube_map:
		texture_type = GL_TEXTURE_CUBE_MAP;
		break;
	}

	CL_TextureStateTracker state_tracker(texture_type, 0);
	glGenTextures(1, &handle);
	glBindTexture(texture_type, handle);
	glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	if (texture_type != GL_TEXTURE_1D)
		glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	if (texture_type == GL_TEXTURE_3D)
		glTexParameteri(texture_type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
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
			glDeleteTextures(1, &handle);
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
	CL_TextureStateTracker state_tracker(texture_type, handle);
	glGenerateMipmap(texture_type);
}

int CL_OpenGLTextureProvider::get_next_power_of_two(int value)
{
    for (int pot_count = 1; pot_count < 32768; pot_count += pot_count)
    {
        if (value <= pot_count)
            return pot_count;
    }
    return 32768;
}

void CL_OpenGLTextureProvider::create(int new_width, int new_height, CL_TextureFormat internal_format, int new_depth)
{
	throw_if_disposed();

	GLint gl_internal_format;
	GLenum gl_pixel_format;
	CL_OpenGL::to_opengl_textureformat(internal_format, gl_internal_format, gl_pixel_format);

	width = new_width;
	height = new_height;
	depth = new_depth;
	CL_TextureStateTracker state_tracker(texture_type, handle);
	if (texture_type == GL_TEXTURE_1D)
	{
		glTexImage1D(
			GL_TEXTURE_1D,            // target
			0,                        // level
			gl_internal_format,       // internalformat
			width,                    // width
			0,                        // border
			gl_pixel_format,          // format 
			GL_UNSIGNED_BYTE,         // type (it really doesn't matter since nothing is uploaded)
			0);                       // texels (0 to avoid uploading)
	}
	else if (depth == 1)
	{
		glTexImage2D(
			GL_TEXTURE_2D,            // target
			0,                        // level
			gl_internal_format,       // internalformat
			width,                    // width
			height,                   // height
			0,                        // border
			gl_pixel_format,          // format 
			GL_UNSIGNED_BYTE,         // type (it really doesn't matter since nothing is uploaded)
			0);                       // texels (0 to avoid uploading)
	}
	else
	{
		glTexImage3D(
			GL_TEXTURE_3D,            // target
			0,                        // level
			gl_internal_format,       // internalformat
			width,                    // width
			height,                   // height
			depth,                    // depth
			0,                        // border
			gl_pixel_format,          // format
			GL_UNSIGNED_BYTE,         // type (it really doesn't matter since nothing is uploaded)
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

	CL_TextureStateTracker state_tracker(texture_type, handle);

	CL_PixelBuffer buffer(
		width, height, cl_abgr8);

	glGetTexImage(texture_type, level, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get_data());

	return buffer.to_format(sized_format);
}

void CL_OpenGLTextureProvider::set_image(CL_PixelBuffer &image, int level)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle);
	if (texture_type == GL_TEXTURE_2D)
	{
		set_texture_image2d(GL_TEXTURE_2D, image, level);
	}
	else if (texture_type == GL_TEXTURE_3D)
	{
		set_texture_image3d(GL_TEXTURE_3D, image, image.get_height() / height, level);
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
	CL_TextureStateTracker state_tracker(texture_type, handle);

	set_texture_image2d(GL_TEXTURE_CUBE_MAP_POSITIVE_X, cube_map_positive_x, level);
	set_texture_image2d(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, cube_map_negative_x, level);
	set_texture_image2d(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, cube_map_positive_y, level);
	set_texture_image2d(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, cube_map_negative_y, level);
	set_texture_image2d(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, cube_map_positive_z, level);
	set_texture_image2d(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, cube_map_negative_z, level);
}

void CL_OpenGLTextureProvider::set_compressed_image(
	int level,
	CL_TextureFormat internal_format,
	int width,
	int height,
	CL_DataBuffer &image)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle);

	GLint gl_internal_format;
	GLenum gl_pixel_format;
	CL_OpenGL::to_opengl_textureformat(internal_format, gl_internal_format, gl_pixel_format);

	glCompressedTexImage2D(texture_type, level, gl_internal_format, width, height, 0, image.get_size(), image.get_data());
}

void CL_OpenGLTextureProvider::set_subimage(
	int x,
	int y,
	const CL_PixelBuffer &image,
	const CL_Rect &src_rect,
	int level)
{
	if (src_rect.left < 0 || src_rect.top < 0 || src_rect.right > image.get_width() || src_rect.bottom > image.get_height())
		throw CL_Exception("Rectangle out of bounds");

	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle);

	// check out if the original texture needs or doesn't need an alpha channel
	//bool needs_alpha = image.get_alpha_mask() || image.has_colorkey();
    bool needs_alpha = true; // Forcing this to true because if the internal format of the texture contains alpha we must upload alpha no matter what

	GLenum format;
	GLenum type;
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
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		const int bytesPerPixel = image.get_bytes_per_pixel();
		glPixelStorei(GL_UNPACK_ROW_LENGTH, image.get_pitch() / bytesPerPixel);
		glPixelStorei(GL_UNPACK_SKIP_PIXELS, src_rect.left);
		glPixelStorei(GL_UNPACK_SKIP_ROWS, src_rect.top);

		CL_OpenGLPixelBufferProvider *buffer_provider = NULL;
		if (image.get_provider())
			buffer_provider = dynamic_cast<CL_OpenGLPixelBufferProvider*>(image.get_provider());

		if (buffer_provider)		// Pixel Buffer Object texture
		{
			if (buffer_provider->get_target() != GL_PIXEL_UNPACK_BUFFER)
				throw CL_Exception("PixelBuffer direction is set to cl_data_from_gpu");
			GLint last_buffer = 0;
			glGetIntegerv(buffer_provider->get_binding(), &last_buffer);
			glBindBuffer(buffer_provider->get_target(), buffer_provider->get_handle());

			// upload
			glTexSubImage2D(
			GL_TEXTURE_2D,            // target
			level,                    // level
			x, y,                     // xoffset, yoffset
			src_rect.get_width(),        // width
			src_rect.get_height(),       // height
			format,					  // format
			type,					  // type
			NULL);                    // texels

			glBindBuffer(buffer_provider->get_target(), last_buffer);

		}
		else					// CPU Texture
		{
			char *data = (char *) image.get_data();
			// upload
			glTexSubImage2D(
			GL_TEXTURE_2D,            // target
			level,                    // level
			x, y,                     // xoffset, yoffset
			src_rect.get_width(),        // width
			src_rect.get_height(),       // height
			format,					  // format
			type,					  // type
			data);                    // texels
		}
		// Restore these unpack values to the default
		glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

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

		format = needs_alpha ? GL_RGBA : GL_RGB;

		// Upload to OpenGL:
		glBindTexture(GL_TEXTURE_2D, handle);

		// change alignment
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		const int bytesPerPixel = buffer.get_bytes_per_pixel();
		glPixelStorei(GL_UNPACK_ROW_LENGTH, buffer.get_pitch() / bytesPerPixel);
		glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
		// upload
		glTexSubImage2D(
			GL_TEXTURE_2D,            // target
			level,                    // level
			x, y,                     // xoffset, yoffset
			buffer.get_width(),        // width
			buffer.get_height(),       // height
			format,                   // format
			GL_UNSIGNED_BYTE,         // type
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
	CL_OpenGL::set_active(static_cast<CL_OpenGLGraphicContextProvider*>(gc));
	CL_TextureStateTracker state_tracker(texture_type, handle);

	GLint gl_internal_format;
	GLenum gl_pixel_format;
	CL_OpenGL::to_opengl_textureformat(internal_format, gl_internal_format, gl_pixel_format);


	glCopyTexImage2D(
		GL_TEXTURE_2D,
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
	CL_OpenGL::set_active(static_cast<CL_OpenGLGraphicContextProvider*>(gc));
	CL_TextureStateTracker state_tracker(texture_type, handle);

	glCopyTexSubImage2D( 
		GL_TEXTURE_2D,
		level, 
		offset_x, 
		offset_y, 
		x, y,
		width, height );
}

void CL_OpenGLTextureProvider::set_min_lod(double min_lod)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle);
	glTexParameterf(texture_type, GL_TEXTURE_MIN_LOD, (GLfloat)min_lod);
}

void CL_OpenGLTextureProvider::set_max_lod(double max_lod)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle);
	glTexParameterf(texture_type, GL_TEXTURE_MAX_LOD, (GLfloat)max_lod);
}

void CL_OpenGLTextureProvider::set_lod_bias(double lod_bias)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle);
	glTexParameterf(texture_type, GL_TEXTURE_LOD_BIAS, (GLfloat)lod_bias);
}

void CL_OpenGLTextureProvider::set_base_level(int base_level)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle);
	glTexParameteri(texture_type, GL_TEXTURE_BASE_LEVEL, base_level);
}

void CL_OpenGLTextureProvider::set_max_level(int max_level)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle);
	glTexParameteri(texture_type, GL_TEXTURE_MAX_LEVEL, max_level);
}

void CL_OpenGLTextureProvider::set_wrap_mode(
	CL_TextureWrapMode wrap_s,
	CL_TextureWrapMode wrap_t,
	CL_TextureWrapMode wrap_r)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle);
	glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, to_enum(wrap_s));
    if (texture_type != GL_TEXTURE_1D)
        glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, to_enum(wrap_t));
    if (texture_type != GL_TEXTURE_1D && texture_type != GL_TEXTURE_2D)
        glTexParameteri(texture_type, GL_TEXTURE_WRAP_R, to_enum(wrap_r));
}

void CL_OpenGLTextureProvider::set_wrap_mode(
	CL_TextureWrapMode wrap_s,
	CL_TextureWrapMode wrap_t)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle);
	glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, to_enum(wrap_s));
	glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, to_enum(wrap_t));
}

void CL_OpenGLTextureProvider::set_wrap_mode(
	CL_TextureWrapMode wrap_s)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle);
	glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, to_enum(wrap_s));
}

void CL_OpenGLTextureProvider::set_min_filter(CL_TextureFilter filter)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle);
	glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, to_enum(filter));
}

void CL_OpenGLTextureProvider::set_mag_filter(CL_TextureFilter filter)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle);

	// Validation (see opengl spec)
	if ( ! ((filter == cl_filter_nearest) || (filter == cl_filter_linear)) )
		throw CL_Exception("cl_filter_nearest, cl_filter_linear are only valid options for the mag filter");

	glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, to_enum(filter));
}

void CL_OpenGLTextureProvider::set_max_anisotropy(float v)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle);
	glTexParameterf(texture_type, GL_TEXTURE_MAX_ANISOTROPY_EXT, v);
}

void CL_OpenGLTextureProvider::set_texture_compare(CL_TextureCompareMode mode, CL_CompareFunction func)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle);
	glTexParameteri(texture_type, GL_TEXTURE_COMPARE_MODE, to_enum(mode));	
	glTexParameteri(texture_type, GL_TEXTURE_COMPARE_FUNC, to_enum(func));	
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLTextureProvider Implementation:


void CL_OpenGLTextureProvider::set_texture_image2d(
	GLuint target,
	CL_PixelBuffer &image,
	int level)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle);

/*
	GL_UNPACK_SWAP_BYTES
	GL_UNPACK_LSB_FIRST
	GL_UNPACK_SKIP_ROWS
	GL_UNPACK_SKIP_PIXELS

	GL_UNPACK_ROW_LENGTH
	GL_UNPACK_ALIGNMENT
	GL_UNPACK_IMAGE_HEIGHT
	GL_UNPACK_SKIP_IMAGES
*/

	// check out if the original texture needs or doesn't need an alpha channel
	bool needs_alpha = image.get_alpha_mask() || image.has_colorkey();

	GLenum format;
	GLenum type;
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

		GLint gl_internal_format;
		GLenum gl_pixel_format;
		CL_OpenGL::to_opengl_textureformat(image.get_format(), gl_internal_format, gl_pixel_format);

		// change alignment
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		const int bytesPerPixel = image.get_bytes_per_pixel();
		glPixelStorei(GL_UNPACK_ROW_LENGTH, image.get_pitch() / bytesPerPixel);
		glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

		char *data = (char *) image.get_data();
		int image_width = image.get_width();
		int image_height = image.get_height();
/*
		int image_width = 1;
		int image_height = 1;
		while (image_width < image.get_width()) image_width *= 2;
		while (image_height < image.get_height()) image_height *= 2;
*/
		glTexImage2D(
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

		format = needs_alpha ? GL_RGBA : GL_RGB;

		// Upload to OpenGL:

		GLint gl_internal_format;
		GLenum gl_pixel_format;
		CL_OpenGL::to_opengl_textureformat(cl_rgba8ui, gl_internal_format, gl_pixel_format);

		// change alignment
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		const int bytesPerPixel = buffer.get_bytes_per_pixel();
		glPixelStorei(GL_UNPACK_ROW_LENGTH, buffer.get_pitch() / bytesPerPixel);
		glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

		// upload
		glTexImage2D(
			target,                   // target
			level,                    // level
			gl_internal_format,           // internalformat
			image.get_width(),        // width
			image.get_height(),       // height
			0,                        // border
			format,                   // format
			GL_UNSIGNED_BYTE,         // type
			buffer.get_data());       // texels
/*

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
*/
	}
}

void CL_OpenGLTextureProvider::set_texture_image3d(
	GLuint target,
	CL_PixelBuffer &image,
	int image_depth,
	int level)
{
	throw_if_disposed();
	CL_TextureStateTracker state_tracker(texture_type, handle);

	GLint gl_internal_format;
	GLenum gl_pixel_format;
	CL_OpenGL::to_opengl_textureformat(image.get_format(), gl_internal_format, gl_pixel_format);

	// check out if the original texture needs or doesn't need an alpha channel
	bool needs_alpha = image.get_alpha_mask() || image.has_colorkey();

	GLenum format;
	GLenum type;
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
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		const int bytesPerPixel = image.get_bytes_per_pixel();
		glPixelStorei(GL_UNPACK_ROW_LENGTH, image.get_pitch() / bytesPerPixel);
		glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

		char *data = (char *) image.get_data();
		int image_width = image.get_width();
		int image_height = image.get_height() / image_depth;

		glTexImage3D(
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

		format = needs_alpha ? GL_RGBA : GL_RGB;

		// Upload to OpenGL:

		// change alignment
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		const int bytesPerPixel = buffer.get_bytes_per_pixel();
		glPixelStorei(GL_UNPACK_ROW_LENGTH, buffer.get_pitch() / bytesPerPixel);
		glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

		int image_width = image.get_width();
		int image_height = image.get_height() / image_depth;

		// upload
		glTexImage3D(
			target,                   // target
			level,                    // level
			gl_internal_format,           // internalformat
			image_width,        // width
			image_height,       // height
			image_depth,       // depth
			0,                        // border
			format,                   // format
			GL_UNSIGNED_BYTE,         // type
			buffer.get_data());       // texels
	}
}

GLenum CL_OpenGLTextureProvider::to_enum(CL_TextureFilter filter)
{
	switch(filter)
	{
	case cl_filter_nearest: return GL_NEAREST;
	case cl_filter_linear: return GL_LINEAR;
	case cl_filter_nearest_mipmap_nearest: return GL_NEAREST_MIPMAP_NEAREST;
	case cl_filter_nearest_mipmap_linear: return GL_NEAREST_MIPMAP_LINEAR;
	case cl_filter_linear_mipmap_nearest: return GL_LINEAR_MIPMAP_NEAREST;
	case cl_filter_linear_mipmap_linear: return GL_LINEAR_MIPMAP_LINEAR;
	default: return GL_NEAREST;
	}
}

GLenum CL_OpenGLTextureProvider::to_enum(CL_TextureWrapMode mode)
{
 	switch(mode)
	{
	case cl_wrap_clamp_to_edge: return GL_CLAMP_TO_EDGE;
	case cl_wrap_repeat: return GL_REPEAT;
	case cl_wrap_mirrored_repeat: return GL_MIRRORED_REPEAT;
	default: return GL_CLAMP_TO_EDGE;
	}
}

GLenum CL_OpenGLTextureProvider::to_enum(CL_TextureCompareMode mode)
{
 	switch(mode)
	{
	case cl_comparemode_none: return GL_NONE;
	case cl_comparemode_compare_r_to_texture: return GL_COMPARE_REF_TO_TEXTURE;		
	default: return GL_NONE;
	}
}

GLenum CL_OpenGLTextureProvider::to_enum(CL_CompareFunction func)
{
	switch( func )
	{
	case cl_comparefunc_never: return GL_NEVER;
	case cl_comparefunc_less: return GL_LESS;
	case cl_comparefunc_lequal: return GL_LEQUAL; 
	case cl_comparefunc_greater: return GL_GREATER; 
	case cl_comparefunc_gequal: return GL_GEQUAL; 
	case cl_comparefunc_equal: return GL_EQUAL; 
	case cl_comparefunc_notequal: return GL_NOTEQUAL; 
	case cl_comparefunc_always: return GL_ALWAYS; 
	default: return GL_LEQUAL;
	}
}

/////////////////////////////////////////////////////////////////////////////

CL_TextureStateTracker::CL_TextureStateTracker(GLuint texture_type, GLuint handle)
{    
	CL_OpenGL::set_active();

#ifdef __APPLE__
	glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint *) &last_bound_texture2d);
    if (handle)
        glBindTexture(GL_TEXTURE_2D, handle);
#else

	if (CL_OpenGL::get_opengl_version_major() < 3)
	{
		last_is_enabled_texture1d = glIsEnabled(GL_TEXTURE_1D);
		last_is_enabled_texture2d = glIsEnabled(GL_TEXTURE_2D);
		last_is_enabled_texture3d = glIsEnabled(GL_TEXTURE_3D);
		last_is_enabled_texture_cube_map = glIsEnabled(GL_TEXTURE_CUBE_MAP);
	}
	glGetIntegerv(GL_TEXTURE_BINDING_1D, (GLint *) &last_bound_texture1d);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint *) &last_bound_texture2d);
	glGetIntegerv(GL_TEXTURE_BINDING_3D, (GLint *) &last_bound_texture3d);
	glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, (GLint *) &last_bound_texture_cube_map);

	if (texture_type == GL_TEXTURE_1D)
	{
		if (CL_OpenGL::get_opengl_version_major() < 3)
		{
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_TEXTURE_3D);
			glDisable(GL_TEXTURE_CUBE_MAP);
			glEnable(GL_TEXTURE_1D);
		}
		glBindTexture(GL_TEXTURE_1D, handle);
	}

	if (texture_type == GL_TEXTURE_2D)
	{
		if (CL_OpenGL::get_opengl_version_major() < 3)
		{
			glDisable(GL_TEXTURE_1D);
			glDisable(GL_TEXTURE_3D);
			glDisable(GL_TEXTURE_CUBE_MAP);
			glEnable(GL_TEXTURE_2D);
		}
		glBindTexture(GL_TEXTURE_2D, handle);
	}

	if (texture_type == GL_TEXTURE_3D)
	{
		if (CL_OpenGL::get_opengl_version_major() < 3)
		{
			glDisable(GL_TEXTURE_1D);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_TEXTURE_CUBE_MAP);
			glEnable(GL_TEXTURE_3D);
		}
		glBindTexture(GL_TEXTURE_3D, handle);
	}

	if (texture_type == GL_TEXTURE_CUBE_MAP)
	{
		if (CL_OpenGL::get_opengl_version_major() < 3)
		{
			glDisable(GL_TEXTURE_1D);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_TEXTURE_3D);
			glEnable(GL_TEXTURE_CUBE_MAP);
		}
		glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
	}
#endif
}

CL_TextureStateTracker::~CL_TextureStateTracker()
{
#ifdef __APPLE__
	glBindTexture(GL_TEXTURE_2D, last_bound_texture2d);
#else

	if (CL_OpenGL::get_opengl_version_major() < 3)
	{
		if (last_is_enabled_texture1d) glEnable(GL_TEXTURE_1D); else glDisable(GL_TEXTURE_1D);
		if (last_is_enabled_texture2d) glEnable(GL_TEXTURE_2D); else glDisable(GL_TEXTURE_2D);
		if (last_is_enabled_texture3d) glEnable(GL_TEXTURE_3D); else glDisable(GL_TEXTURE_3D);
		if(last_is_enabled_texture_cube_map) glEnable(GL_TEXTURE_CUBE_MAP); else glDisable(GL_TEXTURE_CUBE_MAP);
	}

	if (last_is_enabled_texture1d) glBindTexture(GL_TEXTURE_1D, last_bound_texture1d);
	if (last_is_enabled_texture2d) glBindTexture(GL_TEXTURE_2D, last_bound_texture2d);
	if (last_is_enabled_texture3d) glBindTexture(GL_TEXTURE_3D, last_bound_texture3d);
	if (last_is_enabled_texture_cube_map) glBindTexture(GL_TEXTURE_CUBE_MAP, last_bound_texture_cube_map);
#endif
}
