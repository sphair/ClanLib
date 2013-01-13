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
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_manager.h"
#include "API/CSSLayout/CSSDocument/css_property_value.h"
#include "gui_theme_part_impl.h"
#include "gui_component_select_node.h"
#include "gui_component_impl.h"
#include "gui_manager_impl.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/Font/font_metrics.h"
#include "CSSLayout/Layout/LayoutTree/css_background_renderer.h"
#include "CSSLayout/Layout/LayoutTree/css_border_renderer.h"
#include "CSSLayout/Layout/LayoutTree/css_layout_graphics.h"
#include "API/Display/2D/span_layout.h"

#include "CSSLayout/PropertyParsers/css_property_parser.h"
#include "CSSLayout/PropertyParsers/css_property_parsers.h"
#include "API/CSSLayout/CSSTokenizer/css_token.h"

namespace clan
{

GUIThemePart::GUIThemePart()
{
}

GUIThemePart::GUIThemePart(GUIComponent *parent, const std::string &tag_name)
: impl(new GUIThemePart_Impl(parent))
{
	impl->element.set_parent(&parent->impl->element);
	set_tag_name(tag_name);
}

void GUIThemePart::throw_if_null() const
{
	if (!impl)
		throw Exception("GUIThemePart is null");
}


Font GUIThemePart::get_font() const
{
	const CSSBoxProperties &properties = get_css_properties();
	Canvas canvas = impl->component->get_canvas();
	return GUIComponent_Impl::get_font(canvas, properties);
}

const CSSBoxProperties &GUIThemePart::get_css_properties() const
{
	return impl->element.get_css_properties();
}

CSSBoxProperties &GUIThemePart::get_css_properties()
{
	return impl->element.get_css_properties();
}

std::string GUIThemePart::get_tag_name() const
{
	return impl->element.get_tag_name();
}

std::string GUIThemePart::get_id() const
{
	return impl->element.get_id();
}

std::string GUIThemePart::get_class() const
{
	return impl->element.get_class();
}

bool GUIThemePart::get_pseudo_class(const std::string &name) const
{
	return impl->element.get_pseudo_class(name);
}

std::vector<std::string> GUIThemePart::get_pseudo_classes() const
{
	return impl->element.get_pseudo_classes();
}

void GUIThemePart::set_tag_name(const std::string &name)
{
	impl->element.set_tag_name(name);
	update_style();
}
	
void GUIThemePart::set_class(const std::string &name)
{
	impl->element.set_class(name);
	update_style();
}

void GUIThemePart::set_id(const std::string &name)
{
	impl->element.set_id(name);
	update_style();
}

bool GUIThemePart::set_pseudo_class(const std::string &name, bool enable)
{
	bool changed = impl->element.set_pseudo_class(name, enable);
	if (changed)
		update_style();
	return changed;
}

void GUIThemePart::update_style()
{
	CSSDocument document = impl->component->get_gui_manager().get_css_document();
	impl->element.update_style(&impl->component->impl->gui_manager_impl->resource_cache, document);
}

GUICSSUsedValues &GUIThemePart_Impl::get_css_used_values(const Rect &border_box)
{
	if ( element.style_updated() || (last_calculated_border_box != border_box ) )
	{
		last_calculated_border_box = border_box;
		GUICSSUsedValues parent_used_values = component->impl->css_used_values;
		parent_used_values.width = border_box.get_width();
		parent_used_values.height = border_box.get_height();
		GUICSSInitialUsedValues::visit(css_used_values, element.get_css_properties(), parent_used_values);
		GUICSSApplyMinMaxConstraints::visit(css_used_values, element.get_css_properties(), parent_used_values);
	}
	return css_used_values;
}

void GUIThemePart::render_box(Canvas &canvas, const Rect &border_box)
{
	GUICSSUsedValues &css_used_values = impl->get_css_used_values(border_box);

	Rect viewport = impl->component->get_top_level_component()->get_size();
	CSSResourceCache *resource_cache = &impl->component->impl->gui_manager_impl->resource_cache;
	CSSLayoutGraphics graphics(canvas, resource_cache, viewport, 0);

	Rect padding_box = Rect(border_box).shrink(css_used_values.border.left, css_used_values.border.top, css_used_values.border.right, css_used_values.border.bottom);
	Rect content_box = Rect(padding_box).shrink(css_used_values.padding.left, css_used_values.padding.top, css_used_values.padding.right, css_used_values.padding.bottom);

	CSSBackgroundRenderer background(&graphics, resource_cache, impl->element.get_css_properties());
	background.set_border_box(border_box);
	background.set_padding_box(padding_box);
	background.set_content_box(content_box);
	background.set_initial_containing_box(impl->component->get_top_level_component()->get_viewport());
	background.render();

	CSSBorderRenderer border(&graphics, resource_cache, impl->element.get_css_properties());
	border.set_border_box(border_box);
	border.set_border_values(css_used_values.border.left, css_used_values.border.top, css_used_values.border.right, css_used_values.border.bottom);
	border.render();
}

Size GUIThemePart::get_css_size() const
{
	return Size(impl->element.get_css_properties().width.length.value, impl->element.get_css_properties().height.length.value);
}

int GUIThemePart::get_css_width() const
{
	return impl->element.get_css_properties().width.length.value;
}

int GUIThemePart::get_css_height() const
{
	return impl->element.get_css_properties().height.length.value;
}

Rect GUIThemePart::get_render_text_span_box( Canvas &canvas, const std::string &str, const Rect &content_rect ) const
{
	Font font = GUIComponent_Impl::get_font(canvas, impl->element.get_css_properties());
	SpanLayout span = GUIComponent_Impl::create_span_layout(canvas, impl->element, font, str, content_rect);
	return span.get_rect();
}

Rect GUIThemePart::get_render_text_box( Canvas &canvas, const std::string &str, const Rect &content_rect ) const
{
	return GUIComponent_Impl::get_render_text_box(canvas, impl->element, str, content_rect);
}

Size GUIThemePart::get_render_text_size( Canvas &canvas, const std::string &str ) const
{
	// Note, the content box is not important here. But let us create one so "justify" alignment might report sane values
	return GUIComponent_Impl::get_render_text_box(canvas, impl->element, str, impl->component->get_content_box()).get_size();
}

VerticalTextPosition GUIThemePart::get_vertical_text_align(Canvas &canvas, const Rect &content_rect)
{
	Font font = get_font();
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

Rect GUIThemePart::render_text_span( Canvas &canvas, const std::string &text, const Rect &content_rect )
{
	Font font = GUIComponent_Impl::get_font(canvas, impl->element.get_css_properties());
	SpanLayout span = GUIComponent_Impl::create_span_layout(canvas, impl->element, font, text, content_rect);
	span.draw_layout(canvas);
	return span.get_rect();
}

Rect GUIThemePart::render_text( Canvas &canvas, const std::string &text, const Rect &content_box )
{
	Font font = GUIComponent_Impl::get_font(canvas, impl->element.get_css_properties());
	int baseline = content_box.top + font.get_font_metrics().get_ascent();
	return GUIComponent_Impl::render_text(canvas, impl->element, font, text, content_box, baseline, false);
}

Rect GUIThemePart::render_text( Canvas &canvas, const std::string &text, const Rect &content_box, int baseline )
{
	Font font = GUIComponent_Impl::get_font(canvas, impl->element.get_css_properties());
	return GUIComponent_Impl::render_text(canvas, impl->element, font, text, content_box, baseline, false);
}


Rect GUIThemePart::get_content_box(const Rect &render_box_rect) const
{
	GUICSSUsedValues &css_used_values = impl->get_css_used_values(render_box_rect);
	Rect box = render_box_rect;
	box.left += css_used_values.border.left + css_used_values.padding.left;
	box.right -= css_used_values.border.right + css_used_values.padding.right;
	box.top += css_used_values.border.top + css_used_values.padding.top;
	box.bottom -= css_used_values.border.bottom + css_used_values.padding.bottom;
	return box;
}

Rect GUIThemePart::get_border_box(const Rect &content_box_rect) const
{
	GUICSSUsedValues &css_used_values = impl->get_css_used_values(content_box_rect);
	Rect box = content_box_rect;
	box.left -= css_used_values.border.left + css_used_values.padding.left;
	box.right += css_used_values.border.right + css_used_values.padding.right;
	box.top -= css_used_values.border.top + css_used_values.padding.top;
	box.bottom += css_used_values.border.bottom + css_used_values.padding.bottom;
	return box;
}

Rect GUIThemePart::get_content_shrink_box() const
{
	Rect shrink_box;
	GUICSSUsedValues &css_used_values = impl->get_css_used_values(impl->last_calculated_border_box);
	shrink_box.left = css_used_values.border.left + css_used_values.padding.left;
	shrink_box.right = css_used_values.border.right + css_used_values.padding.right;
	shrink_box.top = css_used_values.border.top + css_used_values.padding.top;
	shrink_box.bottom = css_used_values.border.bottom + css_used_values.padding.bottom;
	return shrink_box;
}

std::string GUIThemePart::get_property(const std::string &property, const std::string &default_value) const
{
	throw Exception("GUIThemePart::get_property is currently under construction");
/*
	CSSDocument document = impl->component->get_gui_manager().get_css_document();
	GUIComponentSelectNode select_node(&impl->element);
	std::vector<CSSPropertyValue *> sheet_properties = document.select(&select_node);

	CSSPropertyParsers property_parsers;
	for (size_t i = sheet_properties.size(); i > 0; i--)
	{
		if (property == StringHelp::text_to_lower(sheet_properties[i-1].get_name()))
		{
			size_t pos = 0;
			CSSToken token = GUIComponent_Impl::next_token(pos, sheet_properties[i-1].get_value_tokens(), true);
			return token.value;
		}
	}

	return default_value;
*/
}

int GUIThemePart::get_property_int(const std::string &property, const std::string &default_value) const
{
	return StringHelp::text_to_int(get_property(property, default_value));
}

}
