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

#pragma once

#include "API/Display/api_display.h"
#include <map>

namespace clan
{

class FontDescription;
class Font;

struct FontCacheEntry
{
	FontDescription desc;
	Font font;
};

/// \brief Input events interface.
class CL_API_DISPLAY FontManager_Impl
{
/// \name Construction
/// \{

public:
	FontManager_Impl();

	~FontManager_Impl();


/// \}
/// \name Attributes
/// \{

public:
	Font get_font(const FontDescription &desc) const;

	void flush_cache();

/// \}
/// \name Operations
/// \{

public:
	void register_font(const std::string &font_filename, const std::string &font_typeface);
	FontDescription get_registered_font(const FontDescription &desc);

	void set_font(Font &font, const FontDescription &desc);

/// \}
/// \name Implementation
/// \{
	std::map<std::string /*font_typeface*/, std::string /*font_filename*/ > font_register_cache;

	std::vector<FontCacheEntry> font_cache;

private:
/// \}
};

}
