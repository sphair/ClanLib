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
#define WIN32_LEAN_AND_MEAN
#include "jpeg_provider_generic.h"
#include <setjmp.h>
#ifdef __APPLE__
    #include <libjpeg/jerror.h>
#else
    #include <jerror.h>
#endif
#include "API/Core/System/log.h"
#include "API/Core/System/error.h"
#include "API/Core/IOData/cl_endian.h"

#define INPUT_BUF_SIZE  4096

/////////////////////////////////////////////////////////////////////////////
// CL_JPEGProvider_Generic construction:

CL_JPEGProvider_Generic::CL_JPEGProvider_Generic(
	const std::string &name,
	CL_InputSourceProvider *_provider)
{
	if (_provider == NULL)
	{
		input_provider = CL_InputSourceProvider::create_file_provider(".");
	}
	else
	{
		input_provider = _provider->clone();
	}

	filename = name;
	image = NULL;

	init();
}

CL_JPEGProvider_Generic::~CL_JPEGProvider_Generic()
{
	deinit();
	delete input_provider;
}

/////////////////////////////////////////////////////////////////////////////
// CL_JPEGProvider_Generic attributes:

unsigned int CL_JPEGProvider_Generic::get_red_mask() const
{
	return 0xff0000;
}

unsigned int CL_JPEGProvider_Generic::get_green_mask() const
{
	return 0x00ff00;
}

unsigned int CL_JPEGProvider_Generic::get_blue_mask() const
{
	return 0x0000ff;
}

unsigned int CL_JPEGProvider_Generic::get_alpha_mask() const
{
	return 0x000000;
}

/////////////////////////////////////////////////////////////////////////////
// CL_JPEGProvider_Generic operations:
  
void *CL_JPEGProvider_Generic::get_data()
{
	return image;
}

void CL_JPEGProvider_Generic::init()
{
	struct jpeg_error_mgr jerr;	
	struct jpeg_decompress_struct cinfo;
	//	FILE * infile;		
	JSAMPARRAY buffer;	
	int row_stride;		
	
	input_source = input_provider->open_source(filename);

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_InputSource_src(&cinfo, this);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	row_stride = cinfo.output_width * cinfo.output_components;

	width = cinfo.output_width;
	height = cinfo.output_height;
	pitch = width * 3;

	// FIXME: Where is the allocated memory freed?
	buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 32);

	image = new unsigned char[get_pitch() * cinfo.output_height];

	// RGB Image
	if (cinfo.output_components == 3)
	{
		while (cinfo.output_scanline < cinfo.output_height) 
		{
			jpeg_read_scanlines(&cinfo, buffer, 1);
		
			for(unsigned int i=0; i < get_pitch(); i += 3)
			{
				if( CL_Endian::is_system_big() )
				{
					image[pitch * (cinfo.output_scanline - 1) + i + 0] = buffer[0][i + 0];
					image[pitch * (cinfo.output_scanline - 1) + i + 1] = buffer[0][i + 1];
					image[pitch * (cinfo.output_scanline - 1) + i + 2] = buffer[0][i + 2];
				}
				else
				{
					image[pitch * (cinfo.output_scanline - 1) + i + 0] = buffer[0][i + 2];
					image[pitch * (cinfo.output_scanline - 1) + i + 1] = buffer[0][i + 1];
					image[pitch * (cinfo.output_scanline - 1) + i + 2] = buffer[0][i + 0];
				}
			}
		}
	}
	// Greyscale Image
	else if (cinfo.output_components == 1)
	{
		while (cinfo.output_scanline < cinfo.output_height) 
		{
			jpeg_read_scanlines(&cinfo, buffer, 1);
			
			for(int i=0; i < width; i += 1)
			{
				image[pitch * (cinfo.output_scanline - 1) + 3*i + 0] = buffer[0][i];
				image[pitch * (cinfo.output_scanline - 1) + 3*i + 1] = buffer[0][i];
				image[pitch * (cinfo.output_scanline - 1) + 3*i + 2] = buffer[0][i];
			}			
		}
	}
	else
	{
		throw CL_Error(CL_String::format("CL_JPEGProvider: Unsupported color completion: %1",  cinfo.output_components));
	}
	
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	delete input_source;
	input_source = 0;


	// this could be integrated better, but I'm too tired, so I just hack CL_PixelBuffer
	// support into it. -- mbn 21. feb 2002

	CL_PixelBuffer_Generic::format.enable_colorkey(false);
	CL_PixelBuffer_Generic::format.set_colorkey(0);
	CL_PixelBuffer_Generic::format.set_depth(24);
	CL_PixelBuffer_Generic::format.set_red_mask(get_red_mask());
	CL_PixelBuffer_Generic::format.set_green_mask(get_green_mask());
	CL_PixelBuffer_Generic::format.set_blue_mask(get_blue_mask());
	CL_PixelBuffer_Generic::format.set_alpha_mask(get_alpha_mask());
	CL_PixelBuffer_Generic::pitch = get_pitch();
	CL_PixelBuffer_Generic::width = get_width();
	CL_PixelBuffer_Generic::height = get_height();
}

