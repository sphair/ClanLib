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
**    Mark Page
**    (if your name is missing here, please add it)
*/

/// \addtogroup clanDisplay_Image_Providers clanDisplay Image Providers
/// \{

#pragma once

#include "../api_display.h"
#include <map>
#include "../../Core/Text/string_types.h"
#include "../../Core/IOData/virtual_directory.h"

class CL_PixelBuffer;
class CL_ImageProviderType;

/// \brief Image Provider factory
///
/// \xmlonly !group=Display/Image Providers! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_ImageProviderFactory
{
/// \name Static Attributes
/// \{

public:
	/// \brief Map of the class factories for each provider type.
	static std::map<CL_String, CL_ImageProviderType *> types;

/// \}
/// \name Operations
/// \{

public:
	/// \brief  Attempts to load an image file from 'filename', using the provider identified by 'type'.
	/// \return If the function fails to load the image, a null pixel buffer object is returned.
	///
	/// <p>If the type is an empty string, it uses the extension of
	/// the filename to determine what type it is.</p>
	/// <p>The virtual directory will be used as a the source.</p>
	static CL_PixelBuffer try_load(
		const CL_String &filename,
		const CL_String &type = CL_String(),
		CL_VirtualDirectory directory = CL_VirtualDirectory(),
		CL_String *out_failure_reason = 0);

	/// \brief Loads an image file from 'filename', using the provider
	/// \brief identified by 'type'.
	/** <p>If the type is an empty string, it uses the extension of
	    the filename to determine what type it is.</p>
	    <p>The virtual directory will be used as a the source.</p>*/
	static CL_PixelBuffer load(
		const CL_String &filename,
		const CL_VirtualDirectory &directory,
		const CL_String &type = CL_String());

	static CL_PixelBuffer load(
		const CL_String &fullname,
		const CL_String &type = CL_String());

	static CL_PixelBuffer load(
		CL_IODevice &file,
		const CL_String &type);

	/// \brief Saves the given CL_PixelBuffer to the file given by 'filename'.
	/** <p>If the type is an empty string, it uses the extension of the
	    filename to determine what type it is </p>*/
	static void save(
		CL_PixelBuffer buffer,
		const CL_String &filename,
		CL_VirtualDirectory &directory,
		const CL_String &type = CL_String());

	static void save(
		CL_PixelBuffer buffer,
		const CL_String &fullname,
		const CL_String &type = CL_String());

	static void save(
		CL_PixelBuffer buffer,
		CL_IODevice &file,
		const CL_String &type);
/// \}
};

/// \}
