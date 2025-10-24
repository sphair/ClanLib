/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
*/

#include "GDI/precomp.h"
#include "gdi_texture_provider.h"
#include "API/Display/Image/pixel_buffer.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GDITextureProvider Construction:

CL_GDITextureProvider::CL_GDITextureProvider()
: wrap_s(cl_wrap_repeat), wrap_t(cl_wrap_repeat), min_filter(cl_filter_nearest), mag_filter(cl_filter_nearest)
{
}

CL_GDITextureProvider::~CL_GDITextureProvider()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GDITextureProvider Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_GDITextureProvider Operations:

void CL_GDITextureProvider::destroy()
{
	delete this;
}

void CL_GDITextureProvider::create(int width, int height, int format, int depth)
{
	if (depth != 1)
		throw CL_Exception(cl_text("Unsupported depth specified for GDI target"));
	image = CL_PixelBuffer(width, height, width*4, CL_PixelFormat::argb8888);
}

CL_PixelBuffer CL_GDITextureProvider::get_pixeldata(CL_PixelFormat &format, int level)
{
	if (level != 0)
		throw CL_Exception(cl_text("Unsupported mipmap level specified for GDI target"));
	return image.to_format(format);
}

void CL_GDITextureProvider::set_image(CL_PixelBuffer &new_image, int level, int format)
{
	if (level != 0)
		throw CL_Exception(cl_text("Unsupported mipmap level specified for GDI target"));
	image = new_image.to_format(CL_PixelFormat::argb8888);
}

void CL_GDITextureProvider::set_cube_map(
	CL_PixelBuffer &cube_map_positive_x,
	CL_PixelBuffer &cube_map_negative_x,
	CL_PixelBuffer &cube_map_positive_y,
	CL_PixelBuffer &cube_map_negative_y,
	CL_PixelBuffer &cube_map_positive_z,
	CL_PixelBuffer &cube_map_negative_z,
	int level,
	int format)
{
	throw CL_Exception(cl_text("Cube texture maps not supported by GDI target"));
}

void CL_GDITextureProvider::set_compressed_image(
	int level,
	int format,
	int width,
	int height,
	CL_DataBuffer &image)
{
	throw CL_Exception(cl_text("Compressed textures not supported by GDI target"));
}

void CL_GDITextureProvider::set_subimage(
	int x,
	int y,
	const CL_PixelBufferRef &source_image,
	int level)
{
	if (level != 0)
		throw CL_Exception(cl_text("Unsupported mipmap level specified for GDI target"));
	CL_PixelBuffer temp_source = source_image.to_format(CL_PixelFormat::argb8888);

	int dest_buffer_width = image.get_width();
	int dest_buffer_height = image.get_height();
	int src_buffer_width = temp_source.get_width();
	int src_buffer_height = temp_source.get_height();
	unsigned int *dest_data = static_cast<unsigned int *>(image.get_data());
	const unsigned int *src_data = static_cast<const unsigned int *>(temp_source.get_data());

	for (int yy = 0; yy < source_image.get_height(); yy++)
	{
		if (y+yy >= dest_buffer_height)
			break;

		for (int xx = 0; xx < source_image.get_width(); xx++)
		{
			if (x+xx < dest_buffer_width)
				dest_data[x+xx+(y+yy)*dest_buffer_width] = src_data[xx+yy*src_buffer_width];
			else
				break;
		}
	}
}

void CL_GDITextureProvider::copy_image_from(
	int x,
	int y,
	int width,
	int height,
	int level,
	int format,
	CL_GraphicContextProvider *gc)
{
	throw CL_Exception(cl_text("copy_image_from not supported yet by GDI target"));
}

void CL_GDITextureProvider::copy_subimage_from(
	int offset_x,
	int offset_y,
	int x,
	int y,
	int width,
	int height,
	int level,
	CL_GraphicContextProvider *gc)
{
	throw CL_Exception(cl_text("copy_subimage_from not supported yet by GDI target"));
}

void CL_GDITextureProvider::set_min_lod(double min_lod)
{
}

void CL_GDITextureProvider::set_max_lod(double max_lod)
{
}

void CL_GDITextureProvider::set_lod_bias(double lod_bias)
{
}

void CL_GDITextureProvider::set_base_level(int base_level)
{
}

void CL_GDITextureProvider::set_max_level(int max_level)
{
}

void CL_GDITextureProvider::set_generate_mipmap(bool generate_mipmap)
{
}

void CL_GDITextureProvider::set_wrap_mode(
	CL_TextureWrapMode new_wrap_s,
	CL_TextureWrapMode new_wrap_t,
	CL_TextureWrapMode new_wrap_r)
{
	wrap_s = new_wrap_s;
	wrap_t = new_wrap_t;
}

void CL_GDITextureProvider::set_wrap_mode(
	CL_TextureWrapMode new_wrap_s,
	CL_TextureWrapMode new_wrap_t)
{
	wrap_s = new_wrap_s;
	wrap_t = new_wrap_t;
}

void CL_GDITextureProvider::set_wrap_mode(
	CL_TextureWrapMode new_wrap_s)
{
	wrap_s = new_wrap_s;
}

void CL_GDITextureProvider::set_min_filter(CL_TextureFilter filter)
{
	min_filter = filter;
}

void CL_GDITextureProvider::set_mag_filter(CL_TextureFilter filter)
{
	mag_filter = filter;
}

void CL_GDITextureProvider::set_depth_mode(CL_TextureDepthMode depth_mode)
{
}

void CL_GDITextureProvider::set_texture_compare(CL_TextureCompareMode mode, CL_CompareFunction func)
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GDITextureProvider Implementation:
