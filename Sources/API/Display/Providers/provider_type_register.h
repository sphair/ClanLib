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

#ifndef header_provider_type_register
#define header_provider_type_register

#if _MSC_VER > 1000
#pragma once
#endif

#include "provider_type.h"

//: Class template to register a provider type.
//- !group=Display/Surface Providers!
//- !header=display.h!
template<class ProviderClass>
class CL_ProviderType_Register : public CL_ProviderType
{
//! Construction:
public:
	//: Registers provider type in the CL_ProviderFactory.
	CL_ProviderType_Register(const std::string &type) : CL_ProviderType(type)
	{
	}

//! Operations:
public:
	//: Called to load an image with this provider type.
	virtual CL_PixelBuffer load(
		const std::string &filename,
		CL_InputSourceProvider *input_provider)
	{
		return ProviderClass(filename, input_provider);
	}

	virtual void save(
		CL_PixelBuffer buffer,
		const std::string &filename,
		CL_OutputSourceProvider *output_provider)
	{
		ProviderClass::save(buffer, filename, output_provider);
	}
};

#endif
