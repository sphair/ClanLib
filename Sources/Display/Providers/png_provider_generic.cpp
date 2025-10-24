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

#include "png_provider_generic.h"
#include "API/Core/System/error.h"
#include "API/Core/IOData/cl_endian.h"
#include <utility>

/*
	Known Bugs:

	- this provider might not work with 16bit PNG's 
	- this provider might not work with some grayscale PNG's (don't remember which one)
*/

/////////////////////////////////////////////////////////////////////////////
// CL_PNGProvider_Generic construction:

CL_PNGProvider_Generic::CL_PNGProvider_Generic(
	const std::string &name,
	CL_InputSourceProvider *_provider)
{
	if (_provider == NULL)
		provider = CL_InputSourceProvider::create_file_provider(".");
	else
		provider = _provider->clone();

	trans_col = -1;
	m_uses_src_colorkey = false;

	filename = name;
	image = NULL;

	init();
}

CL_PNGProvider_Generic::~CL_PNGProvider_Generic()
{
	deinit();
	delete provider;
}

/////////////////////////////////////////////////////////////////////////////
// CL_PNGProvider_Generic attributes:

bool CL_PNGProvider_Generic::is_indexed() const
{
	return (format.get_depth()==8);
}

/////////////////////////////////////////////////////////////////////////////
// CL_PNGProvider_Generic operations:

void* CL_PNGProvider_Generic::get_data()
{
	return image;
}

void CL_PNGProvider_Generic::init()
{
	//setting up PNGLIB stuff
	png_ptr = png_create_read_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	
	if (!png_ptr) 
		throw CL_Error ("CL_PNGProvider_Generic: png_create_read_struct() failed");
	
	info_ptr = png_create_info_struct(png_ptr);

	if (!info_ptr)
	{
		png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
		throw CL_Error ("CL_PNGProvider_Generic: png_create_info_struct() failed");
	}

	end_info = png_create_info_struct(png_ptr);

	if (!end_info)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
		cl_assert(false);
	}   
	if (setjmp(png_ptr->jmpbuf))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		cl_assert(false);
	}  
	
	cl_assert(provider != NULL);
	input_source = provider->open_source(filename);
	cl_assert(input_source!=NULL);

	// tell libpng form whom it get the fileData
	png_set_read_fn(png_ptr, this, &CL_PNGProvider_Generic::pngread_file);

	// reading the header infos and actually read data ...
	read_data();

	// remove our data_provider from libpng
	png_set_read_fn(png_ptr,NULL,NULL);

	// free memory ...
	png_destroy_read_struct(&png_ptr, &info_ptr,&end_info);

	delete input_source;


	// this could be integrated better, but I'm too tired, so I just hack CL_PixelBuffer
	// support into it. -- mbn 21. feb 2002

	CL_PixelBuffer_Generic::format.enable_colorkey(uses_src_colorkey());
	CL_PixelBuffer_Generic::format.set_colorkey(get_src_colorkey());
	CL_PixelBuffer_Generic::pitch = get_pitch();
	CL_PixelBuffer_Generic::width = get_width();
	CL_PixelBuffer_Generic::height = get_height();
	if (is_indexed()) CL_PixelBuffer_Generic::format.set_type(pixelformat_index);
}

void CL_PNGProvider_Generic::deinit()
{
	delete[] image;
	image   =  NULL;
}
  
/////////////////////////////////////////////////////////////////////////////
// CL_PNGProvider_Generic implementation:

void CL_PNGProvider_Generic::pngread_file(
	png_structp png_ptr,
	png_bytep data, 
	png_size_t length)
{
	// since this method is static, we need to know who we are ...
	CL_PNGProvider_Generic *instance =  (CL_PNGProvider_Generic *) png_get_io_ptr(png_ptr);
	// no error-checking here ....
	unsigned int read_length = instance->get_input_source ()->read(data, length);
	cl_assert (read_length == length);
}

void CL_PNGProvider_Generic::read_data()
{
	// initial fileinfo
	png_read_info(png_ptr, info_ptr); 
	
	// reduce 16bit/channel to 8Bit/channel
	png_set_strip_16(png_ptr);  
	
	// reread infostruct to reflect the made settings
	png_read_update_info(png_ptr, info_ptr); 

	width  = png_get_image_width(png_ptr,info_ptr);
	height = png_get_image_height(png_ptr,info_ptr);
	color_type = png_get_color_type(png_ptr,info_ptr);

	switch (color_type)
	{
	case PNG_COLOR_TYPE_GRAY:
		read_data_grayscale ();
		break;
	case PNG_COLOR_TYPE_GRAY_ALPHA:
		read_data_grayscale_alpha ();
		break;
	case PNG_COLOR_TYPE_PALETTE:
		read_data_palette ();
		break;
	case PNG_COLOR_TYPE_RGB:
		read_data_rgb ();
		break;

	case PNG_COLOR_TYPE_RGB_ALPHA:
		read_data_rgba ();
		break;
	default:
		throw CL_Error ("CL_PNGProvider_Generic: Unsupported PNG format!");
		break;
	}
}

