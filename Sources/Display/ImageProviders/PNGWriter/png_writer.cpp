/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
#include "png_writer.h"
#include "API/Core/Zip/zlib_compression.h"

namespace clan
{
	void PNGWriter::save(IODevice iodevice, PixelBuffer image)
	{
		PNGWriter writer(iodevice, image);
		writer.save();
	}
	
	PNGWriter::PNGWriter(IODevice iodevice, PixelBuffer src_image) : device(iodevice)
	{
		// This writer only supports RGBA format
		if (src_image.get_bytes_per_pixel() < 8)
			image = src_image.to_format(TextureFormat::rgba8);
		else
			image = src_image.to_format(TextureFormat::rgba16);
	}
	
	void PNGWriter::save()
	{
		write_magic();
		write_headers();
		write_data();
		write_chunk("IEND", nullptr, 0);
	}
	
	void PNGWriter::write_magic()
	{
		unsigned char png_magic[8] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
		device.write(png_magic, 8);
	}
	
	void PNGWriter::write_headers()
	{
		int ppm = (int)std::round(3800 * image.get_pixel_ratio());
		int ppm_x = ppm;
		int ppm_y = ppm;
		
		int width = image.get_width();
		int height = image.get_height();
		int bit_depth = image.get_bytes_per_pixel() == 8 ? 16 : 8;
		int color_type = 6;
		int compression_method = 0;
		int filter_method = 0;
		int interlace_method = 0;
		
		unsigned char idhr[13];
		idhr[0] = (width >> 24) & 0xff;
		idhr[1] = (width >> 16) & 0xff;
		idhr[2] = (width >> 8) & 0xff;
		idhr[3] = width & 0xff;
		idhr[4] = (height >> 24) & 0xff;
		idhr[5] = (height >> 16) & 0xff;
		idhr[6] = (height >> 8) & 0xff;
		idhr[7] = height & 0xff;
		idhr[8] = bit_depth;
		idhr[9] = color_type;
		idhr[10] = compression_method;
		idhr[11] = filter_method;
		idhr[12] = interlace_method;
		
		//unsigned char srgb[1];
		//srgb[0] = 0;
		
		unsigned char phys[9];
		phys[0] = (ppm_x >> 24) & 0xff;
		phys[1] = (ppm_x >> 16) & 0xff;
		phys[2] = (ppm_x >> 8) & 0xff;
		phys[3] = ppm_x & 0xff;
		phys[4] = (ppm_y >> 24) & 0xff;
		phys[5] = (ppm_y >> 16) & 0xff;
		phys[6] = (ppm_y >> 8) & 0xff;
		phys[7] = ppm_y & 0xff;
		phys[8] = 1; // pixels per meter
		
		write_chunk("IHDR", idhr, 13);
		
		if (ppm != 0)
			write_chunk("pHYs", phys, 9);
		
		//write_chunk("sRGB", srgb, 1);
	}
	
	void PNGWriter::write_data()
	{
		//int width = image.get_width();
		int height = image.get_height();
		int bytes_per_pixel = image.get_bytes_per_pixel();
		
		std::vector<unsigned char> scanline_orig;
		std::vector<unsigned char> scanline_filtered;
		scanline_orig.resize((image.get_width() + 1) * bytes_per_pixel);
		scanline_filtered.resize(image.get_width() * bytes_per_pixel + 1);
		
		DataBuffer idat_uncompressed(height * scanline_filtered.size());
		
		for (int y = 0; y < height; y++)
		{
			// Grab scanline
			memcpy(scanline_orig.data() + bytes_per_pixel, image.get_line(y), scanline_orig.size() - bytes_per_pixel);

			// Convert to big endian for 16 bit
			if (bytes_per_pixel == 8)
			{
				for (size_t x = 0; x < scanline_orig.size(); x+=2)
				{
					std::swap(scanline_orig[x], scanline_orig[x + 1]);
				}
			}

			// Filter scanline
			/*
			scanline_filtered[0] = 0; // None filter type
			for (int i = bytes_per_pixel; i < scanline_orig.size(); i++)
			{
				scanline_filtered[i - bytes_per_pixel + 1] = scanline_orig[i];
			}
			*/
			scanline_filtered[0] = 1; // Sub filter type
			for (int i = bytes_per_pixel; i < scanline_orig.size(); i++)
			{
				unsigned char a = scanline_orig[i - bytes_per_pixel];
				unsigned char x = scanline_orig[i];
				scanline_filtered[i - bytes_per_pixel + 1] = x - a;
			}
			
			// Output scanline
			memcpy(idat_uncompressed.get_data<unsigned char>() + y * scanline_filtered.size(), scanline_filtered.data(), scanline_filtered.size());
		}
		
		DataBuffer idat = ZLibCompression::compress(idat_uncompressed, false);
		
		write_chunk("IDAT", idat.get_data(), idat.get_size());
	}
	
	void PNGWriter::write_chunk(const char name[4], const void *data, int size)
	{
		unsigned char size_data[4];
		size_data[0] = (size >> 24) & 0xff;
		size_data[1] = (size >> 16) & 0xff;
		size_data[2] = (size >> 8) & 0xff;
		size_data[3] = size & 0xff;
		device.write(size_data, 4);

		device.write(name, 4);

		device.write(data, size);
		unsigned int crc32 = PNGCRC32::crc(name, data, size);

		unsigned char crc32_data[4];
		crc32_data[0] = (crc32 >> 24) & 0xff;
		crc32_data[1] = (crc32 >> 16) & 0xff;
		crc32_data[2] = (crc32 >> 8) & 0xff;
		crc32_data[3] = crc32 & 0xff;
		device.write(crc32_data, 4);
	}
}
