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
#include "API/CSSLayout/css_property_list.h"
#include "gui_theme_part_impl.h"
#include "gui_component_select_node.h"
#include "gui_component_impl.h"
#include "gui_manager_impl.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/Font/font_metrics.h"
#include "CSSLayout/LayoutTree/css_background_renderer.h"
#include "CSSLayout/LayoutTree/css_border_renderer.h"
#include "CSSLayout/LayoutTree/css_layout_graphics.h"

namespace clan
{

GUIThemePart::GUIThemePart()
{
}

GUIThemePart::GUIThemePart(GUIComponent *parent, const std::string &pseudo_tag_name)
: impl(new GUIThemePart_Impl(parent))
{
	impl->element.set_parent(&parent->impl->element);
	impl->element.set_tag_name(pseudo_tag_name);
}

void GUIThemePart::throw_if_null() const
{
	if (!impl)
		throw Exception("GUIThemePart is null");
}


Font GUIThemePart::get_font() const
{
	return impl->component->get_font(get_css_properties());
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

void GUIThemePart::set_pseudo_class(const std::string &name, bool enable)
{
	if (impl->element.set_pseudo_class(name, enable))
		update_style();
}

void GUIThemePart::update_style()
{
	CSSDocument document = impl->component->get_gui_manager().get_css_document();
	impl->element.update_style(&impl->component->impl->gui_manager_impl->resource_cache, document);
}

void GUIThemePart::render_box(Canvas &canvas, const Rect &content_rect, const Rect &clip_rect)
{
	Rect viewport = impl->component->get_top_level_component()->get_size();
	CSSResourceCache *resource_cache = &impl->component->impl->gui_manager_impl->resource_cache;
	CSSLayoutGraphics graphics(canvas, resource_cache, viewport, 0);

	Rect border_box = content_rect;//impl->component->get_size();
	Rect padding_box = Rect(border_box);//.shrink(impl->css_used_values.border.left, impl->css_used_values.border.top, impl->css_used_values.border.right, impl->css_used_values.border.bottom);
	Rect content_box = Rect(padding_box);//.shrink(impl->css_used_values.padding.left, impl->css_used_values.padding.top, impl->css_used_values.padding.right, impl->css_used_values.padding.bottom);

	CSSBackgroundRenderer background(&graphics, resource_cache, impl->element.get_css_properties());
	background.set_border_box(border_box);
	background.set_padding_box(padding_box);
	background.set_content_box(content_box);
	background.set_initial_containing_box(content_box);
	background.render();

	CSSBorderRenderer border(&graphics, resource_cache, impl->element.get_css_properties());
	border.set_border_box(border_box);
	//border.set_border_values(impl->css_used_values.border.left, impl->css_used_values.border.top, impl->css_used_values.border.right, impl->css_used_values.border.bottom);
	border.render();

}

Size GUIThemePart::get_preferred_size() const
{
	return Size(impl->element.get_css_properties().width.length.value, impl->element.get_css_properties().height.length.value);
}

int GUIThemePart::get_preferred_width() const
{
	return impl->element.get_css_properties().width.length.value;
}

int GUIThemePart::get_preferred_height() const
{
	return impl->element.get_css_properties().height.length.value;
}

Size GUIThemePart::get_text_size( Canvas &canvas, const std::string &str ) const
{
	Font font = get_font();
	return font.get_text_size(canvas, str);
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
	//TODO Fixme - As CL_GUIThemeProvider_Default::render_text()
	font.draw_text(canvas, content_rect.left, content_rect.top, text, impl->element.get_css_properties().color.color);
	return Rect();
}

}