void CL_PNGProvider_Generic::read_data_rgb()
{
	format.set_type(pixelformat_rgba);
	format.set_red_mask(0xff0000);
	format.set_green_mask(0x00ff00);
	format.set_blue_mask(0x0000ff);
	format.set_alpha_mask(0x000000);
	format.set_depth(24);

	pitch = png_get_rowbytes(png_ptr, info_ptr);

	// This is deleted in the unlock () call
	image = new unsigned char[pitch * height];

	// setup pointers to each row in our target image
	png_bytep* row_pointers = new png_bytep[height];
	{
		for (int y = 0; y < height; y++)
			row_pointers[y] = image + (pitch * y);
	}
	png_read_image(png_ptr, row_pointers);
	delete[] row_pointers;

	// swap the colors in the right order
	if (!CL_Endian::is_system_big())
	{
		for(int y = 0; y < height; ++y)
		{
			unsigned char * line = &image[y * pitch];
			for (int i = 0; i < width * 3; i += 3)
				std::swap(line[i + 0], line[i + 2]);
		}
	}
}

void CL_PNGProvider_Generic::read_data_rgba()
{
	format.set_type(pixelformat_rgba);
	format.set_red_mask  (0xff000000);
	format.set_green_mask(0x00ff0000);
	format.set_blue_mask (0x0000ff00);
	format.set_alpha_mask(0x000000ff);

	format.set_depth(32);

	pitch = png_get_rowbytes(png_ptr, info_ptr);

	// This is deleted in the unlock () call
	image = new unsigned char[pitch * height];

	// setup pointers to each row in our target image
	png_bytep* row_pointers = new png_bytep[height];
	{
		for (int y = 0; y < height; y++)
			row_pointers[y] = image + (pitch * y);
	}
	png_read_image(png_ptr, row_pointers);
	delete[] row_pointers;

	if (!CL_Endian::is_system_big())
	{
		for(int y = 0; y < height; ++y)
		{
			unsigned char * line = &image[y * pitch];
			for (int i = 0; i < width * 4; i += 4)
			{
				std::swap(line[i + 0], line[i + 3]);
				std::swap(line[i + 1], line[i + 2]);
			}
		}
	}
}

void CL_PNGProvider_Generic::read_data_grayscale()
{
	format.set_type(pixelformat_rgba);
	format.set_red_mask(0xff000000);
	format.set_green_mask(0x00ff0000);
	format.set_blue_mask(0x0000ff00);
	format.set_alpha_mask(0x000000ff);
	format.set_depth(32);

	int bit_depth = png_get_bit_depth(png_ptr, info_ptr);

	// We expand the grayscale values if necessary, so we always
	// get 8bits per pixel
	if (bit_depth < 8) png_set_expand (png_ptr);

	int rowbytes  = png_get_rowbytes (png_ptr, info_ptr);
	pitch = rowbytes * 4;

	// Allocating the temporary buffer and fill it
	unsigned char* tmp_image = new unsigned char[height * rowbytes];
	png_bytep* row_pointers  = new png_bytep[height];
	{
		for (int y = 0; y < height; y++)
			row_pointers[y] = tmp_image + (rowbytes * y);
	}
	png_read_image(png_ptr, row_pointers);
	delete[] row_pointers;

	// Reading and setting up the transparent colors from the image
	int num_trans = 0;
	png_color_16p trans_values;
	png_bytep trans;
	png_get_tRNS(png_ptr, info_ptr, &trans, &num_trans, &trans_values);
	
	if (num_trans > 0)
	{
		if (trans)
		{
			// This is a Pallette image
			trans_col = trans[0];
		}
		else
		{
			// Non-pallete image
			trans_col = trans_values[0].gray;
		}
	}

	image = new unsigned char[height * pitch];
	// Coverting the data in the tmp buffer to our final data
	{
		for (int i = 0; i < rowbytes * height; i++)
		{
			if (!CL_Endian::is_system_big())
			{
				image[4*i + 0] = tmp_image[i] == trans_col ? 0 : 255;
				image[4*i + 3] = tmp_image[i];
			}
			else
			{
				image[4*i + 3] = tmp_image[i] == trans_col ? 0 : 255;
				image[4*i + 0] = tmp_image[i];
			}
			image[4*i + 1] = tmp_image[i];
			image[4*i + 2] = tmp_image[i];
		}
	}
	delete[] tmp_image;
}