void CL_JPEGProvider_Generic::deinit()
{
	delete[] image;
	image = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CL_JPEGProvider_Generic implementation:

void CL_JPEGProvider_Generic::jpeg_InputSource_src (j_decompress_ptr cinfo, CL_JPEGProvider_Generic* provider)
{
	InputSource_src_ptr src;
	
	if (cinfo->src == NULL) /* first time for this JPEG object? */
	{
		cinfo->src = (struct jpeg_source_mgr*)
			(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
						    sizeof(InputSource_source_mgr));
		src = (InputSource_src_ptr) cinfo->src;
		src->buffer = (JOCTET *)
			(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
						    INPUT_BUF_SIZE * sizeof(JOCTET));
	}

	src = (InputSource_src_ptr) cinfo->src;
	src->pub.init_source = CL_JPEGProvider_Generic::init_source;
	src->pub.fill_input_buffer = CL_JPEGProvider_Generic::fill_input_buffer;
	src->pub.skip_input_data = CL_JPEGProvider_Generic::skip_input_data;
	src->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
	src->pub.term_source = CL_JPEGProvider_Generic::term_source;
	src->jpeg_provider = provider;
	src->pub.bytes_in_buffer = 0; /* forces fill_input_buffer on first read */
	src->pub.next_input_byte = NULL; /* until buffer loaded */
}

void CL_JPEGProvider_Generic::init_source (j_decompress_ptr cinfo)
{
	InputSource_src_ptr src = (InputSource_src_ptr) cinfo->src;
	src->start_of_file = TRUE;	
}

boolean CL_JPEGProvider_Generic::fill_input_buffer (j_decompress_ptr cinfo)
{
	InputSource_src_ptr src = (InputSource_src_ptr) cinfo->src;
	size_t nbytes;

	nbytes = src->jpeg_provider->input_source->read (src->buffer, INPUT_BUF_SIZE);
	
	if (nbytes <= 0) 
	{
		if (src->start_of_file)	/* Treat empty input file as fatal error */
			ERREXIT(cinfo, JERR_INPUT_EMPTY);
		WARNMS(cinfo, JWRN_JPEG_EOF);
		/* Insert a fake EOI marker */
		src->buffer[0] = (JOCTET) 0xFF;
		src->buffer[1] = (JOCTET) JPEG_EOI;
		nbytes = 2;
	}
	src->pub.next_input_byte = src->buffer;
	src->pub.bytes_in_buffer = nbytes;
	src->start_of_file = FALSE;
	
	return TRUE;
}

void CL_JPEGProvider_Generic::skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
	InputSource_src_ptr src = (InputSource_src_ptr) cinfo->src;
	
	/* Just a dumb implementation for now.  Could use fseek() except
	 * it doesn't work on pipes.  Not clear that being smart is worth
	 * any trouble anyway --- large skips are infrequent.
	 */
	if (num_bytes > 0)
	{
		while (num_bytes > (long) src->pub.bytes_in_buffer)
		{
			num_bytes -= (long) src->pub.bytes_in_buffer;
			(void) fill_input_buffer(cinfo);
			/* note we assume that fill_input_buffer will never return FALSE,
			 * so suspension need not be handled.
			 */
		}
		src->pub.next_input_byte += (size_t) num_bytes;
		src->pub.bytes_in_buffer -= (size_t) num_bytes;
	}
}

void CL_JPEGProvider_Generic::term_source (j_decompress_ptr cinfo)
{
	// do nothing
}
