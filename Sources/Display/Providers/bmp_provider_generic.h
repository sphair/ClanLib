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
**    Seth A. Robinson
**    (if your name is missing here, please add it)
*/

#ifndef header_bmp_provider_generic
#define header_bmp_provider_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Display/Providers/bmp_provider.h"
#include "Display/pixel_buffer_generic.h"
#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/inputsource_provider.h"

//: Surface provider that can load BMP files.
class CL_BMPProvider_Generic : public CL_PixelBuffer_Generic
{
//! Construction:
public:
	//: Constructs a surface provider that can read bmp files.
	//param name: Name of the bmp file to load.
	//param provider: Input source provider that delivers the bmp file.
	CL_BMPProvider_Generic(
		std::string name,
		CL_InputSourceProvider *provider);

	//: BMP Provider destructor
	virtual ~CL_BMPProvider_Generic();

//! Operations:
public:
	//: Returns the image data. Provider must be locked before pointer is valid.
	virtual void *get_data() { return image; }

	//: Reference counted lock function. Allocate pixel buffer in this function. 
	virtual void perform_lock() { return; }

	//: Reference counted unlock function. Free pixel buffer in this function.
	virtual void perform_unlock() { return; }

//! Implementation:
private:
	//: Loads the bmp
	void read_bmp(
		std::string _name, 
		CL_InputSource *_datafile);

	//: Image data.
	unsigned char *image;
};

#endif
