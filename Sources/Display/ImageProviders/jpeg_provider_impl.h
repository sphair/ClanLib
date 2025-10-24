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

#pragma once

// Seems VC8.0 suddenly defines boolean in its Platform SDK
// Seems VC7.1 also defines that :)
#if _MSC_VER >= 1301
#define HAVE_BOOLEAN
#endif

#ifdef WIN32
#define XMD_H
#endif

#include <cstdio>
extern "C"
{
	#undef FAR
	#include <jpeglib.h>
}

#include "API/Display/ImageProviders/jpeg_provider.h"
#include "API/Display/Image/pixel_format.h"
#include "API/Display/Image/palette.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/IOData/virtual_directory.h"

/// \brief Surface provider that can load JPEG (.jpg) files.
class CL_JPEGProvider_Impl
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a surface provider that can read JPEG¤ files.
	CL_JPEGProvider_Impl(
		const CL_String &name,
		const CL_VirtualDirectory &directory);

	CL_JPEGProvider_Impl(
		CL_IODevice &input);

	~CL_JPEGProvider_Impl();


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the pitch of the image (bytes per line).
	unsigned int get_pitch() const { return pitch; }

	/// \brief Returns the width of the image.
	unsigned int get_width() const { return width; }

	/// \brief Returns the height of the image.
	unsigned int get_height() const { return height; }

	/// \brief Returns the number of subsprites in the image.
	unsigned int get_num_frames() const { return 1; }

	/// \brief Returns the pixelformat used by the image.
	// EPixelFormat get_pixel_format() const { return RGB888; }

/// \}
/// \name Attributes
/// \{

public:
	CL_TextureFormat sized_format;

	int pitch, width, height;

	CL_Palette palette;


/// \}
/// \name Operations
/// \{

public:
	/// \brief Returns the image data. Provider must be locked before pointer is valid.
	void *get_data();

	/// \brief Loads the JPEG data into the pixel buffer.
	void init();

	/// \brief Cleans up.
	void deinit();


/// \}
/// \name Implementation
/// \{

private:
	// Functions for managing the input handling
	static void jpeg_InputSource_src (j_decompress_ptr cinfo, CL_JPEGProvider_Impl* prov);

	static void init_source (j_decompress_ptr cinfo);

	static boolean fill_input_buffer (j_decompress_ptr cinfo);

	static void skip_input_data (j_decompress_ptr cinfo, long num_bytes);

	static void term_source (j_decompress_ptr cinfo);

	static void jpeg_local_error_exit (jpeg_common_struct* cinfo);

	CL_String filename;

	unsigned char *image;

	int bpp;

	int pos;

	typedef struct
	{
		struct jpeg_source_mgr   pub;           /* public fields */
		CL_JPEGProvider_Impl *   jpeg_provider; /* source stream */
		JOCTET *                 buffer;        /* start of buffer */
		boolean                  start_of_file; /* have we gotten any data yet? */
	} InputSource_source_mgr;

	typedef InputSource_source_mgr* InputSource_src_ptr;

	CL_VirtualDirectory directory;

	CL_IODevice input_source;
/// \}
};


