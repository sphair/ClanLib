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
**    Harry Storbacka
**    Kenneth Gangstoe
*/

#pragma once

#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_theme.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_theme_part_property.h"
#include "API/GUI/gui_component.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Math/rect.h"
#include "API/CSSLayout/css_property.h"
#include "API/CSSLayout/css_document.h"
#include "API/CSSLayout/css_token.h"
#include "API/Core/Crypto/hash_functions.h"
#include "../gui_manager_impl.h"
#include "../gui_css_strings.h"
#include <map>

namespace clan
{

class GUIThemePart_Impl
{
public:
	GUIThemePart_Impl();
	void update_element_name() const;

	std::string get_property(const GUIThemePart &part, const std::string &name, const std::string &hash, const std::string &default_value) const;
	std::string get_css_value(const std::string &name, const std::string &hash, const std::string &default_value) const;
	inline void check_for_update_element_name() const;
	std::string get_css_value_slow(const std::string &name, const std::string &default_value) const;
	GUIManager get_manager() const;
	void check_content_shrink_box_is_cached(const GUIThemePart &part) const;

	GUIComponent *component;
	std::string relative_element_name;
	std::vector<std::string> states;
	mutable std::string element_name;
	mutable std::string last_component_element_name;

	struct PropertyCacheEntry
	{
		PropertyCacheEntry(const std::string &key, const std::string &value)
			: key(key), value(value)
		{
			hash = HashFunctions::sha1(StringHelp::text_to_utf8(key));
		}

		std::string key;
		std::string hash;
		std::string value;
	};

	mutable std::vector<PropertyCacheEntry> properties_cache;
	mutable bool has_cached_content_box_shrink_rect;
	mutable Rect cached_content_box_shrink_rect;
	mutable bool font_loaded;

	GUIThemePartProperty prop_margin_top;
	GUIThemePartProperty prop_margin_left;
	GUIThemePartProperty prop_margin_right;
	GUIThemePartProperty prop_margin_bottom;
	GUIThemePartProperty prop_border_top;
	GUIThemePartProperty prop_border_left;
	GUIThemePartProperty prop_border_right;
	GUIThemePartProperty prop_border_bottom;
	GUIThemePartProperty prop_padding_top;
	GUIThemePartProperty prop_padding_left;
	GUIThemePartProperty prop_padding_right;
	GUIThemePartProperty prop_padding_bottom;
	GUIThemePartProperty prop_minimum_width;
	GUIThemePartProperty prop_minimum_height;
	GUIThemePartProperty prop_maximum_width;
	GUIThemePartProperty prop_maximum_height;
	GUIThemePartProperty prop_preferred_width;
	GUIThemePartProperty prop_preferred_height;
	GUIThemePartProperty prop_font_weight;
	GUIThemePartProperty prop_font_size;
	GUIThemePartProperty prop_font_style;
	GUIThemePartProperty prop_text_decoration;
	GUIThemePartProperty prop_font_family;
};

inline GUIThemePart_Impl::GUIThemePart_Impl()
: component(0), has_cached_content_box_shrink_rect(false)
{
	prop_margin_top = GUIThemePartProperty("margin-top", "0");
	prop_margin_left = GUIThemePartProperty("margin-left", "0");
	prop_margin_right = GUIThemePartProperty("margin-right", "0");
	prop_margin_bottom = GUIThemePartProperty("margin-bottom", "0");
	prop_border_top = GUIThemePartProperty("border-top", "0");
	prop_border_left = GUIThemePartProperty("border-left", "0");
	prop_border_right = GUIThemePartProperty("border-right", "0");
	prop_border_bottom = GUIThemePartProperty("border-bottom", "0");
	prop_padding_top = GUIThemePartProperty("padding-top", "0");
	prop_padding_left = GUIThemePartProperty("padding-left", "0");
	prop_padding_right = GUIThemePartProperty("padding-right", "0");
	prop_padding_bottom = GUIThemePartProperty("padding-bottom", "0");
	prop_minimum_width = GUIThemePartProperty("minimum-width", "0");
	prop_minimum_height = GUIThemePartProperty("minimum-height", "0");
	prop_maximum_width = GUIThemePartProperty("maximum-width", "4026531839");
	prop_maximum_height = GUIThemePartProperty("maximum-height", "4026531839");
	prop_preferred_width = GUIThemePartProperty("preferred-width", "0");
	prop_preferred_height = GUIThemePartProperty("preferred-height", "0");
	prop_font_weight = GUIThemePartProperty("font-weight", CssStr::normal);
	prop_font_size = GUIThemePartProperty("font-size", "-11");
	prop_font_style = GUIThemePartProperty("font-style", CssStr::normal);
	prop_text_decoration = GUIThemePartProperty("text-decoration", CssStr::normal);
	prop_font_family = GUIThemePartProperty("font-family", "Tahoma");
}

inline void GUIThemePart_Impl::update_element_name() const
{
	properties_cache.clear();
	element_name.clear();
	if (!relative_element_name.empty())
		element_name = component->get_element_name() + " " + relative_element_name;
	else
		element_name = component->get_element_name();
	
	std::vector<std::string>::size_type index, size;
	size = states.size();
	for (index = 0; index < size; index++)
		element_name += ":" + states[index];
}

inline std::string GUIThemePart_Impl::get_property(const GUIThemePart &part, const std::string &name, const std::string &hash, const std::string &default_value) const
{
	std::string css_value = get_css_value(name, hash, default_value);

	// Check if theme wants to override the property:
	GUITheme theme = component->get_theme();
	if (!theme.is_null())
		return theme.get_property(part, name, css_value);
	else
		return css_value;
}

inline std::string GUIThemePart_Impl::get_css_value(const std::string &name, const std::string &hash, const std::string &default_value) const
{
	check_for_update_element_name();

	for (std::vector<PropertyCacheEntry>::size_type i=0; i < properties_cache.size(); i++)
	{
		PropertyCacheEntry &entry = properties_cache[i];
		if (entry.hash == hash /* && entry.key == name*/)
			return entry.value;
	}

	return get_css_value_slow(name, default_value);
}

inline void GUIThemePart_Impl::check_for_update_element_name() const
{
	if (component->get_element_name() != last_component_element_name)
	{
		last_component_element_name = component->get_element_name();
		update_element_name();
	}
}

inline std::string GUIThemePart_Impl::get_css_value_slow(const std::string &name, const std::string &default_value) const
{
	std::string css_value = default_value;

	std::vector<CSSProperty> &properties = get_manager().impl->get_properties(element_name);
	for (size_t i = 0; i < properties.size(); i++)
	{
		if (properties[i].get_name() == name)
		{
			const std::vector<CSSToken> &tokens = properties[i].get_value_tokens();
			css_value.clear();
			for (size_t j = 0; j < tokens.size(); j++)
				css_value += tokens[j].value + tokens[j].dimension;
			break;
		}
	}

	PropertyCacheEntry entry(name, css_value);
	properties_cache.push_back(entry);
	return css_value;
}

inline GUIManager GUIThemePart_Impl::get_manager() const
{
	return component->get_gui_manager();
}

inline void GUIThemePart_Impl::check_content_shrink_box_is_cached(const GUIThemePart &part) const
{
	if (has_cached_content_box_shrink_rect)
		return;

	GUITheme theme = component->get_theme();
	if (theme.is_null())
		throw Exception("GUIThemePart: No theme.");

	has_cached_content_box_shrink_rect = true;
	cached_content_box_shrink_rect = part.get_content_shrink_box();
}

}
