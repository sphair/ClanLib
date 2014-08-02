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

#include "GUI/precomp.h"
#include "gui_element.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_manager.h"
#include "API/CSSLayout/CSSDocument/css_document.h"
#include "API/CSSLayout/CSSTokenizer/css_token.h"
#include "gui_component_select_node.h"
#include "API/Display/2D/span_layout.h"
#include "API/Display/Font/font_metrics.h"
#include "../CSSLayout/Layout/LayoutTree/css_used_value.h"

namespace clan
{

GUIElement::GUIElement(CSSResourceCache *resource_cache) : component(0), parent(0), prev_sibling(0), next_sibling(0), first_child(0), last_child(0), style_needs_update(true), computed_values(resource_cache)
{
}

GUIElement::~GUIElement()
{
	while (last_child)
		delete last_child;

	if (prev_sibling)
		prev_sibling->next_sibling = next_sibling;
	if (next_sibling)
		next_sibling->prev_sibling = prev_sibling;
	
	if (prev_sibling == 0 && parent)
		parent->first_child = next_sibling;
	if (next_sibling == 0 && parent)
		parent->last_child = prev_sibling;
}

const CSSComputedValues &GUIElement::get_css_values() const
{
	if (style_needs_update)
	{
		const_cast<GUIElement*>(this)->update_style();
	}

	return computed_values;
}

void GUIElement::set_component(GUIComponent *new_component)
{
	component = new_component;
}

void GUIElement::set_tag_name(const std::string &name)
{
	if (tag_name != name)
	{
		tag_name = name;
		set_style_needs_update();
	}
}

bool GUIElement::set_class(const std::string &name, bool enable, bool allow_repainting)
{
	for (size_t i = 0; i < classes.size(); i++)
	{
		if (classes[i] == name)
		{
			if (!enable)
			{
				classes.erase(classes.begin() + i);
				set_style_needs_update();
				if (allow_repainting)
					component->request_repaint();
				return true;
			}
			return false;
		}
	}
	if (enable)
	{
		classes.push_back(name);
		set_style_needs_update();
		if (allow_repainting)
			component->request_repaint();
		return true;
	}
	return false;
}

void GUIElement::set_id(const std::string &name)
{
	if (id != name)
	{
		id = name;
		set_style_needs_update();
	}
}

void GUIElement::set_style(const std::string &new_style)
{
	style = CSSStyleProperties(new_style);
	set_style_needs_update();
}

void GUIElement::set_style(const CSSPropertyValue &value, bool enable)
{
	style.set_value(value, enable);
	set_style_needs_update();
}


void GUIElement::set_parent(GUIElement *new_parent)
{
	if (new_parent == parent)
	{
		return;
	}

	if (!parent)	// Initial set_parent
	{
		parent = new_parent;

		if (parent->last_child)
		{
			parent->last_child->next_sibling = this;
			prev_sibling = parent->last_child;
			parent->last_child = this;
		}
		else
		{
			parent->first_child = this;
			parent->last_child = this;
		}
	}
	else
	{
		// 1. Remove this component from parents child list.
		if (parent->first_child == parent->last_child)
		{
			parent->first_child = 0;
			parent->last_child = 0;
		}
		else if (parent->first_child == this)
		{
			parent->first_child = next_sibling;
			next_sibling->prev_sibling = 0;
		}
		else if (parent->last_child == this)
		{
			prev_sibling->next_sibling = 0;
			parent->last_child = prev_sibling;
		}
		else
		{
			prev_sibling->next_sibling = next_sibling;
			next_sibling->prev_sibling = prev_sibling;
		}

		// 2. Set this component as last child of new parent.
		if (new_parent->first_child == 0)
		{
			new_parent->first_child = this;
			prev_sibling = 0;
		}
		else if (new_parent->last_child != 0)
		{
			new_parent->last_child->next_sibling = this;
			prev_sibling = new_parent->last_child;
		}
		next_sibling = 0;
		new_parent->last_child = this;
		parent = new_parent;
	}

	if (new_parent)
	{
		computed_values.set_parent(new_parent->computed_values);
	}
	else
	{
		computed_values.set_parent(CSSComputedValues());
	}

	set_style_needs_update();
}

bool GUIElement::get_class(const std::string &name) const
{
	for (size_t i = 0; i < classes.size(); i++)
	{
		if (classes[i] == name)
			return true;
	}
	return false;
}

bool GUIElement::get_pseudo_class(const std::string &name) const
{
	for (size_t i = 0; i < pseudo_classes.size(); i++)
	{
		if (pseudo_classes[i] == name)
			return true;
	}
	return false;
}

bool GUIElement::set_pseudo_class(const std::string &name, bool enable, bool allow_repainting)
{
	for (size_t i = 0; i < pseudo_classes.size(); i++)
	{
		if (pseudo_classes[i] == name)
		{
			if (!enable)
			{
				pseudo_classes.erase(pseudo_classes.begin() + i);
				set_style_needs_update();
				if (allow_repainting)
					component->request_repaint();
				return true;
			}
			return false;
		}
	}
	if (enable)
	{
		pseudo_classes.push_back(name);
		set_style_needs_update();
		if (allow_repainting)
			component->request_repaint();
		return true;
	}
	return false;
}

void GUIElement::set_style_needs_update()
{
	style_needs_update = true;

	GUIElement *child = get_first_child();
	while (child)
	{
		child->set_style_needs_update();
		child = child->get_next_sibling();
	}

	sig_style_changed();
}

void GUIElement::update_style()
{
	GUIComponentSelectNode select_node(this);
	computed_values.set_specified_values(component->get_gui_manager().get_css_document().select(&select_node), style);

	if (!cached_font.is_null())
		cached_font = Font();

	style_needs_update = false;
}

std::string GUIElement::get_property(const std::string &property, const std::string &default_value) const
{
	// TODO: Decode all token types
	// TODO: If property not found, search standard types?
	const CSSComputedValues &computed_values = get_css_values();
	const std::vector<CSSValueGeneric> &generic_values = computed_values.get_generic().generic_values;

	for (size_t cnt = 0; cnt < generic_values.size(); cnt++)
	{
		if (generic_values[cnt].property_name == property)
		{
			const std::vector<CSSToken> &tokens = generic_values[cnt].tokens;
			if (tokens.empty())
				return std::string("");
			return tokens[0].value;
		}
	}
	return default_value;
}

SpanLayout GUIElement::create_span_layout( Canvas &canvas, Font &font, const std::string &text, const Rect &content_rect )
{
	SpanLayout span;

	const CSSComputedTextInherit &properties = get_css_values().get_text_inherit();

	span.add_text(text, font, properties.color.color);

	switch (properties.text_align.type)
	{
		case CSSValueTextAlign::type_left: span.set_align(span_left); break;
		case CSSValueTextAlign::type_center: span.set_align(span_center); break;
		case CSSValueTextAlign::type_right: span.set_align(span_right); break;
		case CSSValueTextAlign::type_justify: span.set_align(span_justify); break;
		default: break;
	}

	span.layout(canvas, content_rect.get_width());
	span.set_position(Point(content_rect.left, content_rect.top));

	return span;
}

Font GUIElement::get_font(Canvas &canvas, const ResourceManager &resources)
{
	const CSSComputedFont &font_properties = get_css_values().get_font();

	// Note: get_css_values() calls update_style() that flushes the cache when required
	if (!cached_font.is_null())
		return cached_font;

	int font_size = used_to_actual(font_properties.font_size.length.value);
	std::string font_name;
	for (size_t i = 0; i < font_properties.font_family.names.size(); i++)
	{
		bool matched = false;
		std::string search_name;
		switch (font_properties.font_family.names[i].type)
		{
		case CSSValueFontFamilyName::type_family_name:
			search_name = StringHelp::text_to_lower(font_properties.font_family.names[i].name);
			//FIXME: See CSSResourceCache, creating font_families list
			//if (font_families.find(search_name) != font_families.end())
			//{
				font_name = font_properties.font_family.names[i].name;
				matched = true;
			//}
			break;
		default:
		case CSSValueFontFamilyName::type_serif:
		case CSSValueFontFamilyName::type_cursive:
		case CSSValueFontFamilyName::type_fantasy:
			font_name = "Times New Roman"; // Ugliest font on the planet.
			matched = true;
			break;
		case CSSValueFontFamilyName::type_sans_serif:
			font_name = "Arial";
			matched = true;
			break;
		case CSSValueFontFamilyName::type_monospace:
			font_name = "Courier New";
			matched = true;
			break;
		}
		if (matched)
			break;
	}
	if (font_name.empty())
		font_name = "Times New Roman";

	int font_weight = 400;
	switch (font_properties.font_weight.type)
	{
	case CSSValueFontWeight::type_100: font_weight = 100; break;
	case CSSValueFontWeight::type_200: font_weight = 200; break;
	case CSSValueFontWeight::type_300: font_weight = 300; break;
	case CSSValueFontWeight::type_400: font_weight = 400; break;
	case CSSValueFontWeight::type_500: font_weight = 500; break;
	case CSSValueFontWeight::type_600: font_weight = 600; break;
	case CSSValueFontWeight::type_700: font_weight = 700; break;
	case CSSValueFontWeight::type_800: font_weight = 800; break;
	case CSSValueFontWeight::type_900: font_weight = 900; break;
	case CSSValueFontWeight::type_normal: font_weight = 400; break;
	case CSSValueFontWeight::type_bold: font_weight = 700; break;
	case CSSValueFontWeight::type_bolder: font_weight = 900; break;
	case CSSValueFontWeight::type_lighter: font_weight = 300; break;
	}
	bool italic = false;
	switch (font_properties.font_style.type)
	{
	case CSSValueFontStyle::type_normal: italic = false; break;
	case CSSValueFontStyle::type_italic: italic = true; break;
	case CSSValueFontStyle::type_oblique: italic = true; break;
	}

	FontDescription font_desc;
	font_desc.set_typeface_name(font_name);
	font_desc.set_height(-font_size);
	font_desc.set_weight(font_weight);
	font_desc.set_italic(italic);
	cached_font = Font::resource(canvas, font_desc, resources);
	return cached_font;

}

Rect GUIElement::render_text( Canvas &canvas, Font &font, const std::string &text, const Rect &content_box, int baseline, bool calculate_text_rect_only )
{
	const CSSComputedTextInherit &properties = get_css_values().get_text_inherit();

	Size text_size = font.get_text_size(canvas, text);

	int offset_x = 0;
	int offset_y = 0;

	switch (properties.text_align.type)
	{
		case CSSValueTextAlign::type_left: break;
		case CSSValueTextAlign::type_center: offset_x = (content_box.get_width() - text_size.width) / 2; break;
		case CSSValueTextAlign::type_right: offset_x = (content_box.get_width() - text_size.width); break;
		case CSSValueTextAlign::type_justify: break;		// Single line text is not justified
		default: break;
	}

	if (!calculate_text_rect_only)
		font.draw_text_ellipsis(canvas, content_box.left + offset_x, baseline, content_box, text, properties.color.color);

	return Rect(content_box.left + offset_x, content_box.top, text_size);
}

Rect GUIElement::get_render_text_box( Canvas &canvas, const std::string &text, const Rect &content_box, const ResourceManager &resources )
{
	Font font = get_font(canvas, resources);
	int baseline = content_box.top + font.get_font_metrics().get_ascent();
	return render_text(canvas, font, text, content_box, baseline, true);
}

}
