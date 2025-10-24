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
#include <map>
#include "API/Display/Providers/targa_provider.h"
#include "API/Core/IOData/cl_endian.h"
#include "API/Core/System/error.h"
#include "API/Core/System/cl_assert.h"
#include "API/Core/System/clanstring.h"
#include "targa_provider_generic.h"

using namespace std;

struct CL_TargaProvider_Generic::TGA_Header
{
	unsigned char id_length;
	unsigned char colormap_type;
	unsigned char image_type_code;
	unsigned short colormap_orig;
	unsigned short colormap_length;
	unsigned char colormap_entry_size;
	short image_x_orig;
	short image_y_orig;
	unsigned short image_width;
	unsigned short image_height;
	unsigned char image_pixel_size;
	unsigned char image_descriptor_byte;
} tga_header;

CL_TargaProvider_Generic::CL_TargaProvider_Generic(
	const std::string name,
	CL_InputSourceProvider *_provider)
{
	CL_InputSourceProvider *provider;
	if (_provider == NULL)
		provider = CL_InputSourceProvider::create_file_provider(".");
	else
		provider = _provider->clone();

	cl_assert(provider != NULL);

	image = NULL;

	CL_InputSource *input_source = provider->open_source(name.c_str());
	cl_assert(input_source != NULL);
	input_source->set_little_endian_mode();

	// read the data
	try
	{
		read_tga(input_source);
	}
	catch (...)
	{
		delete input_source;
		delete provider;
		throw;
	}

	delete input_source;
	delete provider;
}

CL_TargaProvider_Generic::~CL_TargaProvider_Generic()
{
	delete[] image;
	image =  NULL;
}

/*
	Read the file data - a .TGA file can exist in several
	different formats - huffman encoded formats are not supported.
*/
void CL_TargaProvider_Generic::read_tga(CL_InputSource *input_source)
{
	TGA_Header header;
	// read the header
	input_source->seek(0, CL_InputSource::seek_set);
	header.id_length = input_source->read_uint8();
	header.colormap_type = input_source->read_uint8();
	header.image_type_code = input_source->read_uint8();
	header.colormap_orig = input_source->read_uint16();
	header.colormap_length = input_source->read_uint16();
	header.colormap_entry_size = input_source->read_uint8();
	header.image_x_orig = input_source->read_int16();
	header.image_y_orig = input_source->read_int16();
	header.image_width = input_source->read_uint16();
	header.image_height = input_source->read_uint16();
	header.image_pixel_size = input_source->read_uint8();
	header.image_descriptor_byte = input_source->read_uint8();

	// skip the id
	if (header.id_length > 0)
		input_source->seek(header.id_length, CL_InputSource::seek_cur);

	switch (header.image_type_code)
	{
	case 1:
		read_colormapped(input_source, header);
		break;
	case 2:
		read_rgb(input_source, header);
		break;
	case 9:
		read_rle_colormapped(input_source, header);
		break;
	case 10:
		read_rle_rgb(input_source, header);
		break;
	default:
		throw CL_Error("Unsupported TGA filetype encountered");
		break;
	}
}

