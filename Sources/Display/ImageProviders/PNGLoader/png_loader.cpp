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
#include "png_loader.h"
#include "API/Display/Image/pixel_buffer_lock.h"
#include "API/Core/Zip/zlib_compression.h"
#include "API/Core/System/system.h"
#include "Display/ImageProviders/PNGWriter/png_writer.h"

namespace clan
{
	PixelBuffer PNGLoader::load(IODevice iodevice, bool srgb)
	{
		PNGLoader loader(iodevice, srgb);
		return loader.image;
	}

	PNGLoader::PNGLoader(IODevice iodevice, bool force_srgb)
		: file(iodevice), force_srgb(force_srgb), scanline(nullptr), prev_scanline(nullptr), scanline_4ub(nullptr), scanline_4us(nullptr), palette(nullptr)
	{
		read_magic();
		read_chunks();
		decode_header();
		decode_palette();
		decode_colorkey();
		decode_image();
	}

	PNGLoader::~PNGLoader()
	{
		System::aligned_free(scanline);
		System::aligned_free(prev_scanline);
		System::aligned_free(scanline_4ub);
		System::aligned_free(scanline_4us);
		System::aligned_free(palette);
	}

	void PNGLoader::read_magic()
	{
		unsigned char png_magic[8] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
		unsigned char file_magic[8];
		file.read(file_magic, 8);
		if (memcmp(png_magic, file_magic, 8) != 0)
			throw Exception("Invalid PNG image file");
	}

	void PNGLoader::read_chunks()
	{
		file.set_big_endian_mode();

		std::map<std::string, DataBuffer> chunks;

		std::vector<DataBuffer> idat_chunks;
		uint64_t total_idat_size = 0;

		while (true)
		{
			unsigned int length = file.read_uint32();
			char name[5];
			name[4] = 0;
			file.read(name, 4);

			DataBuffer data(length);
			file.read(data.get_data(), data.get_size());

			unsigned int crc32 = file.read_uint32();

			unsigned int compare_crc32 = PNGCRC32::crc(name, data.get_data(), data.get_size());
			if (crc32 != compare_crc32)
				throw Exception("CRC32 error");

			if (name == std::string("IDAT")) // Concatenate all IDAT chunks to one big
			{
				total_idat_size += length;
				idat_chunks.push_back(data);
			}
			else
			{
				chunks[name] = data;
				if (name == std::string("IEND")) // image trailer, which is the last chunk in a PNG datastream.
					break;
			}
		}

		if (total_idat_size >= (1 << 31))
			throw Exception("PNG image file too big!");

		idat = DataBuffer((int)total_idat_size);
		int idat_pos = 0;
		for (auto & idat_chunk : idat_chunks)
		{
			memcpy(idat.get_data() + idat_pos, idat_chunk.get_data(), idat_chunk.get_size());
			idat_pos += idat_chunk.get_size();
		}

		ihdr = chunks["IHDR"];
		plte = chunks["PLTE"];

		trns = chunks["tRNS"];
		chrm = chunks["cHRM"];
		gama = chunks["gAMA"];
		iccp = chunks["iCCP"];
		sbit = chunks["sBIT"];
		srgb = chunks["sRGB"];

		if (ihdr.is_null() || idat_chunks.empty() || ihdr.get_size() != 13) // Always required chunks
			throw Exception("Invalid PNG image file");
	}

	void PNGLoader::decode_header()
	{
		image_width = from_network_order(*reinterpret_cast<unsigned int*>(ihdr.get_data()));
		image_height = from_network_order(*reinterpret_cast<unsigned int*>(ihdr.get_data() + 4));
		bit_depth = *reinterpret_cast<unsigned char*>(ihdr.get_data() + 8);
		color_type = *reinterpret_cast<unsigned char*>(ihdr.get_data() + 9);
		compression_method = *reinterpret_cast<unsigned char*>(ihdr.get_data() + 10);
		filter_method = *reinterpret_cast<unsigned char*>(ihdr.get_data() + 11);
		interlace_method = *reinterpret_cast<unsigned char*>(ihdr.get_data() + 12);

		// Verify image is valid according to the PNG standard (Table 11.1 in http://www.w3.org/TR/PNG/)
		if (color_type == 1 || color_type == 5 || color_type > 6)
			throw Exception("Invalid PNG image file");
		if (bit_depth == 0 || bit_depth == 3 || (bit_depth >= 5 && bit_depth <= 7) || (bit_depth >= 9 && bit_depth <= 15) || bit_depth > 16)
			throw Exception("Invalid PNG image file");
		if ((color_type == 2 || color_type == 4 || color_type == 6) && bit_depth < 8)
			throw Exception("Invalid PNG image file");
		if (color_type == 3 && bit_depth > 8)
			throw Exception("Invalid PNG image file");

		// Verify image uses a known compression, filter and interlace method:
		if (compression_method != 0 || filter_method != 0 || interlace_method > 1)
			throw Exception("Invalid PNG image file");

		// Guard against possible buffer overruns and other edge-cases when image size is gigantic:
		if (image_width >= (1 << 23) || image_height >= (1 << 23))
			throw Exception("PNG image is too big");
	}

