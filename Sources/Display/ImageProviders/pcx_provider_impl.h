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
**    Magnus Norddahl
**    (if your name is missing here, please add it)
*/

#pragma once

#include "API/Display/ImageProviders/pcx_provider.h"
#include "API/Display/Image/pixel_format.h"
#include "API/Display/Image/palette.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/IOData/virtual_directory.h"

/// \brief Image provider that can load PCX files.
class CL_PCXProvider_Impl
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a image provider that can read pcx files.
	///
	/// \param datafile = Input source provider that delivers the pcx file.
	CL_PCXProvider_Impl(
		CL_IODevice &datafile);

	/// \brief PCX Provider destructor
	virtual ~CL_PCXProvider_Impl();


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
	virtual void *get_data() { return image; }


/// \}
/// \name Implementation
/// \{

private:
	/// \brief Loads the pcx
	void read_pcx(
		CL_IODevice &_datafile);

	/// \brief Image data.
	unsigned char *image;
/// \}
};


