/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
#include "API/Core/CSS/css_ruleset.h"
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

		CL_StringRef key;
		CL_String8 hash;
		CL_StringRef value;
	};

	mutable std::vector<PropertyCacheEntry> properties_cache;
	mutable bool has_cached_content_box_shrink_rect;
	mutable CL_Rect cached_content_box_shrink_rect;
	mutable bool font_loaded;

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
	prop_border_top = CL_GUIThemePartProperty(cl_text("border-top"), cl_text("0"));
	prop_border_left = CL_GUIThemePartProperty(cl_text("border-left"), cl_text("0"));
	prop_border_right = CL_GUIThemePartProperty(cl_text("border-right"), cl_text("0"));
	prop_border_bottom = CL_GUIThemePartProperty(cl_text("border-bottom"), cl_text("0"));
	prop_padding_top = CL_GUIThemePartProperty(cl_text("padding-top"), cl_text("0"));
	prop_padding_left = CL_GUIThemePartProperty(cl_text("padding-left"), cl_text("0"));
	prop_padding_right = CL_GUIThemePartProperty(cl_text("padding-right"), cl_text("0"));
	prop_padding_bottom = CL_GUIThemePartProperty(cl_text("padding-bottom"), cl_text("0"));
	prop_minimum_width = CL_GUIThemePartProperty(cl_text("minimum-width"), cl_text("0"));
	prop_minimum_height = CL_GUIThemePartProperty(cl_text("minimum-height"), cl_text("0"));
	prop_maximum_width = CL_GUIThemePartProperty(cl_text("maximum-width"), cl_text("4026531839"));
	prop_maximum_height = CL_GUIThemePartProperty(cl_text("maximum-height"), cl_text("4026531839"));
	prop_preferred_width = CL_GUIThemePartProperty(cl_text("preferred-width"), cl_text("0"));
	prop_preferred_height = CL_GUIThemePartProperty(cl_text("preferred-height"), cl_text("0"));
	prop_font_weight = CL_GUIThemePartProperty(cl_text("font-weight"), CssStr::normal);
	prop_font_size = CL_GUIThemePartProperty(cl_text("font-size"), cl_text("-11"));
	prop_font_style = CL_GUIThemePartProperty(cl_text("font-style"), CssStr::normal);
	prop_text_decoration = CL_GUIThemePartProperty(cl_text("text-decoration"), CssStr::normal);
	prop_font_family = CL_GUIThemePartProperty(cl_text("font-family"), cl_text("Tahoma"));
}

inline void CL_GUIThemePart_Impl::update_element_name() const
{
	properties_cache.clear();
	element_name.clear();
	if (!relative_element_name.empty())
		element_name = component->get_element_name() + cl_text(" ") + relative_element_name;
	else
		element_name = component->get_element_name();
	
	std::vector<CL_String>::size_type index, size;
	size = states.size();
	for (index = 0; index < size; index++)
		element_name += cl_text(":") + states[index];
}

inline CL_StringRef CL_GUIThemePart_Impl::get_property(const CL_GUIThemePart &part, const CL_String &name, const CL_String8 &hash, const CL_String &default_value) const
{
	CL_StringRef css_value = get_css_value(name, hash, default_value);

	// Check if theme wants to override the property:
	CL_GUITheme *theme = component->get_theme();
	if (theme)
		return theme->get_property(part, name, css_value);
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

	// Find relevant CSS rulesets:
	std::vector<CL_CSSRuleSet> &rulesets = get_manager().impl->get_rulesets(element_name);

	std::vector<CL_CSSRuleSet>::size_type index, size;
	size = rulesets.size();

	// Look for properties with the important flag first:
	bool found = false;
	for (index = 0; index < size; index++)
	{
		std::vector<CL_CSSProperty> &properties = rulesets[index].get_properties();
		std::vector<CL_CSSProperty>::size_type index_properties, size_properties;
		size_properties = properties.size();
		for (index_properties = 0; index_properties < size_properties; index_properties++)
		{
			if (properties[index_properties].get_priority() == CL_CSSProperty::priority_important &&
				properties[index_properties].get_name() == name)
			{
				css_value = properties[index_properties].get_value();
				found = true;
				break;
			}
		}
	}

	// Look for normal properties:
	if (!found)
	{
		for (index = 0; index < size; index++)
		{
			if( found ) break;
			std::vector<CL_CSSProperty> &properties = rulesets[index].get_properties();
			std::vector<CL_CSSProperty>::size_type index_properties, size_properties;
			size_properties = properties.size();
			for (index_properties = 0; index_properties < size_properties; index_properties++)
			{
				const CL_StringRef &property_name = properties[index_properties].get_name();
				if (property_name == name)
				{
					css_value = properties[index_properties].get_value();
					found = true;
					break;
				}
			}
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

	CL_GUITheme *theme = component->get_theme();
	if (!theme)
		throw CL_Exception(cl_text("CL_GUIThemePart: No theme."));

	has_cached_content_box_shrink_rect = true;
	cached_content_box_shrink_rect = part.get_content_shrink_box();
}