	void PNGLoader::decode_palette()
	{
		if (color_type == 3) // indexed
		{
			if (plte.is_null() || plte.get_size() % 3 != 0)
				throw Exception("Invalid PNG image file");

			int num_entries = plte.get_size() / 3;
			if (num_entries > 256)
				throw Exception("Invalid PNG image file");

			unsigned char *entries = reinterpret_cast<unsigned char*>(plte.get_data());

			palette = static_cast<Vec4ub *>(System::aligned_alloc(256 * sizeof(Vec4ub)));
			for (int i = 0; i < num_entries; i++)
				palette[i] = Vec4ub(entries[i * 3], entries[i * 3 + 1], entries[i * 3 + 2], 255);

			if (!trns.is_null())
			{
				int num_alpha_entries = trns.get_size();
				if (num_alpha_entries > num_entries)
					throw Exception("Invalid PNG image file");

				unsigned char *alpha_entries = reinterpret_cast<unsigned char*>(trns.get_data());
				for (int i = 0; i < num_alpha_entries; i++)
					palette[i].a = alpha_entries[i];
			}
		}
	}

	void PNGLoader::decode_colorkey()
	{
		has_colorkey = !trns.is_null() && color_type != 3;
		if (has_colorkey)
		{
			if (color_type == 0) // grayscale
			{
				if (trns.get_size() != 2)
					throw Exception("Invalid PNG image file");
				colorkey = Vec3us(from_network_order(*reinterpret_cast<unsigned short*>(trns.get_data())));
			}
			else if (color_type == 2) // truecolor
			{
				if (trns.get_size() != 6)
					throw Exception("Invalid PNG image file");
				unsigned short *values = reinterpret_cast<unsigned short*>(trns.get_data());
				colorkey = Vec3us(from_network_order(values[0]), from_network_order(values[1]), from_network_order(values[2]));
			}
			else
			{
				throw Exception("Invalid PNG image file");
			}
		}
	}

	void PNGLoader::decode_image()
	{
		DataBuffer image_data = ZLibCompression::decompress(idat, false);

		create_image();
		create_scanline_buffers();

		if (interlace_method == 0)
		{
			decode_interlace_none(reinterpret_cast<const unsigned char*>(image_data.get_data()), image_data.get_size());
		}
		else if (interlace_method == 1)
		{
			decode_interlace_adam7(reinterpret_cast<const unsigned char*>(image_data.get_data()), image_data.get_size());
		}
		else
		{
			throw Exception("Invalid PNG image file");
		}
	}

	void PNGLoader::create_image()
	{
		if (bit_depth <= 8)
			image = PixelBuffer(image_width, image_height, force_srgb ? tf_srgb8_alpha8 : tf_rgba8);
		else
			image = PixelBuffer(image_width, image_height, tf_rgba16);
	}

	void PNGLoader::create_scanline_buffers()
	{
		int size = (image_width * bit_depth * get_image_data_channels() + 7) / 8;
		scanline = static_cast<unsigned char *>(System::aligned_alloc(size));
		prev_scanline = static_cast<unsigned char *>(System::aligned_alloc(size));
		scanline_4ub = static_cast<Vec4ub *>(System::aligned_alloc(image_width * sizeof(Vec4ub)));
		scanline_4us = static_cast<Vec4us *>(System::aligned_alloc(image_width * sizeof(Vec4us)));
	}

