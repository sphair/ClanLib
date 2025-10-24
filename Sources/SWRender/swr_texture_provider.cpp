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
*/

#include "SWRender/precomp.h"
#include "swr_texture_provider.h"
#include "API/Display/Image/pixel_buffer.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SWRenderTextureProvider Construction:

CL_SWRenderTextureProvider::CL_SWRenderTextureProvider()
: wrap_s(cl_wrap_repeat), wrap_t(cl_wrap_repeat), min_filter(cl_filter_nearest), mag_filter(cl_filter_nearest)
{
}

CL_SWRenderTextureProvider::~CL_SWRenderTextureProvider()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_SWRenderTextureProvider Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_SWRenderTextureProvider Operations:

void CL_SWRenderTextureProvider::destroy()
{
	delete this;
}

void CL_SWRenderTextureProvider::generate_mipmap()
{
}

void CL_SWRenderTextureProvider::create(int width, int height, CL_TextureFormat internal_format, int depth)
{
	if (depth != 1)
		throw CL_Exception("Unsupported depth specified for GDI target");
	image = CL_PixelBuffer(width, height, cl_argb8);
}

CL_PixelBuffer CL_SWRenderTextureProvider::get_pixeldata(CL_TextureFormat sized_format, int level) const
{
	if (level != 0)
		throw CL_Exception("Unsupported mipmap level specified for GDI target");
	return image.to_format(sized_format);
}

void CL_SWRenderTextureProvider::set_image(CL_PixelBuffer &new_image, int level)
{
	if (level != 0)
		throw CL_Exception("Unsupported mipmap level specified for GDI target");
	image = new_image.to_format(cl_argb8);
}

void CL_SWRenderTextureProvider::set_cube_map(
	CL_PixelBuffer &cube_map_positive_x,
	CL_PixelBuffer &cube_map_negative_x,
	CL_PixelBuffer &cube_map_positive_y,
	CL_PixelBuffer &cube_map_negative_y,
	CL_PixelBuffer &cube_map_positive_z,
	CL_PixelBuffer &cube_map_negative_z,
	int level)
{
	throw CL_Exception("Cube texture maps not supported by GDI target");
}

void CL_SWRenderTextureProvider::set_compressed_image(
	int level,
	CL_TextureFormat internal_format,
	int width,
	int height,
	CL_DataBuffer &image)
{
	throw CL_Exception("Compressed textures not supported by GDI target");
}

void CL_SWRenderTextureProvider::set_subimage(
	int x,
	int y,
	const CL_PixelBuffer &source_image,
	const CL_Rect &src_rect,
	int level)
{
	if (level != 0)
		throw CL_Exception("Unsupported mipmap level specified for GDI target");

	CL_PixelBuffer temp_source(src_rect.get_width(), src_rect.get_height(), cl_argb8);
	source_image.convert(temp_source, CL_Rect(0, 0, temp_source.get_width(), temp_source.get_height()), src_rect);

	int dest_buffer_width = image.get_width();
	int dest_buffer_height = image.get_height();
	int src_buffer_width = temp_source.get_width();
	unsigned int *dest_data = static_cast<unsigned int *>(image.get_data());
	const unsigned int *src_data = static_cast<const unsigned int *>(temp_source.get_data());

	for (int yy = 0; yy < temp_source.get_height(); yy++)
	{
		if (y+yy >= dest_buffer_height)
			break;

		for (int xx = 0; xx < temp_source.get_width(); xx++)
		{
			if (x+xx < dest_buffer_width)
				dest_data[x+xx+(y+yy)*dest_buffer_width] = src_data[xx+yy*src_buffer_width];
			else
				break;
		}
	}
}

void CL_SWRenderTextureProvider::copy_image_from(
	int x,
	int y,
	int width,
	int height,
	int level,
	CL_TextureFormat internal_format,
	CL_GraphicContextProvider *gc)
{
	throw CL_Exception("copy_image_from not supported yet by GDI target");
}

void CL_SWRenderTextureProvider::copy_subimage_from(
	int offset_x,
	int offset_y,
	int x,
	int y,
	int width,
	int height,
	int level,
	CL_GraphicContextProvider *gc)
{
	throw CL_Exception("copy_subimage_from not supported yet by GDI target");
}

void CL_SWRenderTextureProvider::set_min_lod(double min_lod)
{
}

void CL_SWRenderTextureProvider::set_max_lod(double max_lod)
{
}

void CL_SWRenderTextureProvider::set_lod_bias(double lod_bias)
{
}

void CL_SWRenderTextureProvider::set_base_level(int base_level)
{
}

void CL_SWRenderTextureProvider::set_max_level(int max_level)
{
}

void CL_SWRenderTextureProvider::set_wrap_mode(
	CL_TextureWrapMode new_wrap_s,
	CL_TextureWrapMode new_wrap_t,
	CL_TextureWrapMode new_wrap_r)
{
	wrap_s = new_wrap_s;
	wrap_t = new_wrap_t;
}

void CL_SWRenderTextureProvider::set_wrap_mode(
	CL_TextureWrapMode new_wrap_s,
	CL_TextureWrapMode new_wrap_t)
{
	wrap_s = new_wrap_s;
	wrap_t = new_wrap_t;
}

void CL_SWRenderTextureProvider::set_wrap_mode(
	CL_TextureWrapMode new_wrap_s)
{
	wrap_s = new_wrap_s;
}

void CL_SWRenderTextureProvider::set_min_filter(CL_TextureFilter filter)
{
	min_filter = filter;
}

void CL_SWRenderTextureProvider::set_mag_filter(CL_TextureFilter filter)
{
	mag_filter = filter;
}

void CL_SWRenderTextureProvider::set_max_anisotropy(float v)
{
}

void CL_SWRenderTextureProvider::set_texture_compare(CL_TextureCompareMode mode, CL_CompareFunction func)
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_SWRenderTextureProvider Implementation:
