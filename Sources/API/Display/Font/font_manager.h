/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
class Font;

/// \brief Font manager class.
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
	/// \brief Get a font from the cache.
	///
	/// Check returned font with font.is_null()
	Font get_font(const FontDescription &desc) const;

/// \}
/// \name Operations
/// \{

public:

	/// \brief Add a font to the cache
	void set_font(Font &font, const FontDescription &desc);

	/// \bright Remove all fonts from the cache
	void flush_cache();

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<FontManager_Impl> impl;
/// \}
};

}

/// \}

