/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
#include "gl3_texture_provider.h"
#include "gl3_graphic_context_provider.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/Render/texture.h"
#include "API/Display/Render/shared_gc_data.h"
#include "API/GL/opengl_wrap.h"
#include "API/GL/opengl.h"
#include "API/Core/IOData/cl_endian.h"
#include "API/Core/System/databuffer.h"
#include "gl3_pixel_buffer_provider.h"

namespace clan
{
	GL3TextureProvider::GL3TextureProvider(GLuint texture_type, GLuint handle)
		: width(0), height(0), depth(0), array_size(0), handle(handle), texture_type(texture_type)
	{
		SharedGCData::add_disposable(this);
		TextureStateTracker state_tracker(texture_type, handle);
		glGetIntegerv(GL_TEXTURE_WIDTH, &width);
		glGetIntegerv(GL_TEXTURE_HEIGHT, &height);
		glGetIntegerv(GL_TEXTURE_DEPTH, &depth);
	}

	GL3TextureProvider::GL3TextureProvider(TextureDimensions texture_dimensions)
		: width(0), height(0), depth(0), handle(0), texture_type(0)
	{
		create_initial(texture_dimensions);
	}

	GL3TextureProvider::GL3TextureProvider(GL3TextureProvider *orig_texture, TextureDimensions texture_dimensions, TextureFormat texture_format, int min_level, int num_levels, int min_layer, int num_layers)
		: width(0), height(0), depth(0), handle(0), texture_type(0)
	{
		create_initial(texture_dimensions);

		TextureFormat_GL tf = OpenGL::get_textureformat(texture_format);
		if (!tf.valid)
			throw Exception("Texture format not supported by OpenGL");

		if (!glTextureView)
			throw Exception("glTextureView required OpenGL 4.3");

		glTextureView(handle, texture_type, orig_texture->handle, tf.internal_format, min_level, num_levels, min_layer, num_layers);
	}

