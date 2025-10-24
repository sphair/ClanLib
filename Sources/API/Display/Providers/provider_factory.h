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

#ifndef header_provider_factory
#define header_provider_factory

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

#include <map>
#include <string>

class CL_PixelBuffer;
class CL_InputSourceProvider;
class CL_OutputSourceProvider;
class CL_ProviderType;

//: Provider factory
//- !group=Display/Surface Providers!
//- !header=display.h!
class CL_API_DISPLAY CL_ProviderFactory
{
//! Static Attributes:
public:
	//: Map of the class factories for each provider type.
	static std::map<std::string, CL_ProviderType *> types;

//! Operations:
public:
	//: Loads an image file from 'filename', using the provider
	//: identified by 'type'.
	//- <p>If the type is an empty string, it uses the extension of
	//- the filename to determine what type it is.</p>
	//- <p>The input provider will be used as a the source for filename,
	//- if specified.</p>
	static CL_PixelBuffer load(
		const std::string &filename,
		const std::string &type = "",
		CL_InputSourceProvider *input_provider = 0);

	//: Saves the given CL_PixelBuffer to the file given by 'filename'.
	//- <p>If the type is an empty string, it uses the extension of the
	//- filename to determine what type it is </p>
	static void save(
		CL_PixelBuffer buffer,
		const std::string &filename,
		const std::string &type = "",
		CL_OutputSourceProvider *output_provider = 0);
};

#endif