/*
	CODE FOR DECODING TYPE 1 TARGA FILES
*/
void CL_TargaProvider_Generic::read_colormapped(
	CL_InputSource *input_source, 
	const CL_TargaProvider_Generic::TGA_Header &header)
{
	int pitch = 0;
	const int width = header.image_width;
	const int height = header.image_height;

	// if we use a palette that is <= 256 colors, we can convert it to the standard palette type
	if (header.colormap_length <= 256)
	{		
		// read the color map to palette
		{
			for (unsigned int i = 0; i < header.colormap_length; i++)
			{
				int r = 0, g = 0, b = 0, a = 255;
				if (header.colormap_entry_size == 32)
				{
					b = input_source->read_uint8();
					g = input_source->read_uint8();
					r = input_source->read_uint8();
					a = input_source->read_uint8();
				}
				else if (header.colormap_entry_size == 24)
				{
					b = input_source->read_uint8();
					g = input_source->read_uint8();
					r = input_source->read_uint8();
				}
				else if (header.colormap_entry_size == 16)
				{
					// ARRRRRGG GGGBBBBB (little endian)
					const unsigned int color = input_source->read_uint16();
					a = (color & CL_PixelFormat::get_bitmask(1, 15)) ? 255 : 0;
					r = ((color & CL_PixelFormat::get_bitmask(5, 10)) >> 10) << 3;
					g = ((color & CL_PixelFormat::get_bitmask(5, 5)) >> 5) << 3;
					b = ((color & CL_PixelFormat::get_bitmask(5, 0)) >> 0) << 3;
				}
				palette.colors[i] = CL_Color(r, g, b, a);
			}
		}

		pitch = width;

		image = new unsigned char[height * pitch];
		cl_assert(image != 0);

		const bool is_mirrored = (header.image_descriptor_byte & (1 << 5)) ? false : true;
		const int target_y_modifier = is_mirrored ? -1 : 1;
		int targety = is_mirrored ? (height - 1) : 0;
		int index_size = header.image_pixel_size / 8;
		cl_assert(index_size <= int(sizeof(unsigned int)));
		bool big = CL_Endian::is_system_big();
		for (int y = 0; y < height; y++, targety += target_y_modifier)
		{
			unsigned char *p = image + (targety * pitch);
			for (int x = 0; x < width; x++, p++)
			{
				unsigned int index = 0;
				input_source->read(&index, index_size);
				if (big)
					CL_Endian::swap(&index, index_size);
				index -= header.colormap_orig;
				*p = (unsigned char)index;
			}
		}

		CL_PixelBuffer_Generic::format.enable_colorkey(false);
		CL_PixelBuffer_Generic::format.set_colorkey(0);
		CL_PixelBuffer_Generic::format.set_depth(8);
		CL_PixelBuffer_Generic::format.set_type(pixelformat_index);
		CL_PixelBuffer_Generic::pitch = pitch;
		CL_PixelBuffer_Generic::width = width;
		CL_PixelBuffer_Generic::height = height;
	}

	// else we need to convert to the given mode
	else
	{		
		// read the color map to unsigned int in the native format
		map<unsigned int, unsigned int> color_map;
		{
			int c_size = header.colormap_entry_size / 8;
			cl_assert(c_size <= 4);
			bool big = CL_Endian::is_system_big();
			for (int i = header.colormap_orig; i < header.colormap_orig + header.colormap_length; i++)
			{
				unsigned int c = 0;
				input_source->read(&c, c_size);
				if (big) CL_Endian::swap(&c, c_size);
				color_map[i] = c;
			}
		}
		const int bytes_per_pixel = header.colormap_entry_size / 8;
		pitch = width * bytes_per_pixel;

		// set to true if an alpha value > 0 was found (if the color depth supports it)
		int alpha_found = false;
		int alpha_mask;
		if (bytes_per_pixel == 2)
			alpha_mask = 0x8000;
		else if (bytes_per_pixel == 4)
			alpha_mask = 0xff000000;
		else
			alpha_mask = 0;

		image = new unsigned char[height * pitch];
		cl_assert(image != 0);

		const bool is_mirrored = (header.image_descriptor_byte & (1 << 5)) ? false : true;
		const int target_y_modifier = is_mirrored ? -1 : 1;
		int targety = is_mirrored ? (height - 1) : 0;
		int index_size = header.image_pixel_size / 8;
		cl_assert(index_size <= 4);
		bool big = CL_Endian::is_system_big();
		for (int y = 0; y < height; y++, targety += target_y_modifier)
		{
			unsigned char *p = image + (targety * pitch);
			for (int x = 0; x < width; x++, p += bytes_per_pixel)
			{
				unsigned int index = 0;
				input_source->read(&index, index_size);
				if (big) CL_Endian::swap(&index, index_size);

				const unsigned int c = color_map[index];
				memcpy(p, &c, bytes_per_pixel);
				alpha_found |= c & alpha_mask;
			}
		}

		const int bpp = header.colormap_entry_size;

		CL_PixelBuffer_Generic::format.enable_colorkey(false);
		CL_PixelBuffer_Generic::format.set_colorkey(0);
		CL_PixelBuffer_Generic::format.set_depth(bpp);
		if (bpp == 32)
		{
			CL_PixelBuffer_Generic::format.set_red_mask(0x00ff0000);
			CL_PixelBuffer_Generic::format.set_green_mask(0x0000ff00);
			CL_PixelBuffer_Generic::format.set_blue_mask(0x000000ff);
			if (alpha_found)
				CL_PixelBuffer_Generic::format.set_alpha_mask(0xff000000);
			else
				CL_PixelBuffer_Generic::format.set_alpha_mask(0);
		}
		else if (bpp == 24)
		{
			CL_PixelBuffer_Generic::format.set_red_mask(0xff0000);
			CL_PixelBuffer_Generic::format.set_green_mask(0x00ff00);
			CL_PixelBuffer_Generic::format.set_blue_mask(0x0000ff);
			CL_PixelBuffer_Generic::format.set_alpha_mask(0x000000);
		}
		else if (bpp == 16)
		{
			// ARRRRRGG GGGBBBBB (little endian)
			CL_PixelBuffer_Generic::format.set_red_mask(CL_PixelFormat::get_bitmask(5, 10));
			CL_PixelBuffer_Generic::format.set_green_mask(CL_PixelFormat::get_bitmask(5, 5));
			CL_PixelBuffer_Generic::format.set_blue_mask(CL_PixelFormat::get_bitmask(5, 0));
			if (alpha_found)
				CL_PixelBuffer_Generic::format.set_alpha_mask(CL_PixelFormat::get_bitmask(1, 15));
			else
				CL_PixelBuffer_Generic::format.set_alpha_mask(0);
		}
		CL_PixelBuffer_Generic::pitch = pitch;
		CL_PixelBuffer_Generic::width = width;
		CL_PixelBuffer_Generic::height = height;
	}
}

