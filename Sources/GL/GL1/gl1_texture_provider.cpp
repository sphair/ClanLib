/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
#include "API/GL/opengl_defines.h"
#ifndef CLANLIB_OPENGL_ES3

#include "gl1_texture_provider.h"
#include "gl1_graphic_context_provider.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/Render/texture.h"
#include "API/GL/opengl_wrap.h"
#include "API/GL/opengl.h"
#include "API/Core/IOData/cl_endian.h"
#include "API/Core/System/databuffer.h"
#include "API/Display/Render/shared_gc_data.h"
#include "gl1_vertex_array_buffer_provider.h"

#ifndef WIN32
#include "API/Core/Text/console.h"
#endif

namespace clan
{
	GL1TextureProvider::GL1TextureProvider(TextureDimensions texture_dimensions)
	: width(0), height(0), depth(0), handle(0), texture_type(0)
	{
		SharedGCData::add_disposable(this);
		switch (texture_dimensions)
		{
		case TextureDimensions::_1d:
			texture_type = GL_TEXTURE_1D;
			break;
		case TextureDimensions::_2d:
		default:
			texture_type = GL_TEXTURE_2D;
			break;
		case TextureDimensions::_3d:
			texture_type = GL_TEXTURE_3D;
			break;
		case TextureDimensions::_cube:
			texture_type = GL_TEXTURE_CUBE_MAP;
			break;
		}

		OpenGL::set_active();
		glGenTextures(1, &handle);
		GL1TextureStateTracker state_tracker(texture_type, 0);
		glBindTexture(texture_type, handle);
		glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		if (texture_type != GL_TEXTURE_1D)
		{
			glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		if (texture_type == GL_TEXTURE_3D)
			glTexParameteri(texture_type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
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
			if (OpenGL::set_active())
			{
				glDeleteTextures(1, &handle);
			}
		}
	}

	void GL1TextureProvider::generate_mipmap()
	{
	}

	void GL1TextureProvider::create(int new_width, int new_height, int new_depth, int array_size, TextureFormat texture_format, int levels)
	{
		throw_if_disposed();

		GLint gl_internal_format;
		GLenum gl_pixel_format;
		to_opengl_textureformat(texture_format, gl_internal_format, gl_pixel_format);

		if ( (new_width > 32768) || (new_width < 1) )
		{
			throw Exception("Invalid texture width in the GL1 target");
		}

		if ( (texture_type == GL_TEXTURE_2D) || (texture_type == GL_TEXTURE_3D) )
		{
			if ( (new_height > 32768) || (new_height < 1) )
			{
				throw Exception("Invalid texture height in the GL1 target");
			}
		}

		if ( texture_type == GL_TEXTURE_3D )
		{
			if ( (new_depth > 32768) || (new_depth < 1) )
			{
				throw Exception("Invalid texture depth in the GL1 target");
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
			glTexImage1D(
				GL_TEXTURE_1D,			// target
				0,						// level
				gl_internal_format,		// internalformat
				pot_width,				// width
				0,						// border
				gl_pixel_format,		// format
				GL_UNSIGNED_BYTE,		// type (it really doesn't matter since nothing is uploaded)
				nullptr);						// texels (0 to avoid uploading)
		}
		if (texture_type == GL_TEXTURE_2D)
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

			glTexImage2D(
				GL_TEXTURE_2D,			// target
				0,						// level
				gl_internal_format,		// internalformat
				pot_width,				// width
				pot_height,				// height
				0,						// border
				gl_pixel_format,		// format 
				GL_UNSIGNED_BYTE,		// type (it really doesn't matter since nothing is uploaded)
				nullptr);						// texels (0 to avoid uploading)

			// Clear the whole texture if it is npot
			if (!power_of_two_texture)
			{
				PixelBuffer image = PixelBuffer(pot_width, pot_height, TextureFormat::rgba8);
				void *data = image.get_data();
				memset(data, 0, pot_width * pot_height * 4);

				GLenum format;
				GLenum type;
				to_opengl_pixelformat(image, format, type);

				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				const int bytesPerPixel = image.get_bytes_per_pixel();
				glPixelStorei(GL_UNPACK_ROW_LENGTH, image.get_pitch() / bytesPerPixel);
				glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
				glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
				glTexImage2D(
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

			glTexImage3D(
				GL_TEXTURE_3D,			// target
				0,						// level
				gl_internal_format,		// internalformat
				pot_width,				// width
				pot_height,				// height
				pot_depth,				// depth
				0,						// border
				gl_pixel_format,		// format 
				GL_UNSIGNED_BYTE,		// type (it really doesn't matter since nothing is uploaded)
				nullptr);						// texels (0 to avoid uploading)
		}
	}
	PixelBuffer GL1TextureProvider::get_pixeldata(GraphicContext &gc, TextureFormat texture_format, int level) const
	{
		throw_if_disposed();

		OpenGL::set_active(gc);
		GL1TextureStateTracker state_tracker(texture_type, handle);

		GLenum gl_format = 0, gl_type = 0;
		bool supported = to_opengl_pixelformat(texture_format, gl_format, gl_type);
		if (supported)
		{
			PixelBuffer buffer(pot_width, pot_height, texture_format);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glPixelStorei(GL_UNPACK_ROW_LENGTH, buffer.get_pitch() / buffer.get_bytes_per_pixel());
			glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
			glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
			glGetTexImage(texture_type, level, gl_format, gl_type, buffer.get_data());
			return buffer.copy(Rect(0,0, width, height));
		}
		else
		{
			PixelBuffer buffer(pot_width, pot_height, TextureFormat::rgba8);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glPixelStorei(GL_UNPACK_ROW_LENGTH, buffer.get_pitch() / buffer.get_bytes_per_pixel());
			glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
			glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
			glGetTexImage(texture_type, level, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get_data());
			return buffer.copy(Rect(0,0, width, height)).to_format(texture_format);
		}
	}

	void GL1TextureProvider::copy_from(GraphicContext &gc, int x, int y, int slice, int level, const PixelBuffer &ximage, const Rect &src_rect)
	{
		OpenGL::set_active(gc);

		PixelBuffer image = ximage;

		if (src_rect.left < 0 || src_rect.top < 0 || src_rect.right > image.get_width() || src_rect.bottom > image.get_height())
			throw Exception("Rectangle out of bounds");

		throw_if_disposed();
		GL1TextureStateTracker state_tracker(texture_type, handle);

		// check out if the original texture needs or doesn't need an alpha channel
		bool needs_alpha = image.has_transparency();

		GLenum format;
		GLenum type;
		bool conv_needed = !to_opengl_pixelformat(image, format, type);

		// also check for the pitch (GL1 can only skip pixels, not bytes)
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
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			const int bytesPerPixel = image.get_bytes_per_pixel();
			glPixelStorei(GL_UNPACK_ROW_LENGTH, image.get_pitch() / bytesPerPixel);
			glPixelStorei(GL_UNPACK_SKIP_PIXELS, src_rect.left);
			glPixelStorei(GL_UNPACK_SKIP_ROWS, src_rect.top);
		}
		// conversion needed
		else
		{
			bool big_endian = Endian::is_system_big();

			PixelBuffer buffer(
				src_rect.get_width(), src_rect.get_height(),
				needs_alpha ? TextureFormat::rgba8 : TextureFormat::rgb8);
	
			buffer.set_subimage(image, Point(0, 0), src_rect);

			format = needs_alpha ? GL_RGBA : GL_RGB;

			// Upload to GL1:
			glBindTexture(GL_TEXTURE_2D, handle);

			// change alignment
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			const int bytesPerPixel = buffer.get_bytes_per_pixel();
			glPixelStorei(GL_UNPACK_ROW_LENGTH, buffer.get_pitch() / bytesPerPixel);
			glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
			glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

			type = GL_UNSIGNED_BYTE;
			image = buffer;
		}

		// upload
		glTexSubImage2D(
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
					glTexSubImage2D(
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
					glTexSubImage2D(
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
		glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
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
		OpenGL::set_active(static_cast<GL1GraphicContextProvider*>(gc));
		GL1TextureStateTracker state_tracker(texture_type, handle);

		GLint gl_internal_format;
		GLenum gl_pixel_format;
		to_opengl_textureformat(texture_format, gl_internal_format, gl_pixel_format);

		glCopyTexImage2D(
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
		OpenGL::set_active(static_cast<GL1GraphicContextProvider*>(gc));
		GL1TextureStateTracker state_tracker(texture_type, handle);

		glCopyTexSubImage2D( 
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
		glTexParameterf(texture_type, GL_TEXTURE_MIN_LOD, (GLfloat)min_lod);
	}

	void GL1TextureProvider::set_max_lod(double max_lod)
	{
		throw_if_disposed();
		GL1TextureStateTracker state_tracker(texture_type, handle);
		glTexParameterf(texture_type, GL_TEXTURE_MAX_LOD, (GLfloat)max_lod);
	}

	void GL1TextureProvider::set_lod_bias(double lod_bias)
	{
		throw_if_disposed();
		GL1TextureStateTracker state_tracker(texture_type, handle);
		glTexParameterf(texture_type, GL_TEXTURE_LOD_BIAS, (GLfloat)lod_bias);
	}

	void GL1TextureProvider::set_base_level(int base_level)
	{
		throw_if_disposed();
		GL1TextureStateTracker state_tracker(texture_type, handle);
		glTexParameteri(texture_type, GL_TEXTURE_BASE_LEVEL, base_level);
	}

	void GL1TextureProvider::set_max_level(int max_level)
	{
		throw_if_disposed();
		GL1TextureStateTracker state_tracker(texture_type, handle);
		glTexParameteri(texture_type, GL_TEXTURE_MAX_LEVEL, max_level);
	}

	void GL1TextureProvider::set_wrap_mode(
		TextureWrapMode wrap_s,
		TextureWrapMode wrap_t,
		TextureWrapMode wrap_r)
	{
		throw_if_disposed();
		GL1TextureStateTracker state_tracker(texture_type, handle);
		glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, OpenGL::to_enum(wrap_s));
		glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, OpenGL::to_enum(wrap_t));
		glTexParameteri(texture_type, GL_TEXTURE_WRAP_R, OpenGL::to_enum(wrap_r));
	}

	void GL1TextureProvider::set_wrap_mode(
		TextureWrapMode wrap_s,
		TextureWrapMode wrap_t)
	{
		throw_if_disposed();
		GL1TextureStateTracker state_tracker(texture_type, handle);
		glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, OpenGL::to_enum(wrap_s));
		glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, OpenGL::to_enum(wrap_t));
	}

	void GL1TextureProvider::set_wrap_mode(
		TextureWrapMode wrap_s)
	{
		throw_if_disposed();
		GL1TextureStateTracker state_tracker(texture_type, handle);
		glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, OpenGL::to_enum(wrap_s));
	}

	void GL1TextureProvider::set_min_filter(TextureFilter filter)
	{
		throw_if_disposed();
		GL1TextureStateTracker state_tracker(texture_type, handle);
		glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, OpenGL::to_enum(filter));
	}

	void GL1TextureProvider::set_mag_filter(TextureFilter filter)
	{
		throw_if_disposed();
		GL1TextureStateTracker state_tracker(texture_type, handle);
		glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, OpenGL::to_enum(filter));
	}

