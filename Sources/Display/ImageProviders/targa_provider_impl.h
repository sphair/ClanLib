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

#include "API/Core/IOData/iodevice.h"
#include "API/Core/IOData/virtual_directory.h"
#include "API/Display/ImageProviders/targa_provider.h"
#include "API/Display/Image/pixel_format.h"
#include "API/Display/Image/palette.h"

/// \brief Surface provider that can load targa (.tga) files.
class CL_TargaProvider_Impl
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a surface provider that can read targa files.
	///
	/// \param datafile = Input source provider that delivers the targa file.
	CL_TargaProvider_Impl(
		CL_IODevice &datafile);

	/// \brief Target Provider Destructor
	virtual ~CL_TargaProvider_Impl();


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
	virtual void *get_data();


/// \}
/// \name Implementation
/// \{

private:
	struct TGA_Header;

	unsigned char *image;

	void read_tga(CL_IODevice &input_source);

	void read_colormapped(CL_IODevice &input_source, const TGA_Header &header);

	void read_rgb(CL_IODevice &input_source, const TGA_Header &header);

	void read_rle_colormapped(CL_IODevice &input_source, const TGA_Header &header);

	void read_rle_rgb(CL_IODevice &input_source, const TGA_Header &header);
/// \}
};


