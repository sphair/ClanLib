/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "Display/display_precomp.h"
#include "API/Display/pixel_buffer.h"
#include "API/Core/IOData/cl_endian.h"
#include "pixel_buffer_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_PixelBuffer_Generic construction:

/////////////////////////////////////////////////////////////////////////////
// CL_PixelBuffer_Generic operations:

void CL_PixelBuffer_Generic::convert(void* in_buffer, const CL_PixelFormat& in_pf, int in_pitch, void* out_buffer, const CL_PixelFormat& out_pf, int out_pitch, const CL_Size& size)
{
	cl_assert(in_pf.get_type() == pixelformat_rgba);
	cl_assert(out_pf.get_type() == pixelformat_rgba);
	
	const int in_bpp = in_pf.get_depth()/8;
	const int out_bpp = out_pf.get_depth()/8;

	cl_assert(in_bpp <= 4 && in_bpp >= 1);
	cl_assert(out_bpp <= 4 && out_bpp >=1);
	
	char* in_p = static_cast<char*>(in_buffer);
	char* out_p = static_cast<char*>(out_buffer);

	if (in_pf == out_pf)
	{
		const int in_line_bytes = in_bpp*size.width;
		for (int y = 0; y < size.height; y++, in_p += in_pitch, out_p += out_pitch)
			memcpy(out_p, in_p, in_line_bytes);
	}
	else
	{
		const int in_r_shift = CL_PixelFormat::get_mask_shift(in_pf.get_red_mask());
		const int in_g_shift = CL_PixelFormat::get_mask_shift(in_pf.get_green_mask());
		const int in_b_shift = CL_PixelFormat::get_mask_shift(in_pf.get_blue_mask());
		const int in_a_shift = CL_PixelFormat::get_mask_shift(in_pf.get_alpha_mask());

		const int in_r_bits = CL_PixelFormat::get_mask_bits(in_pf.get_red_mask());
		const int in_g_bits = CL_PixelFormat::get_mask_bits(in_pf.get_green_mask());
		const int in_b_bits = CL_PixelFormat::get_mask_bits(in_pf.get_blue_mask());
		const int in_a_bits = CL_PixelFormat::get_mask_bits(in_pf.get_alpha_mask());

		const int in_r_bitmask = CL_PixelFormat::get_bitmask(in_r_bits, 0);
		const int in_g_bitmask = CL_PixelFormat::get_bitmask(in_g_bits, 0);
		const int in_b_bitmask = CL_PixelFormat::get_bitmask(in_b_bits, 0);
		const int in_a_bitmask = CL_PixelFormat::get_bitmask(in_a_bits, 0);

		const int out_r_shift = CL_PixelFormat::get_mask_shift(out_pf.get_red_mask());
		const int out_g_shift = CL_PixelFormat::get_mask_shift(out_pf.get_green_mask());
		const int out_b_shift = CL_PixelFormat::get_mask_shift(out_pf.get_blue_mask());
		const int out_a_shift = CL_PixelFormat::get_mask_shift(out_pf.get_alpha_mask());

		const int out_r_bits = CL_PixelFormat::get_mask_bits(out_pf.get_red_mask());
		const int out_g_bits = CL_PixelFormat::get_mask_bits(out_pf.get_green_mask());
		const int out_b_bits = CL_PixelFormat::get_mask_bits(out_pf.get_blue_mask());
		const int out_a_bits = CL_PixelFormat::get_mask_bits(out_pf.get_alpha_mask());

		const bool in_has_ck = in_pf.has_colorkey();
		unsigned int in_ck = 0;
		if (in_has_ck)
			in_ck = in_pf.get_colorkey();
		const bool out_has_ck = out_pf.has_colorkey();
		unsigned int out_ck = 0;
		if (out_has_ck)
			out_ck = out_pf.get_colorkey();
		
		for (int y = 0; y < size.height; y++, in_p += in_pitch, out_p += out_pitch)
		{
			char* in_p2 = in_p;
			char* out_p2 = out_p;
			for (int x = 0; x < size.width; x++, in_p2 += in_bpp, out_p2 += out_bpp)
			{
				unsigned int in_pixel = 0;
				memcpy(&in_pixel, in_p2, in_bpp);
				
				if (CL_Endian::is_system_big())
				{
					for (int bpp=4; bpp && in_bpp < bpp; bpp--)
						in_pixel >>= 8;
				}

				// decode
				unsigned int in_r = 0, in_g = 0, in_b = 0, in_a = 0xffffffff;
				if (in_r_bitmask)
				{
					in_r = ((in_pixel >> in_r_shift) & in_r_bitmask);
					if (in_r & 1)
						in_r = (in_r << (32 - in_r_bits)) | CL_PixelFormat::get_bitmask(32 - in_r_bits, 0);
					else
						in_r <<= 32 - in_r_bits;
				}
				if (in_g_bitmask)
				{
					in_g = ((in_pixel >> in_g_shift) & in_g_bitmask);
					if (in_g & 1)
						in_g = (in_g << (32 - in_g_bits)) | CL_PixelFormat::get_bitmask(32 - in_g_bits, 0);
					else
						in_g <<= 32 - in_g_bits;
				}
				if (in_b_bitmask)
				{
					in_b = ((in_pixel >> in_b_shift) & in_b_bitmask);
					if (in_b & 1)
						in_b = (in_b << (32 - in_b_bits)) | CL_PixelFormat::get_bitmask(32 - in_b_bits, 0);
					else
						in_b <<= 32 - in_b_bits;
				}
				if (in_a_bitmask)
				{
					in_a = ((in_pixel >> in_a_shift) & in_a_bitmask);
					if (in_a & 1)
						in_a = (in_a << (32 - in_a_bits)) | CL_PixelFormat::get_bitmask(32 - in_a_bits, 0);
					else
						in_a <<= 32 - in_a_bits;
				}

				unsigned int out_pixel;
				bool encode;
				if (!out_a_bits && (in_a == 0) && out_has_ck)
				{
					out_pixel = out_ck;
					encode = false;
				}
				else if (in_has_ck && (in_pixel == in_ck))
				{
					if (out_has_ck)
					{
						out_pixel = out_ck;
						encode = false;
					}
					else
					{
						in_a = 0;
						encode = true;
					}
				}	
				else
					encode = true;

				// encode
				if (encode)
				{
					out_pixel = 0;
					if (out_r_bits)
						out_pixel |= (in_r >> (32 - out_r_bits)) << out_r_shift;
					if (out_g_bits)
						out_pixel |= (in_g >> (32 - out_g_bits)) << out_g_shift;
					if (out_b_bits)
						out_pixel |= (in_b >> (32 - out_b_bits)) << out_b_shift;
					if (out_a_bits)
						out_pixel |= (in_a >> (32 - out_a_bits)) << out_a_shift;
				
					//at this point we need to do one more colorkey check, otherwise it will fail when the input is
					//a different format than the output --mrfun Nov 10 2006

					if (in_has_ck && (in_ck == out_pixel))
					{
						//should be transparent
						out_pixel = 0;
					}
				
				}

				memcpy(out_p2, &out_pixel, out_bpp);
			}
		}
	}
}

