/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

#pragma once

#include "API/Display/Image/pixel_format.h"
#include "API/Display/Image/palette.h"
#include "API/Display/Image/pixel_buffer.h"

class CL_GraphicContext;
class CL_PixelBufferProvider;

/// \brief Texture format datatypes.
enum CL_TextureFormatDataType
{
	cl_unused,
	cl_signed,
	cl_unsigned,
	cl_signed_normalised_fixed_point,
	cl_unsigned_normalised_fixed_point,
	cl_floating_point
};

enum CL_TextureFormatBase
{
	cl_formatbase_unknown,
	cl_formatbase_red,
	cl_formatbase_rg,
	cl_formatbase_rgb,
	cl_formatbase_rgba,
	cl_formatbase_depth_component,
	cl_formatbase_depth_stencil
};

/// \brief Pixel data implementation interface.
class CL_PixelBuffer_Impl
{
/// \name Construction
/// \{

public:
	CL_PixelBuffer_Impl();

	CL_PixelBuffer_Impl(
		int width, int height, CL_TextureFormat sized_format, const void *data_ptr, bool only_reference_data);

	CL_PixelBuffer_Impl(
		int width, int height, CL_TextureFormat sized_format,
		const CL_Palette &palette, const void *data_ptr);

	CL_PixelBuffer_Impl(CL_GraphicContext &gc, int new_width, int new_height, CL_PixelBufferDirection direction, CL_TextureFormat new_sized_format, const void *new_data, CL_BufferUsage new_usage);

	~CL_PixelBuffer_Impl();


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns a pointer to the beginning of the pixel buffer.
	void *get_data() { return data; }

	const void *get_data() const { return data; }

	CL_Colorf get_pixel(int x, int y);

	/// \brief Palette, if 8 bpp pixel format.
	CL_Palette palette;

	/// \brief Buffer width.
	int width;

	/// \brief Buffer height.
	int height;

	/// \brief Boolean indicating if pixel data should be deleted on destruction.
	bool delete_data;

	/// \brief Pixel data.
	unsigned char *data;

	CL_PixelBufferProvider *provider;

	/// \brief True if colorkeying is enabled in the pixel format.
	bool colorkey_enabled;

	/// \brief Colorkey value, if enabled.
	unsigned int colorkey;

/// \}
/// \name Attributes
/// \{

public:

	/// \brief Returns the pixel format
	CL_TextureFormat get_format() const { return sized_format; }

	/// \brief Returns the number of bytes per pixel
	int get_bytes_per_pixel() const { return bytes_per_pixel; }

	/// \brief Returns the red component color mask.
	unsigned int get_red_mask() const { return red_mask; }

	/// \brief Returns the green component color mask.
	unsigned int get_green_mask() const { return green_mask; }

	/// \brief Returns the blue component color mask.
	unsigned int get_blue_mask() const { return blue_mask; }

	/// \brief Returns the alpha component color mask.
	unsigned int get_alpha_mask() const { return alpha_mask; }

	/// \brief Buffer scanline pitch.
	unsigned int get_pitch() const;

/// \}
/// \name Operations
/// \{

public:

	void convert(CL_PixelBuffer &target, const CL_Rect &dest_rect, const CL_Rect &src_rect) const;

/// \}
/// \name Implementation
/// \{

private:
	bool check_supported_conversion_format() const;

	void *aligned_alloc(int size) const;
	void aligned_free(void *ptr) const;

	/// \brief Sets the pixel format type
	void set_format(CL_TextureFormat sized_format);

	/// \brief Converts between two color formats.
	void convert(
		void* in_buffer,
		int in_pitch,
		void* out_buffer,
		CL_PixelBuffer& target_buffer,
		int out_pitch,
		const CL_Size& size) const;

	/// \brief Converts from palette format to color format.
	void convert_pal(
		void* in_buffer,
		int in_pitch,
		void* out_buffer,
		CL_PixelBuffer& target_buffer,
		int out_pitch,
		const CL_Size& size) const;

	/// \brief Pixel format type.
	CL_TextureFormat sized_format;

	/// \brief Pixel format base type.
	CL_TextureFormatBase base_format;

	/// \brief Number of bytes per pixel. 0 = Unknown
	int bytes_per_pixel;

	/// \brief Pixel information
	CL_TextureFormatDataType datatype_color;
	CL_TextureFormatDataType datatype_stencil;
	CL_TextureFormatDataType datatype_depth;

	/// \brief Component bits (0 = unknown or not used)
	unsigned int red_bits;
	unsigned int green_bits;
	unsigned int blue_bits;
	unsigned int alpha_bits;
	unsigned int depth_bits;
	unsigned int stencil_bits;

	/// \brief Component mask (0 = unknown or not used)
	unsigned int red_mask;
	unsigned int green_mask;
	unsigned int blue_mask;
	unsigned int alpha_mask;
	unsigned int depth_mask;
	unsigned int stencil_mask;

/// \}
};