	void GL1TextureProvider::set_max_anisotropy(float v)
	{
	}

	void GL1TextureProvider::set_texture_compare(TextureCompareMode mode, CompareFunction func)
	{
		throw_if_disposed();
		GL1TextureStateTracker state_tracker(texture_type, handle);
		glTexParameteri(texture_type, GL_TEXTURE_COMPARE_MODE, OpenGL::to_enum(mode));	
		glTexParameteri(texture_type, GL_TEXTURE_COMPARE_FUNC, OpenGL::to_enum(func));	
	}

	void GL1TextureProvider::transform_coordinate(const PrimitivesArrayProvider::VertexData &attribute, std::vector<float> &transformed_data, int vertex_offset, int num_vertices, int total_vertices)
	{
		if (attribute.type != VertexAttributeDataType::type_float)
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

	std::unique_ptr<TextureProvider> GL1TextureProvider::create_view(TextureDimensions texture_dimensions, TextureFormat texture_format, int min_level, int num_levels, int min_layer, int num_layers)
	{
		throw Exception("OpenGL 1 does not support texture views");
	}

	void GL1TextureProvider::set_texture_image2d(
		GLuint target,
		PixelBuffer &image,
		int level)
	{
		throw_if_disposed();
		GL1TextureStateTracker state_tracker(texture_type, handle);

		GLint gl_internal_format;
		GLenum gl_pixel_format;
		to_opengl_textureformat(image.get_format(), gl_internal_format, gl_pixel_format);


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
		bool conv_needed = !to_opengl_pixelformat(image, format, type);

		// also check for the pitch (GL1 can only skip pixels, not bytes)
		if (!conv_needed)
		{
			const int bytesPerPixel = image.get_bytes_per_pixel();
			if (image.get_pitch() % bytesPerPixel != 0)
				conv_needed = true;
		}

		// no conversion needed
		if (!conv_needed)
		{
			// Upload to GL1:

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
				needs_alpha ? TextureFormat::rgba8 : TextureFormat::rgb8);
	
			buffer.set_image(image);

			format = needs_alpha ? GL_RGBA : GL_RGB;

			// Upload to OpenGL:

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
		to_opengl_textureformat(image.get_format(), gl_internal_format, gl_pixel_format);

		// check out if the original texture needs or doesn't need an alpha channel
		bool needs_alpha = image.has_transparency();

		GLenum format;
		GLenum type;
		bool conv_needed = !to_opengl_pixelformat(image, format, type);

		// also check for the pitch (GL1 can only skip pixels, not bytes)
		if (!conv_needed)
		{
			const int bytesPerPixel = image.get_bytes_per_pixel();
			if (image.get_pitch() % bytesPerPixel != 0)
				conv_needed = true;
		}

		// no conversion needed
		if (!conv_needed)
		{
			// Upload to GL1:

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
				needs_alpha ? TextureFormat::rgba8 : TextureFormat::rgb8);
	
			buffer.set_image(image);

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

	/////////////////////////////////////////////////////////////////////////////

	GL1TextureStateTracker::GL1TextureStateTracker(GLuint texture_type, GLuint handle)
	{
		OpenGL::set_active();

		last_is_enabled_texture1d = glIsEnabled(GL_TEXTURE_1D);
		last_is_enabled_texture2d = glIsEnabled(GL_TEXTURE_2D);
		last_is_enabled_texture3d = glIsEnabled(GL_TEXTURE_3D);

		last_is_enabled_texture_cube_map = glIsEnabled(GL_TEXTURE_CUBE_MAP);
		glGetIntegerv(GL_TEXTURE_BINDING_1D, (GLint *) &last_bound_texture1d);
		glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint *) &last_bound_texture2d);
		glGetIntegerv(GL_TEXTURE_BINDING_3D, (GLint *) &last_bound_texture3d);

		glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, (GLint *) &last_bound_texture_cube_map);

