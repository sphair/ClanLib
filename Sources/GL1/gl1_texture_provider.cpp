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

#include "GL1/precomp.h"
#include "gl1_texture_provider.h"
#include "gl1_graphic_context_provider.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/Render/texture.h"
#include "API/GL1/opengl1_wrap.h"
#include "API/GL1/opengl1.h"
#include "API/Core/IOData/cl_endian.h"
#include "API/Core/System/databuffer.h"
#include "API/Display/Render/shared_gc_data.h"

#ifndef WIN32
#include "API/Core/Text/console.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_GL1TextureProvider Construction:

CL_GL1TextureProvider::CL_GL1TextureProvider(CL_GL1GraphicContextProvider *gc_provider, CL_TextureDimensions texture_dimensions)
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

	CL_GL1TextureStateTracker state_tracker(texture_type, 0, gc_provider);
	cl1GenTextures(1, &handle);
	cl1BindTexture(texture_type, handle);
	cl1TexParameteri(texture_type, CL_TEXTURE_MIN_FILTER, CL_LINEAR);
	cl1TexParameteri(texture_type, CL_TEXTURE_MAG_FILTER, CL_LINEAR);
	cl1TexParameteri(texture_type, CL_TEXTURE_WRAP_S, CL_CLAMP_TO_EDGE);
	if (texture_type != CL_TEXTURE_1D)
		cl1TexParameteri(texture_type, CL_TEXTURE_WRAP_T, CL_CLAMP_TO_EDGE);
	if (texture_type == CL_TEXTURE_3D)
		cl1TexParameteri(texture_type, CL_TEXTURE_WRAP_R, CL_CLAMP_TO_EDGE);
}

CL_GL1TextureProvider::~CL_GL1TextureProvider()
{
	dispose();
	CL_SharedGCData::remove_disposable(this);
}

void CL_GL1TextureProvider::on_dispose()
{
	if (handle)
	{
		if (CL_GL1::set_active())
		{
			cl1DeleteTextures(1, &handle);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_GL1TextureProvider Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_GL1TextureProvider Operations:

void CL_GL1TextureProvider::generate_mipmap()
{
}

void CL_GL1TextureProvider::create(int new_width, int new_height, CL_TextureFormat internal_format, int new_depth)
{
	throw_if_disposed();

	CLint gl_internal_format;
	CLenum gl_pixel_format;
	CL_GL1::to_opengl_textureformat(internal_format, gl_internal_format, gl_pixel_format);

	if ( (new_width > 32768) || (new_width < 1) )
	{
		throw CL_Exception("Invalid texture width in the GL1 target");
	}

	if ( (texture_type == CL_TEXTURE_2D) || (texture_type == CL_TEXTURE_3D) )
	{
		if ( (new_height > 32768) || (new_height < 1) )
		{
			throw CL_Exception("Invalid texture height in the GL1 target");
		}
	}

	if ( texture_type == CL_TEXTURE_3D )
	{
		if ( (new_depth > 32768) || (new_depth < 1) )
		{
			throw CL_Exception("Invalid texture depth in the GL1 target");
		}
	}

	width = new_width;
	height = new_height;
	depth = new_depth;
	CL_GL1TextureStateTracker state_tracker(texture_type, handle, NULL);

	if (texture_type == CL_TEXTURE_1D)
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
			CL_TEXTURE_1D,			// target
			0,						// level
			gl_internal_format,		// internalformat
			pot_width,				// width
			0,						// border
			gl_pixel_format,		// format
			CL_UNSIGNED_BYTE,		// type (it really doesn't matter since nothing is uploaded)
			0);						// texels (0 to avoid uploading)
	}
	else if (texture_type == CL_TEXTURE_2D)
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
			CL_TEXTURE_2D,			// target
			0,						// level
			gl_internal_format,		// internalformat
			pot_width,				// width
			pot_height,				// height
			0,						// border
			gl_pixel_format,		// format 
			CL_UNSIGNED_BYTE,		// type (it really doesn't matter since nothing is uploaded)
			0);						// texels (0 to avoid uploading)

		// Clear the whole texture if it is npot
		if (!power_of_two_texture)
		{
			CL_PixelBuffer image = CL_PixelBuffer(pot_width, pot_height, cl_rgba8);
			void *data = image.get_data();
			memset(data, 0, pot_width * pot_height * 4);

			CLenum format;
			CLenum type;
			CL_GL1::to_opengl_pixelformat(image, format, type);

			cl1PixelStorei(CL_UNPACK_ALIGNMENT, 1);
			const int bytesPerPixel = image.get_bytes_per_pixel();
			cl1PixelStorei(CL_UNPACK_ROW_LENGTH, image.get_pitch() / bytesPerPixel);
			cl1PixelStorei(CL_UNPACK_SKIP_PIXELS, 0);
			cl1PixelStorei(CL_UNPACK_SKIP_ROWS, 0);

			int image_width = image.get_width();
			int image_height = image.get_height();

			cl1TexImage2D(
				CL_TEXTURE_2D,		// target
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
			CL_TEXTURE_3D,			// target
			0,						// level
			gl_internal_format,		// internalformat
			pot_width,				// width
			pot_height,				// height
			pot_depth,				// depth
			0,						// border
			gl_pixel_format,		// format 
			CL_UNSIGNED_BYTE,		// type (it really doesn't matter since nothing is uploaded)
			0);						// texels (0 to avoid uploading)
	}
}

