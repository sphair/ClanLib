/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
#include "API/Core/System/databuffer.h"
#include "API/Display/Image/pixel_buffer.h"
#include <vector>

namespace clan
{
	class TargaLoader
	{
	public:
		static PixelBuffer load(IODevice iodevice, bool srgb);

	private:
		TargaLoader(IODevice iodevice, bool srgb);
		void read_header();
		void read_image_id();
		void read_color_map();
		void read_image_data();
		void decode_palette();
		void decode_image();
		void decode_color_mapped();
		void decode_true_color();
		void decode_grayscale();

		IODevice file;
		bool srgb;

		unsigned char id_length;
		unsigned char colormap_type;
		unsigned char image_type;

		unsigned short colormap_orig;
		unsigned short colormap_length;
		unsigned short colormap_entry_size;

		unsigned short image_x_orig;
		unsigned short image_y_orig;
		unsigned short image_width;
		unsigned short image_height;
		unsigned char image_pixel_size;
		unsigned char image_descriptor;

		int bytes_per_colormap_entry;
		int bytes_per_pixel_entry;

		unsigned int num_alpha_bits;
		bool right_to_left;
		bool top_down;

		DataBuffer image_id;
		DataBuffer colormap_data;
		std::vector<Vec4ub> palette;
		DataBuffer image_data;

		PixelBuffer image;
	};
}