/*
	CODE FOR DECODING TYPE 2 TARGA FILES
*/
void CL_TargaProvider_Generic::read_rgb(
	CL_InputSource *input_source, 
	const CL_TargaProvider_Generic::TGA_Header &header)
{
	if (header.colormap_type != 0)
	{
		// skip the color map
		input_source->seek(header.colormap_length * (header.colormap_entry_size / 8), CL_InputSource::seek_cur);
	}

	const int width = header.image_width;
	const int height = header.image_height;
	const int bytes_per_pixel = header.image_pixel_size / 8;
	int pitch = width * bytes_per_pixel;

	// set to true if an alpha value > 0 was found (if the color depth supports it)
	int alpha_found = false;
	int alpha_mask;
	if (bytes_per_pixel == 2)
		alpha_mask = 0x8000;
	else if (bytes_per_pixel == 4)
		alpha_mask = 0xff000000;
	else
		alpha_mask = 0;

	image = new unsigned char[pitch * height];
	cl_assert(image != 0);

	const bool is_mirrored = (header.image_descriptor_byte & (1 << 5)) ? false : true;
	const int target_y_modifier = is_mirrored ? -1 : 1;
	int targety = is_mirrored ? (height - 1) : 0;
	int size_c = header.image_pixel_size / 8;
	cl_assert(size_c <= 4);
	bool big = CL_Endian::is_system_big();
	for (int y = 0; y < height; y++, targety += target_y_modifier)
	{
		unsigned char *p = image + (targety * pitch);
		for (int x = 0; x < width; x++, p += bytes_per_pixel)
		{
			unsigned int c = 0;
			input_source->read(&c, size_c);
			if (big) CL_Endian::swap(&c, size_c);

			memcpy(p, &c, bytes_per_pixel);
			alpha_found |= c & alpha_mask;
		}
	}

	const int bpp = header.image_pixel_size;

	CL_PixelBuffer_Generic::format.enable_colorkey(false);
	CL_PixelBuffer_Generic::format.set_colorkey(0);
	CL_PixelBuffer_Generic::format.set_depth(bpp);
	if (bpp == 32)
	{
		CL_PixelBuffer_Generic::format.set_red_mask(0x00ff0000);
		CL_PixelBuffer_Generic::format.set_green_mask(0x0000ff00);
		CL_PixelBuffer_Generic::format.set_blue_mask(0x000000ff);
		if (alpha_found)
			CL_PixelBuffer_Generic::format.set_alpha_mask(0xff000000);
		else
			CL_PixelBuffer_Generic::format.set_alpha_mask(0);
	}
	else if (bpp == 24)
	{
		CL_PixelBuffer_Generic::format.set_red_mask(0xff0000);
		CL_PixelBuffer_Generic::format.set_green_mask(0x00ff00);
		CL_PixelBuffer_Generic::format.set_blue_mask(0x0000ff);
		CL_PixelBuffer_Generic::format.set_alpha_mask(0x000000);
	}
	else if (bpp == 16)
	{
		// ARRRRRGG GGGBBBBB (little endian)
		CL_PixelBuffer_Generic::format.set_red_mask(CL_PixelFormat::get_bitmask(5, 10));
		CL_PixelBuffer_Generic::format.set_green_mask(CL_PixelFormat::get_bitmask(5, 5));
		CL_PixelBuffer_Generic::format.set_blue_mask(CL_PixelFormat::get_bitmask(5, 0));
		if (alpha_found)
			CL_PixelBuffer_Generic::format.set_alpha_mask(CL_PixelFormat::get_bitmask(1, 15));
		else
			CL_PixelBuffer_Generic::format.set_alpha_mask(0);
	}
	CL_PixelBuffer_Generic::pitch = pitch;
	CL_PixelBuffer_Generic::width = width;
	CL_PixelBuffer_Generic::height = height;
}

