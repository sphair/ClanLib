/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
*/

/// \addtogroup clanDisplay_Font clanDisplay Font
/// \{

#pragma once

#include "../api_display.h"
#include <memory>

namespace clan
{

class FontManager_Impl;
class FontDescription;

class Canvas;

/// \brief Font manager class.
///
/// \xmlonly !group=Display/Font! !header=display.h! \endxmlonly
class CL_API_DISPLAY FontManager
{
/// \name Construction
/// \{

public:
	FontManager();

	~FontManager();

/// \}
/// \name Attributes
/// \{

public:

/// \}
/// \name Operations
/// \{

public:

	/// \brief Registers a font for lookup when creating fonts.
	void register_font(const std::string &font_filename, const std::string &font_typeface);

	/// \brief Get the registered font or the best font match if not found
	//
	/// \param desc = Description
	///
	/// \return Updated description
	FontDescription get_registered_font(const FontDescription &desc);

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<FontManager_Impl> impl;
/// \}
};

}

/// \}

