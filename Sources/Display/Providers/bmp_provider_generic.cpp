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
**    Seth A. Robinson
**    (if your name is missing here, please add it)
*/

#include "Display/display_precomp.h"
#include "bmp_provider_generic.h"
#include "API/Core/System/error.h"
#include "API/Core/IOData/cl_endian.h"

//this pragma should be ok with GCC 3+ and MSVC6,7,8
#pragma pack(push, 1)

typedef struct BMPTopHeaderStruct {
	char type1, type2;                 /* Magic identifier            */
	unsigned int size;                       /* File size in bytes          */
	unsigned short reserved1, reserved2;
	unsigned int offset;                     /* Offset to image data, bytes */
} BMPTOPHEADER;


//the odd renaming is to not clash with the windows version
typedef struct GtagBITMAPINFOHEADER {    
	//should these be unsigned ints?
	unsigned int   biSize;
	int    biWidth;
	int    biHeight;
	unsigned short    biPlanes;
	unsigned short    biBitCount;
	unsigned int   biCompression;
	unsigned int   biSizeImage;
	int    biXPelsPerMeter;
	int    biYPelsPerMeter;
	unsigned int   biClrUsed;
	unsigned int   biClrImportant;
} GBITMAPINFOHEADER;

#pragma pack(pop)

/////////////////////////////////////////////////////////////////////////////
// CL_BMPProvider_Generic construction:

CL_BMPProvider_Generic::CL_BMPProvider_Generic(
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
		read_bmp(_name, datafile);
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

CL_BMPProvider_Generic::~CL_BMPProvider_Generic()
{
	delete[] image;
	image = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CL_BMPProvider_Generic implementation:

void CL_BMPProvider_Generic::read_bmp(
	std::string _name, 
	CL_InputSource *_datafile)
{
	// make sure its valid
	cl_assert(_datafile != NULL);
	
	BMPTOPHEADER top_header;
	GBITMAPINFOHEADER main_header;
	
	//I was using a cl_assert, but how it does an "abort" on linux is bad, it leaves my system in the wrong
	//screen resolution, doesn't clean up etc
	
	if (sizeof(BMPTOPHEADER) != 14 )
	{
		throw CL_Error("read_bmp: The struct isn't being packed right on this platform");
	}


	int read = _datafile->read(&top_header, sizeof(BMPTOPHEADER));
	if (read != sizeof(BMPTOPHEADER)) throw CL_Error("File not big enough to read the BMP header");

	
	SWAP_IF_BIG(top_header.offset);

	if (top_header.type1 != 'B' || top_header.type2 != 'M') //verify that it's a real bitmap file
	{
		throw CL_Error("BMP Loader:  Doesn't appear to be a valid bitmap, missing header");
	}

	
	//read the next part of the header

	read = _datafile->read(&main_header, sizeof(GBITMAPINFOHEADER));
	if (read != sizeof(GBITMAPINFOHEADER)) throw CL_Error("File not big enough to read the BMP header");

	if (main_header.biBitCount <= 4)
	{
		throw CL_Error("BMP loader does not support 1,2 or 4 bit bitmaps yet."); 
	}

	//let's work on big endian too
	SWAP_IF_BIG(main_header.biWidth);
	SWAP_IF_BIG(main_header.biHeight);
	SWAP_IF_BIG(main_header.biCompression);
	SWAP_IF_BIG(main_header.biClrUsed);
	SWAP_IF_BIG(main_header.biBitCount);

	int width = main_header.biWidth;
	int height = main_header.biHeight;
	int pitch = main_header.biWidth * main_header.biBitCount/8;
	int pitch_offset = 0;
	while ((pitch+pitch_offset)%4) {pitch_offset++;} //what's needed to pad it to a 32 byte boundry

	if (height < 0)
	{
		throw CL_Error("BMP loader does not support top-down bitmaps yet"); 
	}

	if (main_header.biCompression != 0)
	{
		throw CL_Error("BMP loader does not support compressed bitmaps yet"); 
	}

	
	if (main_header.biBitCount == 8)
	{
		//it's a paletted image
		int colors = 256;
	
		if (main_header.biClrUsed != 0)
		{
			//looks like they don't use all the colors available
			colors = main_header.biClrUsed;
		}

		//load the palette info.  Note:  Bitmaps are BGR format

		unsigned char r,g,b;

		for (int i=0; i < colors; i++)
		{
			b = _datafile->read_uint8();
			g = _datafile->read_uint8();
			r = _datafile->read_uint8();
			_datafile->read_uint8(); //filler
			palette[i].set_color(r, g, b);
		}

		CL_PixelBuffer_Generic::format.set_type(pixelformat_index);

	} else
	{
		    //it's a high color image of some sort
		CL_PixelBuffer_Generic::format.set_alpha_mask(0);

		if (main_header.biBitCount == 16)
		{
			CL_PixelBuffer_Generic::format.set_blue_mask(0x001f);
			CL_PixelBuffer_Generic::format.set_green_mask(0x03e0);
			CL_PixelBuffer_Generic::format.set_red_mask(0x7c00);

		} else 	if (main_header.biBitCount == 24)
		{
			//why does only 24 bit have this endian stuff?  This looks like a bug to me, will check on OSX first though.. -Seth	

			if (CL_Endian::is_system_big())
			{
				CL_PixelBuffer_Generic::format.set_red_mask(0x0000ff);
				CL_PixelBuffer_Generic::format.set_green_mask(0x00ff00);
				CL_PixelBuffer_Generic::format.set_blue_mask(0xff0000);


			} else
			{
				CL_PixelBuffer_Generic::format.set_blue_mask(0x0000ff);
				CL_PixelBuffer_Generic::format.set_green_mask(0x00ff00);
				CL_PixelBuffer_Generic::format.set_red_mask(0xff0000);
			}
		} else
		{
				//32 bit with alpha
			
				CL_PixelBuffer_Generic::format.set_red_mask(0x00ff0000);
				CL_PixelBuffer_Generic::format.set_green_mask(0x0000ff00);
				CL_PixelBuffer_Generic::format.set_blue_mask(0x000000ff);
				CL_PixelBuffer_Generic::format.set_alpha_mask(0xff000000);
		}
		
	}


		//next is the image data
		image = new unsigned char[pitch*height];
		cl_assert(image != NULL);
		
		//skip the crap in the file
		_datafile->seek(top_header.offset, CL_InputSource::seek_set);


		unsigned char *p = &image[ pitch* (height-1)]; //skip to the last row, bitmaps are written upside down
		for (int y =0; y < height; y++)
		{
			_datafile->read(p, pitch);
			if (pitch_offset)
			{
				_datafile->seek(pitch_offset, CL_InputSource::seek_cur);
			}
			p -= pitch;
		}

	CL_PixelBuffer_Generic::format.set_depth(main_header.biBitCount);
	CL_PixelBuffer_Generic::format.enable_colorkey(false);
	CL_PixelBuffer_Generic::format.set_colorkey(0);
	CL_PixelBuffer_Generic::pitch = pitch;
	CL_PixelBuffer_Generic::width = width;
	CL_PixelBuffer_Generic::height = height;

}
