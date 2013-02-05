/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

#include "LegacyGL/precomp.h"
#include "gl1_texture_provider.h"
#include "gl1_graphic_context_provider.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/Render/texture.h"
#include "opengl1_wrap.h"
#include "opengl1.h"
#include "API/Core/IOData/cl_endian.h"
#include "API/Core/System/databuffer.h"
#include "API/Display/Render/shared_gc_data.h"
#include "gl1_vertex_array_buffer_provider.h"

#ifndef WIN32
#include "API/Core/Text/console.h"
#endif

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GL1TextureProvider Construction:

GL1TextureProvider::GL1TextureProvider(TextureDimensions texture_dimensions)
: width(0), height(0), depth(0), handle(0), texture_type(0)
{
	SharedGCData::add_disposable(this);
	switch (texture_dimensions)
	{
	case texture_1d:
		texture_type = GL_TEXTURE_1D;
		break;
	case texture_2d:
	default:
		texture_type = GL_TEXTURE_2D;
		break;
	case texture_3d:
		texture_type = GL_TEXTURE_3D;
		break;
	case texture_cube:
		texture_type = GL_TEXTURE_CUBE_MAP;
		break;
	}

	GL1::set_active();
	cl1GenTextures(1, &handle);
	GL1TextureStateTracker state_tracker(texture_type, 0);
	cl1BindTexture(texture_type, handle);
	cl1TexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	cl1TexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	cl1TexParameteri(texture_type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	if (texture_type != GL_TEXTURE_1D)
		cl1TexParameteri(texture_type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	if (texture_type == GL_TEXTURE_3D)
		cl1TexParameteri(texture_type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

GL1TextureProvider::~GL1TextureProvider()
{
	dispose();
	SharedGCData::remove_disposable(this);
}

void GL1TextureProvider::on_dispose()
{
	if (handle)
	{
		if (GL1::set_active())
		{
			cl1DeleteTextures(1, &handle);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// GL1TextureProvider Attributes:

/////////////////////////////////////////////////////////////////////////////
// GL1TextureProvider Operations:

void GL1TextureProvider::generate_mipmap()
{
}

void GL1TextureProvider::create(int new_width, int new_height, int new_depth, int array_size, TextureFormat texture_format, int levels)
{
	throw_if_disposed();

	GLint gl_internal_format;
	GLenum gl_pixel_format;
	GL1::to_opengl_textureformat(texture_format, gl_internal_format, gl_pixel_format);

	if ( (new_width > 32768) || (new_width < 1) )
	{
		throw Exception("Invalid texture width in the LegacyGL target");
	}

	if ( (texture_type == GL_TEXTURE_2D) || (texture_type == GL_TEXTURE_3D) )
	{
		if ( (new_height > 32768) || (new_height < 1) )
		{
			throw Exception("Invalid texture height in the LegacyGL target");
		}
	}

	if ( texture_type == GL_TEXTURE_3D )
	{
		if ( (new_depth > 32768) || (new_depth < 1) )
		{
			throw Exception("Invalid texture depth in the LegacyGL target");
		}
	}

	width = new_width;
	height = new_height;
	depth = new_depth;
	GL1TextureStateTracker state_tracker(texture_type, handle);

	if (texture_type == GL_TEXTURE_1D)
	{
		pot_width = get_next_power_of_two(new_width);
		if (pot_width == new_width)
		{
			power_of_two_texture=true;
		}
		else
		{
			power_of_two_texture=false;
		}

		pot_ratio_width = (float) width / pot_width;
		cl1TexImage1D(
			GL_TEXTURE_1D,			// target
			0,						// level
			gl_internal_format,		// internalformat
			pot_width,				// width
			0,						// border
			gl_pixel_format,		// format
			GL_UNSIGNED_BYTE,		// type (it really doesn't matter since nothing is uploaded)
			0);						// texels (0 to avoid uploading)
	}
	else if (texture_type == GL_TEXTURE_2D)
	{
		pot_width = get_next_power_of_two(new_width);
		pot_height = get_next_power_of_two(new_height);
		if ( (pot_width == new_width) && (pot_height == new_height))
		{
			power_of_two_texture=true;
		}
		else
		{
			power_of_two_texture=false;
		}
		pot_ratio_width = (float) width / pot_width;
		pot_ratio_height = (float) height / pot_height;

		cl1TexImage2D(
			GL_TEXTURE_2D,			// target
			0,						// level
			gl_internal_format,		// internalformat
			pot_width,				// width
			pot_height,				// height
			0,						// border
			gl_pixel_format,		// format 
			GL_UNSIGNED_BYTE,		// type (it really doesn't matter since nothing is uploaded)
			0);						// texels (0 to avoid uploading)

		// Clear the whole texture if it is npot
		if (!power_of_two_texture)
		{
			PixelBuffer image = PixelBuffer(pot_width, pot_height, tf_rgba8);
			void *data = image.get_data();
			memset(data, 0, pot_width * pot_height * 4);

			GLenum format;
			GLenum type;
			GL1::to_opengl_pixelformat(image, format, type);

			cl1PixelStorei(GL_UNPACK_ALIGNMENT, 1);
			const int bytesPerPixel = image.get_bytes_per_pixel();
			cl1PixelStorei(GL_UNPACK_ROW_LENGTH, image.get_pitch() / bytesPerPixel);
			cl1PixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
			cl1PixelStorei(GL_UNPACK_SKIP_ROWS, 0);

			cl1TexImage2D(
				GL_TEXTURE_2D,		// target
				0,					// level
				gl_internal_format,	// internalformat
				pot_width,			// width
				pot_height,			// height
				0,					// border
				format,				// format
				type,				// type
				data);				// texels

		}
	}
	else
	{
		pot_width = get_next_power_of_two(new_width);
		pot_height = get_next_power_of_two(new_height);
		pot_depth = get_next_power_of_two(new_depth);
		pot_ratio_width = (float) width / pot_width;
		pot_ratio_height = (float) height / pot_height;
		pot_ratio_depth = (float) depth / pot_depth;
		if ( (pot_width == new_width) && (pot_height == new_height) && (pot_depth == new_depth))
		{
			power_of_two_texture=true;
		}
		else
		{
			power_of_two_texture=false;
		}

		cl1TexImage3D(
			GL_TEXTURE_3D,			// target
			0,						// level
			gl_internal_format,		// internalformat
			pot_width,				// width
			pot_height,				// height
			pot_depth,				// depth
			0,						// border
			gl_pixel_format,		// format 
			GL_UNSIGNED_BYTE,		// type (it really doesn't matter since nothing is uploaded)
			0);						// texels (0 to avoid uploading)
	}
}
PixelBuffer GL1TextureProvider::get_pixeldata(GraphicContext &gc, TextureFormat texture_format, int level) const
{
	throw_if_disposed();

	GL1::set_active(gc);
	GL1TextureStateTracker state_tracker(texture_type, handle);

	GLenum gl_format = 0, gl_type = 0;
	bool supported = GL1::to_opengl_pixelformat(texture_format, gl_format, gl_type);
	if (supported)
	{
		PixelBuffer buffer(width, height, texture_format);
		cl1GetTexImage(texture_type, level, gl_format, gl_type, buffer.get_data());
		return buffer;
	}
	else
	{
		PixelBuffer buffer(width, height, tf_rgba8);
		cl1GetTexImage(texture_type, level, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get_data());
		return buffer.to_format(texture_format);
	}
}

void GL1TextureProvider::copy_from(GraphicContext &gc, int x, int y, int slice, int level, const PixelBuffer &ximage, const Rect &src_rect)
{
	GL1::set_active(gc);

	PixelBuffer image = ximage;

	if (src_rect.left < 0 || src_rect.top < 0 || src_rect.right > image.get_width(), src_rect.bottom > image.get_height())
		throw Exception("Rectangle out of bounds");

	throw_if_disposed();
	GL1TextureStateTracker state_tracker(texture_type, handle);

	// check out if the original texture needs or doesn't need an alpha channel
	bool needs_alpha = image.has_transparency();

	GLenum format;
	GLenum type;
	bool conv_needed = !GL1::to_opengl_pixelformat(image, format, type);

	// also check for the pitch (LegacyGL can only skip pixels, not bytes)
	if (!conv_needed)
	{
		const int bytesPerPixel = image.get_bytes_per_pixel();
		if (image.get_pitch() % bytesPerPixel != 0)
			conv_needed = true;
	}

	// no conversion needed
	if (!conv_needed)
	{
		// change alignment
		cl1PixelStorei(GL_UNPACK_ALIGNMENT, 1);
		const int bytesPerPixel = image.get_bytes_per_pixel();
		cl1PixelStorei(GL_UNPACK_ROW_LENGTH, image.get_pitch() / bytesPerPixel);
		cl1PixelStorei(GL_UNPACK_SKIP_PIXELS, src_rect.left);
		cl1PixelStorei(GL_UNPACK_SKIP_ROWS, src_rect.top);

	}
	// conversion needed
	else
	{
		bool big_endian = Endian::is_system_big();

		PixelBuffer buffer(
			src_rect.get_width(), src_rect.get_height(),
			needs_alpha ? tf_rgba8 : tf_rgb8);
	
		buffer.set_subimage(image, Point(0, 0), src_rect);

		format = needs_alpha ? GL_RGBA : GL_RGB;

		// Upload to LegacyGL:
		cl1BindTexture(GL_TEXTURE_2D, handle);

		// change alignment
		cl1PixelStorei(GL_UNPACK_ALIGNMENT, 1);
		const int bytesPerPixel = buffer.get_bytes_per_pixel();
		cl1PixelStorei(GL_UNPACK_ROW_LENGTH, buffer.get_pitch() / bytesPerPixel);
		cl1PixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		cl1PixelStorei(GL_UNPACK_SKIP_ROWS, 0);

		type = GL_UNSIGNED_BYTE;
		image = buffer;
	}

	// upload
	cl1TexSubImage2D(
		GL_TEXTURE_2D,            // target
		level,                    // level
		x, y,                     // xoffset, yoffset
		src_rect.get_width(),        // width
		src_rect.get_height(),       // height
		format,					  // format
		type,					  // type
		image.get_data());        // texels

	if (!power_of_two_texture)
	{
		// TODO: This needs corrected.It should be optimised and currently it does not write to the lower right quadrant

		// Check extend the right edge
		int right_edge = x + image.get_width();
		if ( right_edge >= width )
		{
			char *edge_data = (char *) image.get_data();
			edge_data += image.get_bytes_per_pixel() * (width-1);

			for(int edge_cnt = right_edge; edge_cnt < pot_width; edge_cnt++)
			{
				cl1TexSubImage2D(
					GL_TEXTURE_2D,		// target
					level,				// level
					edge_cnt, y,		// xoffset, yoffset
					1,					// width
					src_rect.get_height(),	// height
					format,				// format
					type,				// type
					edge_data);				// texels
			}
		}
		// Check extend the bottom edge
		int bottom_edge = y + image.get_height();
		if ( bottom_edge >= height )
		{
			char *edge_data = (char *) image.get_data();
			edge_data += image.get_pitch() * (height-1);

			for(int edge_cnt = bottom_edge; edge_cnt < pot_height; edge_cnt++)
			{
				cl1TexSubImage2D(
					GL_TEXTURE_2D,		// target
					level,				// level
					x, edge_cnt,		// xoffset, yoffset
					src_rect.get_width(),		// width
					1,	// height
					format,				// format
					type,				// type
					edge_data);				// texels
			}
		}
	}
	// Restore these unpack values to the default
	cl1PixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	cl1PixelStorei(GL_UNPACK_SKIP_ROWS, 0);
}

void GL1TextureProvider::copy_image_from(
	int x,
	int y,
	int width,
	int height,
	int level,
	TextureFormat texture_format,
	GraphicContextProvider *gc)
{
	throw_if_disposed();
	GL1::set_active(static_cast<GL1GraphicContextProvider*>(gc));
	GL1TextureStateTracker state_tracker(texture_type, handle);

	GLint gl_internal_format;
	GLenum gl_pixel_format;
	GL1::to_opengl_textureformat(texture_format, gl_internal_format, gl_pixel_format);

	cl1CopyTexImage2D(
		GL_TEXTURE_2D,
		level,
		gl_internal_format,
		x, y,
		width, height,
		0);
}

void GL1TextureProvider::copy_subimage_from(
	int offset_x,
	int offset_y,
	int x,
	int y,
	int width,
	int height,
	int level,
	GraphicContextProvider *gc)
{
	throw_if_disposed();
	GL1::set_active(static_cast<GL1GraphicContextProvider*>(gc));
	GL1TextureStateTracker state_tracker(texture_type, handle);

	cl1CopyTexSubImage2D( 
		GL_TEXTURE_2D,
		level, 
		offset_x, 
		offset_y, 
		x, y,
		width, height );
}

void GL1TextureProvider::set_min_lod(double min_lod)
{
	throw_if_disposed();
	GL1TextureStateTracker state_tracker(texture_type, handle);
	cl1TexParameterf(texture_type, GL_TEXTURE_MIN_LOD, (GLfloat)min_lod);
}

void GL1TextureProvider::set_max_lod(double max_lod)
{
	throw_if_disposed();
	GL1TextureStateTracker state_tracker(texture_type, handle);
	cl1TexParameterf(texture_type, GL_TEXTURE_MAX_LOD, (GLfloat)max_lod);
}

void GL1TextureProvider::set_lod_bias(double lod_bias)
{
	throw_if_disposed();
	GL1TextureStateTracker state_tracker(texture_type, handle);
	cl1TexParameterf(texture_type, GL_TEXTURE_LOD_BIAS, (GLfloat)lod_bias);
}

void GL1TextureProvider::set_base_level(int base_level)
{
	throw_if_disposed();
	GL1TextureStateTracker state_tracker(texture_type, handle);
	cl1TexParameteri(texture_type, GL_TEXTURE_BASE_LEVEL, base_level);
}

void GL1TextureProvider::set_max_level(int max_level)
{
	throw_if_disposed();
	GL1TextureStateTracker state_tracker(texture_type, handle);
	cl1TexParameteri(texture_type, GL_TEXTURE_MAX_LEVEL, max_level);
}

void GL1TextureProvider::set_wrap_mode(
	TextureWrapMode wrap_s,
	TextureWrapMode wrap_t,
	TextureWrapMode wrap_r)
{
	throw_if_disposed();
	GL1TextureStateTracker state_tracker(texture_type, handle);
	cl1TexParameteri(texture_type, GL_TEXTURE_WRAP_S, to_enum(wrap_s));
	cl1TexParameteri(texture_type, GL_TEXTURE_WRAP_T, to_enum(wrap_t));
	cl1TexParameteri(texture_type, GL_TEXTURE_WRAP_R, to_enum(wrap_r));
}

void GL1TextureProvider::set_wrap_mode(
	TextureWrapMode wrap_s,
	TextureWrapMode wrap_t)
{
	throw_if_disposed();
	GL1TextureStateTracker state_tracker(texture_type, handle);
	cl1TexParameteri(texture_type, GL_TEXTURE_WRAP_S, to_enum(wrap_s));
	cl1TexParameteri(texture_type, GL_TEXTURE_WRAP_T, to_enum(wrap_t));
}

void GL1TextureProvider::set_wrap_mode(
	TextureWrapMode wrap_s)
{
	throw_if_disposed();
	GL1TextureStateTracker state_tracker(texture_type, handle);
	cl1TexParameteri(texture_type, GL_TEXTURE_WRAP_S, to_enum(wrap_s));
}

void GL1TextureProvider::set_min_filter(TextureFilter filter)
{
	throw_if_disposed();
	GL1TextureStateTracker state_tracker(texture_type, handle);
	cl1TexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, to_enum(filter));
}

void GL1TextureProvider::set_mag_filter(TextureFilter filter)
{
	throw_if_disposed();
	GL1TextureStateTracker state_tracker(texture_type, handle);
	cl1TexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, to_enum(filter));
}

void GL1TextureProvider::set_max_anisotropy(float v)
{
}

void GL1TextureProvider::set_texture_compare(TextureCompareMode mode, CompareFunction func)
{
	throw_if_disposed();
	GL1TextureStateTracker state_tracker(texture_type, handle);
	cl1TexParameteri(texture_type, GL_TEXTURE_COMPARE_MODE, to_enum(mode));	
	cl1TexParameteri(texture_type, GL_TEXTURE_COMPARE_FUNC, to_enum(func));	
}

void GL1TextureProvider::transform_coordinate(const PrimitivesArrayProvider::VertexData &attribute, std::vector<float> &transformed_data, int vertex_offset, int num_vertices, int total_vertices)
{
	if (attribute.type != type_float)
	{
		throw Exception("Implement me: Texture coord npot transformation (not float) (GL1 target)");
	}

	int desired_size = attribute.size * (total_vertices);
	if (transformed_data.size() < desired_size)
		transformed_data.resize(desired_size);

	int size = attribute.size;

	int stride_float;
	if (attribute.stride)
	{
		stride_float = (attribute.stride / sizeof(float));
	}
	else
	{
		stride_float = size;
	}

	int source_position = vertex_offset * stride_float;

	GL1VertexArrayBufferProvider *vertex_array_ptr = static_cast<GL1VertexArrayBufferProvider *>(attribute.array_provider);
	if (!vertex_array_ptr)
		throw Exception("Invalid BindBuffer Provider");

	const char *data_ptr = ((const char *) vertex_array_ptr->get_data()) + attribute.offset;

	float *src = (float *) data_ptr;
	float *dest = &transformed_data[vertex_offset * size];

	stride_float -= size;

	if (size==1)
	{
		for (int vertex_count=0; vertex_count < num_vertices; vertex_count++, source_position += stride_float)
		{
			*(dest++) = src[source_position++] * pot_ratio_width;
		}
	}
	else if (size==2)
	{
		for (int vertex_count=0; vertex_count < num_vertices; vertex_count++, source_position += stride_float)
		{
			*(dest++) = src[source_position++] * pot_ratio_width;
			*(dest++) = src[source_position++] * pot_ratio_height;
		}
	}
	else if (size==3)
	{
		for (int vertex_count=0; vertex_count < num_vertices; vertex_count++, source_position += stride_float)
		{
			*(dest++) = src[source_position++] * pot_ratio_width;
			*(dest++) = src[source_position++] * pot_ratio_height;
			*(dest++) = src[source_position++] * pot_ratio_depth;
		}
	}
}

TextureProvider *GL1TextureProvider::create_view(TextureDimensions texture_dimensions, TextureFormat texture_format, int min_level, int num_levels, int min_layer, int num_layers)
{
	throw Exception("OpenGL 1 does not support texture views");
}

/////////////////////////////////////////////////////////////////////////////
// GL1TextureProvider Implementation:


void GL1TextureProvider::set_texture_image2d(
	GLuint target,
	PixelBuffer &image,
	int level)
{
	throw_if_disposed();
	GL1TextureStateTracker state_tracker(texture_type, handle);

	GLint gl_internal_format;
	GLenum gl_pixel_format;
	GL1::to_opengl_textureformat(image.get_format(), gl_internal_format, gl_pixel_format);


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
	bool needs_alpha = image.has_transparency();

	GLenum format;
	GLenum type;
	bool conv_needed = !GL1::to_opengl_pixelformat(image, format, type);

	// also check for the pitch (LegacyGL can only skip pixels, not bytes)
	if (!conv_needed)
	{
		const int bytesPerPixel = image.get_bytes_per_pixel();
		if (image.get_pitch() % bytesPerPixel != 0)
			conv_needed = true;
	}

	// no conversion needed
	if (!conv_needed)
	{
		// Upload to LegacyGL:

		// change alignment
		cl1PixelStorei(GL_UNPACK_ALIGNMENT, 1);
		const int bytesPerPixel = image.get_bytes_per_pixel();
		cl1PixelStorei(GL_UNPACK_ROW_LENGTH, image.get_pitch() / bytesPerPixel);
		cl1PixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		cl1PixelStorei(GL_UNPACK_SKIP_ROWS, 0);

		char *data = (char *) image.get_data();
		int image_width = image.get_width();
		int image_height = image.get_height();
/*
		int image_width = 1;
		int image_height = 1;
		while (image_width < image.get_width()) image_width *= 2;
		while (image_height < image.get_height()) image_height *= 2;
*/
		cl1TexImage2D(
			target,                   // target
			level,                    // level
			gl_internal_format,           // internalformat
			image_width,              // width
			image_height,             // height
			0,						 // border
			format,                   // format
			type,                     // type
			data);                    // texels
	}
	// conversion needed
	else
	{
		bool big_endian = Endian::is_system_big();

		PixelBuffer buffer(
			image.get_width(), image.get_height(),
			needs_alpha ? tf_rgba8 : tf_rgb8);
	
		buffer.set_image(image);

		format = needs_alpha ? GL_RGBA : GL_RGB;

		// Upload to OpenGL:

		// change alignment
		cl1PixelStorei(GL_UNPACK_ALIGNMENT, 1);
		const int bytesPerPixel = buffer.get_bytes_per_pixel();
		cl1PixelStorei(GL_UNPACK_ROW_LENGTH, buffer.get_pitch() / bytesPerPixel);
		cl1PixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		cl1PixelStorei(GL_UNPACK_SKIP_ROWS, 0);

		// upload
		cl1TexImage2D(
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
		cl1TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		cl1TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		cl1TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		cl1TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		*/
	}
}

void GL1TextureProvider::set_texture_image3d(
	GLuint target,
	PixelBuffer &image,
	int image_depth,
	int level)
{
	throw_if_disposed();
	GL1TextureStateTracker state_tracker(texture_type, handle);

	GLint gl_internal_format;
	GLenum gl_pixel_format;
	GL1::to_opengl_textureformat(image.get_format(), gl_internal_format, gl_pixel_format);

	// check out if the original texture needs or doesn't need an alpha channel
	bool needs_alpha = image.has_transparency();

	GLenum format;
	GLenum type;
	bool conv_needed = !GL1::to_opengl_pixelformat(image, format, type);

	// also check for the pitch (LegacyGL can only skip pixels, not bytes)
	if (!conv_needed)
	{
		const int bytesPerPixel = image.get_bytes_per_pixel();
		if (image.get_pitch() % bytesPerPixel != 0)
			conv_needed = true;
	}

	// no conversion needed
	if (!conv_needed)
	{
		// Upload to LegacyGL:

		// change alignment
		cl1PixelStorei(GL_UNPACK_ALIGNMENT, 1);
		const int bytesPerPixel = image.get_bytes_per_pixel();
		cl1PixelStorei(GL_UNPACK_ROW_LENGTH, image.get_pitch() / bytesPerPixel);
		cl1PixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		cl1PixelStorei(GL_UNPACK_SKIP_ROWS, 0);

		char *data = (char *) image.get_data();
		int image_width = image.get_width();
		int image_height = image.get_height() / image_depth;

		cl1TexImage3D(
			target,                   // target
			level,                    // level
			gl_internal_format,           // internalformat
			image_width,              // width
			image_height,             // height
			image_depth,             // depth
			0,						 // border
			format,                   // format
			type,                     // type
			data);                    // texels
	}
	// conversion needed
	else
	{
		bool big_endian = Endian::is_system_big();

		PixelBuffer buffer(
			image.get_width(), image.get_height(),
			needs_alpha ? tf_rgba8 : tf_rgb8);
	
		buffer.set_image(image);

		format = needs_alpha ? GL_RGBA : GL_RGB;

		// Upload to OpenGL:

		// change alignment
		cl1PixelStorei(GL_UNPACK_ALIGNMENT, 1);
		const int bytesPerPixel = buffer.get_bytes_per_pixel();
		cl1PixelStorei(GL_UNPACK_ROW_LENGTH, buffer.get_pitch() / bytesPerPixel);
		cl1PixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		cl1PixelStorei(GL_UNPACK_SKIP_ROWS, 0);

		int image_width = image.get_width();
		int image_height = image.get_height() / image_depth;

		// upload
		cl1TexImage3D(
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

GLenum GL1TextureProvider::to_enum(TextureFilter filter)
{
	switch(filter)
	{
	case filter_nearest: return GL_NEAREST;
	case filter_linear: return GL_LINEAR;
	case filter_nearest_mipmap_nearest: return GL_NEAREST_MIPMAP_NEAREST;
	case filter_nearest_mipmap_linear: return GL_NEAREST_MIPMAP_LINEAR;
	case filter_linear_mipmap_nearest: return GL_LINEAR_MIPMAP_NEAREST;
	case filter_linear_mipmap_linear: return GL_LINEAR_MIPMAP_LINEAR;
	default: return GL_NEAREST;
	}
}

GLenum GL1TextureProvider::to_enum(TextureWrapMode mode)
{
 	switch(mode)
	{
	case wrap_clamp_to_edge: return GL_CLAMP_TO_EDGE;
	case wrap_repeat: return GL_REPEAT;
	case wrap_mirrored_repeat: return GL_MIRRORED_REPEAT;
	default: return GL_CLAMP_TO_EDGE;
	}
}

GLenum GL1TextureProvider::to_enum(TextureCompareMode mode)
{
 	switch(mode)
	{
	case comparemode_none: return GL_NONE;
	case comparemode_compare_r_to_texture: return GL_COMPARE_R_TO_TEXTURE;		
	default: return GL_NONE;
	}
}

GLenum GL1TextureProvider::to_enum(CompareFunction func)
{
	switch( func )
	{
	case compare_never: return GL_NEVER;
	case compare_less: return GL_LESS;
	case compare_lequal: return GL_LEQUAL; 
	case compare_greater: return GL_GREATER; 
	case compare_gequal: return GL_GEQUAL; 
	case compare_equal: return GL_EQUAL; 
	case compare_notequal: return GL_NOTEQUAL; 
	case compare_always: return GL_ALWAYS; 
	default: return GL_LEQUAL;
	}
}

/////////////////////////////////////////////////////////////////////////////

GL1TextureStateTracker::GL1TextureStateTracker(GLuint texture_type, GLuint handle)
{
	GL1::set_active();

	last_is_enabled_texture1d = cl1IsEnabled(GL_TEXTURE_1D);
	last_is_enabled_texture2d = cl1IsEnabled(GL_TEXTURE_2D);
	last_is_enabled_texture3d = cl1IsEnabled(GL_TEXTURE_3D);
	last_is_enabled_texture_cube_map = cl1IsEnabled(GL_TEXTURE_CUBE_MAP);
	cl1GetIntegerv(GL_TEXTURE_BINDING_1D, (GLint *) &last_bound_texture1d);
	cl1GetIntegerv(GL_TEXTURE_BINDING_2D, (GLint *) &last_bound_texture2d);
	cl1GetIntegerv(GL_TEXTURE_BINDING_3D, (GLint *) &last_bound_texture3d);
	cl1GetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, (GLint *) &last_bound_texture_cube_map);

	if (texture_type == GL_TEXTURE_1D)
	{
		cl1Disable(GL_TEXTURE_2D);
		cl1Disable(GL_TEXTURE_3D);
		cl1Disable(GL_TEXTURE_CUBE_MAP);
		cl1Enable(GL_TEXTURE_1D);
		cl1BindTexture(GL_TEXTURE_1D, handle);
	}

	if (texture_type == GL_TEXTURE_2D)
	{
		cl1Disable(GL_TEXTURE_1D);
		cl1Disable(GL_TEXTURE_3D);
		cl1Disable(GL_TEXTURE_CUBE_MAP);
		cl1Enable(GL_TEXTURE_2D);
		cl1BindTexture(GL_TEXTURE_2D, handle);
	}

	if (texture_type == GL_TEXTURE_3D)
	{
		cl1Disable(GL_TEXTURE_1D);
		cl1Disable(GL_TEXTURE_2D);
		cl1Disable(GL_TEXTURE_CUBE_MAP);
		cl1Enable(GL_TEXTURE_3D);
		cl1BindTexture(GL_TEXTURE_3D, handle);
	}

	if (texture_type == GL_TEXTURE_CUBE_MAP)
	{
		cl1Disable(GL_TEXTURE_1D);
		cl1Disable(GL_TEXTURE_2D);
		cl1Disable(GL_TEXTURE_3D);
		cl1Enable(GL_TEXTURE_CUBE_MAP);
		cl1BindTexture(GL_TEXTURE_CUBE_MAP, handle);
	}
}

GL1TextureStateTracker::~GL1TextureStateTracker()
{
	if (last_is_enabled_texture1d) cl1Enable(GL_TEXTURE_1D); else cl1Disable(GL_TEXTURE_1D);
	if (last_is_enabled_texture2d) cl1Enable(GL_TEXTURE_2D); else cl1Disable(GL_TEXTURE_2D);
	if (last_is_enabled_texture3d) cl1Enable(GL_TEXTURE_3D); else cl1Disable(GL_TEXTURE_3D);
	if (last_is_enabled_texture_cube_map) cl1Enable(GL_TEXTURE_CUBE_MAP); else cl1Disable(GL_TEXTURE_CUBE_MAP);

	if (last_is_enabled_texture1d) cl1BindTexture(GL_TEXTURE_1D, last_bound_texture1d);
	if (last_is_enabled_texture2d) cl1BindTexture(GL_TEXTURE_2D, last_bound_texture2d);
	if (last_is_enabled_texture3d) cl1BindTexture(GL_TEXTURE_3D, last_bound_texture3d);
	if (last_is_enabled_texture_cube_map) cl1BindTexture(GL_TEXTURE_CUBE_MAP, last_bound_texture_cube_map);
}

int GL1TextureProvider::get_next_power_of_two(int value)
{
	for (int pot_count = 1; pot_count < 32768; pot_count += pot_count)
	{
		if (value <= pot_count)
			return pot_count;
	}
	return 32768;
}

}
