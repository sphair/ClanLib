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
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message_pointer.h"
#include "API/Display/2D/image.h"
#include "Layout/gui_css_box_visitor.h"
#include "Layout/gui_layout_content.h"
#include "gui_component_impl.h"
#include "gui_manager_impl.h"
#include "gui_css_strings.h"
#include "gui_component_select_node.h"
#include "API/Display/2D/span_layout.h"
#include "../CSSLayout/Layout/LayoutTree/css_used_value.h"
#include "API/Display/Font/font_metrics.h"

namespace clan
{

GUIComponent_Impl::GUIComponent_Impl(const std::shared_ptr<GUIManager_Impl> &init_gui_manager, GUIComponent *parent_or_owner, bool toplevel)
: gui_manager(init_gui_manager), parent(0), prev_sibling(0), next_sibling(0), first_child(0), last_child(0),
  focus_policy(GUIComponent::focus_refuse), allow_resize(false), clip_children(false), enabled(true),
  visible(true), activated(false), default_handler(false), cancel_handler(false),
  constant_repaint(false), blocks_default_action_when_focused(false), is_selected_in_group(false), use_auto_geometry(true), double_click_enabled(true), pointer_inside_component(false)
{
	if (parent_or_owner)
		element.set_parent(&parent_or_owner->impl->element);

	gui_manager_impl = gui_manager.lock().get();

	if (!toplevel)
	{
		parent = parent_or_owner;
	}
}

GUIComponent_Impl *GUIComponent_Impl::create_from_parent(GUIComponent *parent)
{
	if (parent)
		return new GUIComponent_Impl(parent->get_gui_manager().impl, parent, false);
	else
		throw Exception("Cannot create child component with no parent!");
}

GUIComponent_Impl *GUIComponent_Impl::create_from_manager(GUIManager *manager)
{
	if (manager)
		return new GUIComponent_Impl(manager->impl, 0, true);
	else
		throw Exception("Cannot create component with a null manager!");
}

GUIComponent_Impl *GUIComponent_Impl::create_from_owner(GUIComponent *owner)
{
	if (owner)
		return new GUIComponent_Impl(owner->get_gui_manager().impl, owner, true);
	else
		throw Exception("Cannot create child component with a null owner!");
}

GUIComponent_Impl::~GUIComponent_Impl()
{
	while (last_child)
		delete last_child;

	if (prev_sibling)
		prev_sibling->impl->next_sibling = next_sibling;
	if (next_sibling)
		next_sibling->impl->prev_sibling = prev_sibling;
	
	if (prev_sibling == 0 && parent)
		parent->impl->first_child = next_sibling;
	if (next_sibling == 0 && parent)
		parent->impl->last_child = prev_sibling;

	gui_manager_impl->remove_component(this);
}

void GUIComponent_Impl::set_window_geometry(Rect box, bool client_area)
{
	if (parent == 0)
	{
		GUITopLevelWindow *handle = gui_manager.lock()->get_toplevel_window(component);
		Rect old_viewport = gui_manager.lock()->window_manager.get_geometry(handle, true);

		gui_manager.lock()->window_manager.set_geometry(handle, box, client_area);

		Rect new_viewport = gui_manager.lock()->window_manager.get_geometry(handle, true);
		if (old_viewport.get_size() != new_viewport.get_size())
		{
			window_resized();
		}
	}
}

void GUIComponent_Impl::window_resized()
{
	GUITopLevelWindow *handle = gui_manager.lock()->get_toplevel_window(component);
	Rect new_viewport = gui_manager.lock()->window_manager.get_geometry(handle, true);
	geometry = Rect(Point(), new_viewport.get_size());
	geometry_updated(true);
}

void GUIComponent_Impl::set_auto_geometry(Rect new_geometry)
{
	if (use_auto_geometry && geometry != new_geometry)
	{
		bool resized = geometry.get_size() != new_geometry.get_size();

		if (component->get_parent_component())
			component->get_parent_component()->request_repaint(geometry);

		geometry = new_geometry;
		geometry_updated(resized);
	}
}

void GUIComponent_Impl::set_manual_geometry(Rect new_geometry)
{
	if (use_auto_geometry || geometry != new_geometry)
	{
		use_auto_geometry = false;

		bool resized = geometry.get_size() != new_geometry.get_size();

		if (component->get_parent_component())
			component->get_parent_component()->request_repaint(geometry);

		geometry = new_geometry;
		geometry_updated(resized);

		component->update_layout();
	}
}

void GUIComponent_Impl::reset_geometry()
{
	if (!use_auto_geometry)
	{
		use_auto_geometry = true;

		component->update_layout();
	}
}

/*
Rect GUIComponent_Impl::get_geometry() const
{
	CSSUsedValue x = css_used_values.left + css_used_values.margin.left;
	CSSUsedValue y = css_used_values.top + css_used_values.margin.top;

	CSSUsedValue used_border_box_width = css_used_values.width + css_used_values.padding.left + css_used_values.padding.right + css_used_values.border.left + css_used_values.border.right;
	CSSUsedValue used_border_box_height = css_used_values.height + css_used_values.padding.top + css_used_values.padding.bottom + css_used_values.border.top + css_used_values.border.bottom;

	CSSActualValue x1 = (CSSActualValue)(x);
	CSSActualValue y1 = (CSSActualValue)(y);
	CSSActualValue x2 = (CSSActualValue)(x + used_border_box_width + 0.5f);
	CSSActualValue y2 = (CSSActualValue)(y + used_border_box_height + 0.5f);
	return Rect(x1, y1, x2, y2);
}

void GUIComponent_Impl::set_css_geometry(const Rect &new_geometry)
{
	// Set new geometry
	css_used_values.left = new_geometry.left - css_used_values.margin.left;
	css_used_values.top = new_geometry.top - css_used_values.margin.top;
	css_used_values.width = new_geometry.get_width() - (css_used_values.padding.left + css_used_values.padding.right + css_used_values.border.left + css_used_values.border.right);
	css_used_values.height = new_geometry.get_height() - (css_used_values.padding.top + css_used_values.padding.bottom + css_used_values.border.top + css_used_values.border.bottom);
}
*/

void GUIComponent_Impl::geometry_updated(bool geometry_was_resized)
{
	if (!layout.is_null())
		layout.set_geometry(geometry.get_size());

	if (geometry_was_resized)
	{
		if (!func_resized.is_null())
			func_resized.invoke();
	}

	component->request_repaint();
}

void GUIComponent_Impl::invoke_enablemode_changed()
{
	if (!func_enablemode_changed.is_null())
		func_enablemode_changed.invoke();

	GUIComponent *cur = first_child;
	while (cur)
	{
		cur->impl->invoke_enablemode_changed();
		cur = cur->get_next_sibling();
	}
}

void GUIComponent_Impl::layout_content()
{
	GUILayoutContent visitor;
	visitor.node(this);
}

void GUIComponent_Impl::visit_children(GUICSSBoxVisitor *visitor, bool recursive)
{
	for (GUIComponent *child = first_child; child != 0; child = child->get_next_sibling())
	{
		visitor->node(child->impl.get());
		if (recursive)
			child->impl->visit_children(visitor, true);
	}
}

void GUIComponent_Impl::on_process_message(std::shared_ptr<GUIMessage> &msg)
{
	if (!enabled)
		return;

	std::shared_ptr<GUIMessage_Pointer> pointer = std::dynamic_pointer_cast<GUIMessage_Pointer>(msg);
	if (pointer)
	{
		if (pointer->pointer_type == GUIMessage_Pointer::pointer_enter)
		{
			component->set_pseudo_class(CssStr::hot, true);
			//msg->consumed = true;	<-- Disabled, else the LineEdit ibeam cursor is not shown
		}
		else if (pointer->pointer_type == GUIMessage_Pointer::pointer_leave)
		{
			component->set_pseudo_class(CssStr::hot, false);
			//msg->consumed = true;	<-- Disabled, else the LineEdit ibeam cursor is not shown
		}
	}
}

CSSToken GUIComponent_Impl::next_token(size_t &pos, const std::vector<CSSToken> &tokens, bool skip_whitespace)
{
	CSSToken token;
	do
	{
		if (pos != tokens.size())
		{
			token = tokens[pos];
			pos++;
		}
		else
		{
			token = CSSToken();
		}
	} while (token.type == CSSToken::type_whitespace);
	return token;
}

SpanLayout GUIComponent_Impl::create_span_layout( Canvas &canvas, GUIElement &element, Font &font, const std::string &text, const Rect &content_rect )
{
	SpanLayout span;

	const CSSComputedBox &properties = element.get_css_values().get_box();

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

Font GUIComponent_Impl::get_font(Canvas &canvas, const CSSComputedBox &properties)
{
	int font_size = used_to_actual(properties.font_size.length.value);
	std::string font_name;
	for (size_t i = 0; i < properties.font_family.names.size(); i++)
	{
		bool matched = false;
		std::string search_name;
		switch (properties.font_family.names[i].type)
		{
		case CSSValueFontFamilyName::type_family_name:
			search_name = StringHelp::text_to_lower(properties.font_family.names[i].name);
			//FIXME: See CSSResourceCache, creating font_families list
			//if (font_families.find(search_name) != font_families.end())
			//{
				font_name = properties.font_family.names[i].name;
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
	switch (properties.font_weight.type)
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
	switch (properties.font_style.type)
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
	return Font(canvas, font_desc);

}

Rect GUIComponent_Impl::render_text( Canvas &canvas, GUIElement &element, Font &font, const std::string &text, const Rect &content_box, int baseline, bool calculate_text_rect_only )
{
	const CSSComputedBox &properties = element.get_css_values().get_box();

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

Rect GUIComponent_Impl::get_render_text_box( Canvas &canvas, GUIElement &element, const std::string &text, const Rect &content_box )
{
	Font font = GUIComponent_Impl::get_font(canvas, element.get_css_values().get_box());
	int baseline = content_box.top + font.get_font_metrics().get_ascent();
	return render_text(canvas, element, font, text, content_box, baseline, true);
}

}
