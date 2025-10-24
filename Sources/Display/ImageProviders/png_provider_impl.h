/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

#include "API/Display/ImageProviders/png_provider.h"
#include "API/Display/Image/pixel_format.h"
#include "API/Display/Image/palette.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/IOData/virtual_directory.h"
#include <png.h>

class CL_PNGProvider_Impl
{
/// \name Construction
/// \{

public:
	CL_PNGProvider_Impl(
		const CL_String &name,
		const CL_VirtualDirectory &directory);

	CL_PNGProvider_Impl(CL_IODevice &input);

	virtual ~CL_PNGProvider_Impl();


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the CL_IODevice.
	/// \brief (used by libpng-callback CL_PNGProvider::pngread_file() )
	CL_IODevice &get_input_source() { return input_source; }

	/// \brief Returns the pitch of the image (bytes per line).
	unsigned int get_pitch() const { return pitch; }

	/// \brief Returns the width of the image.
	unsigned int get_width() const { return width; }

	/// \brief Returns the height of the image.
	unsigned int get_height() const { return height; }

	/// \brief Returns the number of subsprites in the image.
	unsigned int get_num_frames() const { return 1; }

	/// \brief Returns the transparency color used.
	unsigned int get_src_colorkey() const { return trans_col; }

	/// \brief Returns whether a source colorkey is used.
	bool uses_src_colorkey() const { return m_uses_src_colorkey; }

	CL_TextureFormat sized_format;

	int pitch, width, height;

	CL_Palette palette;


/// \}
/// \name Operations
/// \{

public:
	/// \brief Returns the image data. Provider must be locked before pointer is valid.
	virtual void *get_data();

	/// \brief Loads the PNG into the pixel buffer.
	virtual void init();

	/// \brief Cleans up.
	virtual void deinit();


/// \}
/// \name Implementation
/// \{

private:
	/// \brief Callback used by libpng to retrieve the filedata.
	/** <p>Calls get_input_source().read(data, length)).</p>*/
	static void pngread_file(png_structp png_ptr, png_bytep data, png_size_t length);

	static void pngread_error_handler(png_structp png_ptr, png_const_charp msg);

	void read_data();

	void read_data_rgb();

	void read_data_rgba();

	void read_data_grayscale();

	void read_data_grayscale_alpha();

	void read_data_palette();

	CL_String filename;

	bool m_uses_src_colorkey;

	unsigned char *image;

	int trans_col;

	int color_type;

	CL_VirtualDirectory directory;

	CL_IODevice input_source;

	png_structp png_ptr;

	png_infop info_ptr;

	png_infop end_info;

/// \}
};



