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

#ifndef header_jpeg_provider_generic
#define header_jpeg_provider_generic

#if _MSC_VER > 1000
#pragma once
#endif

#if _MSC_VER >= 1300
#define HAVE_BOOLEAN
#endif

#ifdef WIN32
#define XMD_H
#endif

#include <stdio.h>
extern "C"
{
	#undef FAR

	#ifdef __APPLE__
	#include <libjpeg/jpeglib.h>
	#else
	#include <jpeglib.h>
	#endif
}

#include "API/Display/Providers/jpeg_provider.h"
#include "Display/pixel_buffer_generic.h"

#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/inputsource_provider.h"

class CL_InputSourceProvider;

//: Surface provider that can load JPEG (.jpg) files.
class CL_JPEGProvider_Generic : public CL_PixelBuffer_Generic
{
//! Construction:
public:
	//: Constructs a surface provider that can read JPEG¤ files.
	CL_JPEGProvider_Generic(
		const std::string &name, 
		CL_InputSourceProvider *provider);

	~CL_JPEGProvider_Generic();

//! Attributes:
public:
	//: Returns the pitch of the image (bytes per line).
	unsigned int get_pitch() const { return pitch; }

	//: Returns the width of the image.
	unsigned int get_width() const { return width; }

	//: Returns the height of the image.
	unsigned int get_height() const { return height; }
	
	//: Returns the number of subsprites in the image.
	unsigned int get_num_frames() const { return 1; }

	//: Returns the pixelformat used by the image.
	// EPixelFormat get_pixel_format() const { return RGB888; }

	//: Returns the red color mask used by the target.
	unsigned int get_red_mask() const;

	//: Returns the green color mask by the target.
	unsigned int get_green_mask() const;

	//: Returns the blue color mask by the target.
	unsigned int get_blue_mask() const;

	//: Returns the alpha mask by the target.
	unsigned int get_alpha_mask() const;

//! Operations:
public:
	//: Returns the image data. Provider must be locked before pointer is valid.
	void *get_data();

	//: Reference counted lock function. Allocate pixel buffer in this function. 
	virtual void perform_lock() { return; }

	//: Reference counted unlock function. Free pixel buffer in this function.
	virtual void perform_unlock() { return; }

	//: Loads the JPEG data into the pixel buffer.
	void init();

	//: Cleans up.
	void deinit();

//! Implementation:
private:
	// Functions for managing the input handling
	static void jpeg_InputSource_src (j_decompress_ptr cinfo, CL_JPEGProvider_Generic* prov); 

	static void init_source (j_decompress_ptr cinfo);

	static boolean fill_input_buffer (j_decompress_ptr cinfo);

	static void skip_input_data (j_decompress_ptr cinfo, long num_bytes);

	static void term_source (j_decompress_ptr cinfo);

	std::string filename;

	unsigned char *image;

	int pitch;

	int width, height;

	int bpp;

	int pos;

	typedef struct
	{
		struct jpeg_source_mgr   pub;           /* public fields */
		CL_JPEGProvider_Generic* jpeg_provider; /* source stream */
		JOCTET *                 buffer;        /* start of buffer */
		boolean                  start_of_file; /* have we gotten any data yet? */
	} InputSource_source_mgr;
	
	typedef InputSource_source_mgr* InputSource_src_ptr;

	CL_InputSourceProvider* input_provider;

	CL_InputSource* input_source;
};

#endif