void CL_GL1TextureProvider::destroy()
{
	delete this;
}

CL_PixelBuffer CL_GL1TextureProvider::get_pixeldata(CL_TextureFormat sized_format, int level) const
{
	throw_if_disposed();

	// todo: be smart here and request the closest matching opengl1 format.

	CL_GL1TextureStateTracker state_tracker(texture_type, handle, NULL);

	CL_PixelBuffer buffer(
		pot_width, pot_height,
		cl_abgr8);

	cl1GetTexImage(texture_type, level, CL_RGBA, CL_UNSIGNED_BYTE, buffer.get_data());

	CL_PixelBuffer buffer_ref(width, height, buffer.get_format(), buffer.get_data());

	return buffer.to_format(sized_format);
}

void CL_GL1TextureProvider::set_image(CL_PixelBuffer &image, int level)
{
	throw_if_disposed();
	CL_GL1TextureStateTracker state_tracker(texture_type, handle, NULL);
	if (texture_type == CL_TEXTURE_2D)
	{
		set_texture_image2d(CL_TEXTURE_2D, image, level);
	}
	else if (texture_type == CL_TEXTURE_3D)
	{
		set_texture_image3d(CL_TEXTURE_3D, image, image.get_height() / height, level);
	}
}

void CL_GL1TextureProvider::set_cube_map(
	CL_PixelBuffer &cube_map_positive_x,
	CL_PixelBuffer &cube_map_negative_x,
	CL_PixelBuffer &cube_map_positive_y,
	CL_PixelBuffer &cube_map_negative_y,
	CL_PixelBuffer &cube_map_positive_z,
	CL_PixelBuffer &cube_map_negative_z,
	int level)
{
	throw_if_disposed();
	CL_GL1TextureStateTracker state_tracker(texture_type, handle, NULL);

	set_texture_image2d(CL_TEXTURE_CUBE_MAP_POSITIVE_X, cube_map_positive_x, level);
	set_texture_image2d(CL_TEXTURE_CUBE_MAP_NEGATIVE_X, cube_map_negative_x, level);
	set_texture_image2d(CL_TEXTURE_CUBE_MAP_POSITIVE_Y, cube_map_positive_y, level);
	set_texture_image2d(CL_TEXTURE_CUBE_MAP_NEGATIVE_Y, cube_map_negative_y, level);
	set_texture_image2d(CL_TEXTURE_CUBE_MAP_POSITIVE_Z, cube_map_positive_z, level);
	set_texture_image2d(CL_TEXTURE_CUBE_MAP_NEGATIVE_Z, cube_map_negative_z, level);
}