		if (texture_type == GL_TEXTURE_1D)
		{
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_TEXTURE_3D);
			glDisable(GL_TEXTURE_CUBE_MAP);
			glEnable(GL_TEXTURE_1D);
			glBindTexture(GL_TEXTURE_1D, handle);
		}
		if (texture_type == GL_TEXTURE_2D)
		{
			glDisable(GL_TEXTURE_1D);
			glDisable(GL_TEXTURE_3D);
			glDisable(GL_TEXTURE_CUBE_MAP);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, handle);
		}

		if (texture_type == GL_TEXTURE_3D)
		{
			glDisable(GL_TEXTURE_1D);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_TEXTURE_CUBE_MAP);
			glEnable(GL_TEXTURE_3D);
			glBindTexture(GL_TEXTURE_3D, handle);
		}

		if (texture_type == GL_TEXTURE_CUBE_MAP)
		{
			glDisable(GL_TEXTURE_1D);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_TEXTURE_3D);
			glEnable(GL_TEXTURE_CUBE_MAP);
			glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
		}
	}

	GL1TextureStateTracker::~GL1TextureStateTracker()
	{
		if (last_is_enabled_texture1d) glEnable(GL_TEXTURE_1D); else glDisable(GL_TEXTURE_1D);
		if (last_is_enabled_texture2d) glEnable(GL_TEXTURE_2D); else glDisable(GL_TEXTURE_2D);
		if (last_is_enabled_texture3d) glEnable(GL_TEXTURE_3D); else glDisable(GL_TEXTURE_3D);

		if (last_is_enabled_texture_cube_map) glEnable(GL_TEXTURE_CUBE_MAP); else glDisable(GL_TEXTURE_CUBE_MAP);

		if (last_is_enabled_texture1d) glBindTexture(GL_TEXTURE_1D, last_bound_texture1d);
		if (last_is_enabled_texture2d) glBindTexture(GL_TEXTURE_2D, last_bound_texture2d);
		if (last_is_enabled_texture3d) glBindTexture(GL_TEXTURE_3D, last_bound_texture3d);
		if (last_is_enabled_texture_cube_map) glBindTexture(GL_TEXTURE_CUBE_MAP, last_bound_texture_cube_map);
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

	void GL1TextureProvider::to_opengl_textureformat(TextureFormat format, GLint &gl_internal_format, GLenum &gl_pixel_format)
	{
		switch (format)
		{
		// base internal format

		// sized internal format

			// TODO: Should this really be here?
			case TextureFormat::stencil_index1: gl_internal_format = GL_STENCIL_INDEX1; gl_pixel_format = GL_STENCIL_INDEX; break;
			case TextureFormat::stencil_index4: gl_internal_format = GL_STENCIL_INDEX4; gl_pixel_format = GL_STENCIL_INDEX; break;
			case TextureFormat::stencil_index8: gl_internal_format = GL_STENCIL_INDEX8; gl_pixel_format = GL_STENCIL_INDEX; break;
			case TextureFormat::stencil_index16: gl_internal_format = GL_STENCIL_INDEX16; gl_pixel_format = GL_STENCIL_INDEX; break;

			//case TextureFormat::r8: gl_internal_format = GL_R8; gl_pixel_format = GL_RED; break;
			//case TextureFormat::r8_snorm: gl_internal_format = GL_R8_SNORM; gl_pixel_format = GL_RED; break;
			//case TextureFormat::r16: gl_internal_format = GL_R16; gl_pixel_format = GL_RED; break;
			//case TextureFormat::r16_snorm: gl_internal_format = GL_R16_SNORM; gl_pixel_format = GL_RED; break;
			//case TextureFormat::rg8: gl_internal_format = GL_RG8; gl_pixel_format = GL_RG; break;
			//case TextureFormat::rg8_snorm: gl_internal_format = GL_RG8_SNORM; gl_pixel_format = GL_RG; break;
			//case TextureFormat::rg16: gl_internal_format = GL_RG16; gl_pixel_format = GL_RG; break;
			//case TextureFormat::rg16_snorm: gl_internal_format = GL_RG16_SNORM; gl_pixel_format = GL_RG; break;
			case TextureFormat::r3_g3_b2: gl_internal_format = GL_R3_G3_B2; gl_pixel_format = GL_RGB; break;
			case TextureFormat::rgb4: gl_internal_format = GL_RGB4; gl_pixel_format = GL_RGB; break;
			case TextureFormat::rgb5: gl_internal_format = GL_RGB5; gl_pixel_format = GL_RGB; break;
			case TextureFormat::rgb8: gl_internal_format = GL_RGB8; gl_pixel_format = GL_RGB; break;
			case TextureFormat::rgb10: gl_internal_format = GL_RGB10; gl_pixel_format = GL_RGB; break;
			case TextureFormat::rgb12: gl_internal_format = GL_RGB12; gl_pixel_format = GL_RGB; break;
			case TextureFormat::rgb16: gl_internal_format = GL_RGB16; gl_pixel_format = GL_RGB; break;
			//case TextureFormat::rgb16_snorm: gl_internal_format = GL_RGB16_SNORM; gl_pixel_format = GL_RGB; break;
			case TextureFormat::rgba2: gl_internal_format = GL_RGBA2; gl_pixel_format = GL_RGBA; break;
			case TextureFormat::rgba4: gl_internal_format = GL_RGBA4; gl_pixel_format = GL_RGBA; break;
			case TextureFormat::rgb5_a1: gl_internal_format = GL_RGB5_A1; gl_pixel_format = GL_RGBA; break;
			case TextureFormat::rgba8: gl_internal_format = GL_RGBA8; gl_pixel_format = GL_RGBA; break;
			case TextureFormat::bgra8: gl_internal_format = GL_RGBA8; gl_pixel_format = GL_BGRA; break;
			case TextureFormat::bgr8: gl_internal_format = GL_RGB8; gl_pixel_format = GL_BGR; break;
			//case TextureFormat::rgba8_snorm: gl_internal_format = GL_RGBA8_SNORM; gl_pixel_format = GL_RGBA; break;
			case TextureFormat::rgb10_a2: gl_internal_format = GL_RGB10_A2; gl_pixel_format = GL_RGBA; break;
			case TextureFormat::rgba12: gl_internal_format = GL_RGBA12; gl_pixel_format = GL_RGBA; break;
			case TextureFormat::rgba16: gl_internal_format = GL_RGBA16; gl_pixel_format = GL_RGBA; break;
			//case TextureFormat::rgba16_snorm: gl_internal_format = GL_RGBA16_SNORM; gl_pixel_format = GL_RGBA; break;
			//case TextureFormat::srgb8: gl_internal_format = GL_SRGB8; gl_pixel_format = GL_RGB; break;
			//case TextureFormat::srgb8_alpha8: gl_internal_format = GL_SRGB8_ALPHA8; gl_pixel_format = GL_RGBA; break;
			//case TextureFormat::r16f: gl_internal_format = GL_R16F; gl_pixel_format = GL_RED; break;
			//case TextureFormat::rg16f: gl_internal_format = GL_RG16F; gl_pixel_format = GL_RG; break;
			//case TextureFormat::rgb16f: gl_internal_format = GL_RGB16F; gl_pixel_format = GL_RGB; break;
			//case TextureFormat::rgba16f: gl_internal_format = GL_RGBA16F; gl_pixel_format = GL_RGBA; break;
			//case TextureFormat::r32f: gl_internal_format = GL_R32F; gl_pixel_format = GL_RED; break;
			//case TextureFormat::rg32f: gl_internal_format = GL_RG32F; gl_pixel_format = GL_RG; break;
			//case TextureFormat::rgb32f: gl_internal_format = GL_RGB32F; gl_pixel_format = GL_RGB; break;
			//case TextureFormat::rgba32f: gl_internal_format = GL_RGBA32F; gl_pixel_format = GL_RGBA; break;
			//case TextureFormat::r11f_g11f_b10f: gl_internal_format = GL_R11F_G11F_B10F; gl_pixel_format = GL_RGB; break;
			//case TextureFormat::rgb9_e5: gl_internal_format = GL_RGB9_E5; gl_pixel_format = GL_RGB; break;
			//case TextureFormat::r8i: gl_internal_format = GL_R8I; gl_pixel_format = GL_RED; break;
			//case TextureFormat::r8ui: gl_internal_format = GL_R8UI; gl_pixel_format = GL_RED; break;
			//case TextureFormat::r16i: gl_internal_format = GL_R16I; gl_pixel_format = GL_RED; break;
			//case TextureFormat::r16ui: gl_internal_format = GL_R16UI; gl_pixel_format = GL_RED; break;
			//case TextureFormat::r32i: gl_internal_format = GL_R32I; gl_pixel_format = GL_RED; break;
			//case TextureFormat::r32ui: gl_internal_format = GL_R32UI; gl_pixel_format = GL_RED; break;
			//case TextureFormat::rg8i: gl_internal_format = GL_RG8I; gl_pixel_format = GL_RG; break;
			//case TextureFormat::rg8ui: gl_internal_format = GL_RG8UI; gl_pixel_format = GL_RG; break;
			//case TextureFormat::rg16i: gl_internal_format = GL_RG16I; gl_pixel_format = GL_RG; break;
			//case TextureFormat::rg16ui: gl_internal_format = GL_RG16UI; gl_pixel_format = GL_RG; break;
			//case TextureFormat::rg32i: gl_internal_format = GL_RG32I; gl_pixel_format = GL_RG; break;
			//case TextureFormat::rg32ui: gl_internal_format = GL_RG32UI; gl_pixel_format = GL_RG; break;
			//case TextureFormat::rgb8i: gl_internal_format = GL_RGB8I; gl_pixel_format = GL_RGB; break;
			//case TextureFormat::rgb8ui: gl_internal_format = GL_RGB8UI; gl_pixel_format = GL_RGB; break;
			//case TextureFormat::rgb16i: gl_internal_format = GL_RGB16I; gl_pixel_format = GL_RGB; break;
			//case TextureFormat::rgb16ui: gl_internal_format = GL_RGB16UI; gl_pixel_format = GL_RGB; break;
			//case TextureFormat::rgb32i: gl_internal_format = GL_RGB32I; gl_pixel_format = GL_RGB; break;
			//case TextureFormat::rgb32ui: gl_internal_format = GL_RGB32UI; gl_pixel_format = GL_RGB; break;
			//case TextureFormat::rgba8i: gl_internal_format = GL_RGBA8I; gl_pixel_format = GL_RGBA; break;
			//case TextureFormat::rgba8ui: gl_internal_format = GL_RGBA8UI; gl_pixel_format = GL_RGBA; break;
			//case TextureFormat::rgba16i: gl_internal_format = GL_RGBA16I; gl_pixel_format = GL_RGBA; break;
			//case TextureFormat::rgba16ui: gl_internal_format = GL_RGBA16UI; gl_pixel_format = GL_RGBA; break;
			//case TextureFormat::rgba32i: gl_internal_format = GL_RGBA32I; gl_pixel_format = GL_RGBA; break;
			//case TextureFormat::rgba32ui: gl_internal_format = GL_RGBA32UI; gl_pixel_format = GL_RGBA; break;
			case TextureFormat::depth_component16: gl_internal_format = GL_DEPTH_COMPONENT16; gl_pixel_format = GL_DEPTH_COMPONENT; break;
			case TextureFormat::depth_component24: gl_internal_format = GL_DEPTH_COMPONENT24; gl_pixel_format = GL_DEPTH_COMPONENT; break;
			case TextureFormat::depth_component32: gl_internal_format = GL_DEPTH_COMPONENT32; gl_pixel_format = GL_DEPTH_COMPONENT; break;
			//case TextureFormat::depth_component32f: gl_internal_format = GL_DEPTH_COMPONENT32F; gl_pixel_format = GL_DEPTH_COMPONENT; break;
			//case TextureFormat::depth24_stencil8: gl_internal_format = GL_DEPTH24_STENCIL8; gl_pixel_format = GL_DEPTH_STENCIL; break;
			//case TextureFormat::depth32f_stencil8: gl_internal_format = GL_DEPTH32F_STENCIL8; gl_pixel_format = GL_DEPTH_STENCIL; break;

			//case TextureFormat::compressed_red: gl_internal_format = GL_COMPRESSED_RED; gl_pixel_format = GL_RED; break;
			//case TextureFormat::compressed_rg: gl_internal_format = GL_COMPRESSED_RG; gl_pixel_format = GL_RG; break;
			case TextureFormat::compressed_rgb: gl_internal_format = GL_COMPRESSED_RGB; gl_pixel_format = GL_RGB; break;
			case TextureFormat::compressed_rgba: gl_internal_format = GL_COMPRESSED_RGBA; gl_pixel_format = GL_RGBA; break;
			//case TextureFormat::compressed_srgb: gl_internal_format = GL_COMPRESSED_SRGB; gl_pixel_format = GL_RGB; break;
			//case TextureFormat::compressed_srgb_alpha: gl_internal_format = GL_COMPRESSED_SRGB_ALPHA; gl_pixel_format = GL_RGBA; break;
			//case TextureFormat::compressed_red_rgtc1: gl_internal_format = GL_COMPRESSED_RED_RGTC1; gl_pixel_format = GL_RED; break;
			//case TextureFormat::compressed_signed_red_rgtc1: gl_internal_format = GL_COMPRESSED_SIGNED_RED_RGTC1; gl_pixel_format = GL_RED; break;
			//case TextureFormat::compressed_rg_rgtc2: gl_internal_format = GL_COMPRESSED_RG_RGTC2; gl_pixel_format = GL_RG; break;
			//case TextureFormat::compressed_signed_rg_rgtc2: gl_internal_format = GL_COMPRESSED_SIGNED_RG_RGTC2; gl_pixel_format = GL_RG; break;

			case TextureFormat::compressed_rgb_s3tc_dxt1: gl_internal_format = GL_COMPRESSED_RGB_S3TC_DXT1_EXT; gl_pixel_format = GL_RGB; break;
			case TextureFormat::compressed_rgba_s3tc_dxt1: gl_internal_format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; gl_pixel_format = GL_RGBA; break;
			case TextureFormat::compressed_rgba_s3tc_dxt3: gl_internal_format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; gl_pixel_format = GL_RGBA; break;
			case TextureFormat::compressed_rgba_s3tc_dxt5: gl_internal_format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; gl_pixel_format = GL_RGBA; break;
			//case TextureFormat::compressed_srgb_s3tc_dxt1: gl_internal_format = GL_COMPRESSED_SRGB_S3TC_DXT1_EXT; gl_pixel_format = GL_RGB; break;
			//case TextureFormat::compressed_srgb_alpha_s3tc_dxt1: gl_internal_format = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT; gl_pixel_format = GL_RGBA; break;
			//case TextureFormat::compressed_srgb_alpha_s3tc_dxt3: gl_internal_format = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT; gl_pixel_format = GL_RGBA; break;
			//case TextureFormat::compressed_srgb_alpha_s3tc_dxt5: gl_internal_format = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT; gl_pixel_format = GL_RGBA; break;

			default:
				throw Exception(string_format("Unsupported TextureFormat (%1)", static_cast<int>(format)));
		}
	}

	bool GL1TextureProvider::to_opengl_pixelformat(TextureFormat texture_format, GLenum &format, GLenum &type)
	{
		bool valid = false;

		//TODO: We should really use Endian::is_system_big()

		//TODO: All the formats in this switch are not supported - Maybe they can be

		switch (texture_format)
		{
			case TextureFormat::rgba8:
			{
				valid = true;
				type = GL_UNSIGNED_BYTE;
				format = GL_RGBA;
				break;
			}
			case TextureFormat::rgb8:
			{
				valid = true;
				type = GL_UNSIGNED_BYTE;
				format = GL_RGB;
				break;
			}
			case TextureFormat::bgr8: 
			{
				valid = true;
				type = GL_UNSIGNED_BYTE;
				format = GL_BGR;
				break;
			}
			case TextureFormat::bgra8: 
			{
				valid = true;
				type = GL_UNSIGNED_BYTE;
				format = GL_BGRA;
				break;
			}
			case TextureFormat::stencil_index1: break;
			case TextureFormat::stencil_index4: break;
			case TextureFormat::stencil_index8: break;
			case TextureFormat::stencil_index16: break;
			case TextureFormat::r8:
			{
				valid = true;
				type = GL_UNSIGNED_BYTE;
				format = GL_RED;
				break;
			}
			case TextureFormat::r8_snorm:
			{
				valid = true;
				type = GL_BYTE;
				format = GL_RED;
				break;
			}
			case TextureFormat::r16:
			{
				valid = true;
				type = GL_UNSIGNED_SHORT;
				format = GL_RED;
				break;
			}
			case TextureFormat::r16_snorm:
			{
				valid = true;
				type = GL_SHORT;
				format = GL_RED;
				break;
			}
			case TextureFormat::rg8: break;
			case TextureFormat::rg8_snorm: break;
			case TextureFormat::rg16: break;
			case TextureFormat::rg16_snorm: break;
			case TextureFormat::r3_g3_b2:
			{
				valid = true;
				type = GL_UNSIGNED_BYTE_3_3_2;
				format = GL_RGB;
				break;
			}
			case TextureFormat::rgb4: break;
			case TextureFormat::rgb5: break;
			case TextureFormat::rgb8_snorm:
			{
				valid = true;
				type = GL_BYTE;
				format = GL_RGB;
				break;
			}
			case TextureFormat::rgb10: break;
			case TextureFormat::rgb12: break;
			case TextureFormat::rgb16: break;
			case TextureFormat::rgb16_snorm: break;
			case TextureFormat::rgba2: break;
			case TextureFormat::rgba4:
			{
				valid = true;
				type = GL_UNSIGNED_SHORT_4_4_4_4;
				format = GL_RGBA;
				break;
			}
			case TextureFormat::rgb5_a1:
			{
				valid = true;
				type = GL_UNSIGNED_SHORT_5_5_5_1;
				format = GL_RGBA;
				break;
			}
			case TextureFormat::rgba8_snorm: break;
			case TextureFormat::rgb10_a2: break;
			case TextureFormat::rgba12: break;
			case TextureFormat::rgba16: break;
			case TextureFormat::rgba16_snorm: break;
			case TextureFormat::srgb8:
			{
				valid = true;
				type = GL_UNSIGNED_BYTE;
				format = GL_RGB;
				break;
			}
			case TextureFormat::srgb8_alpha8:
			{
				valid = true;
				type = GL_UNSIGNED_BYTE;
				format = GL_RGBA;
				break;
			}
			case TextureFormat::r16f: break;
			case TextureFormat::rg16f: break;
			case TextureFormat::rgb16f: break;
			case TextureFormat::rgba16f: break;

			case TextureFormat::r32f:
			{
				valid = true;
				type = GL_FLOAT;
				format = GL_RED;
				break;
			}
			case TextureFormat::rg32f: break;
			case TextureFormat::rgb32f:
			{
				valid = true;
				type = GL_FLOAT;
				format = GL_RGB;
				break;
			}
		
			case TextureFormat::rgba32f:
			{
				valid = true;
				type = GL_FLOAT;
				format = GL_RGBA;
				break;
			}
		
			case TextureFormat::r11f_g11f_b10f: break;
			case TextureFormat::rgb9_e5: break;
			case TextureFormat::r8i:
			{
				valid = true;
				type = GL_BYTE;
				format = GL_RED;
				break;
			}
			case TextureFormat::r8ui:
			{
				valid = true;
				type = GL_UNSIGNED_BYTE;
				format = GL_RED;
				break;
			}
			case TextureFormat::r16i:
			{
				valid = true;
				type = GL_SHORT;
				format = GL_RED;
				break;
			}
			case TextureFormat::r16ui:
			{
				valid = true;
				type = GL_UNSIGNED_SHORT;
				format = GL_RED;
				break;
			}
			case TextureFormat::r32i:
			{
				valid = true;
				type = GL_INT;
				format = GL_RED;
				break;
			}
			case TextureFormat::r32ui:
			{
				valid = true;
				type = GL_UNSIGNED_INT;
				format = GL_RED;
				break;
			}
			case TextureFormat::rg8i: break;
			case TextureFormat::rg8ui: break;
			case TextureFormat::rg16i: break;
			case TextureFormat::rg16ui: break;
			case TextureFormat::rg32i: break;
			case TextureFormat::rg32ui: break;
			case TextureFormat::rgb8i:
			{
				valid = true;
				type = GL_BYTE;
				format = GL_RGB;
				break;
			}
			case TextureFormat::rgb8ui:
			{
				valid = true;
				type = GL_UNSIGNED_BYTE;
				format = GL_RGB;
				break;
			}
			case TextureFormat::rgb16i: break;
			case TextureFormat::rgb16ui: break;
			case TextureFormat::rgb32i: break;
			case TextureFormat::rgb32ui: break;
			case TextureFormat::rgba8i:
			{
				valid = true;
				type = GL_BYTE;
				format = GL_RGBA;
				break;
			}
			case TextureFormat::rgba8ui:
			{
				valid = true;
				type = GL_UNSIGNED_BYTE;
				format = GL_RGBA;
				break;
			}
			case TextureFormat::rgba16i: break;
			case TextureFormat::rgba16ui: break;
			case TextureFormat::rgba32i: break;
			case TextureFormat::rgba32ui: break;
			case TextureFormat::depth_component16: break;
			case TextureFormat::depth_component24: break;
			case TextureFormat::depth_component32: break;
			case TextureFormat::depth_component32f: break;
			case TextureFormat::depth24_stencil8: break;
			case TextureFormat::depth32f_stencil8: break;
			case TextureFormat::compressed_red: break;
			case TextureFormat::compressed_rg: break;
			case TextureFormat::compressed_rgb: break;
			case TextureFormat::compressed_rgba: break;
			case TextureFormat::compressed_srgb: break;
			case TextureFormat::compressed_srgb_alpha: break;
			case TextureFormat::compressed_red_rgtc1: break;
			case TextureFormat::compressed_signed_red_rgtc1: break;
			case TextureFormat::compressed_rg_rgtc2: break;
			case TextureFormat::compressed_signed_rg_rgtc2: break;

			case TextureFormat::compressed_rgb_s3tc_dxt1: break;
			case TextureFormat::compressed_rgba_s3tc_dxt1: break;
			case TextureFormat::compressed_rgba_s3tc_dxt3: break;
			case TextureFormat::compressed_rgba_s3tc_dxt5: break;
			case TextureFormat::compressed_srgb_s3tc_dxt1: break;
			case TextureFormat::compressed_srgb_alpha_s3tc_dxt1: break;
			case TextureFormat::compressed_srgb_alpha_s3tc_dxt3: break;
			case TextureFormat::compressed_srgb_alpha_s3tc_dxt5: break;
		}

		return valid;
	}

	bool GL1TextureProvider::to_opengl_pixelformat(const PixelBuffer &pbuffer, GLenum &format, GLenum &type)
	{
		return to_opengl_pixelformat(pbuffer.get_format(), format, type);
	}
}
#endif
