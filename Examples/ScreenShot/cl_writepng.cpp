/*
 * Save a CL_Surface in PNG format
 * 
 * usage:
 * 		...
 * 		CL_Surface surface( "gfx/image1", &res );
 * 		WritePNG::write_png( &surface , "image.png" )
 * 		...
 * 
 * This uses some code from Greg Roelofs wpng program (included in libpng)
 * hence the following copyright notice.
 */

/*---------------------------------------------------------------------------

   wpng - simple PNG-writing program

  ---------------------------------------------------------------------------

      Copyright (c) 1998-2000 Greg Roelofs.  All rights reserved.

      This software is provided "as is," without warranty of any kind,
      express or implied.  In no event shall the author or contributors
      be held liable for any damages arising in any way from the use of
      this software.

      Permission is granted to anyone to use this software for any purpose,
      including commercial applications, and to alter it and redistribute
      it freely, subject to the following restrictions:

      1. Redistributions of source code must retain the above copyright
         notice, disclaimer, and this list of conditions.
      2. Redistributions in binary form must reproduce the above copyright
         notice, disclaimer, and this list of conditions in the documenta-
         tion and/or other materials provided with the distribution.
      3. All advertising materials mentioning features or use of this
         software must display the following acknowledgment:

            This product includes software developed by Greg Roelofs
            and contributors for the book, "PNG: The Definitive Guide,"
            published by O'Reilly and Associates.

  ---------------------------------------------------------------------------*/


#include <ClanLib/display.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <setjmp.h>     /* for jmpbuf declaration in writepng.h */
#include <time.h>
#include <iostream>

#include "writepng.h"
#include "cl_writepng.h"

mainprog_info WritePNG::wpng_info;

void WritePNG::write_png( CL_PixelBuffer pbuf, std::string path )
{
	pbuf.lock();

	int rc = 0;
	int error = 0;
	
	wpng_info.width = pbuf.get_width();
	wpng_info.height = pbuf.get_height();
	
	wpng_info.sample_depth = 8;
	
	CL_PixelFormat format = pbuf.get_format();
	wpng_info.bpp = format.get_depth();
	
	wpng_info.outfile = NULL;
	wpng_info.image_data = NULL;
	wpng_info.row_pointers = NULL;
	wpng_info.interlaced = false;
	wpng_info.have_bg = false;
	wpng_info.have_time = false;
	wpng_info.have_text = 0;
	wpng_info.gamma = 1.0;
	
    /* open the output file, or register an error and abort */
	if ((wpng_info.outfile = fopen( path.c_str(), "wb") ) == NULL)
	{
		std::cout << "couldn't open file \"" << path.c_str() << "\" for writing" << std::endl;
		++error;
    }
	
	// allocate libpng stuff, initialize transformations, write pre-IDAT data
	
	if ((rc = writepng_init(&wpng_info)) != 0)
	{
		switch (rc)
		{
			case 2:
				fprintf(stderr, "WritePNG: libpng initialization problem (longjmp)\n");
			break;
			case 4:
				fprintf(stderr, "WritePNG: insufficient memory\n");
				break;
			case 11:
				fprintf(stderr, "WritePNG: internal logic error (unexpected PNM type)\n");
				break;
			default:
				fprintf(stderr, "WritePNG: unknown writepng_init() error\n");
				break;
        }
        exit(rc);
    }
	
	std::cout << "Encoding image data...\n";
	
	long j;
	
	wpng_info.image_data = (unsigned char *)malloc( pbuf.get_pitch() );
		
	if (wpng_info.image_data == NULL)
	{
		fprintf(stderr, "WritePNG:  insufficient memory for row data\n");
		writepng_cleanup(&wpng_info);
		cleanup();
		exit(5);
	}
		
	error = 0;
		
	unsigned char *data = (unsigned char*)pbuf.get_data();
		
	for (j = wpng_info.height;  j > 0L;  --j)
	{
		memcpy( wpng_info.image_data, data, pbuf.get_pitch() );
		data += pbuf.get_pitch();
			
		unsigned char* img_data = wpng_info.image_data;
		
		int i=0;
			
		if( pbuf.get_format().get_depth() == 24 )
		{
			for( i=0; i < wpng_info.width; i++ )
			{
				// swap red and blue
				unsigned char tmp = img_data[0];
				img_data[0] = img_data[2];
				img_data[2] = tmp;
				img_data += 3;
			}
		}
		else if( pbuf.get_format().get_depth() == 32 )
		{
			for( i=0; i < wpng_info.width; i++ )
			{
				// cl format: ARGB,  PNG format: BGRA
				unsigned char tmp[4] = {img_data[3], img_data[2], img_data[1], img_data[0]};
			  	img_data[0] = tmp[0];
				img_data[1] = tmp[1];
				img_data[2] = tmp[2];
				img_data[3] = tmp[3];
				img_data += 4;
			}
		}
		else
		{
			std::cout << "WritePNG: unsupported bit depth: " << pbuf.get_format().get_depth() << std::endl;
		}
			
		if( writepng_encode_row(&wpng_info) != 0)
		{
			fprintf(stderr, "WritePNG:  libpng problem (longjmp) while writing row %ld\n",
			wpng_info.height-j);
			++error;
			break;
		}
	}
		
		
	if (error)
	{
		writepng_cleanup(&wpng_info);
		cleanup();
		exit(2);
	}
	if (writepng_encode_finish(&wpng_info) != 0)
	{
		fprintf(stderr, "WritePNG:  error on final libpng call\n");
		writepng_cleanup(&wpng_info);
		cleanup();
		exit(2);
	}
	
	/* OK, we're done (successfully):  clean up all resources and quit */

	pbuf.unlock();

	fprintf(stderr, "Done.\n");
	fflush(stderr);
	
	writepng_cleanup(&wpng_info);
	cleanup();
}


void WritePNG::cleanup()
{
	if (wpng_info.outfile)
	{
		fclose(wpng_info.outfile);
		wpng_info.outfile = NULL;
	}
	
	if (wpng_info.image_data)
	{
		free(wpng_info.image_data);
		wpng_info.image_data = NULL;
	}
}