void CL_GL1TextureProvider::set_compressed_image(
	int level,
	CL_TextureFormat internal_format,
	int width,
	int height,
	CL_DataBuffer &image)
{
	throw_if_disposed();
	CL_GL1TextureStateTracker state_tracker(texture_type, handle, NULL);

	CLint gl_internal_format;
	CLenum gl_pixel_format;
	CL_GL1::to_opengl_textureformat(internal_format, gl_internal_format, gl_pixel_format);

	cl1CompressedTexImage2D(texture_type, level, gl_internal_format, width, height, 0, image.get_size(), image.get_data());
}

void CL_GL1TextureProvider::set_subimage(
	int x,
	int y,
	const CL_PixelBuffer &ximage,
	const CL_Rect &src_rect,
	int level)
{
	CL_PixelBuffer image = ximage;

	if (src_rect.left < 0 || src_rect.top < 0 || src_rect.right > image.get_width(), src_rect.bottom > image.get_height())
		throw CL_Exception("Rectangle out of bounds");

	throw_if_disposed();
	CL_GL1TextureStateTracker state_tracker(texture_type, handle, NULL);

	// check out if the original texture needs or doesn't need an alpha channel
	bool needs_alpha = image.get_alpha_mask() || image.has_colorkey();

	CLenum format;
	CLenum type;
	bool conv_needed = !CL_GL1::to_opengl_pixelformat(image, format, type);

	// also check for the pitch (OpenGL1 can only skip pixels, not bytes)
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
		cl1PixelStorei(CL_UNPACK_ALIGNMENT, 1);
		const int bytesPerPixel = image.get_bytes_per_pixel();
		cl1PixelStorei(CL_UNPACK_ROW_LENGTH, image.get_pitch() / bytesPerPixel);
		cl1PixelStorei(CL_UNPACK_SKIP_PIXELS, src_rect.left);
		cl1PixelStorei(CL_UNPACK_SKIP_ROWS, src_rect.top);

	}
	// conversion needed
	else
	{
		bool big_endian = CL_Endian::is_system_big();

		CL_PixelBuffer buffer;

		if (!big_endian)
			buffer = CL_PixelBuffer(
				src_rect.get_width(), src_rect.get_height(),
				needs_alpha ? cl_abgr8 : cl_bgr8); // OpenGL1 RGB/RGBA is always big endian
		else
			buffer = CL_PixelBuffer(
				src_rect.get_width(), src_rect.get_height(),
				needs_alpha ? cl_rgba8 : cl_rgb8);
	
		image.convert(buffer, CL_Rect(0, 0, buffer.get_size()), src_rect);

		format = needs_alpha ? CL_RGBA : CL_RGB;

		// Upload to OpenGL1:
		cl1BindTexture(CL_TEXTURE_2D, handle);

		// change alignment
		cl1PixelStorei(CL_UNPACK_ALIGNMENT, 1);
		const int bytesPerPixel = buffer.get_bytes_per_pixel();
		cl1PixelStorei(CL_UNPACK_ROW_LENGTH, buffer.get_pitch() / bytesPerPixel);
		cl1PixelStorei(CL_UNPACK_SKIP_PIXELS, 0);
		cl1PixelStorei(CL_UNPACK_SKIP_ROWS, 0);

		type = CL_UNSIGNED_BYTE;
		image = buffer;
	}

	// upload
	cl1TexSubImage2D(
		CL_TEXTURE_2D,            // target
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
					CL_TEXTURE_2D,		// target
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
					CL_TEXTURE_2D,		// target
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
	cl1PixelStorei(CL_UNPACK_SKIP_PIXELS, 0);
	cl1PixelStorei(CL_UNPACK_SKIP_ROWS, 0);
}

