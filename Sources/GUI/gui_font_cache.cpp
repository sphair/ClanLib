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
**    Mark Page
*/

#include "GUI/precomp.h"
#include "gui_font_cache.h"
#include "API/Display/Font/font.h"
#include <vector>
#include <map>

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GUIFontCache_Impl Class:

class GUIFontCache_Impl
{
public:
	GUIFontCache_Impl()
	{
	}

	~GUIFontCache_Impl()
	{
	}

	struct DescCacheEntry
	{
		std::vector<std::string> states;
		FontDescription font_desc;
	};

	struct FontCacheEntry
	{
		FontDescription desc;
		Font font;
	};

	std::vector<FontCacheEntry> font_cache;
	std::map<std::string /*css_name*/, std::vector<DescCacheEntry> > font_desc_cache;
	
	FontDescription get_font_description(const std::string &css_name, const std::vector<std::string> &states)
	{
		std::map<std::string, std::vector<DescCacheEntry> >::iterator find_it;
		find_it = font_desc_cache.find(css_name);
		if (find_it != font_desc_cache.end())
		{
			std::vector<DescCacheEntry> &cache_entries = (*find_it).second;
			std::vector<DescCacheEntry>::iterator it_cache;
			for (it_cache = cache_entries.begin(); it_cache != cache_entries.end(); ++it_cache)
			{
				std::vector<std::string> &cache_states = (*it_cache).states;
				int total_states = cache_states.size();
				int matching_states = 0;

				std::vector<std::string>::iterator it;
				for (it = cache_states.begin(); it != cache_states.end(); ++it)
				{
					if (find_state((*it), states))
						matching_states++;
				}

				if (total_states == matching_states)
					return (*it_cache).font_desc;
			}
		}

		return FontDescription::create_null_object();
	}

	Font get_font(const std::string &css_name, const std::vector<std::string> &states)
	{
		FontDescription desc = get_font_description(css_name, states);
		return (get_font(desc));
	}

	Font get_font(const FontDescription &desc)
	{
		if (desc.is_null())
			return Font();

		std::vector<FontCacheEntry>::iterator it;
		for (it = font_cache.begin(); it != font_cache.end(); ++it)
		{
			if ((*it).desc == desc)
				return (*it).font;
		}
		return Font();
	}

	void set_font(const Font &font, const FontDescription &desc, const std::string &css_name, const std::vector<std::string> &states)
	{
		DescCacheEntry desc_entry;
		desc_entry.font_desc = desc;
		desc_entry.states = states;
		font_desc_cache[css_name].push_back(desc_entry);

		FontCacheEntry font_entry;
		font_entry.desc = desc;
		font_entry.font = font;
		font_cache.push_back(font_entry);
	}

private:
	bool find_state(const std::string &state, const std::vector<std::string> &state_vector)
	{
		std::vector<std::string>::const_iterator it;
		for (it = state_vector.begin(); it != state_vector.end(); ++it)
		{
			if ((*it) == state)
				return true;
		}

		return false;
	}
};

/////////////////////////////////////////////////////////////////////////////
// GUIFontCache Construction:

GUIFontCache::GUIFontCache() : impl(std::shared_ptr<GUIFontCache_Impl>(new GUIFontCache_Impl()))
{
}
	
GUIFontCache::~GUIFontCache()
{
}

/////////////////////////////////////////////////////////////////////////////
// GUIFontCache Attributes:

Font GUIFontCache::get_font(const std::string &css_name, const std::vector<std::string> &states)
{
	return impl->get_font(css_name, states);
}

Font GUIFontCache::get_font(const FontDescription &desc)
{
	return impl->get_font(desc);
}
/////////////////////////////////////////////////////////////////////////////
// GUIFontCache Operations:

void GUIFontCache::set_font(const Font &font, const FontDescription &desc, const std::string &css_name, const std::vector<std::string> &states)
{
	impl->set_font(font, desc, css_name, states);
}

/////////////////////////////////////////////////////////////////////////////
// GUIFontCache Implementation:

}