void CL_PNGProvider_Generic::read_data_grayscale_alpha()
{
	format.set_type(pixelformat_rgba);
	format.set_red_mask(0xff000000);
	format.set_green_mask(0x00ff0000);
	format.set_blue_mask(0x0000ff00);
	format.set_alpha_mask(0x000000ff);
	format.set_depth(32);

	pitch = width * 4;

	int bit_depth = png_get_bit_depth(png_ptr,info_ptr);
	int rowbytes  = png_get_rowbytes(png_ptr, info_ptr);

	// We expand the grayscale values if necessare, so we always
	// get 8bits per pixel
	if (bit_depth < 8) png_set_expand (png_ptr);

	// Allocating the temporary buffer
	unsigned char* tmp_image = new unsigned char[height * rowbytes];
	png_bytep* row_pointers = new png_bytep[height];
	{
		for (int y = 0; y < height; y++)
			row_pointers[y] = tmp_image + (rowbytes * y);
	}
 	png_read_image(png_ptr, row_pointers);
	delete[] row_pointers;

	// Creating the final image out of tmp_image
	image = new unsigned char[pitch * height];
	for (int i = 0; i < rowbytes * height; i += 2)
	{
		if (!CL_Endian::is_system_big())
		{
			image[2*i + 0] = tmp_image[i + 1];
			image[2*i + 3] = tmp_image[i + 0];
		}
		else
		{
			image[2*i + 0] = tmp_image[i + 0];
			image[2*i + 3] = tmp_image[i + 1];
		}

		image[2*i + 1] = tmp_image[i + 0];
		image[2*i + 2] = tmp_image[i + 0];
	}

	delete[] tmp_image;
}

void CL_PNGProvider_Generic::read_data_palette()
{
	
	format.set_type(pixelformat_index);
	format.set_red_mask(0x00000000);
	format.set_green_mask(0x00000000);
	format.set_blue_mask(0x00000000);
	format.set_alpha_mask(0x00000000);
	format.set_depth(8);

	int bit_depth = png_get_bit_depth(png_ptr,info_ptr);
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	unsigned char* tmp_image = new unsigned char[height * rowbytes];
	// Allocating the temporary buffer (will be deleted some
	// screens below
	png_bytep* row_pointers = new png_bytep[height];
	{
		for (int y = 0; y < height; y++)
			row_pointers[y] = tmp_image + (rowbytes * y);
	}
	png_read_image(png_ptr, row_pointers);
	
	if (bit_depth == 8)
	{
		// We don't need to convert the data, so we can use
		// what we got
		image = tmp_image;
		pitch = rowbytes;
	}
	else
	{
		// We need to convert the data
		pitch = width;

		int y,x;

		switch (bit_depth)
		{
		case 1:
			image = new unsigned char[height * rowbytes * 8];
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < rowbytes; x++)
				{
					image[y*pitch + 8*x + 0] = row_pointers[y][x] >> 7;
					image[y*pitch + 8*x + 1] = row_pointers[y][x] >> 6 & 0x1;
					image[y*pitch + 8*x + 2] = row_pointers[y][x] >> 5 & 0x1;
					image[y*pitch + 8*x + 3] = row_pointers[y][x] >> 4 & 0x1;
					image[y*pitch + 8*x + 4] = row_pointers[y][x] >> 3 & 0x1;
					image[y*pitch + 8*x + 5] = row_pointers[y][x] >> 2 & 0x1;
					image[y*pitch + 8*x + 6] = row_pointers[y][x] >> 1 & 0x1;
					image[y*pitch + 8*x + 7] = row_pointers[y][x] & 0x1;
				}
			}
			break;
		case 2:
			image = new unsigned char[height * rowbytes * 4];
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < rowbytes; x++)
				{
					image[y*pitch + 4*x + 0] = row_pointers[y][x] >> 6;
					image[y*pitch + 4*x + 1] = row_pointers[y][x] >> 4 & 0x3;
					image[y*pitch + 4*x + 2] = row_pointers[y][x] >> 2 & 0x3;
					image[y*pitch + 4*x + 3] = row_pointers[y][x] & 0x3;
				}
			}
			break;
		case 4:
			image = new unsigned char[height * rowbytes * 2];
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < rowbytes; x++)
				{
					image[y*pitch + 2*x + 0] = row_pointers[y][x] >> 4;
					image[y*pitch + 2*x + 1] = row_pointers[y][x] & 0x0f;
				}
			}
			break;
		default:
			throw CL_Error ("CL_PNGProvider_Generic: Unhandled bit depth");
		}
		delete[] tmp_image;
	}
	delete[] row_pointers;

	// Read the png palette and create the CL_Palette 
	int num_colors = 256;
	png_colorp png_palette;
	png_get_PLTE(png_ptr, info_ptr, &png_palette, &num_colors);
	if (num_colors > 256)
		num_colors = 256; // clanlib currently only support palette sizes of max 256.
		
	{
		for (int k = 0; k < num_colors; k++)
			palette[k].set_color(png_palette[k].red, png_palette[k].green, png_palette[k].blue);
	}

	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
	{
		// Reading and setting up the transparent colors from the image
		int num_trans = 0;
		png_color_16p trans_values;
		png_bytep trans;

		png_get_tRNS(png_ptr, info_ptr, &trans, &num_trans, &trans_values);

		//Trans information exists for each color in the palette, it doesn't mean
		//that color is actually transparent.   We need to check its alpha for that data.
		//Ideally, we should probably detect if more than one is transparent and convert it to a full
		//alpha, but that seems like overkill for now. -mrfun

		for (int i=0; i < num_trans; i++)
		{
			if (trans[i] == 0)
			{
				trans_col = i;
				m_uses_src_colorkey = true;
				break; //we only support one color
			}
		}
		
	}

}