void CL_GL1TextureProvider::copy_image_from(
	int x,
	int y,
	int width,
	int height,
	int level,
	CL_TextureFormat internal_format,
	CL_GraphicContextProvider *gc)
{
	throw_if_disposed();
	CL_GL1TextureStateTracker state_tracker(texture_type, handle, static_cast<CL_GL1GraphicContextProvider*>(gc));

	CLint gl_internal_format;
	CLenum gl_pixel_format;
	CL_GL1::to_opengl_textureformat(internal_format, gl_internal_format, gl_pixel_format);

	cl1CopyTexImage2D(
		CL_TEXTURE_2D,
		level,
		gl_internal_format,
		x, y,
		width, height,
		0);
}

void CL_GL1TextureProvider::copy_subimage_from(
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
	CL_GL1TextureStateTracker state_tracker(texture_type, handle, static_cast<CL_GL1GraphicContextProvider*>(gc));

	cl1CopyTexSubImage2D( 
		CL_TEXTURE_2D,
		level, 
		offset_x, 
		offset_y, 
		x, y,
		width, height );
}

void CL_GL1TextureProvider::set_min_lod(double min_lod)
{
	throw_if_disposed();
	CL_GL1TextureStateTracker state_tracker(texture_type, handle, NULL);
	cl1TexParameterf(texture_type, CL_TEXTURE_MIN_LOD, (CLfloat)min_lod);
}

void CL_GL1TextureProvider::set_max_lod(double max_lod)
{
	throw_if_disposed();
	CL_GL1TextureStateTracker state_tracker(texture_type, handle, NULL);
	cl1TexParameterf(texture_type, CL_TEXTURE_MAX_LOD, (CLfloat)max_lod);
}

void CL_GL1TextureProvider::set_lod_bias(double lod_bias)
{
	throw_if_disposed();
	CL_GL1TextureStateTracker state_tracker(texture_type, handle, NULL);
	cl1TexParameterf(texture_type, CL_TEXTURE_LOD_BIAS, (CLfloat)lod_bias);
}

void CL_GL1TextureProvider::set_base_level(int base_level)
{
	throw_if_disposed();
	CL_GL1TextureStateTracker state_tracker(texture_type, handle, NULL);
	cl1TexParameteri(texture_type, CL_TEXTURE_BASE_LEVEL, base_level);
}

void CL_GL1TextureProvider::set_max_level(int max_level)
{
	throw_if_disposed();
	CL_GL1TextureStateTracker state_tracker(texture_type, handle, NULL);
	cl1TexParameteri(texture_type, CL_TEXTURE_MAX_LEVEL, max_level);
}

void CL_GL1TextureProvider::set_wrap_mode(
	CL_TextureWrapMode wrap_s,
	CL_TextureWrapMode wrap_t,
	CL_TextureWrapMode wrap_r)
{
	throw_if_disposed();
	CL_GL1TextureStateTracker state_tracker(texture_type, handle, NULL);
	cl1TexParameteri(texture_type, CL_TEXTURE_WRAP_S, to_enum(wrap_s));
	cl1TexParameteri(texture_type, CL_TEXTURE_WRAP_T, to_enum(wrap_t));
	cl1TexParameteri(texture_type, CL_TEXTURE_WRAP_R, to_enum(wrap_r));
}

void CL_GL1TextureProvider::set_wrap_mode(
	CL_TextureWrapMode wrap_s,
	CL_TextureWrapMode wrap_t)
{
	throw_if_disposed();
	CL_GL1TextureStateTracker state_tracker(texture_type, handle, NULL);
	cl1TexParameteri(texture_type, CL_TEXTURE_WRAP_S, to_enum(wrap_s));
	cl1TexParameteri(texture_type, CL_TEXTURE_WRAP_T, to_enum(wrap_t));
}