	int PNGLoader::get_image_data_channels()
	{
		switch (color_type)
		{
		case 0: return 1; // grayscale
		case 2: return 3; // truecolor
		case 3: return 1; // indexed
		case 4: return 2; // grayscale with alpha
		case 6: return 4; // truecolor with alpha
		default: throw Exception("Invalid PNG image file");
		}
	}

	void PNGLoader::decode_interlace_none(const unsigned char *data, int data_length)
	{
		int scanline_size = (image_width * bit_depth * get_image_data_channels() + 7) / 8;
		if ((1 + scanline_size) * image_height > data_length)
			throw Exception("Invalid PNG image file");

		for (size_t i = 0; i < scanline_size; i++)
			scanline[i] = 0;

		int data_pos = 0;
		PixelBufferLockAny pixels(image);
		for (int y = 0; y < image_height; y++)
		{
			unsigned char *tmp = scanline;
			scanline = prev_scanline;
			prev_scanline = tmp;

			int predictor_type = data[data_pos++];
			memcpy(scanline, data + data_pos, scanline_size);
			data_pos += scanline_size;

			filter_scanline(predictor_type, scanline_size);

			if (bit_depth <= 8)
				convert_scanline_4ub(image_width);
			else
				convert_scanline_4us(image_width);

			unsigned char *output_line = pixels.get_row(y);
			if (bit_depth <= 8)
				memcpy(output_line, scanline_4ub, image_width * 4);
			else
				memcpy(output_line, scanline_4us, image_width * 8);
		}
	}

	void PNGLoader::decode_interlace_adam7(const unsigned char *data, int data_length)
	{
		int scanline_size = (image_width * bit_depth * get_image_data_channels() + 7) / 8;

		int data_pos = 0;
		int channels = get_image_data_channels();

		int starting_row[7] = { 0, 0, 4, 0, 2, 0, 1 };
		int starting_col[7] = { 0, 4, 0, 2, 0, 1, 0 };
		int row_increment[7] = { 8, 8, 8, 4, 4, 2, 2 };
		int col_increment[7] = { 8, 8, 4, 4, 2, 2, 1 };
		//int block_height[7]  = { 8, 8, 4, 4, 2, 2, 1 };
		//int block_width[7]   = { 8, 4, 4, 2, 2, 1, 1 };

		PixelBufferLockAny pixels(image);
		unsigned char *output = pixels.get_data();
		int output_pitch = pixels.get_pitch();
		for (int pass = 0; pass < 7; pass++)
		{
			for (size_t i = 0; i < scanline_size; i++)
				scanline[i] = 0;

			for (int y = starting_row[pass]; y < image_height; y += row_increment[pass])
			{
				if (starting_col[pass] < image_width)
				{
					unsigned char *tmp = scanline;
					scanline = prev_scanline;
					prev_scanline = tmp;

					int scanline_pixel_length = (image_width - starting_col[pass] + col_increment[pass] - 1) / col_increment[pass];
					int scanline_byte_length = (scanline_pixel_length * bit_depth * channels + 7) / 8;

					if (data_pos + scanline_byte_length > data_length)
						throw Exception("Invalid PNG image file");

					int predictor_type = data[data_pos++];
					memcpy(scanline, data + data_pos, scanline_byte_length);
					data_pos += scanline_byte_length;

					filter_scanline(predictor_type, scanline_byte_length);

					if (bit_depth <= 8)
						convert_scanline_4ub(scanline_pixel_length);
					else
						convert_scanline_4us(scanline_pixel_length);

					int scanline_pos = 0;
					for (int x = starting_col[pass]; x < image_width; x += col_increment[pass])
					{
						if (bit_depth <= 8)
							*reinterpret_cast<Vec4ub*>(output + y * output_pitch + x * 4) = scanline_4ub[scanline_pos++];
						else
							*reinterpret_cast<Vec4us*>(output + y * output_pitch + x * 8) = scanline_4us[scanline_pos++];
					}
				}
			}
		}
	}