/*
	CODE FOR DECODING TYPE 9 TARGA FILES
*/
void CL_TargaProvider_Generic::read_rle_colormapped(
	CL_InputSource *input_source, 
	const CL_TargaProvider_Generic::TGA_Header &header)
{
	int pitch = 0;
	const int width = header.image_width;
	const int height = header.image_height;

	const unsigned int type_mask = CL_PixelFormat::get_bitmask(1, 7);
	const unsigned int count_mask = CL_PixelFormat::get_bitmask(7, 0);

	// if we use a palette that is <= 256 colors, we can convert it to the standard palette type
	if (header.colormap_length <= 256)
	{		
		// read the color map to palette
		{
			for (unsigned int i = 0; i < header.colormap_length; i++)
			{
				int r = 0, g = 0, b = 0, a = 255;
				if (header.colormap_entry_size == 32)
				{
					b = input_source->read_uint8();
					g = input_source->read_uint8();
					r = input_source->read_uint8();
					a = input_source->read_uint8();
				}
				else if (header.colormap_entry_size == 24)
				{
					b = input_source->read_uint8();
					g = input_source->read_uint8();
					r = input_source->read_uint8();
				}
				else if (header.colormap_entry_size == 16)
				{
					// ARRRRRGG GGGBBBBB (little endian)
					const unsigned int color = input_source->read_uint16();
					a = (color & CL_PixelFormat::get_bitmask(1, 15)) ? 255 : 0;
					r = ((color & CL_PixelFormat::get_bitmask(5, 10)) >> 10) << 3;
					g = ((color & CL_PixelFormat::get_bitmask(5, 5)) >> 5) << 3;
					b = ((color & CL_PixelFormat::get_bitmask(5, 0)) >> 0) << 3;
				}
				palette.colors[i] = CL_Color(r, g, b, a);
			}
		}

		pitch = width;

		image = new unsigned char[height * pitch];
		cl_assert(image != 0);

		// repetitions left of an RLE packed and its associated color
		unsigned int repe_left = 0, repe_color = 0;
		// pixels of a raw packet left
		unsigned int raw_left = 0;

		const bool is_mirrored = (header.image_descriptor_byte & (1 << 5)) ? false : true;
		const int target_y_modifier = is_mirrored ? -1 : 1;
		int targety = is_mirrored ? (height - 1) : 0;
		int size_c = header.image_pixel_size / 8;
		cl_assert(size_c <= 4);
		bool big = CL_Endian::is_system_big();
		for (int y = 0; y < height; y++, targety += target_y_modifier)
		{
			unsigned char *p = image + (targety * pitch);
			for (int x = 0; x < width; x++, p++)
			{
				if (!repe_left && !raw_left)
				{
					// read packet type
					unsigned char ptype = input_source->read_uint8();
					if (ptype & type_mask)
					{
						repe_left = (ptype & count_mask) + 1;
						unsigned int c = 0;
						input_source->read(&c, size_c);
						if (big) CL_Endian::swap(&c, size_c);
						repe_color = c - header.colormap_orig;
					}
					else
						raw_left = (ptype & count_mask) + 1;
				}

				if (repe_left)
				{
					*p = (unsigned char)repe_color;
					repe_left--;
				}
				else if (raw_left)
				{
					unsigned int c = 0;
					input_source->read(&c, size_c);
					if (big) CL_Endian::swap(&c, size_c);
					c -= header.colormap_orig;
					*p = (unsigned char)c;
					raw_left--;
				}
			}
		}

		CL_PixelBuffer_Generic::format.enable_colorkey(false);
		CL_PixelBuffer_Generic::format.set_colorkey(0);
		CL_PixelBuffer_Generic::format.set_depth(8);
		CL_PixelBuffer_Generic::format.set_type(pixelformat_index);
		CL_PixelBuffer_Generic::pitch = pitch;
		CL_PixelBuffer_Generic::width = width;
		CL_PixelBuffer_Generic::height = height;
	}

	// else we need to convert to the given mode
	else
	{		
		// read the color map to unsigned int in the native format
		map<unsigned int, unsigned int> color_map;
		{
			int size_c = header.colormap_entry_size / 8;
			cl_assert(size_c <= 4);
			bool big = CL_Endian::is_system_big();
			for (int i = header.colormap_orig; i < header.colormap_orig + header.colormap_length; i++)
			{
				unsigned int c = 0;
				input_source->read(&c, size_c);
				if (big) CL_Endian::swap(&c, size_c);
				color_map[i] = c;
			}
		}
		const int bytes_per_pixel = header.colormap_entry_size / 8;
		pitch = width * bytes_per_pixel;

		image = new unsigned char[height * pitch];
		cl_assert(image != 0);

		// set to true if an alpha value > 0 was found (if the color depth supports it)
		int alpha_found = false;
		int alpha_mask;
		if (bytes_per_pixel == 2)
			alpha_mask = 0x8000;
		else if (bytes_per_pixel == 4)
			alpha_mask = 0xff000000;
		else
			alpha_mask = 0;

		// repetitions left of an RLE packed and its associated color
		unsigned int repe_left = 0, repe_color = 0;
		// pixels of a raw packet left
		unsigned int raw_left = 0;

		const bool is_mirrored = (header.image_descriptor_byte & (1 << 5)) ? false : true;
		const int target_y_modifier = is_mirrored ? -1 : 1;
		int targety = is_mirrored ? (height - 1) : 0;
		int size_c = header.image_pixel_size / 8;
		cl_assert(size_c <= 4);
		bool big = CL_Endian::is_system_big();
		for (int y = 0; y < height; y++, targety += target_y_modifier)
		{
			unsigned char *p = image + (targety * pitch);
			for (int x = 0; x < width; x++, p += bytes_per_pixel)
			{
				if (!repe_left && !raw_left)
				{
					// read packet type
					unsigned char ptype = input_source->read_uint8();
					if (ptype & type_mask)
					{
						repe_left = (ptype & count_mask) + 1;
						unsigned int c = 0;
						input_source->read(&c, size_c);
						if (big) CL_Endian::swap(&c, size_c);
						repe_color = c;
					}
					else
						raw_left = (ptype & count_mask) + 1;
				}

				if (repe_left)
				{
					const unsigned int c = color_map[repe_color];
					memcpy(p, &c, bytes_per_pixel);
					alpha_found |= c & alpha_mask;
					--repe_left;
				}
				else if (raw_left)
				{
					unsigned int i = 0;
					input_source->read(&i, size_c);
					if (big) CL_Endian::swap(&i, size_c);
					const unsigned int c = color_map[i];
					memcpy(p, &c, bytes_per_pixel);
					alpha_found |= c & alpha_mask;
					--raw_left;
				}
			}
		}

		const int bpp = header.colormap_entry_size;

		CL_PixelBuffer_Generic::format.enable_colorkey(false);
		CL_PixelBuffer_Generic::format.set_colorkey(0);
		CL_PixelBuffer_Generic::format.set_depth(bpp);
		if (bpp == 32)
		{
			CL_PixelBuffer_Generic::format.set_red_mask(0x00ff0000);
			CL_PixelBuffer_Generic::format.set_green_mask(0x0000ff00);
			CL_PixelBuffer_Generic::format.set_blue_mask(0x000000ff);
			if (alpha_found)
				CL_PixelBuffer_Generic::format.set_alpha_mask(0xff000000);
			else
				CL_PixelBuffer_Generic::format.set_alpha_mask(0);
		}
		else if (bpp == 24)
		{
			CL_PixelBuffer_Generic::format.set_red_mask(0xff0000);
			CL_PixelBuffer_Generic::format.set_green_mask(0x00ff00);
			CL_PixelBuffer_Generic::format.set_blue_mask(0x0000ff);
			CL_PixelBuffer_Generic::format.set_alpha_mask(0x000000);
		}
		else if (bpp == 16)
		{
			// ARRRRRGG GGGBBBBB (little endian)
			CL_PixelBuffer_Generic::format.set_red_mask(CL_PixelFormat::get_bitmask(5, 10));
			CL_PixelBuffer_Generic::format.set_green_mask(CL_PixelFormat::get_bitmask(5, 5));
			CL_PixelBuffer_Generic::format.set_blue_mask(CL_PixelFormat::get_bitmask(5, 0));
			if (alpha_found)
				CL_PixelBuffer_Generic::format.set_alpha_mask(CL_PixelFormat::get_bitmask(1, 15));
			else
				CL_PixelBuffer_Generic::format.set_alpha_mask(0);
		}
		CL_PixelBuffer_Generic::pitch = pitch;
		CL_PixelBuffer_Generic::width = width;
		CL_PixelBuffer_Generic::height = height;
	}

}

