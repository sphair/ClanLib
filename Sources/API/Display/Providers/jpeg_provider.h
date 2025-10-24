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

//! clanDisplay="Surface Providers"
//! header=display.h

#ifndef header_jpeg_provider
#define header_jpeg_provider

#ifdef CL_API_DLL
#ifdef CL_DISPLAY_EXPORT
#define CL_API_DISPLAY __declspec(dllexport)
#else
#define CL_API_DISPLAY __declspec(dllimport)
#endif
#else
#define CL_API_DISPLAY
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "../pixel_buffer.h"
#include <string>

class CL_InputSourceProvider;
class CL_OutputSourceProvider;

//: Surface provider that can load JPEG (.jpg) files.
//- !group=Display/Surface Providers!
//- !header=display.h!
class CL_JPEGProvider : public CL_PixelBuffer
{
//! Construction:
public:
	//: Constructs a surface provider that can read JPEG files.
	//param name: Name of the JPEG file to load.
	//param provider: Input source provider that delivers the JPEG file.
	CL_JPEGProvider(
		const std::string &name, 
		CL_InputSourceProvider *provider = 0);

	//: Save the given PixelBuffer into a JPEG
	//param buffer: The CL_PixelBuffer to save, format doesn't matter its converted if needed
	//param filename: to which the JPEG file is saved
	//param output_provider:
	//param quality: the quality level of the JPEG (0-100), 100 being best quality
	static void save(
		CL_PixelBuffer buffer,
		const std::string &filename,
		CL_OutputSourceProvider *output_provider = 0,
		int quality = 85);
};

#endif
