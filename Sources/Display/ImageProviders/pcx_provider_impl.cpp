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
**    (if your name is missing here, please add it)
*/

#include "Display/precomp.h"
#include "pcx_provider_impl.h"
#include "API/Core/System/exception.h"

/////////////////////////////////////////////////////////////////////////////
// CL_PCXProvider_Impl construction:

CL_PCXProvider_Impl::CL_PCXProvider_Impl(
	CL_IODevice &datafile)
{
	image = NULL;

	bool little_endian = datafile.is_little_endian();
	if (!little_endian)
		datafile.set_little_endian_mode();

	read_pcx(datafile);

	if (!little_endian)
		datafile.set_big_endian_mode();

}

CL_PCXProvider_Impl::~CL_PCXProvider_Impl()
{
	delete[] image;
	image = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CL_PCXProvider_Impl implementation:

void CL_PCXProvider_Impl::read_pcx(
	CL_IODevice &_datafile)
{
	// Read the file header and initialize the variables
	// This method was created to breakup the PCX decoding code
	// so the initializing code was separated from the actual
	// decoding algorithm

	unsigned char header[128];  

	{
		int read = _datafile.read(header, 128);
		if (read != 128)
			throw CL_Exception("File not big enough to read the PCX header");
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
		throw CL_Exception("Not a PCX file");

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
		throw CL_Exception("PCX version unsupported");

	const int total_line_bytes = pcx_header.bytes_per_line * pcx_header.nplanes;

	int pitch = 0;

	// decide which pixelformat to use (8 bit palette or 24 bit)
	if (pcx_header.nplanes == 3 && pcx_header.bits_per_pixel_per_plane == 8)
	{
		// 24 bit RGB mode
		pitch = width * 3;
		image = new unsigned char[pitch * height];
		if (image == 0)
			throw CL_Exception("Unable to allocate memory for pcx image");

		unsigned char *p = image;
		const int extra = total_line_bytes - pitch;

		// get the data
		if (!pcx_header.encoding)
		{
			// uncompressed
			for (int y = 0; y < height; y++)
			{
				_datafile.read(p, pitch);
				if (extra > 0)
					_datafile.seek(extra, CL_IODevice::seek_cur);
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
					int ch = _datafile.read_uint8();
					if ((ch & 0xC0) == 0xC0)
					{
						count = ch & 0x3F;
						ch = _datafile.read_uint8();
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
		CL_PCXProvider_Impl::sized_format = cl_bgr8;
		CL_PCXProvider_Impl::pitch = pitch;
		CL_PCXProvider_Impl::width = width;
		CL_PCXProvider_Impl::height = height;
	}
	else if (pcx_header.nplanes == 1 && pcx_header.bits_per_pixel_per_plane == 8)
	{
		// 8 bit indexed mode
		pitch = width;
		image = new unsigned char[pitch * height];
		if (image == 0)
			throw CL_Exception("Unable to allocate memory for pcx image");

		unsigned char *p = image;
		const int extra = total_line_bytes - pitch;

		// get the data
		if (!pcx_header.encoding)
		{
			// uncompressed
			for (int y = 0; y < height; y++)
			{
				_datafile.read(p, pitch);
				if (extra > 0)
					_datafile.seek(extra, CL_IODevice::seek_cur);
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
					int ch = _datafile.read_uint8();
					if ((ch & 0xC0) == 0xC0)
					{
						count = ch & 0x3F;
						ch = _datafile.read_uint8();
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
		if (_datafile.read_int8() != 12)
		{
			delete[] image;
			throw CL_Exception("Palette not found");
		}
		for (int i = 0; i < 256; i++)
		{
			const int r = _datafile.read_uint8();
			const int g = _datafile.read_uint8();
			const int b = _datafile.read_uint8();
			palette[i].set_color(r, g, b);
		}

		CL_PCXProvider_Impl::sized_format = cl_color_index;
		CL_PCXProvider_Impl::pitch = pitch;
		CL_PCXProvider_Impl::width = width;
		CL_PCXProvider_Impl::height = height;
	}
	else
		throw CL_Exception("Unsupported PCX format");
}
