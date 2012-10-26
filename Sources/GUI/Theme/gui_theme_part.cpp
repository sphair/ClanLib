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

#include "GUI/precomp.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Core/Text/console_logger.h"
#include "../gui_component_impl.h"
#include "../gui_manager_impl.h"
#include "../gui_font_cache.h"
#include "../gui_css_strings.h"
#include "gui_theme_part_impl.h"
#include "gui_theme_part_property_impl.h"
#include "API/GUI/gui_theme_part_property.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GUIThemePart Construction:

GUIThemePart::GUIThemePart()
{
}

GUIThemePart::GUIThemePart(
	GUIComponent *component,
	const std::string &relative_element_name)
: impl(new GUIThemePart_Impl(component))
{
}
	
GUIThemePart::~GUIThemePart()
{
}

/////////////////////////////////////////////////////////////////////////////
// GUIThemePart Attributes:

std::string GUIThemePart::get_element_name() const
{
	return impl->element_name;
}

bool GUIThemePart::get_state(const std::string &name) const
{
	std::vector<std::string>::size_type index, size;
	size = impl->states.size();
	for (index = 0; index < size; index++)
	{
		if (impl->states[index] == name)
			return true;
	}
	return false;
}


std::string GUIThemePart::get_property(const GUIThemePartProperty &property) const
{
	return property.impl->get_value(*this);
}

int GUIThemePart::get_property_int(const GUIThemePartProperty &property) const
{
	return StringHelp::text_to_int(property.impl->get_value(*this));
}

GUIComponent *GUIThemePart::get_component() const
{
	return impl->component;
}

Rect GUIThemePart::get_content_box(const Rect &render_box_rect) const
{
	impl->check_content_shrink_box_is_cached(*this);
	Rect box = render_box_rect;
	Rect &R = impl->cached_content_box_shrink_rect;
	box.shrink(R.left, R.top, R.right, R.bottom);
	return box;
}

Rect GUIThemePart::get_render_box(const Rect &content_box_rect) const
{
	impl->check_content_shrink_box_is_cached(*this);
	Rect box = content_box_rect;

	Rect &R = impl->cached_content_box_shrink_rect;
	box.expand(R.left, R.top, R.right, R.bottom);
	return box;
}

Rect GUIThemePart::get_content_shrink_box() const
{
	Rect shrink_box(0,0,0,0);

	int margin_top = get_property_int(impl->prop_margin_top);
	int margin_left = get_property_int(impl->prop_margin_left);
	int margin_right = get_property_int(impl->prop_margin_right);
	int margin_bottom = get_property_int(impl->prop_margin_bottom);

	int border_top = get_property_int(impl->prop_border_top);
	int border_left = get_property_int(impl->prop_border_left);
	int border_right = get_property_int(impl->prop_border_right);
	int border_bottom = get_property_int(impl->prop_border_bottom);

	int padding_top = get_property_int(impl->prop_padding_top);
	int padding_left = get_property_int(impl->prop_padding_left);
	int padding_right = get_property_int(impl->prop_padding_right);
	int padding_bottom = get_property_int(impl->prop_padding_bottom);

	shrink_box.left += margin_left+ border_left + padding_left;
	shrink_box.right += margin_right + border_right + padding_right;
	shrink_box.top += margin_top + border_top + padding_top;
	shrink_box.bottom += margin_bottom + border_bottom + padding_bottom;

	return shrink_box;
}

Size GUIThemePart::get_minimum_size() const
{
	return Size(get_property_int(impl->prop_minimum_width), get_property_int(impl->prop_minimum_height));
}

int GUIThemePart::get_minimum_width() const
{
	return get_minimum_size().width;
}

int GUIThemePart::get_minimum_height() const
{
	return get_minimum_size().height;
}

Size GUIThemePart::get_maximum_size() const
{
	return Size(get_property_int(impl->prop_maximum_width), get_property_int(impl->prop_maximum_height));
}

int GUIThemePart::get_maximum_width() const
{
	return get_maximum_size().width;
}

int GUIThemePart::get_maximum_height() const
{
	return get_maximum_size().height;
}

