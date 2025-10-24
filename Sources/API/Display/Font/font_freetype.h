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
**    Mark Page
*/

/// \addtogroup clanDisplay_Font clanDisplay Font
/// \{

#pragma once

#include "../api_display.h"
#include "font.h"

class CL_FontProvider_Freetype;
class CL_TextureGroup;

/// \brief Freetype Font class.
///
/// \xmlonly !group=Display/Font! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_Font_Freetype : public CL_Font
{
/// \name Construction
/// \{
public:
	/// \brief Constructs freetype font.
	CL_Font_Freetype();

	/// \brief Constructs a Font Freetype
	///
	/// \param typeface_name = String Ref
	/// \param height = value
	CL_Font_Freetype(const CL_StringRef &typeface_name, int height);

	/// \brief Constructs a Font Freetype
	///
	/// \param typeface_name = String Ref
	/// \param height = value
	/// \param file = IODevice
	CL_Font_Freetype(const CL_StringRef &typeface_name, int height, CL_IODevice &file);

	/// \brief Constructs a Font Freetype
	///
	/// \param typeface_name = String Ref
	/// \param height = value
	/// \param directory = Virtual Directory
	CL_Font_Freetype(const CL_StringRef &typeface_name, int height, const CL_VirtualDirectory &directory);

	/// \brief Constructs a Font Freetype
	///
	/// \param desc = Font Description
	CL_Font_Freetype(const CL_FontDescription &desc);

	/// \brief Constructs a Font Freetype
	///
	/// \param desc = Font Description
	/// \param file = IODevice
	CL_Font_Freetype(const CL_FontDescription &desc, CL_IODevice &file);

	/// \brief Constructs a Font Freetype
	///
	/// \param desc = Font Description
	/// \param directory = Virtual Directory
	CL_Font_Freetype(const CL_FontDescription &desc, const CL_VirtualDirectory &directory);

	/// \brief Constructs a Font Freetype
	///
	/// \param resource_id = String Ref
	/// \param resources = Resource Manager
	CL_Font_Freetype(const CL_StringRef &resource_id, CL_ResourceManager *resources);

	~CL_Font_Freetype();

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Retrieves the font provider.
	CL_FontProvider_Freetype *get_provider() const;

/// \}
/// \name Operations
/// \{
public:
	/// \brief Set the texture font to use a specified texture group
	void set_texture_group(CL_TextureGroup &new_texture_group);

/// \}
/// \name Implementation
/// \{
private:
/// \}
};

/// \}
