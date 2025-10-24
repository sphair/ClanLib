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

#ifndef header_provider_type
#define header_provider_type

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

#include <string>
#include "../pixel_buffer.h"
#include "../../Core/System/error.h"

class CL_PixelBuffer;
class CL_InputSourceProvider;
class CL_OutputSourceProvider;

//: Provider type
//- !group=Display/Surface Providers!
//- !header=display.h!
class CL_API_DISPLAY CL_ProviderType
{
//! Construction:
public:
	//: Registers a provider type in the CL_ProviderFactory.
	CL_ProviderType(const std::string &type);

	//: Unregisters a provider type in the CL_ProviderFactory.
	virtual ~CL_ProviderType();

//! Operations:
public:
	//: Called to load an image with this provider type.
	virtual CL_PixelBuffer load(
		const std::string &filename,
		CL_InputSourceProvider *input_provider)=0;

	//: Called to save a given PixelBuffer to a file
	virtual void save(
		CL_PixelBuffer buffer,
		const std::string &filename,
		CL_OutputSourceProvider *output_provider)=0;

//! Implementation:
private:
};

#endif
