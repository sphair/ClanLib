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
#include "pcx_provider_generic.h"
#include "API/Core/System/error.h"

/////////////////////////////////////////////////////////////////////////////
// CL_PCXProvider_Generic construction:

CL_PCXProvider_Generic::CL_PCXProvider_Generic(
	std::string _name, 
	CL_InputSourceProvider *_provider)
{
	CL_InputSourceProvider *provider = _provider != NULL ? _provider->clone() : CL_InputSourceProvider::create_file_provider(".");
	cl_assert(provider != NULL);
	
	image = NULL;

	CL_InputSource *datafile = provider->open_source(_name.c_str());
	datafile->set_little_endian_mode();
	try
	{
		read_pcx(_name, datafile);
	}
	catch (...)
	{
		delete datafile;
		delete provider;
		throw;
	}
	delete datafile;
	delete provider;
}

CL_PCXProvider_Generic::~CL_PCXProvider_Generic()
{
	delete[] image;
	image = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CL_PCXProvider_Generic implementation:

void CL_PCXProvider_Generic::read_pcx(
	std::string _name, 
	CL_InputSource *_datafile)
{
	// make sure its valid
	cl_assert(_datafile != NULL);
	
	// Read the file header and initialize the variables
	//: This method was created to breakup the PCX decoding code
	//: so the initializing code was separated from the actual
	//: decoding algorithm

	unsigned char header[128];  

	{
		int read = _datafile->read(header, 128);
		if (read != 128)
			throw CL_Error("File not big enough to read the PCX header");
	}

	// only the useful fields that will be used more than once
	struct PCXHeader {
		unsigned char version;
		unsigned char encoding;
		unsigned char bits_per_pixel_per_plane;
		short xmin;
		short ymin;
		short xmax;
		short ymax;
		unsigned char color_map[48];
		unsigned char nplanes;
		unsigned short bytes_per_line;
	} pcx_header;

	if (header[0] != 10)
		throw CL_Error("Not a PCX file");

	pcx_header.version = header[1];
	pcx_header.encoding = header[2];
	pcx_header.bits_per_pixel_per_plane = header[3];
	pcx_header.xmin = (header[5] << 8) | header[4];	
	pcx_header.ymin = (header[7] << 8) | header[6];
	pcx_header.xmax = (header[9] << 8) | header[8];
	pcx_header.ymax = (header[11] << 8) | header[10];

	const int width = (pcx_header.xmax - pcx_header.xmin) + 1;
	const int height = (pcx_header.ymax - pcx_header.ymin) + 1;

	memcpy(pcx_header.color_map, &header[16], sizeof(pcx_header.color_map));
	pcx_header.nplanes = header[65];
	pcx_header.bytes_per_line = (header[67] << 8) | header[66];

	// both 8bit with palette and 24bit rgb modes require version 5
	if (pcx_header.version < 5)
		throw CL_Error("PCX version unsupported");

	const int total_line_bytes = pcx_header.bytes_per_line * pcx_header.nplanes;

	int pitch = 0;

	// decide which pixelformat to use (8 bit palette or 24 bit)
	if (pcx_header.nplanes == 3 && pcx_header.bits_per_pixel_per_plane == 8)
	{
		// 24 bit RGB mode
		pitch = width * 3;
		image = new unsigned char[pitch * height];
		cl_assert(image != NULL);

		unsigned char *p = image;
		const int extra = total_line_bytes - pitch;

		// get the data
		if (!pcx_header.encoding)
		{
			// uncompressed
			for (int y = 0; y < height; y++)
			{
				_datafile->read(p, pitch);
				if (extra > 0)
					_datafile->seek(extra, CL_InputSource::seek_cur);
				p += pitch;
			}
		}
		else
		{
			// compressed
			for (int y = 0; y < height; y++)
			{
				int count;
				// first red component
				int x = 0, xx = 0, comp = 0;
				while (x < total_line_bytes)
				{
					int ch = _datafile->read_uint8();
					if ((ch & 0xC0) == 0xC0)
					{
						count = ch & 0x3F;
						ch = _datafile->read_uint8();
					}
					else
						count = 1;

					while (count--)
					{
						if (xx < width)
							image[(y * pitch) + ((xx * 3) + comp)] = ch;
						x++;
						if (x == pcx_header.bytes_per_line)
						{
							// blue component now
							comp = 1;
							xx = 0;
						}
						else if (x == pcx_header.bytes_per_line*2)
						{
							// green component now
							comp = 2;
							xx = 0;
						}
						else
							xx++;
					}
				}
			}
		}
		CL_PixelBuffer_Generic::format.enable_colorkey(false);
		CL_PixelBuffer_Generic::format.set_colorkey(0);
		CL_PixelBuffer_Generic::format.set_depth(24);
		CL_PixelBuffer_Generic::format.set_red_mask(0x0000ff);
		CL_PixelBuffer_Generic::format.set_green_mask(0x00ff00);
		CL_PixelBuffer_Generic::format.set_blue_mask(0xff0000);
		CL_PixelBuffer_Generic::format.set_alpha_mask(0);
		CL_PixelBuffer_Generic::pitch = pitch;
		CL_PixelBuffer_Generic::width = width;
		CL_PixelBuffer_Generic::height = height;
	}
	else if (pcx_header.nplanes == 1 && pcx_header.bits_per_pixel_per_plane == 8)
	{
		// 8 bit indexed mode
		pitch = width;
		image = new unsigned char[pitch * height];
		cl_assert(image != NULL);

		unsigned char *p = image;
		const int extra = total_line_bytes - pitch;

		// get the data
		if (!pcx_header.encoding)
		{
			// uncompressed
			for (int y = 0; y < height; y++)
			{
				_datafile->read(p, pitch);
				if (extra > 0)
					_datafile->seek(extra, CL_InputSource::seek_cur);
				p += pitch;
			}
		}
		else
		{
			// compressed
			for (int y = 0; y < height; y++)
			{
				int count;
				int x = 0;
				while (x < total_line_bytes)
				{
					int ch = _datafile->read_uint8();
					if ((ch & 0xC0) == 0xC0)
					{
						count = ch & 0x3F;
						ch = _datafile->read_uint8();
					}
					else
						count = 1;
					
					while (count--)
					{
						if (x < width)
						{
							*p = ch;
							p++;
						}
						x++;
					}
				}
			}
		}

		// read the palette
		if (_datafile->read_int8() != 12)
		{
			delete[] image;
			throw CL_Error("Palette not found");
		}
		for (int i = 0; i < 256; i++)
		{
			const int r = _datafile->read_uint8();
			const int g = _datafile->read_uint8();
			const int b = _datafile->read_uint8();
			palette[i].set_color(r, g, b);
		}

		CL_PixelBuffer_Generic::format.enable_colorkey(false);
		CL_PixelBuffer_Generic::format.set_colorkey(0);
		CL_PixelBuffer_Generic::format.set_depth(8);
		CL_PixelBuffer_Generic::format.set_type(pixelformat_index);
		CL_PixelBuffer_Generic::pitch = pitch;
		CL_PixelBuffer_Generic::width = width;
		CL_PixelBuffer_Generic::height = height;
	}
	else
		throw CL_Error("Unsupported PCX format");
}