void CL_GL1TextureProvider::set_wrap_mode(
	CL_TextureWrapMode wrap_s)
{
	throw_if_disposed();
	CL_GL1TextureStateTracker state_tracker(texture_type, handle, NULL);
	cl1TexParameteri(texture_type, CL_TEXTURE_WRAP_S, to_enum(wrap_s));
}

void CL_GL1TextureProvider::set_min_filter(CL_TextureFilter filter)
{
	throw_if_disposed();
	CL_GL1TextureStateTracker state_tracker(texture_type, handle, NULL);
	cl1TexParameteri(texture_type, CL_TEXTURE_MIN_FILTER, to_enum(filter));
}

void CL_GL1TextureProvider::set_mag_filter(CL_TextureFilter filter)
{
	throw_if_disposed();
	CL_GL1TextureStateTracker state_tracker(texture_type, handle, NULL);
	cl1TexParameteri(texture_type, CL_TEXTURE_MAG_FILTER, to_enum(filter));
}

void CL_GL1TextureProvider::set_max_anisotropy(float v)
{
}

void CL_GL1TextureProvider::set_texture_compare(CL_TextureCompareMode mode, CL_CompareFunction func)
{
	throw_if_disposed();
	CL_GL1TextureStateTracker state_tracker(texture_type, handle, NULL);
	cl1TexParameteri(texture_type, CL_TEXTURE_COMPARE_MODE, to_enum(mode));	
	cl1TexParameteri(texture_type, CL_TEXTURE_COMPARE_FUNC, to_enum(func));	
}

