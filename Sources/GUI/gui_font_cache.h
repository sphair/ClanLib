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
**    Harry Storbacka
*/

#pragma once

#include <memory>
#include <vector>

namespace clan
{

class Font;
class FontDescription;
class GUIFontCache_Impl;

class GUIFontCache
{
/// \name Construction
/// \{

public:
	GUIFontCache();

	~GUIFontCache();


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Get cached font. Returns a null font if the specified font not cached.
	Font get_font(const std::string &css_name, const std::vector<std::string> &states);

	/// \brief Get cached font. Returns a null font if the specified font not cached.
	Font get_font(const FontDescription &desc);

/// \}
/// \name Operations
/// \{

public:
	/// \brief Add font to font cache.
	void set_font(const Font &font, const FontDescription &font_desc, const std::string &css_name, const std::vector<std::string> &states);

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<GUIFontCache_Impl> impl;
/// \}
};

}
