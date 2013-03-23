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

#pragma once

#include "API/Display/TargetProviders/texture_provider.h"
#include "API/Display/Image/pixel_buffer.h"

namespace clan
{

class CL_SWRenderTextureProvider : public CL_TextureProvider
{
/// \name Construction
/// \{

public:
	CL_SWRenderTextureProvider();

	~CL_SWRenderTextureProvider();


/// \}
/// \name Attributes
/// \{

public:
	CL_PixelBuffer &get_image() { return image; };
	const CL_PixelBuffer &get_image() const { return image; };

/// \}
/// \name Operations
/// \{

public:
	void destroy();

	void generate_mipmap();

	void create(int width, int height, CL_TextureFormat internal_format, int depth);

	CL_PixelBuffer get_pixeldata(CL_TextureFormat sized_format, int level) const;

	void set_image(CL_PixelBuffer &image, int level);

	void set_cube_map(
		CL_PixelBuffer &cube_map_positive_x,
		CL_PixelBuffer &cube_map_negative_x,
		CL_PixelBuffer &cube_map_positive_y,
		CL_PixelBuffer &cube_map_negative_y,
		CL_PixelBuffer &cube_map_positive_z,
		CL_PixelBuffer &cube_map_negative_z,
		int level);

	void set_compressed_image(
		int level,
		CL_TextureFormat internal_format,
		int width,
		int height,
		CL_DataBuffer &image);

	void set_subimage(
		int x,
		int y,
		const CL_PixelBuffer &image,
		const CL_Rect &src_rect,
		int level);

	void copy_image_from(
		int x,
		int y,
		int width,
		int height,
		int level,
		CL_TextureFormat internal_format,
		CL_GraphicContextProvider *gc);

	void copy_subimage_from(
		int offset_x,
		int offset_y,
		int x,
		int y,
		int width,
		int height,
		int level,
		CL_GraphicContextProvider *gc);

	void set_min_lod(double min_lod);

	void set_max_lod(double max_lod);

	void set_lod_bias(double lod_bias);

	void set_base_level(int base_level);

	void set_max_level(int max_level);

	void set_wrap_mode(
		CL_TextureWrapMode wrap_s,
		CL_TextureWrapMode wrap_t,
		CL_TextureWrapMode wrap_r);

	void set_wrap_mode(
		CL_TextureWrapMode wrap_s,
		CL_TextureWrapMode wrap_t);

	void set_wrap_mode(
		CL_TextureWrapMode wrap_s);

	void set_min_filter(CL_TextureFilter filter);

	void set_mag_filter(CL_TextureFilter filter);

	void set_max_anisotropy(float v);

	void set_texture_compare(CL_TextureCompareMode mode, CL_CompareFunction func);


/// \}
/// \name Implementation
/// \{

private:
	CL_PixelBuffer image;
	CL_TextureWrapMode wrap_s, wrap_t;
	CL_TextureFilter min_filter, mag_filter;
/// \}
};

}

