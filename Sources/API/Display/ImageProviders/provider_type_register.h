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
**    Magnus Norddahl
**    (if your name is missing here, please add it)
*/

/// \addtogroup clanDisplay_Image_Providers clanDisplay Image Providers
/// \{

#pragma once

#include "../api_display.h"
#include "../../Core/IOData/virtual_directory.h"
#include "provider_type.h"

/// \brief Class template to register a provider type.
///
/// \xmlonly !group=Display/Image Providers! !header=display.h! \endxmlonly
template<class ProviderClass>
class CL_ProviderType_Register : public CL_ImageProviderType
{
/// \name Construction
/// \{

public:
	/// \brief Registers provider type in the CL_ProviderFactory.
	CL_ProviderType_Register(const CL_String &type) : CL_ImageProviderType(type)
	{
	}

/// \}
/// \name Operations
/// \{

public:
	/// \brief Called to load an image with this provider type.
	virtual CL_PixelBuffer load(
		const CL_String &filename,
		const CL_VirtualDirectory &directory)
	{
		return ProviderClass::load(filename, directory);
	}

	virtual CL_PixelBuffer load(
		CL_IODevice &file)
	{
		return ProviderClass::load(file);
	}

	virtual void save(
		CL_PixelBuffer buffer,
		const CL_String &filename,
		CL_VirtualDirectory &directory)
	{
		ProviderClass::save(buffer, filename, directory);
	}

	virtual void save(
		CL_PixelBuffer buffer,
		CL_IODevice &file)
	{
		ProviderClass::save(buffer, file);
	}

/// \}
};

/// \}
