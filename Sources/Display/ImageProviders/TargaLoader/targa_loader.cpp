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

#include "Display/precomp.h"
#include "targa_loader.h"
#include "API/Display/Image/pixel_buffer_lock.h"

namespace clan
{
	PixelBuffer TargaLoader::load(IODevice iodevice, bool srgb)
	{
		TargaLoader loader(iodevice, srgb);
		return loader.image;
	}

	TargaLoader::TargaLoader(IODevice iodevice, bool srgb)
		: file(iodevice), srgb(srgb)
	{
		read_header();
		read_image_id();
		read_color_map();
		read_image_data();
		decode_palette();
		decode_image();
	}

	void TargaLoader::read_header()
	{
		id_length = file.read_uint8();
		colormap_type = file.read_uint8();
		image_type = file.read_uint8();

		colormap_orig = file.read_uint16();
		colormap_length = file.read_uint16();
		colormap_entry_size = file.read_uint8();

		image_x_orig = file.read_int16();
		image_y_orig = file.read_int16();
		image_width = file.read_uint16();
		image_height = file.read_uint16();
		image_pixel_size = file.read_uint8();
		image_descriptor = file.read_uint8();

		if (colormap_type > 1)
			throw Exception("Invalid or unsupported targa image file");

		if (colormap_type == 0)
			colormap_length = 0;

		bytes_per_colormap_entry = (colormap_entry_size + 7) / 8;
		bytes_per_pixel_entry = (image_pixel_size + 7) / 8;

		if (bytes_per_pixel_entry > 4)
			throw Exception("Unsupported targa image file");

		num_alpha_bits = image_descriptor & 0x0f;
		right_to_left = (image_descriptor & 0x10) != 0;
		top_down = (image_descriptor & 0x20) != 0;
	}

	void TargaLoader::read_image_id()
	{
		image_id = DataBuffer(id_length);
		file.read(image_id.get_data(), image_id.get_size());
	}

	void TargaLoader::read_color_map()
	{
		colormap_data = DataBuffer(bytes_per_colormap_entry * colormap_length);
		file.read(colormap_data.get_data(), colormap_data.get_size());
	}

	void TargaLoader::read_image_data()
	{
		image_data = DataBuffer(bytes_per_pixel_entry * image_width * image_height);

		if (image_type == 9 || image_type == 10 || image_type == 11) // RLE compressed
		{
			DataBuffer rle_data(file.get_size() - file.get_position());
			file.read(rle_data.get_data(), rle_data.get_size());

			unsigned char *input = reinterpret_cast<unsigned char*>(rle_data.get_data());
			unsigned char *output = reinterpret_cast<unsigned char*>(image_data.get_data());
			int pixels_left = image_width * image_height;
			int input_available = rle_data.get_size();
			while (pixels_left > 0 && input_available > 0)
			{
				int code = *input;
				int count = (code & 0x7f) + 1;
				bool rle_packet = (code & 0x80) != 0;

				input++;
				input_available--;

				if (rle_packet)
				{
					if (bytes_per_pixel_entry > input_available && pixels_left >= count) // Check for buffer overruns
						break;

					for (int i = 0; i < count; i++)
						memcpy(output + i * bytes_per_pixel_entry, input, bytes_per_pixel_entry);

					input += bytes_per_pixel_entry;
				}
				else
				{
					if (count * bytes_per_pixel_entry >= input_available && pixels_left >= count) // Check for buffer overruns
						break;

					memcpy(output, input, count * bytes_per_pixel_entry);

					input += count * bytes_per_pixel_entry;
				}

				output += bytes_per_pixel_entry * count;
				pixels_left -= count;
			}
		}
		else
		{
			file.read(image_data.get_data(), image_data.get_size());
		}
	}

	void TargaLoader::decode_palette()
	{
		palette.resize(colormap_length);
		if (image_type == 1 || image_type == 9)
		{
			if (colormap_entry_size == 32)
			{
				unsigned int *colormap = reinterpret_cast<unsigned int*>(colormap_data.get_data());
				for (int i = 0; i < colormap_length; i++)
				{
					palette[i].a = (num_alpha_bits != 0) ? ((colormap[i] >> 24) & 0xff) : 255;
					palette[i].r = (colormap[i] >> 16) & 0xff;
					palette[i].g = (colormap[i] >> 8) & 0xff;
					palette[i].b = colormap[i] & 0xff;
				}
			}
			else if (colormap_entry_size == 24)
			{
				unsigned char *colormap = reinterpret_cast<unsigned char*>(colormap_data.get_data());
				for (int i = 0; i < colormap_length; i++)
				{
					palette[i].b = colormap[i * 3];
					palette[i].g = colormap[(i + 1) * 3];
					palette[i].r = colormap[(i + 2) * 3];
					palette[i].a = 255;
				}
			}
			else if (colormap_entry_size == 16) // 5,5,5,1
			{
				unsigned short *colormap = reinterpret_cast<unsigned short*>(colormap_data.get_data());
				for (int i = 0; i < colormap_length; i++)
				{
					int alpha_bit = (num_alpha_bits != 0) ? ((colormap[i] >> 15) & 0x1) : 1;
					palette[i].a = alpha_bit ? 255 : 1;
					palette[i].r = ((colormap[i] >> 10) & 0x1f) << 3;
					palette[i].g = ((colormap[i] >> 5) & 0x1f) << 3;
					palette[i].b = (colormap[i] & 0x1f) << 3;
				}
			}
			else
			{
				throw Exception("Unsupported targa image file");
			}
		}
	}

