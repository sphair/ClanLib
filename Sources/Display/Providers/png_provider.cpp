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
#include <errno.h>
#include <stdio.h>
#include <iostream>
#include "API/Core/System/error.h"
#include "API/Display/Providers/png_provider.h"
#include "png_provider_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_PNGProvider construction:

CL_PNGProvider::CL_PNGProvider(
	const std::string &name,
	CL_InputSourceProvider *provider)
: CL_PixelBuffer(
	new CL_PNGProvider_Generic(
		name,
		provider))
{
}

void
CL_PNGProvider::save(
	CL_PixelBuffer buffer,
	const std::string &filename,
	CL_OutputSourceProvider *output_provider)
{
	if (output_provider)
		throw CL_Error("Saving to OutputSource currently not supported");

	buffer.lock();
	if (buffer.get_format() != CL_PixelFormat::abgr8888)
	{
		CL_PixelBuffer newbuf(buffer.get_width(), buffer.get_height(), 
									 buffer.get_width()*4, CL_PixelFormat::abgr8888);
		buffer.unlock();
		buffer.convert(newbuf);
		buffer = newbuf;
		buffer.lock();
	}

	FILE* fp;
	fp = fopen(filename.c_str (), "wb");
	if (fp == NULL)
      throw CL_Error(strerror(errno));
	
	png_structp png_ptr;
	png_infop info_ptr;

	png_ptr  = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	info_ptr = png_create_info_struct(png_ptr);

	png_init_io(png_ptr, fp);

	png_set_IHDR(png_ptr, info_ptr, 
					 buffer.get_width(), buffer.get_height(), 8 /* bitdepth */,
					 PNG_COLOR_TYPE_RGBA,
					 PNG_INTERLACE_NONE, 
					 PNG_COMPRESSION_TYPE_BASE, 
					 PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);

	png_uint_32 height = buffer.get_height();
	png_uint_32 row_bytes = buffer.get_width()*4;

	png_byte* image = new png_byte[height * row_bytes];
	png_bytep* row_pointers = new png_bytep[height];

	// fill the image with data
	for (int i = 0; i < buffer.get_width()*buffer.get_height()*4; ++i)
      image[i] = static_cast<unsigned char*>(buffer.get_data())[i];

	// generate row pointers
	for (unsigned int k = 0; k < height; k++)
      row_pointers[k] = image + (k * row_bytes);

	png_write_image(png_ptr, row_pointers);

	png_write_end(png_ptr, info_ptr);

	png_destroy_write_struct( &png_ptr, &info_ptr );

	delete[] image;
	delete[] row_pointers;

	fclose(fp);

	buffer.unlock();	
}

/* EOF */
