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
#include "API/Core/Text/string_allocator.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Math/rect.h"
#include "API/Core/Math/sha1.h"
#include "API/Core/CSS/css_property.h"
#include "API/Core/CSS/css_document.h"
#include "API/Core/Math/hash_functions.h"
#include "gui_manager_impl.h"
#include "Components/../gui_css_strings.h"
#include <map>

class CL_GUIThemePart_Impl
{
public:
	CL_GUIThemePart_Impl();
	void update_element_name() const;

	CL_StringRef get_property(const CL_GUIThemePart &part, const CL_String &name, const CL_String8 &hash, const CL_String &default_value) const;
	CL_StringRef get_css_value(const CL_String &name, const CL_String8 &hash, const CL_String &default_value) const;
	inline void check_for_update_element_name() const;
	CL_StringRef get_css_value_slow(const CL_String &name, const CL_String &default_value) const;
	CL_GUIManager get_manager() const;
	void check_content_shrink_box_is_cached(const CL_GUIThemePart &part) const;

	CL_GUIComponent *component;
	CL_String relative_element_name;
	std::vector<CL_String> states;
	mutable CL_String element_name;
	mutable CL_String last_component_element_name;

	struct PropertyCacheEntry
	{
		PropertyCacheEntry(const CL_StringRef &key, const CL_StringRef &value)
			: key(key), value(value)
		{
			hash = CL_HashFunctions::sha1(CL_StringHelp::text_to_utf8(key));
		}

		CL_String key;
		CL_String8 hash;
		CL_String value;
	};

	mutable std::vector<PropertyCacheEntry> properties_cache;
	mutable bool has_cached_content_box_shrink_rect;
	mutable CL_Rect cached_content_box_shrink_rect;
	mutable bool font_loaded;

	CL_GUIThemePartProperty prop_margin_top;
	CL_GUIThemePartProperty prop_margin_left;
	CL_GUIThemePartProperty prop_margin_right;
	CL_GUIThemePartProperty prop_margin_bottom;
	CL_GUIThemePartProperty prop_border_top;
	CL_GUIThemePartProperty prop_border_left;
	CL_GUIThemePartProperty prop_border_right;
	CL_GUIThemePartProperty prop_border_bottom;
	CL_GUIThemePartProperty prop_padding_top;
	CL_GUIThemePartProperty prop_padding_left;
	CL_GUIThemePartProperty prop_padding_right;
	CL_GUIThemePartProperty prop_padding_bottom;
	CL_GUIThemePartProperty prop_minimum_width;
	CL_GUIThemePartProperty prop_minimum_height;
	CL_GUIThemePartProperty prop_maximum_width;
	CL_GUIThemePartProperty prop_maximum_height;
	CL_GUIThemePartProperty prop_preferred_width;
	CL_GUIThemePartProperty prop_preferred_height;
	CL_GUIThemePartProperty prop_font_weight;
	CL_GUIThemePartProperty prop_font_size;
	CL_GUIThemePartProperty prop_font_style;
	CL_GUIThemePartProperty prop_text_decoration;
	CL_GUIThemePartProperty prop_font_family;
};

inline CL_GUIThemePart_Impl::CL_GUIThemePart_Impl()
: component(0), has_cached_content_box_shrink_rect(false)
{
	prop_margin_top = CL_GUIThemePartProperty("margin-top", "0");
	prop_margin_left = CL_GUIThemePartProperty("margin-left", "0");
	prop_margin_right = CL_GUIThemePartProperty("margin-right", "0");
	prop_margin_bottom = CL_GUIThemePartProperty("margin-bottom", "0");
	prop_border_top = CL_GUIThemePartProperty("border-top", "0");
	prop_border_left = CL_GUIThemePartProperty("border-left", "0");
	prop_border_right = CL_GUIThemePartProperty("border-right", "0");
	prop_border_bottom = CL_GUIThemePartProperty("border-bottom", "0");
	prop_padding_top = CL_GUIThemePartProperty("padding-top", "0");
	prop_padding_left = CL_GUIThemePartProperty("padding-left", "0");
	prop_padding_right = CL_GUIThemePartProperty("padding-right", "0");
	prop_padding_bottom = CL_GUIThemePartProperty("padding-bottom", "0");
	prop_minimum_width = CL_GUIThemePartProperty("minimum-width", "0");
	prop_minimum_height = CL_GUIThemePartProperty("minimum-height", "0");
	prop_maximum_width = CL_GUIThemePartProperty("maximum-width", "4026531839");
	prop_maximum_height = CL_GUIThemePartProperty("maximum-height", "4026531839");
	prop_preferred_width = CL_GUIThemePartProperty("preferred-width", "0");
	prop_preferred_height = CL_GUIThemePartProperty("preferred-height", "0");
	prop_font_weight = CL_GUIThemePartProperty("font-weight", CssStr::normal);
	prop_font_size = CL_GUIThemePartProperty("font-size", "-11");
	prop_font_style = CL_GUIThemePartProperty("font-style", CssStr::normal);
	prop_text_decoration = CL_GUIThemePartProperty("text-decoration", CssStr::normal);
	prop_font_family = CL_GUIThemePartProperty("font-family", "Tahoma");
}

inline void CL_GUIThemePart_Impl::update_element_name() const
{
	properties_cache.clear();
	element_name.clear();
	if (!relative_element_name.empty())
		element_name = component->get_element_name() + " " + relative_element_name;
	else
		element_name = component->get_element_name();
	
	std::vector<CL_String>::size_type index, size;
	size = states.size();
	for (index = 0; index < size; index++)
		element_name += ":" + states[index];
}

inline CL_StringRef CL_GUIThemePart_Impl::get_property(const CL_GUIThemePart &part, const CL_String &name, const CL_String8 &hash, const CL_String &default_value) const
{
	CL_StringRef css_value = get_css_value(name, hash, default_value);

	// Check if theme wants to override the property:
	CL_GUITheme theme = component->get_theme();
	if (!theme.is_null())
		return theme.get_property(part, name, css_value);
	else
		return css_value;
}

inline CL_StringRef CL_GUIThemePart_Impl::get_css_value(const CL_String &name, const CL_String8 &hash, const CL_String &default_value) const
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

inline void CL_GUIThemePart_Impl::check_for_update_element_name() const
{
	if (component->get_element_name() != last_component_element_name)
	{
		last_component_element_name = component->get_element_name();
		update_element_name();
	}
}

inline CL_StringRef CL_GUIThemePart_Impl::get_css_value_slow(const CL_String &name, const CL_String &default_value) const
{
	CL_StringRef css_value = default_value;

	std::vector<CL_CSSProperty> &properties = get_manager().impl->get_properties(element_name);
	for (size_t i = 0; i < properties.size(); i++)
	{
		if (properties[i].get_name() == name)
		{
			css_value = properties[i].get_value();
			break;
		}
	}

	PropertyCacheEntry entry(name, css_value);
	properties_cache.push_back(entry);
	return css_value;
}

inline CL_GUIManager CL_GUIThemePart_Impl::get_manager() const
{
	return component->get_gui_manager();
}

inline void CL_GUIThemePart_Impl::check_content_shrink_box_is_cached(const CL_GUIThemePart &part) const
{
	if (has_cached_content_box_shrink_rect)
		return;

	CL_GUITheme theme = component->get_theme();
	if (theme.is_null())
		throw CL_Exception("CL_GUIThemePart: No theme.");

	has_cached_content_box_shrink_rect = true;
	cached_content_box_shrink_rect = part.get_content_shrink_box();
}