Size GUIThemePart::get_preferred_size() const
{
	return Size(get_property_int(impl->prop_preferred_width), get_property_int(impl->prop_preferred_height));
}

int GUIThemePart::get_preferred_width() const
{
	return get_preferred_size().width;
}

int GUIThemePart::get_preferred_height() const
{
	return get_preferred_size().height;
}

Font GUIThemePart::get_font() const
{
	GUIFontCache &font_cache = impl->component->get_gui_manager().impl->font_cache;
	Font font = font_cache.get_font(get_element_name(), impl->states);
	if (!font.is_null())
		return font;

	std::string font_weight = get_property(impl->prop_font_weight);
	int weight = 0;
	if (font_weight == "normal")
		weight = 400;
	else if (font_weight == "bold" || font_weight == "bolder")
		weight = 700;
	else if (font_weight == "light" || font_weight == "lighter")
		weight = 300;
	else
		weight = StringHelp::text_to_int(font_weight);

	int font_size = get_property_int(impl->prop_font_size);

	bool italic = (get_property(impl->prop_font_style) == "italic");
	bool underline = (get_property(impl->prop_text_decoration) == "underline");
	bool strikeout = false;

	GUIComponent *component = impl->component;
	impl->font_loaded = true;

	const std::string typeface_name = get_property(impl->prop_font_family);

	// Build the font details
	FontDescription desc;
	desc.set_height(font_size);
	desc.set_weight(weight);
	desc.set_italic(italic);
	desc.set_underline(underline);
	desc.set_strikeout(strikeout);
	desc.set_typeface_name(typeface_name);

	font = font_cache.get_font(desc);	// Check to see if matching font description in the font cache
	if (!font.is_null())
		return font;

	GUIManager manager = impl->component->get_gui_manager();
	font = manager.get_registered_font(desc);
	if (font.is_null())
	{
		Canvas &canvas = component->get_canvas();
		font = Font(canvas, desc);
	}
	
	font_cache.set_font(font, desc, get_element_name(), impl->states);

	return font;
}

Size GUIThemePart::get_text_size( Canvas &canvas, const std::string &str ) const
{
	Font font = get_font();
	return font.get_text_size(canvas, str);
}

/////////////////////////////////////////////////////////////////////////////
// GUIThemePart Operations:

bool GUIThemePart::set_state(const std::string &name, bool flag)
{
	std::vector<std::string>::size_type index, size;
	size = impl->states.size();
	if (flag)
	{
		for (index = 0; index < size; index++)
			if (impl->states[index] == name)
				return false;
		impl->states.push_back(name);
		return true;
	}
	else
	{
		for (index = 0; index < size; index++)
		{
			if (impl->states[index] == name)
			{
				impl->states.erase(impl->states.begin() + index);
				return true;
			}
		}
		return false;
	}
}

void GUIThemePart::render_box(Canvas &canvas, const Rect &rect, const Rect &clip_rect)
{
	GUITheme theme = impl->component->get_theme();
	if (!theme.is_null())
		theme.render_box(canvas, *this, rect, clip_rect);
}

GUIThemePart::VerticalTextPosition GUIThemePart::get_vertical_text_align(Canvas &canvas, Font &font, const Rect &content_rect)
{
	// See diagram in: Documentation\Overview\fonts.html (Font Metrics)

	FontMetrics metrics = font.get_font_metrics();
	float align_height = metrics.get_ascent() - metrics.get_internal_leading();
	float content_height = content_rect.get_height();
	float baseline = (content_height + align_height) / 2.0f;

	VerticalTextPosition result;
	result.baseline = baseline + content_rect.top;
	result.top = result.baseline - metrics.get_ascent();
	result.bottom = result.baseline + metrics.get_descent();
	return result;
}

Rect GUIThemePart::render_text( Canvas &canvas, const std::string &text, const Rect &content_rect, const Rect &clip_rect )
{
	Font font = get_font();

	GUITheme theme = impl->component->get_theme();
	if (!theme.is_null())
		return theme.render_text(canvas, *this, font, text, content_rect, clip_rect);

	return Rect(0,0,0,0);
}

/////////////////////////////////////////////////////////////////////////////
// GUIThemePart Implementation:

}