	void GL3TextureProvider::create_initial(TextureDimensions texture_dimensions)
	{
		switch (texture_dimensions)
		{
		case texture_1d:
			texture_type = GL_TEXTURE_1D;
			break;
		case texture_1d_array:
			texture_type = GL_TEXTURE_1D_ARRAY;
			break;
		case texture_2d:
			texture_type = GL_TEXTURE_2D;
			break;
		case texture_2d_array:
			texture_type = GL_TEXTURE_2D_ARRAY;
			break;
		case texture_3d:
			texture_type = GL_TEXTURE_3D;
			break;
		case texture_cube:
			texture_type = GL_TEXTURE_CUBE_MAP;
			break;
		default:
			throw Exception("Unsupported texture type");
		}

		SharedGCData::add_disposable(this);

		TextureStateTracker state_tracker(texture_type, 0);
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

	GL3TextureProvider::~GL3TextureProvider()
	{
		dispose();
		SharedGCData::remove_disposable(this);
	}

	void GL3TextureProvider::on_dispose()
	{
		if (handle)
		{
			if (OpenGL::set_active())
			{
				glDeleteTextures(1, &handle);
			}
		}
	}

	void GL3TextureProvider::generate_mipmap()
	{
		throw_if_disposed();
		TextureStateTracker state_tracker(texture_type, handle);
		glGenerateMipmap(texture_type);
	}

	void GL3TextureProvider::create(int new_width, int new_height, int new_depth, int new_array_size, TextureFormat texture_format, int levels)
	{
		throw_if_disposed();

		TextureFormat_GL tf = OpenGL::get_textureformat(texture_format);
		if (!tf.valid)
			throw Exception("Texture format not supported by OpenGL");

		width = new_width;
		height = new_height;
		depth = new_depth;
		array_size = new_array_size;
		gl_internal_format = tf.internal_format;

		TextureStateTracker state_tracker(texture_type, handle);

		// To do: move these settings to API
		GLsizei samples = 1;
		GLboolean fixed_sample_locations = GL_FALSE;

		if (levels == 0)
		{
			do
			{
				levels++;
			} while (max(width >> levels, 1) != 1 || max(height >> levels, 1) != 1);
		}

		// Emulate glTexStorage behavior so we can support older versions of OpenGL
		for (int level = 0; level < levels; level++)
		{
			int mip_width = max(width >> level, 1);
			int mip_height = max(height >> level, 1);

			if (texture_type == GL_TEXTURE_1D)
			{
				glTexImage1D(GL_TEXTURE_1D, level, tf.internal_format, mip_width, 0, tf.pixel_format, tf.pixel_datatype, nullptr);
			}
			else if (texture_type == GL_TEXTURE_1D_ARRAY)
			{
				glTexImage2D(GL_TEXTURE_1D_ARRAY, level, tf.internal_format, mip_width, array_size, 0, tf.pixel_format, tf.pixel_datatype, nullptr);
			}
			else if (texture_type == GL_TEXTURE_2D)
			{
				if (PixelBuffer::is_compressed(texture_format))
				{
					unsigned data_size = PixelBuffer::get_data_size(Size(mip_width, mip_height), texture_format);
					glCompressedTexImage2D(GL_TEXTURE_2D, level, tf.internal_format, mip_width, mip_height, 0, data_size, nullptr);
				}
				else
				{
					glTexImage2D(GL_TEXTURE_2D, level, tf.internal_format, mip_width, mip_height, 0, tf.pixel_format, tf.pixel_datatype, nullptr);
				}
			}
			else if (texture_type == GL_TEXTURE_2D_ARRAY)
			{
				glTexImage3D(GL_TEXTURE_2D_ARRAY, level, tf.internal_format, mip_width, mip_height, array_size, 0, tf.pixel_format, tf.pixel_datatype, nullptr);
			}
			else if (texture_type == GL_TEXTURE_2D_MULTISAMPLE)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, tf.internal_format, mip_width, mip_height, fixed_sample_locations);
			}
			else if (texture_type == GL_TEXTURE_2D_MULTISAMPLE_ARRAY)
			{
				glTexImage3DMultisample(GL_TEXTURE_2D_MULTISAMPLE_ARRAY, samples, tf.internal_format, mip_width, mip_height, array_size, fixed_sample_locations);
			}
			else if (texture_type == GL_TEXTURE_CUBE_MAP)
			{
				for (int i = 0; i < 6; i++)
					glTexImage2D(OpenGL::to_cube_target(i), level, tf.internal_format, mip_width, mip_height, 0, tf.pixel_format, tf.pixel_datatype, nullptr);
			}
			else if (texture_type == GL_TEXTURE_CUBE_MAP_ARRAY)
			{
				array_size /= 6;
				for (int i = 0; i < 6; i++)
					glTexImage3D(OpenGL::to_cube_target(i), level, tf.internal_format, mip_width, mip_height, array_size, 0, tf.pixel_format, tf.pixel_datatype, nullptr);
			}
			else
			{
				glTexImage3D(GL_TEXTURE_3D, level, tf.internal_format, mip_width, mip_height, depth, 0, tf.pixel_format, tf.pixel_datatype, nullptr);
			}
		}
	}

	PixelBuffer GL3TextureProvider::get_pixeldata(GraphicContext &gc, TextureFormat texture_format, int level) const
	{
		throw_if_disposed();

		TextureStateTracker state_tracker(texture_type, handle);

		TextureFormat_GL tf = OpenGL::get_textureformat(texture_format);
		if (tf.valid)
		{
			PixelBuffer buffer(width, height, texture_format);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glPixelStorei(GL_UNPACK_ROW_LENGTH, buffer.get_pitch() / buffer.get_bytes_per_pixel());
			glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
			glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
			glGetTexImage(texture_type, level, tf.pixel_format, tf.pixel_datatype, buffer.get_data());
			return buffer;
		}
		else
		{
			PixelBuffer buffer(width, height, tf_bgra8);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glPixelStorei(GL_UNPACK_ROW_LENGTH, buffer.get_pitch() / buffer.get_bytes_per_pixel());
			glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
			glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
			glGetTexImage(texture_type, level, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get_data());
			return buffer.to_format(texture_format);
		}
	}

	void GL3TextureProvider::copy_from(GraphicContext &gc, int x, int y, int slice, int level, const PixelBuffer &src, const Rect &src_rect)
	{
		throw_if_disposed();
		if (src_rect.left < 0 || src_rect.top < 0 || src_rect.right > src.get_width() || src_rect.bottom > src.get_height())
			throw Exception("Rectangle out of bounds");

		TextureFormat_GL tf = OpenGL::get_textureformat(src.get_format());
		bool conv_needed = !tf.valid;

		PixelBuffer src_converted;
		if (conv_needed)
		{
			src_converted = src.to_format(tf_rgba8); // To do: it should use the internal format here (monkey function missing for this)
			tf = OpenGL::get_textureformat(src_converted.get_format());
		}
		else
		{
			src_converted = src;
		}

		OpenGL::set_active(gc);
		TextureStateTracker state_tracker(texture_type, handle);

		const unsigned char *data = nullptr;
		GL3PixelBufferProvider *buffer_provider = nullptr;
		GLint last_buffer = 0;
		buffer_provider = dynamic_cast<GL3PixelBufferProvider*>(src_converted.get_provider());
		if (buffer_provider)
		{

			glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, &last_buffer);
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, buffer_provider->get_handle());
		}
		else
		{
			data = src_converted.get_data<unsigned char>();
		}

		if (src_converted.is_compressed())
		{
			if (Rect(Point(0, 0), src.get_size()) != src_rect)
				throw Exception("Entire pixel buffer must be used for compressed texture uploads");

			unsigned data_size = src.get_data_size();

			if (texture_type == GL_TEXTURE_1D)
			{
				glCompressedTexSubImage1D(texture_type, level, x, src_rect.get_width(), tf.pixel_datatype, data_size, data);
			}
			else if (texture_type == GL_TEXTURE_1D_ARRAY)
			{
				glCompressedTexSubImage2D(texture_type, level, x, slice, src_rect.get_width(), 1, tf.pixel_datatype, data_size, data);
			}
			else if (texture_type == GL_TEXTURE_2D || texture_type == GL_TEXTURE_2D_MULTISAMPLE)
			{
				glCompressedTexSubImage2D(texture_type, level, x, y, src_rect.get_width(), src_rect.get_height(), tf.pixel_datatype, data_size, data);
			}
			else if (texture_type == GL_TEXTURE_2D_ARRAY || texture_type == GL_TEXTURE_2D_MULTISAMPLE_ARRAY || texture_type == GL_TEXTURE_3D)
			{
				glCompressedTexSubImage3D(texture_type, level, x, y, slice, src_rect.get_width(), src_rect.get_height(), 1, tf.pixel_datatype, data_size, data);
			}
			else if (texture_type == GL_TEXTURE_CUBE_MAP)
			{
				glCompressedTexSubImage2D(OpenGL::to_cube_target(slice), level, x, y, src_rect.get_width(), src_rect.get_height(), tf.pixel_datatype, data_size, data);
			}
			else if (texture_type == GL_TEXTURE_CUBE_MAP_ARRAY)
			{
				glCompressedTexSubImage3D(OpenGL::to_cube_target(slice % 6), level, x, y, slice / 6, src_rect.get_width(), src_rect.get_height(), 1, tf.pixel_datatype, data_size, data);
			}
		}
		else
		{
			const int bytes_per_pixel = src_converted.get_bytes_per_pixel();

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glPixelStorei(GL_UNPACK_ROW_LENGTH, src_converted.get_pitch() / bytes_per_pixel);
			glPixelStorei(GL_UNPACK_SKIP_PIXELS, src_rect.left);
			glPixelStorei(GL_UNPACK_SKIP_ROWS, src_rect.top);

			if (texture_type == GL_TEXTURE_1D)
			{
				glTexSubImage1D(texture_type, level, x, src_rect.get_width(), tf.pixel_format, tf.pixel_datatype, data);
			}
			else if (texture_type == GL_TEXTURE_1D_ARRAY)
			{
				glTexSubImage2D(texture_type, level, x, slice, src_rect.get_width(), 1, tf.pixel_format, tf.pixel_datatype, data);
			}
			else if (texture_type == GL_TEXTURE_2D || texture_type == GL_TEXTURE_2D_MULTISAMPLE)
			{
				glTexSubImage2D(texture_type, level, x, y, src_rect.get_width(), src_rect.get_height(), tf.pixel_format, tf.pixel_datatype, data);
			}
			else if (texture_type == GL_TEXTURE_2D_ARRAY || texture_type == GL_TEXTURE_2D_MULTISAMPLE_ARRAY || texture_type == GL_TEXTURE_3D)
			{
				glTexSubImage3D(texture_type, level, x, y, slice, src_rect.get_width(), src_rect.get_height(), 1, tf.pixel_format, tf.pixel_datatype, data);
			}
			else if (texture_type == GL_TEXTURE_CUBE_MAP)
			{
				glTexSubImage2D(OpenGL::to_cube_target(slice), level, x, y, src_rect.get_width(), src_rect.get_height(), tf.pixel_datatype, tf.pixel_datatype, data);
			}
			else if (texture_type == GL_TEXTURE_CUBE_MAP_ARRAY)
			{
				glTexSubImage3D(OpenGL::to_cube_target(slice % 6), level, x, y, slice / 6, src_rect.get_width(), src_rect.get_height(), 1, tf.pixel_format, tf.pixel_datatype, data);
			}

			// Restore these unpack values to the default
			glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
			glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
		}

		if (buffer_provider)
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, last_buffer);
	}

	void GL3TextureProvider::copy_image_from(
		int x,
		int y,
		int width,
		int height,
		int level,
		TextureFormat texture_format,
		GraphicContextProvider *gc)
	{
		throw_if_disposed();
		OpenGL::set_active(static_cast<GL3GraphicContextProvider*>(gc));
		TextureStateTracker state_tracker(texture_type, handle);

		TextureFormat_GL tf = OpenGL::get_textureformat(texture_format);
		if (!tf.valid)
			throw Exception("Texture format not supported by OpenGL");

		glCopyTexImage2D(
			GL_TEXTURE_2D,
			level,
			tf.internal_format,
			x, y,
			width, height,
			0);
	}

	void GL3TextureProvider::copy_subimage_from(
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
		OpenGL::set_active(static_cast<GL3GraphicContextProvider*>(gc));
		TextureStateTracker state_tracker(texture_type, handle);

		glCopyTexSubImage2D(
			GL_TEXTURE_2D,
			level,
			offset_x,
			offset_y,
			x, y,
			width, height);
	}

	void GL3TextureProvider::set_min_lod(double min_lod)
	{
		throw_if_disposed();
		TextureStateTracker state_tracker(texture_type, handle);
		glTexParameterf(texture_type, GL_TEXTURE_MIN_LOD, (GLfloat)min_lod);
	}

	void GL3TextureProvider::set_max_lod(double max_lod)
	{
		throw_if_disposed();
		TextureStateTracker state_tracker(texture_type, handle);
		glTexParameterf(texture_type, GL_TEXTURE_MAX_LOD, (GLfloat)max_lod);
	}

	void GL3TextureProvider::set_lod_bias(double lod_bias)
	{
		throw_if_disposed();
		TextureStateTracker state_tracker(texture_type, handle);
		glTexParameterf(texture_type, GL_TEXTURE_LOD_BIAS, (GLfloat)lod_bias);
	}

	void GL3TextureProvider::set_base_level(int base_level)
	{
		throw_if_disposed();
		TextureStateTracker state_tracker(texture_type, handle);
		glTexParameteri(texture_type, GL_TEXTURE_BASE_LEVEL, base_level);
	}

	void GL3TextureProvider::set_max_level(int max_level)
	{
		throw_if_disposed();
		TextureStateTracker state_tracker(texture_type, handle);
		glTexParameteri(texture_type, GL_TEXTURE_MAX_LEVEL, max_level);
	}

	void GL3TextureProvider::set_wrap_mode(
		TextureWrapMode wrap_s,
		TextureWrapMode wrap_t,
		TextureWrapMode wrap_r)
	{
		throw_if_disposed();
		TextureStateTracker state_tracker(texture_type, handle);
		glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, OpenGL::to_enum(wrap_s));
		if (texture_type != GL_TEXTURE_1D)
			glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, OpenGL::to_enum(wrap_t));
		if (texture_type != GL_TEXTURE_1D && texture_type != GL_TEXTURE_2D)
			glTexParameteri(texture_type, GL_TEXTURE_WRAP_R, OpenGL::to_enum(wrap_r));
	}

	void GL3TextureProvider::set_wrap_mode(
		TextureWrapMode wrap_s,
		TextureWrapMode wrap_t)
	{
		throw_if_disposed();
		TextureStateTracker state_tracker(texture_type, handle);
		glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, OpenGL::to_enum(wrap_s));
		glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, OpenGL::to_enum(wrap_t));
	}

	void GL3TextureProvider::set_wrap_mode(
		TextureWrapMode wrap_s)
	{
		throw_if_disposed();
		TextureStateTracker state_tracker(texture_type, handle);
		glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, OpenGL::to_enum(wrap_s));
	}

	void GL3TextureProvider::set_min_filter(TextureFilter filter)
	{
		throw_if_disposed();
		TextureStateTracker state_tracker(texture_type, handle);
		glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, OpenGL::to_enum(filter));
	}

	void GL3TextureProvider::set_mag_filter(TextureFilter filter)
	{
		throw_if_disposed();
		TextureStateTracker state_tracker(texture_type, handle);

		// Validation (see opengl spec)
		if (!((filter == filter_nearest) || (filter == filter_linear)))
			throw Exception("filter_nearest, filter_linear are only valid options for the mag filter");

		glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, OpenGL::to_enum(filter));
	}

	void GL3TextureProvider::set_max_anisotropy(float v)
	{
		throw_if_disposed();
		TextureStateTracker state_tracker(texture_type, handle);
		glTexParameterf(texture_type, GL_TEXTURE_MAX_ANISOTROPY_EXT, v);
	}

	void GL3TextureProvider::set_texture_compare(TextureCompareMode mode, CompareFunction func)
	{
		throw_if_disposed();
		TextureStateTracker state_tracker(texture_type, handle);
		glTexParameteri(texture_type, GL_TEXTURE_COMPARE_MODE, OpenGL::to_enum(mode));
		glTexParameteri(texture_type, GL_TEXTURE_COMPARE_FUNC, OpenGL::to_enum(func));
	}

	TextureProvider *GL3TextureProvider::create_view(TextureDimensions texture_dimensions, TextureFormat texture_format, int min_level, int num_levels, int min_layer, int num_layers)
	{
		return new GL3TextureProvider(this, texture_dimensions, texture_format, min_level, num_levels, min_layer, num_layers);
	}

	/////////////////////////////////////////////////////////////////////////////

	TextureStateTracker::TextureStateTracker(GLuint target, GLuint handle)
		: target(target), previous_handle(0)
	{
		OpenGL::set_active();

		switch (target)
		{
		case GL_TEXTURE_1D:
			glGetIntegerv(GL_TEXTURE_BINDING_1D, (GLint *)&previous_handle);
			glBindTexture(GL_TEXTURE_1D, handle);
			break;
		case GL_TEXTURE_1D_ARRAY:
			glGetIntegerv(GL_TEXTURE_BINDING_1D_ARRAY, (GLint *)&previous_handle);
			glBindTexture(GL_TEXTURE_1D_ARRAY, handle);
			break;
		case GL_TEXTURE_2D:
			glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint *)&previous_handle);
			glBindTexture(GL_TEXTURE_2D, handle);
			break;
		case GL_TEXTURE_2D_ARRAY:
			glGetIntegerv(GL_TEXTURE_BINDING_2D_ARRAY, (GLint *)&previous_handle);
			glBindTexture(GL_TEXTURE_2D_ARRAY, handle);
			break;
		case GL_TEXTURE_2D_MULTISAMPLE:
			glGetIntegerv(GL_TEXTURE_BINDING_2D_MULTISAMPLE, (GLint *)&previous_handle);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, handle);
			break;
		case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
			glGetIntegerv(GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY, (GLint *)&previous_handle);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE_ARRAY, handle);
			break;
		case GL_TEXTURE_3D:
			glGetIntegerv(GL_TEXTURE_BINDING_3D, (GLint *)&previous_handle);
			glBindTexture(GL_TEXTURE_3D, handle);
			break;
		case GL_TEXTURE_CUBE_MAP:
			glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, (GLint *)&previous_handle);
			glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
			break;
		case GL_TEXTURE_CUBE_MAP_ARRAY:
			glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP_ARRAY, (GLint *)&previous_handle);
			glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, handle);
			break;
		default:
			throw Exception("Unknown texture target");
		}
	}

	TextureStateTracker::~TextureStateTracker()
	{
		switch (target)
		{
		case GL_TEXTURE_1D:
			glBindTexture(GL_TEXTURE_1D, previous_handle);
			break;
		case GL_TEXTURE_1D_ARRAY:
			glBindTexture(GL_TEXTURE_1D_ARRAY, previous_handle);
			break;
		case GL_TEXTURE_2D:
			glBindTexture(GL_TEXTURE_2D, previous_handle);
			break;
		case GL_TEXTURE_2D_ARRAY:
			glBindTexture(GL_TEXTURE_2D_ARRAY, previous_handle);
			break;
		case GL_TEXTURE_2D_MULTISAMPLE:
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, previous_handle);
			break;
		case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE_ARRAY, previous_handle);
			break;
		case GL_TEXTURE_3D:
			glBindTexture(GL_TEXTURE_3D, previous_handle);
			break;
		case GL_TEXTURE_CUBE_MAP:
			glBindTexture(GL_TEXTURE_CUBE_MAP, previous_handle);
			break;
		case GL_TEXTURE_CUBE_MAP_ARRAY:
			glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, previous_handle);
			break;
		}
	}
}