	void TargaLoader::decode_image()
	{
		image = PixelBuffer(image_width, image_height, srgb ? tf_srgb8_alpha8 : tf_rgba8);

		// single color-map index for Pseudo-Color
		// Attribute, Red, Green and Blue ordered data for True-Color
		// independent color-map indices for Direct-Color

		if (image_type == 0) // no image data
		{
		}
		else if (image_type == 1 || image_type == 9) // color-mapped
		{
			decode_color_mapped();
		}
		else if (image_type == 2 || image_type == 10) // true-color
		{
			decode_true_color();
		}
		else if (image_type == 3 || image_type == 11) // black-and-white
		{
			decode_grayscale();
		}
		else
		{
			throw Exception("Invalid or unsupported targa image file");
		}
	}

	void TargaLoader::decode_color_mapped()
	{
		PixelBufferLock4ub pixels(image);
		unsigned char *input = reinterpret_cast<unsigned char*>(image_data.get_data());
		for (int y = 0; y < image_height; y++)
		{
			Vec4ub *output_line = pixels.get_row(top_down ? y : image_height - y - 1);
			for (int x = 0; x < image_width; x++)
			{
				int index = 0;
				memcpy(&index, input + (x + y * image_width) * bytes_per_pixel_entry, bytes_per_pixel_entry);
				index = clamp(index - colormap_orig, 0, (int)colormap_length - 1);

				output_line[right_to_left ? image_width - 1 - x : x] = palette[index];
			}
		}
	}

	void TargaLoader::decode_true_color()
	{
		PixelBufferLock4ub pixels(image);
		if (image_pixel_size == 32)
		{
			for (int y = 0; y < image_height; y++)
			{
				unsigned int *input_line = reinterpret_cast<unsigned int*>(image_data.get_data()) + y * image_width;
				Vec4ub *output_line = pixels.get_row(top_down ? y : image_height - y - 1);
				for (int x = 0; x < image_width; x++)
				{
					output_line[right_to_left ? image_width - 1 - x : x] = Vec4ub(
						(input_line[x] >> 16) & 0xff,
						(input_line[x] >> 8) & 0xff,
						input_line[x] & 0xff,
						(num_alpha_bits != 0) ? ((input_line[x] >> 24) & 0xff) : 255);
				}
			}
		}
		else if (image_pixel_size == 24)
		{
			for (int y = 0; y < image_height; y++)
			{
				unsigned char *input_line = reinterpret_cast<unsigned char*>(image_data.get_data()) + y * image_width * 3;
				Vec4ub *output_line = pixels.get_row(top_down ? y : image_height - y - 1);
				for (int x = 0; x < image_width; x++)
				{
					output_line[right_to_left ? image_width - 1 - x : x] = Vec4ub(
						input_line[(x + 2) * 3],
						input_line[(x + 1) * 3],
						input_line[x * 3],
						255);
				}
			}
		}
		else if (image_pixel_size == 16)
		{
			for (int y = 0; y < image_height; y++)
			{
				unsigned short *input_line = reinterpret_cast<unsigned short*>(image_data.get_data()) + y * image_width;
				Vec4ub *output_line = pixels.get_row(top_down ? y : image_height - y - 1);
				for (int x = 0; x < image_width; x++)
				{
					output_line[right_to_left ? image_width - 1 - x : x] = Vec4ub(
						((input_line[x] >> 10) & 0x1f) << 3,
						((input_line[x] >> 5) & 0x1f) << 3,
						(input_line[x] & 0x1f) << 3,
						(num_alpha_bits != 0) ? ((input_line[x] >> 15) & 0x1) : 1);
				}
			}
		}
		else
		{
			throw Exception("Unsupported targa image file");
		}
	}

	void TargaLoader::decode_grayscale()
	{
		PixelBufferLock4ub pixels(image);
		if (image_pixel_size == 8)
		{
			for (int y = 0; y < image_height; y++)
			{
				unsigned char *input_line = reinterpret_cast<unsigned char*>(image_data.get_data()) + y * image_width;
				Vec4ub *output_line = pixels.get_row(top_down ? y : image_height - y - 1);
				for (int x = 0; x < image_width; x++)
				{
					output_line[right_to_left ? image_width - 1 - x : x] = Vec4ub(input_line[x], input_line[x], input_line[x], 255);
				}
			}
		}
		else
		{
			throw Exception("Unsupported targa image file");
		}
	}
}
