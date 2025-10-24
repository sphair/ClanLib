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
**    Mark Page
*/

#include "SDL/precomp.h"
#include "sdl_texture_provider.h"
#include "API/Display/Image/pixel_buffer.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SDLTextureProvider Construction:

CL_SDLTextureProvider::CL_SDLTextureProvider()
: surface(NULL), wrap_s(cl_wrap_repeat), wrap_t(cl_wrap_repeat), min_filter(cl_filter_nearest), mag_filter(cl_filter_nearest)
{
}

CL_SDLTextureProvider::~CL_SDLTextureProvider()
{
	delete_surface();
}

/////////////////////////////////////////////////////////////////////////////
// CL_SDLTextureProvider Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_SDLTextureProvider Operations:

void CL_SDLTextureProvider::destroy()
{
	delete this;
}

void CL_SDLTextureProvider::create(int width, int height, int format, int depth)
{
	delete_surface();

	if (depth != 1)
		throw CL_Exception(cl_text("Unsupported depth specified for SDL target"));

    Uint32 rmask, gmask, bmask, amask;

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
       on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
	surface = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, width, height, 32, rmask, gmask, bmask, amask);

	if (!surface)
		throw CL_Exception(cl_text("Cannot create surface for SDL target"));
}

CL_PixelBuffer CL_SDLTextureProvider::get_pixeldata(CL_PixelFormat &format, int level)
{
	if (level != 0)
		throw CL_Exception(cl_text("Unsupported mipmap level specified for SDL target"));

	throw CL_Exception(cl_text("get_pixeldata not supported for SDL target"));
	return CL_PixelBuffer();
}

void CL_SDLTextureProvider::set_image(CL_PixelBuffer &new_image, int level, int format)
{
	delete_surface();

	if (level != 0)
		throw CL_Exception(cl_text("Unsupported mipmap level specified for SDL target"));

	CL_PixelFormat pixel_format = new_image.get_format();
	SDL_Surface *temp_surface =  SDL_CreateRGBSurfaceFrom(new_image.get_data(), new_image.get_width(), new_image.get_height(), 
		pixel_format.get_depth(),  new_image.get_pitch(),
		pixel_format.get_red_mask(), pixel_format.get_green_mask(), pixel_format.get_blue_mask(), pixel_format.get_alpha_mask());

	if (!temp_surface)
		throw CL_Exception(cl_text("Cannot create temporary surface for SDL target"));

	SDL_SetAlpha(temp_surface, 0, 0);
	surface = SDL_DisplayFormatAlpha(temp_surface);
	SDL_FreeSurface(temp_surface);

	if (!surface)
		throw CL_Exception(cl_text("Cannot create surface for SDL target"));
}

void CL_SDLTextureProvider::set_cube_map(
	CL_PixelBuffer &cube_map_positive_x,
	CL_PixelBuffer &cube_map_negative_x,
	CL_PixelBuffer &cube_map_positive_y,
	CL_PixelBuffer &cube_map_negative_y,
	CL_PixelBuffer &cube_map_positive_z,
	CL_PixelBuffer &cube_map_negative_z,
	int level,
	int format)
{
	throw CL_Exception(cl_text("Cube texture maps not supported by SDL target"));
}

void CL_SDLTextureProvider::set_compressed_image(
	int level,
	int format,
	int width,
	int height,
	CL_DataBuffer &image)
{
	throw CL_Exception(cl_text("Compressed textures not supported by SDL target"));
}

void CL_SDLTextureProvider::set_subimage(
	int x,
	int y,
	const CL_PixelBufferRef &source_image,
	int level)
{
	if (level != 0)
		throw CL_Exception(cl_text("Unsupported mipmap level specified for SDL target"));

	CL_PixelBuffer temp_source = source_image.to_format(CL_PixelFormat::argb8888);

	CL_PixelFormat pixel_format = temp_source.get_format();

	// Note, we cast the const away from the source_image, this is okay (!)

	SDL_Surface *temp_surface =  SDL_CreateRGBSurfaceFrom((void *) temp_source.get_data(),temp_source.get_width(), temp_source.get_height(), 
		pixel_format.get_depth(),  temp_source.get_pitch(),
		pixel_format.get_red_mask(), pixel_format.get_green_mask(), pixel_format.get_blue_mask(), pixel_format.get_alpha_mask());

	if (!temp_surface)
		throw CL_Exception(cl_text("Cannot create temporary surface for SDL target"));

	SDL_Rect source_rect;
	source_rect.x = 0;
	source_rect.y = 0;
	source_rect.w = temp_source.get_width();
	source_rect.h = temp_source.get_height();

	SDL_Rect dest_rect;
	dest_rect.x = x;
	dest_rect.y = y;
	dest_rect.w = temp_source.get_width();
	dest_rect.h = temp_source.get_height();

	SDL_SetAlpha(temp_surface, 0, 0);
	int rcode = SDL_BlitSurface(temp_surface, &source_rect, surface, &dest_rect);
	SDL_FreeSurface(temp_surface);
}

void CL_SDLTextureProvider::copy_image_from(
	int x,
	int y,
	int width,
	int height,
	int level,
	int format,
	CL_GraphicContextProvider *gc)
{
	throw CL_Exception(cl_text("copy_image_from not supported yet by SDL target"));
}

void CL_SDLTextureProvider::copy_subimage_from(
	int offset_x,
	int offset_y,
	int x,
	int y,
	int width,
	int height,
	int level,
	CL_GraphicContextProvider *gc)
{
	throw CL_Exception(cl_text("copy_subimage_from not supported yet by SDL target"));
}

void CL_SDLTextureProvider::set_min_lod(double min_lod)
{
}

void CL_SDLTextureProvider::set_max_lod(double max_lod)
{
}

void CL_SDLTextureProvider::set_lod_bias(double lod_bias)
{
}

void CL_SDLTextureProvider::set_base_level(int base_level)
{
}

void CL_SDLTextureProvider::set_max_level(int max_level)
{
}

void CL_SDLTextureProvider::set_generate_mipmap(bool generate_mipmap)
{
}

void CL_SDLTextureProvider::set_wrap_mode(
	CL_TextureWrapMode new_wrap_s,
	CL_TextureWrapMode new_wrap_t,
	CL_TextureWrapMode new_wrap_r)
{
	wrap_s = new_wrap_s;
	wrap_t = new_wrap_t;
}

void CL_SDLTextureProvider::set_wrap_mode(
	CL_TextureWrapMode new_wrap_s,
	CL_TextureWrapMode new_wrap_t)
{
	wrap_s = new_wrap_s;
	wrap_t = new_wrap_t;
}

void CL_SDLTextureProvider::set_wrap_mode(
	CL_TextureWrapMode new_wrap_s)
{
	wrap_s = new_wrap_s;
}

void CL_SDLTextureProvider::set_min_filter(CL_TextureFilter filter)
{
	min_filter = filter;
}

void CL_SDLTextureProvider::set_mag_filter(CL_TextureFilter filter)
{
	mag_filter = filter;
}

void CL_SDLTextureProvider::set_depth_mode(CL_TextureDepthMode depth_mode)
{
}

void CL_SDLTextureProvider::set_texture_compare(CL_TextureCompareMode mode, CL_CompareFunction func)
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_SDLTextureProvider Implementation:

void CL_SDLTextureProvider::delete_surface()
{
	if (surface)
	{
		SDL_FreeSurface(surface);
		surface = NULL;
	}
}