	void PNGLoader::filter_scanline(int predictor_type, int scanline_byte_length)
	{
		int channels = get_image_data_channels();
		switch (predictor_type)
		{
		case 0: break; // none
		case 1: predictor_sub(scanline, prev_scanline, scanline_byte_length, channels, bit_depth); break;
		case 2: predictor_up(scanline, prev_scanline, scanline_byte_length, channels, bit_depth); break;
		case 3: predictor_average(scanline, prev_scanline, scanline_byte_length, channels, bit_depth); break;
		case 4: predictor_paeth(scanline, prev_scanline, scanline_byte_length, channels, bit_depth); break;
		default: throw Exception("Invalid PNG image file");
		}
	}

	void PNGLoader::predictor_sub(unsigned char *scanline, const unsigned char *prev_scanline, int byte_length, int channels, int bit_depth)
	{
		int bytes_per_pixel = channels * ((bit_depth + 7) / 8);
		for (int i = 0; i < byte_length; i++)
		{
			int x = scanline[i];
			int a = i >= bytes_per_pixel ? scanline[i - bytes_per_pixel] : 0;
			scanline[i] = x + a;
		}
	}

	void PNGLoader::predictor_up(unsigned char *scanline, const unsigned char *prev_scanline, int byte_length, int channels, int bit_depth)
	{
		//int bytes_per_pixel = channels * ((bit_depth + 7) / 8);
		for (int i = 0; i < byte_length; i++)
		{
			int x = scanline[i];
			int b = prev_scanline[i];
			scanline[i] = x + b;
		}
	}

	void PNGLoader::predictor_average(unsigned char *scanline, const unsigned char *prev_scanline, int byte_length, int channels, int bit_depth)
	{
		int bytes_per_pixel = channels * ((bit_depth + 7) / 8);
		for (int i = 0; i < byte_length; i++)
		{
			int x = scanline[i];
			int a = i >= bytes_per_pixel ? scanline[i - bytes_per_pixel] : 0;
			int b = prev_scanline[i];
			scanline[i] = x + (a + b) / 2;
		}
	}

	void PNGLoader::predictor_paeth(unsigned char *scanline, const unsigned char *prev_scanline, int byte_length, int channels, int bit_depth)
	{
		int bytes_per_pixel = channels * ((bit_depth + 7) / 8);
		for (int i = 0; i < byte_length; i++)
		{
			int x = scanline[i];
			int a = i >= bytes_per_pixel ? scanline[i - bytes_per_pixel] : 0;
			int b = prev_scanline[i];
			int c = i >= bytes_per_pixel ? prev_scanline[i - bytes_per_pixel] : 0;
			int p = a + b - c;
			int pa = abs((p - a));
			int pb = abs((p - b));
			int pc = abs(p - c);
			int pr;
			if (pa <= pb && pa <= pc)
				pr = a;
			else if (pb <= pc)
				pr = b;
			else
				pr = c;
			scanline[i] = x + pr;
		}
	}

	void PNGLoader::convert_scanline_4ub(int scanline_pixel_length)
	{
		switch (color_type)
		{
		case 0: grayscale_to_4ub(scanline_pixel_length); break;
		case 2: truecolor_to_4ub(scanline_pixel_length); break;
		case 3: indexed_to_4ub(scanline_pixel_length); break;
		case 4: grayscale_alpha_to_4ub(scanline_pixel_length); break;
		case 6: truecolor_alpha_to_4ub(scanline_pixel_length); break;
		default: throw Exception("Invalid PNG image file");
		}
	}

	void PNGLoader::convert_scanline_4us(int scanline_pixel_length)
	{
		switch (color_type)
		{
		case 0: grayscale_to_4us(scanline_pixel_length); break;
		case 2: truecolor_to_4us(scanline_pixel_length); break;
		case 4: grayscale_alpha_to_4us(scanline_pixel_length); break;
		case 6: truecolor_alpha_to_4us(scanline_pixel_length); break;
		default: throw Exception("Invalid PNG image file");
		}
	}

