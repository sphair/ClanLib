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
*/

#pragma once

#include "API/Core/IOData/iodevice.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Core/System/databuffer.h"
#include <map>

namespace clan
{
	class PNGLoader
	{
	public:
		static PixelBuffer load(IODevice iodevice, bool srgb);

	private:
		PNGLoader(IODevice iodevice, bool force_srgb);
		~PNGLoader();
		void read_magic();
		void read_chunks();
		void decode_header();
		void decode_palette();
		void decode_colorkey();
		void decode_image();
		void decode_interlace_none(const unsigned char *data, int data_length);
		void decode_interlace_adam7(const unsigned char *data, int data_length);

		void create_image();
		void create_scanline_buffers();
		int get_image_data_channels();

		void filter_scanline(int predictor_type, int scanline_byte_length);
		static void predictor_sub(unsigned char *scanline, const unsigned char *prev_scanline, int byte_length, int channels, int bit_depth);
		static void predictor_up(unsigned char *scanline, const unsigned char *prev_scanline, int byte_length, int channels, int bit_depth);
		static void predictor_average(unsigned char *scanline, const unsigned char *prev_scanline, int byte_length, int channels, int bit_depth);
		static void predictor_paeth(unsigned char *scanline, const unsigned char *prev_scanline, int byte_length, int channels, int bit_depth);

		void convert_scanline_4ub(int scanline_pixel_length);
		void convert_scanline_4us(int scanline_pixel_length);

		void grayscale_to_4ub(int count);
		void truecolor_to_4ub(int count);
		void indexed_to_4ub(int count);
		void grayscale_alpha_to_4ub(int count);
		void truecolor_alpha_to_4ub(int count);

		void grayscale_to_4us(int count);
		void truecolor_to_4us(int count);
		void grayscale_alpha_to_4us(int count);
		void truecolor_alpha_to_4us(int count);

		static int abs(int a) { return a >= 0 ? a : -a; }

		inline unsigned int from_network_order(unsigned int v)
		{
			unsigned char *p = reinterpret_cast<unsigned char *>(&v);
			return (static_cast<unsigned int>(p[0]) << 24) | (static_cast<unsigned int>(p[1]) << 16) | (static_cast<unsigned int>(p[2]) << 8) | p[3];
		}

		inline unsigned short from_network_order(unsigned short v)
		{
			unsigned char *p = reinterpret_cast<unsigned char *>(&v);
			return (static_cast<unsigned int>(p[0]) << 8) | p[1];
		}

		IODevice file;
		bool force_srgb;

		PixelBuffer image;

		DataBuffer ihdr; // image header, which is the first chunk in a PNG datastream.
		DataBuffer plte; // palette table associated with indexed PNG images.
		DataBuffer idat; // image data chunks.

		DataBuffer trns; // Transparency information
		DataBuffer chrm; // Colour space information (5 chunks)
		DataBuffer gama;
		DataBuffer iccp;
		DataBuffer sbit;
		DataBuffer srgb;

		unsigned int image_width;
		unsigned int image_height;
		unsigned char bit_depth;
		unsigned char color_type;
		unsigned char compression_method;
		unsigned char filter_method;
		unsigned char interlace_method;

		unsigned char *scanline;
		unsigned char *prev_scanline;
		Vec4ub *scanline_4ub;
		Vec4us *scanline_4us;

		Vec4ub *palette;
		Vec3us colorkey;
		bool has_colorkey;
	};
}