/*
	CODE FOR DECODING TYPE 10 TARGA FILES
*/
void CL_TargaProvider_Generic::read_rle_rgb(
	CL_InputSource *input_source, 
	const CL_TargaProvider_Generic::TGA_Header &header)
{
	if (header.colormap_type != 0)
	{
		// skip the color map
		input_source->seek(header.colormap_length * (header.colormap_entry_size / 8), CL_InputSource::seek_cur);
	}

	const int width = header.image_width;
	const int height = header.image_height;

	const unsigned int type_mask = CL_PixelFormat::get_bitmask(1, 7);
	const unsigned int count_mask = CL_PixelFormat::get_bitmask(7, 0);

	const int bytes_per_pixel = header.image_pixel_size / 8;
	pitch = width * bytes_per_pixel;

	image = new unsigned char[height * pitch];
	cl_assert(image != 0);

	// set to true if an alpha value > 0 was found (if the color depth supports it)
	int alpha_found = false;
	int alpha_mask;
	if (bytes_per_pixel == 2)
		alpha_mask = 0x8000;
	else if (bytes_per_pixel == 4)
		alpha_mask = 0xff000000;
	else
		alpha_mask = 0;

	// repetitions left of an RLE packed and its associated color
	unsigned int repe_left = 0, repe_color = 0;
	// pixels of a raw packet left
	unsigned int raw_left = 0;

	const bool is_mirrored = (header.image_descriptor_byte & (1 << 5)) ? false : true;
	const int target_y_modifier = is_mirrored ? -1 : 1;
	int targety = is_mirrored ? (height - 1) : 0;
	int size_c = header.image_pixel_size / 8;
	cl_assert(size_c <= 4);
	bool big = CL_Endian::is_system_big();
	for (int y = 0; y < height; y++, targety += target_y_modifier)
	{
		unsigned char *p = image + (targety * pitch);
		for (int x = 0; x < width; x++, p += bytes_per_pixel)
		{
			if (!repe_left && !raw_left)
			{
				// read packet type
				unsigned char ptype = input_source->read_uint8();
				if (ptype & type_mask)
				{
					repe_left = (ptype & count_mask) + 1;
					unsigned int c = 0;
					input_source->read(&c, size_c);
					if (big) CL_Endian::swap(&c, size_c);
					repe_color = c;
				}
				else
					raw_left = (ptype & count_mask) + 1;
			}

			if (repe_left)
			{
				memcpy(p, &repe_color, bytes_per_pixel);
				alpha_found |= repe_color & alpha_mask;
				--repe_left;
			}
			else if (raw_left)
			{
				unsigned int c = 0;
				input_source->read(&c, size_c);
				if (big) CL_Endian::swap(&c, size_c);
				memcpy(p, &c, bytes_per_pixel);
				alpha_found |= c & alpha_mask;
				--raw_left;
			}
		}
	}

	const int bpp = header.image_pixel_size;

	CL_PixelBuffer_Generic::format.enable_colorkey(false);
	CL_PixelBuffer_Generic::format.set_colorkey(0);
	CL_PixelBuffer_Generic::format.set_depth(bpp);
	if (bpp == 32)
	{
		CL_PixelBuffer_Generic::format.set_red_mask(0x00ff0000);
		CL_PixelBuffer_Generic::format.set_green_mask(0x0000ff00);
		CL_PixelBuffer_Generic::format.set_blue_mask(0x000000ff);
		if (alpha_found)
			CL_PixelBuffer_Generic::format.set_alpha_mask(0xff000000);
		else
			CL_PixelBuffer_Generic::format.set_alpha_mask(0);
	}
	else if (bpp == 24)
	{
		CL_PixelBuffer_Generic::format.set_red_mask(0xff0000);
		CL_PixelBuffer_Generic::format.set_green_mask(0x00ff00);
		CL_PixelBuffer_Generic::format.set_blue_mask(0x0000ff);
		CL_PixelBuffer_Generic::format.set_alpha_mask(0x000000);
	}
	else if (bpp == 16)
	{
		// ARRRRRGG GGGBBBBB (little endian)
		CL_PixelBuffer_Generic::format.set_red_mask(CL_PixelFormat::get_bitmask(5, 10));
		CL_PixelBuffer_Generic::format.set_green_mask(CL_PixelFormat::get_bitmask(5, 5));
		CL_PixelBuffer_Generic::format.set_blue_mask(CL_PixelFormat::get_bitmask(5, 0));
		if (alpha_found)
			CL_PixelBuffer_Generic::format.set_alpha_mask(CL_PixelFormat::get_bitmask(1, 15));
		else
			CL_PixelBuffer_Generic::format.set_alpha_mask(0);
	}
	CL_PixelBuffer_Generic::pitch = pitch;
	CL_PixelBuffer_Generic::width = width;
	CL_PixelBuffer_Generic::height = height;

}

void *CL_TargaProvider_Generic::get_data()
{
	return image;
}
