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

#include "Display/precomp.h"
#include "API/Core/Text/string_help.h"
#include "API/Display/Font/font_description.h"
#include "API/Display/Font/font.h"
#include "font_manager_impl.h"

#ifndef WIN32
#include "../X11/font_config.h"
#endif

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// FontManager_Impl Construction:

FontManager_Impl::FontManager_Impl()
{
}

FontManager_Impl::~FontManager_Impl()
{
}

/////////////////////////////////////////////////////////////////////////////
// FontManager_Impl Attributes:

Font FontManager_Impl::get_font(const FontDescription &desc) const
{
	if (desc.is_null())
		return Font();

	std::vector<FontCacheEntry>::const_iterator it;
	for (it = font_cache.begin(); it != font_cache.end(); ++it)
	{
		if ((*it).desc == desc)
			return (*it).font;
	}
	return Font();

}

/////////////////////////////////////////////////////////////////////////////
// FontManager_Impl Operations:

void FontManager_Impl::register_font(const std::string &font_filename, const std::string &font_typeface)
{
	std::map<std::string, std::string >::iterator find_it;
	find_it = font_register_cache.find(font_typeface);
	if (find_it == font_register_cache.end())	// Ensure not already registered
	{
		font_register_cache[font_typeface] = font_filename;
	}

}

FontDescription FontManager_Impl::get_registered_font(const FontDescription &desc)
{
	int average_width = desc.get_average_width();
	int height = desc.get_height();

	FontDescription new_desc;
	new_desc.clone(desc);
	new_desc.set_average_width(average_width);
	new_desc.set_height(height);

	// Check for a registered font
	std::map<std::string, std::string >::iterator find_it;
	find_it = font_register_cache.find(desc.get_typeface_name());
	if (find_it != font_register_cache.end())	// Found the registered font
	{
		new_desc.set_filename(find_it->second);
	}
	else
	{
#ifndef WIN32
#if !defined(__APPLE__)
        // Obtain the best matching font file from fontconfig.
		FontConfig &fc = FontConfig::instance();
		std::string font_file_path = fc.match_font(new_desc);
		new_desc.set_filename(font_file_path);
#endif
#endif
	}
	return new_desc;
}


void FontManager_Impl::set_font(Font &font, const FontDescription &desc)
{
	FontCacheEntry font_entry;
	font_entry.desc = desc;
	font_entry.font = font;
	font_cache.push_back(font_entry);
}

void FontManager_Impl::flush_cache()
{
	font_cache.clear();
}

/////////////////////////////////////////////////////////////////////////////
// FontManager_Impl Implementation:

}
