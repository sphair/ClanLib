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

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// SWRenderTextureProvider Construction:

SWRenderTextureProvider::SWRenderTextureProvider()
: wrap_s(wrap_repeat), wrap_t(wrap_repeat), min_filter(filter_nearest), mag_filter(filter_nearest)
{
}

SWRenderTextureProvider::~SWRenderTextureProvider()
{
}

/////////////////////////////////////////////////////////////////////////////
// SWRenderTextureProvider Attributes:

/////////////////////////////////////////////////////////////////////////////
// SWRenderTextureProvider Operations:

void SWRenderTextureProvider::generate_mipmap()
{
}
void SWRenderTextureProvider::create(int width, int height, int depth, int array_size, TextureFormat texture_format, int levels)
{
	if (depth != 1)
		throw Exception("Unsupported depth specified for GDI target");
	image = PixelBuffer(width, height, tf_bgra8);
}

PixelBuffer SWRenderTextureProvider::get_pixeldata(GraphicContext &gc, TextureFormat texture_format, int level) const
{
	if (level != 0)
		throw Exception("Unsupported mipmap level specified for GDI target");
	return image.to_format(texture_format);
}

/*FIXME
void SWRenderTextureProvider::set_image(PixelBuffer &new_image, int level)
{
	if (level != 0)
		throw Exception("Unsupported mipmap level specified for GDI target");
	image = new_image.to_format(tf_bgra8);
}
*/

/*FIXME
void SWRenderTextureProvider::set_subimage(
	int x,
	int y,
	const PixelBuffer &source_image,
	const Rect &src_rect,
	int level)
{
	if (level != 0)
		throw Exception("Unsupported mipmap level specified for GDI target");

	PixelBuffer temp_source(src_rect.get_width(), src_rect.get_height(), tf_bgra8);
	source_image.convert(temp_source, Rect(0, 0, temp_source.get_width(), temp_source.get_height()), src_rect);

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
*/

void SWRenderTextureProvider::copy_image_from(
	int x,
	int y,
	int width,
	int height,
	int level,
	TextureFormat internal_format,
	GraphicContextProvider *gc)
{
	throw Exception("copy_image_from not supported yet by GDI target");
}

void SWRenderTextureProvider::copy_subimage_from(
	int offset_x,
	int offset_y,
	int x,
	int y,
	int width,
	int height,
	int level,
	GraphicContextProvider *gc)
{
	throw Exception("copy_subimage_from not supported yet by GDI target");
}

void SWRenderTextureProvider::set_min_lod(double min_lod)
{
}

void SWRenderTextureProvider::set_max_lod(double max_lod)
{
}

void SWRenderTextureProvider::set_lod_bias(double lod_bias)
{
}

void SWRenderTextureProvider::set_base_level(int base_level)
{
}

void SWRenderTextureProvider::set_max_level(int max_level)
{
}

void SWRenderTextureProvider::set_wrap_mode(
	TextureWrapMode new_wrap_s,
	TextureWrapMode new_wrap_t,
	TextureWrapMode new_wrap_r)
{
	wrap_s = new_wrap_s;
	wrap_t = new_wrap_t;
}

void SWRenderTextureProvider::set_wrap_mode(
	TextureWrapMode new_wrap_s,
	TextureWrapMode new_wrap_t)
{
	wrap_s = new_wrap_s;
	wrap_t = new_wrap_t;
}

void SWRenderTextureProvider::set_wrap_mode(
	TextureWrapMode new_wrap_s)
{
	wrap_s = new_wrap_s;
}

void SWRenderTextureProvider::set_min_filter(TextureFilter filter)
{
	min_filter = filter;
}

void SWRenderTextureProvider::set_mag_filter(TextureFilter filter)
{
	mag_filter = filter;
}

void SWRenderTextureProvider::set_max_anisotropy(float v)
{
}

void SWRenderTextureProvider::set_texture_compare(TextureCompareMode mode, CompareFunction func)
{
}

void SWRenderTextureProvider::copy_from(GraphicContext &gc, int x, int y, int slice, int level, const PixelBuffer &src, const Rect &src_rect)
{
	//FIXME: Stub
}
TextureProvider *SWRenderTextureProvider::create_view(TextureDimensions texture_dimensions, TextureFormat texture_format, int min_level, int num_levels, int min_layer, int num_layers)
{
	//FIXME: Stub
	throw Exception("Stub");
}

/////////////////////////////////////////////////////////////////////////////
// SWRenderTextureProvider Implementation:

}