void CL_GL1TextureProvider::transform_coordinate(const CL_PrimitivesArrayData::VertexData &attribute, std::vector<float> &transformed_data, int vertex_offset, int num_vertices, int total_vertices)
{
	if (attribute.type != cl_type_float)
	{
		throw CL_Exception("Implement me: Texture coord npot transformation (not float) (GL1 target)");
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

	float *src = (float *) attribute.data;
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

/////////////////////////////////////////////////////////////////////////////
// CL_GL1TextureProvider Implementation:


void CL_GL1TextureProvider::set_texture_image2d(
	CLuint target,
	CL_PixelBuffer &image,
	int level)
{
	throw_if_disposed();
	CL_GL1TextureStateTracker state_tracker(texture_type, handle, NULL);

	CLint gl_internal_format;
	CLenum gl_pixel_format;
	CL_GL1::to_opengl_textureformat(image.get_format(), gl_internal_format, gl_pixel_format);


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
	bool conv_needed = !CL_GL1::to_opengl_pixelformat(image, format, type);

	// also check for the pitch (OpenGL1 can only skip pixels, not bytes)
	if (!conv_needed)
	{
		const int bytesPerPixel = image.get_bytes_per_pixel();
		if (image.get_pitch() % bytesPerPixel != 0)
			conv_needed = true;
	}

	// no conversion needed
	if (!conv_needed)
	{
		// Upload to OpenGL1:

		// change alignment
		cl1PixelStorei(CL_UNPACK_ALIGNMENT, 1);
		const int bytesPerPixel = image.get_bytes_per_pixel();
		cl1PixelStorei(CL_UNPACK_ROW_LENGTH, image.get_pitch() / bytesPerPixel);
		cl1PixelStorei(CL_UNPACK_SKIP_PIXELS, 0);
		cl1PixelStorei(CL_UNPACK_SKIP_ROWS, 0);

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
		cl1PixelStorei(CL_UNPACK_ALIGNMENT, 1);
		const int bytesPerPixel = buffer.get_bytes_per_pixel();
		cl1PixelStorei(CL_UNPACK_ROW_LENGTH, buffer.get_pitch() / bytesPerPixel);
		cl1PixelStorei(CL_UNPACK_SKIP_PIXELS, 0);
		cl1PixelStorei(CL_UNPACK_SKIP_ROWS, 0);

		// upload
		cl1TexImage2D(
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
		cl1TexParameteri(CL_TEXTURE_2D, CL_TEXTURE_MIN_FILTER, CL_LINEAR);
		cl1TexParameteri(CL_TEXTURE_2D, CL_TEXTURE_MAG_FILTER, CL_LINEAR);
		cl1TexParameteri(CL_TEXTURE_2D, CL_TEXTURE_WRAP_S, CL_CLAMP_TO_EDGE);
		cl1TexParameteri(CL_TEXTURE_2D, CL_TEXTURE_WRAP_T, CL_CLAMP_TO_EDGE);
		*/
	}
}

void CL_GL1TextureProvider::set_texture_image3d(
	CLuint target,
	CL_PixelBuffer &image,
	int image_depth,
	int level)
{
	throw_if_disposed();
	CL_GL1TextureStateTracker state_tracker(texture_type, handle, NULL);

	CLint gl_internal_format;
	CLenum gl_pixel_format;
	CL_GL1::to_opengl_textureformat(image.get_format(), gl_internal_format, gl_pixel_format);

	// check out if the original texture needs or doesn't need an alpha channel
	bool needs_alpha = image.get_alpha_mask() || image.has_colorkey();

	CLenum format;
	CLenum type;
	bool conv_needed = !CL_GL1::to_opengl_pixelformat(image, format, type);

	// also check for the pitch (OpenGL1 can only skip pixels, not bytes)
	if (!conv_needed)
	{
		const int bytesPerPixel = image.get_bytes_per_pixel();
		if (image.get_pitch() % bytesPerPixel != 0)
			conv_needed = true;
	}

	// no conversion needed
	if (!conv_needed)
	{
		// Upload to OpenGL1:

		// change alignment
		cl1PixelStorei(CL_UNPACK_ALIGNMENT, 1);
		const int bytesPerPixel = image.get_bytes_per_pixel();
		cl1PixelStorei(CL_UNPACK_ROW_LENGTH, image.get_pitch() / bytesPerPixel);
		cl1PixelStorei(CL_UNPACK_SKIP_PIXELS, 0);
		cl1PixelStorei(CL_UNPACK_SKIP_ROWS, 0);

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
		cl1PixelStorei(CL_UNPACK_ALIGNMENT, 1);
		const int bytesPerPixel = buffer.get_bytes_per_pixel();
		cl1PixelStorei(CL_UNPACK_ROW_LENGTH, buffer.get_pitch() / bytesPerPixel);
		cl1PixelStorei(CL_UNPACK_SKIP_PIXELS, 0);
		cl1PixelStorei(CL_UNPACK_SKIP_ROWS, 0);

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
			CL_UNSIGNED_BYTE,         // type
			buffer.get_data());       // texels

	}
}

CLenum CL_GL1TextureProvider::to_enum(CL_TextureFilter filter)
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

CLenum CL_GL1TextureProvider::to_enum(CL_TextureWrapMode mode)
{
 	switch(mode)
	{
	case cl_wrap_clamp_to_edge: return CL_CLAMP_TO_EDGE;
	case cl_wrap_repeat: return CL_REPEAT;
	case cl_wrap_mirrored_repeat: return CL_MIRRORED_REPEAT;
	default: return CL_CLAMP_TO_EDGE;
	}
}

CLenum CL_GL1TextureProvider::to_enum(CL_TextureCompareMode mode)
{
 	switch(mode)
	{
	case cl_comparemode_none: return CL_NONE;
	case cl_comparemode_compare_r_to_texture: return CL_COMPARE_R_TO_TEXTURE;		
	default: return CL_NONE;
	}
}

CLenum CL_GL1TextureProvider::to_enum(CL_CompareFunction func)
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

CL_GL1TextureStateTracker::CL_GL1TextureStateTracker(CLuint texture_type, CLuint handle, CL_GL1GraphicContextProvider *gc_provider)
{
	// If the gc_provider is unknown, we need to use the first active provider
	if (!gc_provider)
	{
		CL_GL1::set_active();
	}
	else
	{
		CL_GL1::set_active(gc_provider);
	}

	last_is_enabled_texture1d = cl1IsEnabled(CL_TEXTURE_1D);
	last_is_enabled_texture2d = cl1IsEnabled(CL_TEXTURE_2D);
	last_is_enabled_texture3d = cl1IsEnabled(CL_TEXTURE_3D);
	last_is_enabled_texture_cube_map = cl1IsEnabled(CL_TEXTURE_CUBE_MAP);
	cl1GetIntegerv(CL_TEXTURE_BINDING_1D, (CLint *) &last_bound_texture1d);
	cl1GetIntegerv(CL_TEXTURE_BINDING_2D, (CLint *) &last_bound_texture2d);
	cl1GetIntegerv(CL_TEXTURE_BINDING_3D, (CLint *) &last_bound_texture3d);
	cl1GetIntegerv(CL_TEXTURE_BINDING_CUBE_MAP, (CLint *) &last_bound_texture_cube_map);

	if (texture_type == CL_TEXTURE_1D)
	{
		cl1Disable(CL_TEXTURE_2D);
		cl1Disable(CL_TEXTURE_3D);
		cl1Disable(CL_TEXTURE_CUBE_MAP);
		cl1Enable(CL_TEXTURE_1D);
		cl1BindTexture(CL_TEXTURE_1D, handle);
	}

	if (texture_type == CL_TEXTURE_2D)
	{
		cl1Disable(CL_TEXTURE_1D);
		cl1Disable(CL_TEXTURE_3D);
		cl1Disable(CL_TEXTURE_CUBE_MAP);
		cl1Enable(CL_TEXTURE_2D);
		cl1BindTexture(CL_TEXTURE_2D, handle);
	}

	if (texture_type == CL_TEXTURE_3D)
	{
		cl1Disable(CL_TEXTURE_1D);
		cl1Disable(CL_TEXTURE_2D);
		cl1Disable(CL_TEXTURE_CUBE_MAP);
		cl1Enable(CL_TEXTURE_3D);
		cl1BindTexture(CL_TEXTURE_3D, handle);
	}

	if (texture_type == CL_TEXTURE_CUBE_MAP)
	{
		cl1Disable(CL_TEXTURE_1D);
		cl1Disable(CL_TEXTURE_2D);
		cl1Disable(CL_TEXTURE_3D);
		cl1Enable(CL_TEXTURE_CUBE_MAP);
		cl1BindTexture(CL_TEXTURE_CUBE_MAP, handle);
	}
}

CL_GL1TextureStateTracker::~CL_GL1TextureStateTracker()
{
	if (last_is_enabled_texture1d) cl1Enable(CL_TEXTURE_1D); else cl1Disable(CL_TEXTURE_1D);
	if (last_is_enabled_texture2d) cl1Enable(CL_TEXTURE_2D); else cl1Disable(CL_TEXTURE_2D);
	if (last_is_enabled_texture3d) cl1Enable(CL_TEXTURE_3D); else cl1Disable(CL_TEXTURE_3D);
	if (last_is_enabled_texture_cube_map) cl1Enable(CL_TEXTURE_CUBE_MAP); else cl1Disable(CL_TEXTURE_CUBE_MAP);

	if (last_is_enabled_texture1d) cl1BindTexture(CL_TEXTURE_1D, last_bound_texture1d);
	if (last_is_enabled_texture2d) cl1BindTexture(CL_TEXTURE_2D, last_bound_texture2d);
	if (last_is_enabled_texture3d) cl1BindTexture(CL_TEXTURE_3D, last_bound_texture3d);
	if (last_is_enabled_texture_cube_map) cl1BindTexture(CL_TEXTURE_CUBE_MAP, last_bound_texture_cube_map);
}

int CL_GL1TextureProvider::get_next_power_of_two(int value)
{
	for (int pot_count = 1; pot_count < 32768; pot_count += pot_count)
	{
		if (value <= pot_count)
			return pot_count;
	}
	return 32768;
}
