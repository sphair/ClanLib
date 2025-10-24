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

#ifndef header_png_provider_generic
#define header_png_provider_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Display/Providers/png_provider.h"
#include "Display/pixel_buffer_generic.h"

#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/inputsource_provider.h"

#ifdef __APPLE__
	#include <libpng/png.h>
#else
	#include <png.h>
#endif

class CL_PNGProvider_Generic : public CL_PixelBuffer_Generic
{
//! Construction:
public:
	CL_PNGProvider_Generic(
		const std::string &name, 
		CL_InputSourceProvider *provider = 0);

	~CL_PNGProvider_Generic();

//! Attributes:
public:
	//: returns Pointer to CL_InputSource
	//: (used by libpng-callback CL_PNGProvider::pngread_file() )
	CL_InputSource *get_input_source() { return input_source; }

	//: Returns the pitch of the image (bytes per line).
	unsigned int get_pitch() const { return pitch; }

	//: Returns the width of the image.
	unsigned int get_width() const { return width; }

	//: Returns the height of the image.
	unsigned int get_height() const { return height; }
	
	//: Returns the number of subsprites in the image.
	unsigned int get_num_frames() const { return 1; }

	//: Returns the transparency color used.
	unsigned int get_src_colorkey() const { return trans_col; }

	//: Returns whether a source colorkey is used.
	bool uses_src_colorkey() const { return m_uses_src_colorkey; }

	//: Returns whether the target uses an indexed color mode or not.
	bool is_indexed() const;
	
//! Operations:
public:
	//: Returns the image data. Provider must be locked before pointer is valid.
	virtual void *get_data();

	//: Reference counted lock function. Allocate pixel buffer in this function. 
	virtual void perform_lock() { return; }

	//: Reference counted unlock function. Free pixel buffer in this function.
	virtual void perform_unlock() { return; }

	//: Loads the PNG into the pixel buffer.
	virtual void init();

	//: Cleans up.
	virtual void deinit();
	
//! Implementation:
private:
	//: Callback used by libpng to retrieve the filedata. 
	//: (calls get_input_source()->read_uchar8())
	static void pngread_file(png_structp png_ptr, png_bytep data, png_size_t length);

	void read_data();

	void read_data_rgb();

	void read_data_rgba();

	void read_data_grayscale();

	void read_data_grayscale_alpha();

	void read_data_palette();

	std::string filename;

	bool m_uses_src_colorkey;
	
	unsigned char *image;
	int pitch;
	int no_sprs;
	int width;
	int height;

	int trans_col;
	int color_type;

	CL_InputSourceProvider *provider;
	CL_InputSource *input_source;

	//PNGlib stuff:
	png_structp png_ptr;
	png_infop info_ptr;
	png_infop end_info;
};

#endif