void CL_PixelBuffer_Generic::convert_pal(void* in_buffer, const CL_PixelFormat& in_pf, int in_pitch, const CL_Palette& in_pal, void* out_buffer, const CL_PixelFormat& out_pf, int out_pitch, const CL_Size& size)
{
	cl_assert(in_pf.get_type() == pixelformat_index);
	cl_assert(out_pf.get_type() == pixelformat_rgba);

	const int in_bpp = in_pf.get_depth()/8;
	const int out_bpp = out_pf.get_depth()/8;
	char* in_p = static_cast<char*>(in_buffer);
	char* out_p = static_cast<char*>(out_buffer);

	const int out_r_shift = CL_PixelFormat::get_mask_shift(out_pf.get_red_mask());
	const int out_g_shift = CL_PixelFormat::get_mask_shift(out_pf.get_green_mask());
	const int out_b_shift = CL_PixelFormat::get_mask_shift(out_pf.get_blue_mask());
	const int out_a_shift = CL_PixelFormat::get_mask_shift(out_pf.get_alpha_mask());

	const int out_r_bits = CL_PixelFormat::get_mask_bits(out_pf.get_red_mask());
	const int out_g_bits = CL_PixelFormat::get_mask_bits(out_pf.get_green_mask());
	const int out_b_bits = CL_PixelFormat::get_mask_bits(out_pf.get_blue_mask());
	const int out_a_bits = CL_PixelFormat::get_mask_bits(out_pf.get_alpha_mask());
	const bool big = CL_Endian::is_system_big();

	const bool in_has_ck = in_pf.has_colorkey();
	unsigned int in_ck = 0;
	if (in_has_ck)
		in_ck = in_pf.get_colorkey();
	
	for (int y = 0; y < size.height; y++, in_p += in_pitch, out_p += out_pitch)
	{
		char* in_p2 = in_p;
		char* out_p2 = out_p;
		for (int x = 0; x < size.width; x++, in_p2 += in_bpp, out_p2 += out_bpp)
		{
			//I took out the SWAP_IF_BIG/memset, this will always be a single byte - mrfun
			unsigned int in_pixel = (unsigned char)*in_p2;
			
			// decode
			unsigned int in_r = 0, in_g = 0, in_b = 0, in_a = 0xffffffff;
			in_r = in_pal.colors[in_pixel].get_red();
			if (in_r & 1)
				in_r = (in_r << (32 - 8)) | CL_PixelFormat::get_bitmask(32 - 8, 0);
			else
				in_r <<= 32 - 8;
			in_g = in_pal.colors[in_pixel].get_green();
			if (in_g & 1)
				in_g = (in_g << (32 - 8)) | CL_PixelFormat::get_bitmask(32 - 8, 0);
			else
				in_g <<= 32 - 8;
			in_b = in_pal.colors[in_pixel].get_blue();
			if (in_b & 1)
				in_b = (in_b << (32 - 8)) | CL_PixelFormat::get_bitmask(32 - 8, 0);
			else
				in_b <<= 32 - 8;
			in_a = in_pal.colors[in_pixel].get_alpha();
			if (in_a & 1)
				in_a = (in_a << (32 - 8)) | CL_PixelFormat::get_bitmask(32 - 8, 0);
			else
				in_a <<= 32 - 8;

			unsigned int out_pixel;
			
			if (in_has_ck && (in_pal.colors[in_pixel].color == in_ck))
			{
				in_a = 0;
			}	
	
				out_pixel = 0;
				if (out_r_bits)
					out_pixel |= (in_r >> (32 - out_r_bits)) << out_r_shift;
				if (out_g_bits)
					out_pixel |= (in_g >> (32 - out_g_bits)) << out_g_shift;
				if (out_b_bits)
					out_pixel |= (in_b >> (32 - out_b_bits)) << out_b_shift;
				if (out_a_bits)
					out_pixel |= (in_a >> (32 - out_a_bits)) << out_a_shift;
			
				//this fixes a bug on big endian when converting 8 bit to 24 bit, the
				//garbage alpha was copied instead of a color -mrfun
				if (big && out_bpp == 3)
				{
					memcpy(out_p2, ((char*)&out_pixel)+1, out_bpp);
				} else
				{
					memcpy(out_p2, &out_pixel, out_bpp);
				}

		}
	}
}