	void PNGLoader::grayscale_to_4ub(int count)
	{
		unsigned char *input = scanline;
		if (bit_depth == 1)
		{
			if (!has_colorkey)
			{
				for (int i = 0; i < count; i++)
				{
					int shift = i % 8;
					unsigned char value = (input[i / 8] >> shift) & 1;
					value = static_cast<int>(value)* 255;
					scanline_4ub[i] = Vec4ub(value, value, value, 255);
				}
			}
			else
			{
				for (int i = 0; i < count; i++)
				{
					int shift = i % 8;
					unsigned char value = (input[i / 8] >> shift) & 1;
					unsigned char alpha = (value != colorkey.r) ? 255 : 0;
					value = static_cast<int>(value)* 255;
					scanline_4ub[i] = Vec4ub(value, value, value, alpha);
				}
			}
		}
		else if (bit_depth == 2)
		{
			if (!has_colorkey)
			{
				for (int i = 0; i < count; i++)
				{
					int shift = (i % 4) * 2;
					unsigned char value = (input[i / 4] >> shift) & 3;
					value = (static_cast<int>(value)* 255 + 1) / 2;
					scanline_4ub[i] = Vec4ub(value, value, value, 255);
				}
			}
			else
			{
				for (int i = 0; i < count; i++)
				{
					int shift = (i % 4) * 2;
					unsigned char value = (input[i / 4] >> shift) & 3;
					unsigned char alpha = (value != colorkey.r) ? 255 : 0;
					value = (static_cast<int>(value)* 255 + 1) / 2;
					scanline_4ub[i] = Vec4ub(value, value, value, alpha);
				}
			}
		}
		else if (bit_depth == 4)
		{
			if (!has_colorkey)
			{
				for (int i = 0; i < count; i++)
				{
					int shift = (i % 2) * 4;
					unsigned char value = (input[i / 4] >> shift) & 15;
					value = (static_cast<int>(value)* 255 + 8) / 16;
					scanline_4ub[i] = Vec4ub(value, value, value, 255);
				}
			}
			else
			{
				for (int i = 0; i < count; i++)
				{
					int shift = (i % 2) * 4;
					unsigned char value = (input[i / 4] >> shift) & 15;
					unsigned char alpha = (value != colorkey.r) ? 255 : 0;
					value = (static_cast<int>(value)* 255 + 8) / 16;
					scanline_4ub[i] = Vec4ub(value, value, value, alpha);
				}
			}
		}
		else if (bit_depth == 8)
		{
			if (!has_colorkey)
			{
				for (int i = 0; i < count; i++)
				{
					unsigned char value = input[i];
					scanline_4ub[i] = Vec4ub(value, value, value, 255);
				}
			}
			else
			{
				for (int i = 0; i < count; i++)
				{
					unsigned char value = input[i];
					unsigned char alpha = (value != colorkey.r) ? 255 : 0;
					scanline_4ub[i] = Vec4ub(value, value, value, alpha);
				}
			}
		}
		else
		{
			throw Exception("Invalid PNG image file");
		}
	}

	void PNGLoader::truecolor_to_4ub(int count)
	{
		if (bit_depth != 8)
			throw Exception("Invalid PNG image file");

		unsigned char *input = scanline;

		if (!has_colorkey)
		{
			for (int i = 0; i < count; i++)
			{
				unsigned char red = input[i * 3 + 0];
				unsigned char green = input[i * 3 + 1];
				unsigned char blue = input[i * 3 + 2];
				scanline_4ub[i] = Vec4ub(red, green, blue, 255);
			}
		}
		else
		{
			for (int i = 0; i < count; i++)
			{
				unsigned char red = input[i * 3 + 0];
				unsigned char green = input[i * 3 + 1];
				unsigned char blue = input[i * 3 + 2];
				unsigned char alpha = 255;
				if (red == colorkey.r && green == colorkey.g && blue == colorkey.b)
					alpha = 0;
				scanline_4ub[i] = Vec4ub(red, green, blue, alpha);
			}
		}
	}

	void PNGLoader::indexed_to_4ub(int count)
	{
		unsigned char *input = scanline;
		if (bit_depth == 1)
		{
			for (int i = 0; i < count; i++)
			{
				int shift = i % 8;
				unsigned char value = (input[i / 8] >> shift) & 1;
				scanline_4ub[i] = palette[value];
			}
		}
		else if (bit_depth == 2)
		{
			for (int i = 0; i < count; i++)
			{
				int shift = (i % 4) * 2;
				unsigned char value = (input[i / 4] >> shift) & 3;
				scanline_4ub[i] = palette[value];
			}
		}
		else if (bit_depth == 4)
		{
			for (int i = 0; i < count; i++)
			{
				int shift = (i % 2) * 4;
				unsigned char value = (input[i / 4] >> shift) & 15;
				scanline_4ub[i] = palette[value];
			}
		}
		else if (bit_depth == 8)
		{
			for (int i = 0; i < count; i++)
			{
				unsigned char value = input[i];
				scanline_4ub[i] = palette[value];
			}
		}
		else
		{
			throw Exception("Invalid PNG image file");
		}
	}

