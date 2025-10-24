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
*/

#pragma once

#include "API/Display/Image/pixel_format.h"
#include "API/Display/Image/palette.h"

/// \brief Pixel data implementation interface.
class CL_PixelBuffer_Impl
{
/// \name Construction
/// \{

public:
	CL_PixelBuffer_Impl();

	CL_PixelBuffer_Impl(
		int width, int height, int pitch, const CL_PixelFormat &format, const void *data_ptr);

	CL_PixelBuffer_Impl(
		int width, int height, int pitch, const CL_PixelFormat &format,
		const CL_Palette &palette, const void *data_ptr);

	~CL_PixelBuffer_Impl();


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns a pointer to the beginning of the pixel buffer.
	void *get_data() { return data; }

	const void *get_data() const { return data; }

	/// \brief Buffer pixel format description.
	CL_PixelFormat format;

	/// \brief Palette, if 8 bpp pixel format.
	CL_Palette palette;

	/// \brief Buffer scanline pitch.
	unsigned int pitch;

	/// \brief Buffer width.
	int width;

	/// \brief Buffer height.
	int height;

	/// \brief Boolean indicating if pixel data should be deleted on destruction.
	bool delete_data;

	/// \brief Pixel data.
	unsigned char *data;


/// \}
/// \name Operations
/// \{

public:
	/// \brief Converts between two RGBA formats.
	static void convert(
		void* in_buffer,
		const CL_PixelFormat& in_pf,
		int in_pitch,
		void* out_buffer,
		const CL_PixelFormat& out_pf,
		int out_pitch,
		const CL_Size& size);

	/// \brief Converts from palette format to RGBA.
	static void convert_pal(
		void* in_buffer,
		const CL_PixelFormat& in_pf,
		int in_pitch,
		const CL_Palette* in_pal,
		void* out_buffer,
		const CL_PixelFormat& out_pf,
		int out_pitch,
		const CL_Size& size);


/// \}
/// \name Implementation
/// \{

private:
/// \}
};


