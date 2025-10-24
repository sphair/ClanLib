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

#ifndef header_targaprovider_generic
#define header_targaprovider_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Display/Providers/targa_provider.h"
#include "Display/pixel_buffer_generic.h"

#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/inputsource_provider.h"

//: Surface provider that can load targa (.tga) files.
class CL_TargaProvider_Generic : public CL_PixelBuffer_Generic
{
//! Construction:
public:
	//: Constructs a surface provider that can read targa files.
	//param name: Name of the targa file to load.
	//param provider: Input source provider that delivers the targa file.
	CL_TargaProvider_Generic(
		const std::string name,
		CL_InputSourceProvider *provider);

	//: Target Provider Destructor
	virtual ~CL_TargaProvider_Generic();

//! Operations:
public:
	//: Returns the image data. Provider must be locked before pointer is valid.
	virtual void *get_data();

	//: Reference counted lock function. Allocate pixel buffer in this function. 
	virtual void perform_lock() { return; }

	//: Reference counted unlock function. Free pixel buffer in this function.
	virtual void perform_unlock() { return; }

//! Implementation:
private:
	struct TGA_Header;

	unsigned char *image;
							
	void read_tga(CL_InputSource *input_source);

	void read_colormapped(CL_InputSource *input_source, const TGA_Header &header);
	void read_rgb(CL_InputSource *input_source, const TGA_Header &header);
	void read_rle_colormapped(CL_InputSource *input_source, const TGA_Header &header);
	void read_rle_rgb(CL_InputSource *input_source, const TGA_Header &header);
};

#endif