	void PNGLoader::grayscale_alpha_to_4ub(int count)
	{
		if (bit_depth != 8)
			throw Exception("Invalid PNG image file");

		unsigned char *input = scanline;
		for (int i = 0; i < count; i++)
		{
			unsigned char value = input[i * 2];
			unsigned char alpha = input[i * 2 + 1];
			scanline_4ub[i] = Vec4ub(value, value, value, alpha);
		}
	}

	void PNGLoader::truecolor_alpha_to_4ub(int count)
	{
		if (bit_depth != 8)
			throw Exception("Invalid PNG image file");

		unsigned char *input = scanline;
		for (int i = 0; i < count; i++)
		{
			unsigned char red = input[i * 4 + 0];
			unsigned char green = input[i * 4 + 1];
			unsigned char blue = input[i * 4 + 2];
			unsigned char alpha = input[i * 4 + 3];
			scanline_4ub[i] = Vec4ub(red, green, blue, alpha);
		}
	}

	void PNGLoader::grayscale_to_4us(int count)
	{
		if (bit_depth != 16)
			throw Exception("Invalid PNG image file");

		unsigned short *input = reinterpret_cast<unsigned short*>(scanline);
		if (!has_colorkey)
		{
			for (int i = 0; i < count; i++)
			{
				unsigned short value = from_network_order(input[i]);
				scanline_4us[i] = Vec4us(value, value, value, 65535);
			}
		}
		else
		{
			for (int i = 0; i < count; i++)
			{
				unsigned short value = from_network_order(input[i]);
				unsigned short alpha = (value != colorkey.r) ? 65535 : 0;
				scanline_4us[i] = Vec4us(value, value, value, alpha);
			}
		}
	}

	void PNGLoader::truecolor_to_4us(int count)
	{
		if (bit_depth != 16)
			throw Exception("Invalid PNG image file");

		unsigned short *input = reinterpret_cast<unsigned short*>(scanline);
		if (!has_colorkey)
		{
			for (int i = 0; i < count; i++)
			{
				unsigned short red = from_network_order(input[i * 3 + 0]);
				unsigned short green = from_network_order(input[i * 3 + 1]);
				unsigned short blue = from_network_order(input[i * 3 + 2]);
				scanline_4us[i] = Vec4us(red, green, blue, 65535);
			}
		}
		else
		{
			for (int i = 0; i < count; i++)
			{
				unsigned short red = from_network_order(input[i * 3 + 0]);
				unsigned short green = from_network_order(input[i * 3 + 1]);
				unsigned short blue = from_network_order(input[i * 3 + 2]);
				unsigned short alpha = 65535;
				if (red == colorkey.r && green == colorkey.g && blue == colorkey.b)
					alpha = 0;
				scanline_4us[i] = Vec4us(red, green, blue, alpha);
			}
		}
	}

	void PNGLoader::grayscale_alpha_to_4us(int count)
	{
		if (bit_depth != 16)
			throw Exception("Invalid PNG image file");

		unsigned short *input = reinterpret_cast<unsigned short*>(scanline);
		for (int i = 0; i < count; i++)
		{
			unsigned short value = from_network_order(input[i * 2]);
			unsigned short alpha = from_network_order(input[i * 2 + 1]);
			scanline_4us[i] = Vec4us(value, value, value, alpha);
		}
	}

	void PNGLoader::truecolor_alpha_to_4us(int count)
	{
		if (bit_depth != 16)
			throw Exception("Invalid PNG image file");

		unsigned short *input = reinterpret_cast<unsigned short*>(scanline);
		for (int i = 0; i < count; i++)
		{
			unsigned short red = from_network_order(input[i * 4 + 0]);
			unsigned short green = from_network_order(input[i * 4 + 1]);
			unsigned short blue = from_network_order(input[i * 4 + 2]);
			unsigned short alpha = from_network_order(input[i * 4 + 3]);
			scanline_4us[i] = Vec4us(red, green, blue, alpha);
		}
	}
}
