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
**    Harry Storbacka
*/

#include "GUI/precomp.h"
#include "API/Core/IOData/file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/XML/dom_document.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/XML/dom_text.h"
#include "API/Core/Text/string_help.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Window/input_context.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/2D/span_layout.h"
#include "API/Display/Font/font_metrics.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_layout_corners.h"
#include "API/GUI/gui_window_manager.h"
#include "API/CSSLayout/CSSDocument/css_document.h"
#include "API/CSSLayout/CSSDocument/css_property_value.h"
#include "API/CSSLayout/CSSTokenizer/css_token.h"
#include "CSSLayout/Layout/LayoutTree/css_background_renderer.h"
#include "CSSLayout/Layout/LayoutTree/css_border_renderer.h"
#include "CSSLayout/Layout/LayoutTree/css_layout_graphics.h"
#include "CSSLayout/PropertyParsers/css_property_parser.h"
#include "CSSLayout/PropertyParsers/css_property_parsers.h"
#include "gui_component_impl.h"
#include "gui_manager_impl.h"
#include "gui_xml_loader_version_1_0.h"
#include "gui_component_select_node.h"
#include "gui_element.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GUIComponent Construction:

GUIComponent::GUIComponent(GUIComponent *parent, const std::string &tag_name)
: impl(GUIComponent_Impl::create_from_parent(parent))
{
	impl->component = this;
	impl->element.set_component(this);

	if (impl->parent->impl->last_child)
	{
		impl->parent->impl->last_child->impl->next_sibling = this;
		impl->prev_sibling = impl->parent->impl->last_child;
		impl->parent->impl->last_child = this;
	}
	else
	{
		impl->parent->impl->first_child = this;
		impl->parent->impl->last_child = this;
	}
	set_tag_name(tag_name);

}

GUIComponent::GUIComponent(GUIManager *manager, const GUITopLevelDescription &description, const std::string &tag_name)
: impl(GUIComponent_Impl::create_from_manager(manager))
{
	impl->component = this;
	impl->element.set_component(this);

	impl->allow_resize = description.get_allow_resize();
	impl->visible = description.is_visible();
	impl->gui_manager.lock()->add_component(this, 0, description);
	impl->window_resized();
	set_tag_name(tag_name);

	request_repaint();
}

GUIComponent::GUIComponent(GUIComponent *owner, const GUITopLevelDescription &description, const std::string &tag_name)
: impl(GUIComponent_Impl::create_from_owner(owner))
{
	impl->component = this;
	impl->element.set_component(this);

	impl->allow_resize = description.get_allow_resize();
	impl->visible = description.is_visible();
	impl->gui_manager.lock()->add_component(this, owner, description);
	impl->window_resized();
	set_tag_name(tag_name);

	request_repaint();
}

GUIComponent::~GUIComponent()
{
}

/////////////////////////////////////////////////////////////////////////////
// GUIComponent Attributes:

Rect GUIComponent::get_geometry() const
{
	return impl->geometry;
}

Rect GUIComponent::get_viewport() const
{
	if (impl->parent)
	{
		return get_top_level_component()->get_viewport();
	}
	else
	{
		return impl->gui_manager.lock()->window_manager.get_geometry(impl->gui_manager.lock()->get_toplevel_window(this), true);
	}
}

Rect GUIComponent::get_content_box() const
{
	Rect box = Rect(Point(), impl->geometry.get_size());
	box.left += impl->css_used_values.border.left + impl->css_used_values.padding.left;
	box.right -= impl->css_used_values.border.right + impl->css_used_values.padding.right;
	box.top += impl->css_used_values.border.top + impl->css_used_values.padding.top;
	box.bottom -= impl->css_used_values.border.bottom + impl->css_used_values.padding.bottom;
	return box;
}

Rect GUIComponent::get_window_geometry() const
{
	if (impl->parent == 0)
	{
		return impl->gui_manager.lock()->window_manager.get_geometry(
			impl->gui_manager.lock()->get_toplevel_window(this), false);
	}
	else
	{
		throw Exception("Component is not a top-level component");
	}
}

int GUIComponent::get_width() const
{
	return impl->geometry.get_width();
}

int GUIComponent::get_height() const
{
	return impl->geometry.get_height();
}

Size GUIComponent::get_size() const
{
	return impl->geometry.get_size();
}

std::string GUIComponent::get_tag_name() const
{
	return impl->element.get_tag_name();
}

std::string GUIComponent::get_id() const
{
	return impl->element.get_id();
}

bool GUIComponent::get_class(const std::string &name) const
{
	return impl->element.get_class(name);
}

std::vector<std::string> GUIComponent::get_classes() const
{
	return impl->element.get_classes();
}

bool GUIComponent::get_pseudo_class(const std::string &name) const
{
	return impl->element.get_pseudo_class(name);
}

std::vector<std::string> GUIComponent::get_pseudo_classes() const
{
	return impl->element.get_pseudo_classes();
}

const CSSComputedValues &GUIComponent::get_css_values() const
{
	return impl->element.get_css_values();
}

bool GUIComponent::has_focus() const
{
	return impl->gui_manager.lock()->has_focus(this);
}

GUIComponent::FocusPolicy GUIComponent::get_focus_policy() const
{
	return impl->focus_policy;
}

std::string GUIComponent::get_component_group_name() const
{
	return impl->group_name;
}

bool GUIComponent::is_selected_in_group() const
{
	return impl->is_selected_in_group;
}

bool GUIComponent::get_blocks_default_action() const
{
	return impl->blocks_default_action_when_focused;
}

ResourceManager GUIComponent::get_resources() const
{
	return impl->gui_manager.lock()->resources;
}

GUIManager GUIComponent::get_gui_manager() const
{
	return GUIManager(impl->gui_manager.lock());
}

const GUIComponent *GUIComponent::get_parent_component() const
{
	return impl->parent;
}

GUIComponent *GUIComponent::get_parent_component()
{
	return impl->parent;
}

const GUIComponent *GUIComponent::get_owner_component() const
{
	return impl->gui_manager.lock()->get_owner_component(this);
}

GUIComponent *GUIComponent::get_owner_component()
{
	return impl->gui_manager.lock()->get_owner_component(this);
}

std::vector<GUIComponent *> GUIComponent::get_child_components() const
{
	std::vector<GUIComponent *> children;
	GUIComponent *cur_child = impl->first_child;
	while (cur_child)
	{
		children.push_back(cur_child);
		cur_child = cur_child->get_next_sibling();
	}
	return children;
}

std::vector<GUIComponent*> GUIComponent::get_child_component_group(const std::string &group_name) const
{
	std::vector<GUIComponent *> group;
	GUIComponent *cur_child = impl->first_child;
	while (cur_child)
	{
		if (cur_child->get_component_group_name() == group_name)
		{
			group.push_back(cur_child);
		}
		cur_child = cur_child->get_next_sibling();
	}
	return group;
}

GUIComponent *GUIComponent::get_group_selected_component()
{
	GUIComponent *parent = get_parent_component();

	if (parent)
	{
		std::vector<GUIComponent*> group = parent->get_child_component_group(get_component_group_name());
		std::vector<GUIComponent*>::size_type i;
		for (i = 0; i < group.size(); i++)
		{
			if (group[i]->is_selected_in_group())
			{
				return group[i];
			}
		}
	}

	return 0;
}

const GUIComponent *GUIComponent::get_first_child() const
{
	return impl->first_child;
}

GUIComponent *GUIComponent::get_first_child()
{
	return impl->first_child;
}

const GUIComponent *GUIComponent::get_last_child() const
{
	return impl->last_child;
}

GUIComponent *GUIComponent::get_last_child()
{
	return impl->last_child;
}

const GUIComponent *GUIComponent::get_previous_sibling() const
{
	return impl->prev_sibling;
}

GUIComponent *GUIComponent::get_previous_sibling()
{
	return impl->prev_sibling;
}

const GUIComponent *GUIComponent::get_next_sibling() const
{
	return impl->next_sibling;
}

bool GUIComponent::is_descendant_of(GUIComponent *component)
{
	if(impl->parent == 0)
		return false;

	if(impl->parent == component)
		return true;

	return impl->parent->is_descendant_of(component);
}

bool GUIComponent::is_ancestor_of(GUIComponent *component)
{
	GUIComponent *cur_child = impl->first_child;
	while (cur_child)
	{
		if(cur_child == component)
			return true;

		if(cur_child->is_ancestor_of(component))
			return true;

		cur_child = cur_child->get_next_sibling();
	}

	return false;
}

GUIComponent *GUIComponent::get_next_sibling()
{
	return impl->next_sibling;
}

bool GUIComponent::has_child_components() const
{
	return (impl->first_child != 0);
}

Canvas GUIComponent::get_canvas() const
{
	const GUIComponent *root_component = get_top_level_component();

	std::vector<GUITopLevelWindow>::size_type pos, size;
	size = impl->gui_manager.lock()->root_components.size();
	for (pos = 0; pos < size; pos++)
	{
		GUITopLevelWindow *cur = impl->gui_manager.lock()->root_components[pos];
		if (cur->component == root_component)
			return impl->gui_manager.lock()->window_manager.get_canvas(cur);
	}
	throw Exception("Canvas not found");
}

InputContext GUIComponent::get_ic()
{
	const GUIComponent *root_component = get_top_level_component();

	std::vector<GUITopLevelWindow>::size_type pos, size;
	size = impl->gui_manager.lock()->root_components.size();
	for (pos = 0; pos < size; pos++)
	{
		GUITopLevelWindow *cur = impl->gui_manager.lock()->root_components[pos];
		if (cur->component == root_component)
			return impl->gui_manager.lock()->window_manager.get_ic(cur);
	}
	throw Exception("Input Context not found");
}

bool GUIComponent::is_enabled() const
{
	const GUIComponent *parent = get_parent_component();
	if (parent)
	{
		if (parent->is_enabled() == false)
			return false;
	}
	return impl->enabled;
}

bool GUIComponent::is_visible() const
{
	const GUIComponent *parent = get_parent_component();
	if (parent)
	{
		if (parent->is_visible() == false)
			return false;
	}
	return impl->visible;
}

bool GUIComponent::is_active() const
{
	return impl->activated;
}

GUIComponent *GUIComponent::get_component_at(const Point &point)
{
	if (is_visible() == false)
	{
		return 0;
	}
	else
	{
		Rect parent_rect = get_size();
		if( parent_rect.contains(point) )  
		{
			std::vector<GUIComponent *> children = get_child_components();
			std::size_t pos, size = children.size();

			for( pos=size; pos>0; pos-- )
			{
				GUIComponent *child = children[pos-1];
				if(child->is_visible())
				{
					if (child->get_geometry().contains(point))
					{
						Point P = point;
						P.x -= child->get_geometry().left;
						P.y -= child->get_geometry().top;
						GUIComponent *comp = child->get_component_at(P);
						if (comp)
							return comp;
						else
							return child;
					}
				}
			}
			return this;
		}
		else
		{
			return 0;
		}
	}
}

Point GUIComponent::window_to_component_coords(const Point &point) const
{
	Point P = point;

	const GUIComponent *current = this;
	while (current->get_parent_component())
	{
		Rect g = current->get_geometry();
		P.x -= g.left;
		P.y -= g.top;
		current = current->get_parent_component();
	}

	return P;
}

Rect GUIComponent::window_to_component_coords(const Rect &rect) const
{
	Point tl = window_to_component_coords(rect.get_top_left());
	Point br = window_to_component_coords(rect.get_bottom_right());
	return Rect(tl.x, tl.y, br.x, br.y);
}

Point GUIComponent::component_to_window_coords(const Point &point) const
{
	Point P = point;

	const GUIComponent *current = this;
	while (current->get_parent_component())
	{
		Rect g = current->get_geometry();
		P.x += g.left;
		P.y += g.top;
		current = current->get_parent_component();
	}

	return P;
}

Rect GUIComponent::component_to_window_coords(const Rect &rect) const
{
	Point tl = component_to_window_coords(rect.get_top_left());
	Point br = component_to_window_coords(rect.get_bottom_right());
	return Rect(tl.x, tl.y, br.x, br.y);
}

Point GUIComponent::screen_to_component_coords(const Point &screen_point) const
{
	GUITopLevelWindow *toplevel_window = impl->gui_manager.lock()->get_toplevel_window(this);
	Point client_point = impl->gui_manager.lock()->window_manager.screen_to_window(toplevel_window, screen_point, true);
	return window_to_component_coords(client_point);
}

Point GUIComponent::component_to_screen_coords(const Point &component_point) const
{
	GUITopLevelWindow *toplevel_window = impl->gui_manager.lock()->get_toplevel_window(this);
	Point client_point = component_to_window_coords(component_point);
	return impl->gui_manager.lock()->window_manager.window_to_screen(toplevel_window, client_point, true);
}

GUIComponent *GUIComponent::get_top_level_component()
{
	GUIComponent *parent = this;

	while (true)
	{
		if (parent->get_parent_component())
			parent = parent->get_parent_component();
		else 
			break;
	}

	return parent;
}

const GUIComponent *GUIComponent::get_top_level_component() const
{
	const GUIComponent *parent = this;

	while (true)
	{
		if (parent->get_parent_component())
			parent = parent->get_parent_component();
		else 
			break;
	}

	return parent;
}

GUITopLevelWindow *GUIComponent::get_top_level_window() const
{
	return impl->gui_manager.lock()->get_toplevel_window(this);
}

bool GUIComponent::get_allow_resize() const
{
	return impl->allow_resize;
}

bool GUIComponent::get_clip_children() const
{
	return impl->clip_children;
}

float GUIComponent::get_preferred_content_width()
{
	return 0.0f;
}

float GUIComponent::get_preferred_content_height(float content_width)
{
	return 0.0f;
}

GUILayout GUIComponent::get_layout() const
{
	return impl->layout;
}

DisplayWindow GUIComponent::get_display_window() const
{
	GUITopLevelWindow *toplevel = impl->gui_manager.lock()->get_toplevel_window(this);
	return impl->gui_manager.lock()->window_manager.get_display_window(toplevel);
}

bool GUIComponent::get_constant_repaint() const
{
	return impl->constant_repaint;
}

/////////////////////////////////////////////////////////////////////////////
// GUIComponent Events:

Callback_v2<Canvas &, const Rect &> &GUIComponent::func_render()
{
	return impl->func_render;
}

Callback_v1<std::shared_ptr<GUIMessage> &> &GUIComponent::func_process_message()
{
	return impl->func_process_message;
}

Callback_0<bool> &GUIComponent::func_close()
{
	return impl->func_close;
}

Callback_0<bool> &GUIComponent::func_activated()
{
	return impl->func_activated;
}

Callback_0<bool> &GUIComponent::func_deactivated()
{
	return impl->func_deactivated;
}

Callback_0<bool> &GUIComponent::func_focus_lost()
{
	return impl->func_focus_lost;
}

Callback_v1<std::shared_ptr<GUIMessage> &> &GUIComponent::func_filter_message()
{
	return impl->func_filter_message;
}

Callback_0<bool> &GUIComponent::func_focus_gained()
{
	return impl->func_focus_gained;
}

Callback_0<bool> &GUIComponent::func_pointer_enter()
{
	return impl->func_pointer_enter;
}

Callback_0<bool> &GUIComponent::func_pointer_exit()
{
	return impl->func_pointer_exit;
}

Callback_1<bool, const InputEvent &> &GUIComponent::func_input()
{
	return impl->func_input;
}

Callback_1<bool, const InputEvent &> &GUIComponent::func_input_pressed()
{
	return impl->func_input_pressed;
}

Callback_1<bool, const InputEvent &> &GUIComponent::func_input_released()
{
	return impl->func_input_released;
}

Callback_1<bool, const InputEvent &> &GUIComponent::func_input_doubleclick()
{
	return impl->func_input_doubleclick;
}

Callback_1<bool, const InputEvent &> &GUIComponent::func_input_pointer_moved()
{
	return impl->func_input_pointer_moved;
}

Callback_v1<bool> &GUIComponent::func_visibility_change()
{
	return impl->func_visibility_change;
}

Signal_v0 &GUIComponent::sig_style_changed()
{
	return impl->element.sig_style_changed;
}

Callback_v0 &GUIComponent::func_enablemode_changed()
{
	return impl->func_enablemode_changed;
}

Callback_v0 &GUIComponent::func_resized()
{
	return impl->func_resized;
}

Callback_2<GUIComponent*,GUIComponent*, std::string> &GUIComponent::func_create_custom_component()
{
	return impl->func_create_custom_component;
}

bool GUIComponent::is_default()
{
	return impl->default_handler;
}

bool GUIComponent::is_cancel()
{
	return impl->cancel_handler;
}

bool GUIComponent::is_double_click_enabled() const
{
	return impl->double_click_enabled;
}

Font GUIComponent::get_font() const
{
	Canvas canvas = get_canvas();
	return impl->element.get_font(canvas, get_resources());
}

/////////////////////////////////////////////////////////////////////////////
// GUIComponent Operations:

void GUIComponent::render(Canvas &canvas, const Rect &clip_rect, bool include_children)
{
	if (!impl->visible)
		return;

	Rect viewport = get_top_level_component()->get_size();
	CSSResourceCache *resource_cache = &impl->gui_manager_impl->resource_cache;
	CSSLayoutGraphics graphics(canvas, resource_cache, viewport, 0);

	Rect border_box = get_size();
	Rect padding_box = Rect(border_box).shrink(impl->css_used_values.border.left, impl->css_used_values.border.top, impl->css_used_values.border.right, impl->css_used_values.border.bottom);
	Rect content_box = Rect(padding_box).shrink(impl->css_used_values.padding.left, impl->css_used_values.padding.top, impl->css_used_values.padding.right, impl->css_used_values.padding.bottom);

	CSSBackgroundRenderer background(&graphics, resource_cache, impl->element.get_css_values().get_background());
	background.set_border_box(border_box);
	background.set_padding_box(padding_box);
	background.set_content_box(content_box);
	background.set_initial_containing_box(get_top_level_component()->get_viewport());
	background.render();

	CSSBorderRenderer border(&graphics, resource_cache, impl->element.get_css_values().get_border());
	border.set_border_box(border_box);
	border.set_border_values(impl->css_used_values.border.left, impl->css_used_values.border.top, impl->css_used_values.border.right, impl->css_used_values.border.bottom);
	border.render();

	if (impl->func_render.is_null() == false)
	{
		impl->func_render.invoke(canvas, clip_rect);
	}
	else
	{
		//GUIThemePart part(this);
		//Rect geometry = get_size();
		//part.render_box(canvas, RectPS(0, 0, geometry.get_width(), geometry.get_height()), clip_rect);
	}

	if (include_children)
	{
		if (impl->clip_children)
		{
			push_cliprect(canvas, impl->clip_children_rect);
		}

		GUIComponent *cur = impl->first_child;
		while (cur)
		{
			Rect cur_geometry = cur->get_geometry();

			Rect update_rect = component_to_window_coords(clip_rect);
			update_rect.overlap(component_to_window_coords(cur_geometry));
			if (update_rect.get_width() > 0 && update_rect.get_height() > 0)
			{
				Rect child_dirty_rect = cur->window_to_component_coords(update_rect);

				canvas.push_translate((float)cur_geometry.left, (float)cur_geometry.top);
				cur->render(canvas, child_dirty_rect, true);
				canvas.pop_modelview();
			}
			cur = cur->impl->next_sibling;
		}

		if (impl->clip_children)
		{
			pop_cliprect(canvas);
		}
	}
}

void GUIComponent::paint()
{
	paint(get_size());
}

void GUIComponent::paint(const Rect &clip_rect)
{
	Rect update_region = component_to_window_coords(clip_rect);
	GUIComponent *toplevel_component = get_top_level_component();
	GUITopLevelWindow *toplevel_window = impl->gui_manager.lock()->get_toplevel_window(this);

	Canvas canvas = impl->gui_manager.lock()->window_manager.begin_paint(toplevel_window, update_region);
	toplevel_component->render(canvas, update_region, true);
	impl->gui_manager.lock()->window_manager.end_paint(canvas, toplevel_window, update_region);
}

int GUIComponent::exec()
{
	GUIComponent *owner_component = get_owner_component();
	if (owner_component)
		owner_component->get_top_level_component()->set_enabled(false);
	get_gui_manager().exec();
	if (owner_component)
	{
		// To do: Move this special modal dialog code to the window manager.
		// Important: Do not remove any of these calls even though they seem pointless, redundant or even wrong.
		// They workaround various bugs in the focus handling in Windows itself.
#ifdef WIN32
		set_visible(false, false);
#endif
		owner_component->get_top_level_component()->set_enabled(true);
#ifdef WIN32
		SetActiveWindow(owner_component->get_top_level_component()->get_display_window().get_hwnd());
#endif
	}
	return get_gui_manager().get_exit_code();
}

void GUIComponent::exit_with_code(int exit_code)
{
	get_gui_manager().exit_with_code(exit_code);
}

void GUIComponent::set_geometry(Rect geometry)
{
	impl->set_manual_geometry(geometry);
}

void GUIComponent::reset_geometry()
{
	impl->reset_geometry();
}

void GUIComponent::set_window_geometry(Rect geometry, bool client_area)
{
	impl->set_window_geometry(geometry, client_area);
}

void GUIComponent::capture_mouse(bool capture)
{
	if (capture)
	{
		get_gui_manager().set_capture_component(this, true);
	}
	else
	{
		if (get_gui_manager().get_capture_component() == this)
			get_gui_manager().set_capture_component(this, false);	
	}
}

void GUIComponent::capture_proximity(bool capture)
{
	if (capture)
	{
		get_gui_manager().set_tablet_proximity_component(this, true);
	}
	else
	{
		get_gui_manager().set_tablet_proximity_component(this, false);	
	}
}

void GUIComponent::set_tag_name(const std::string &name)
{
	impl->element.set_tag_name(name);
}
	
bool GUIComponent::set_class(const std::string &name, bool enable)
{
	return impl->element.set_class(name, enable, true);
}

void GUIComponent::set_id(const std::string &name)
{
	impl->element.set_id(name);
}

void GUIComponent::set_style(const std::string &name)
{
	impl->element.set_style(name);
}

void GUIComponent::set_style(const CSSPropertyValue &value, bool enable)
{
	impl->element.set_style(value, enable);
}

bool GUIComponent::set_pseudo_class(const std::string &name, bool enable)
{
	return impl->element.set_pseudo_class(name, enable, true);
}

void GUIComponent::update_layout()
{
	if (impl->parent)
	{
		impl->parent->impl->layout_content();
		impl->parent->request_repaint();
	}
	else
	{
		impl->layout_content();
		request_repaint();
	}

	// ** Use this instead if impl->layout_content() "base visitor" flag to visit_css() does not work as expected **
	//  get_top_level_component()->impl->layout_content();
	//   get_top_level_component()->request_repaint();
}

void GUIComponent::set_enabled(bool enable)
{
	if (impl->enabled != enable)
	{
		impl->enabled = enable;
		impl->invoke_enablemode_changed();
		if (impl->parent == 0)
			impl->gui_manager.lock()->set_enabled(this, enable);
	}
}

void GUIComponent::set_visible(bool visible, bool activate_root_win)
{
	bool visibility_changed = visible != impl->visible;
	if (visibility_changed)
	{
		request_repaint();
		if (!impl->func_visibility_change.is_null())
			impl->func_visibility_change.invoke(visible);
	}

	impl->visible = visible;

	if (visibility_changed)
	{
		get_top_level_component()->update_layout();
	}

	if (impl->parent == 0)
	{
		GUITopLevelWindow *toplevel_window = impl->gui_manager.lock()->get_toplevel_window(this);
		impl->gui_manager.lock()->window_manager.set_visible(toplevel_window, visible, activate_root_win);
	}
}

void GUIComponent::set_focus(bool enable)
{
	if (enable)
	{
		impl->gui_manager.lock()->gain_focus(this);
	}
	else
	{
		impl->gui_manager.lock()->loose_focus(this);
	}
}
	
void GUIComponent::set_focus_policy(FocusPolicy policy)
{
	impl->focus_policy = policy;
}

void GUIComponent::set_component_group_name(const std::string &str)
{
	impl->group_name = str;
}

GUIComponent *GUIComponent::get_named_item(const std::string &id)
{
	GUIComponent *cur = impl->first_child;
	while (cur && cur != this)
	{
		if (cur->get_id() == id)
			return cur;

		if (cur->impl->first_child)
			cur = cur->impl->first_child;
		else if (cur->impl->next_sibling)
			cur = cur->impl->next_sibling;
		else
		{
			while (cur != this)
			{
				if (cur->impl->parent->impl->next_sibling)
				{
					cur = cur->impl->parent->impl->next_sibling;
					break;
				}
				else
					cur = cur->impl->parent;
			}
		}
	}

	return 0;
}

void GUIComponent::create_components(const DomDocument &gui_xml)
{
	DomDocument const_hack = gui_xml;

	// Check if loaded document uses namespaces and if its a clanlib resources xml document:
	DomElement doc_element = const_hack.get_document_element();
	if (doc_element.get_namespace_uri() == "http://clanlib.org/xmlns/gui-1.0")
	{
		if (doc_element.get_local_name() != "gui")
			throw Exception("ClanLib GUI XML documents must begin with a gui element.");

		GUIXMLLoaderVersion_1_0 loader(this, impl->layout);
		loader.set_create_custom_callback(&impl->func_create_custom_component);
		loader.load(const_hack);
	}
	else
	{
		throw Exception("XML document is not a ClanLib GUI XML document.");
	}
}

void GUIComponent::create_components(const std::string &filename, const FileSystem &fs)
{
	IODevice device;
	device = fs.open_file(filename);
	create_components(device);
}

void GUIComponent::create_components(IODevice &file)
{
	DomDocument doc;
	doc.load(file); 
	create_components(doc);
}

void GUIComponent::create_components(const std::string &fullname)
{
	std::string path = PathHelp::get_fullpath(fullname, PathHelp::path_type_file);
	std::string filename = PathHelp::get_filename(fullname, PathHelp::path_type_file);
	FileSystem vfs(path);
	create_components(filename, vfs);
}

void GUIComponent::request_repaint(Rect request_repaint)
{
	get_gui_manager().request_repaint(component_to_window_coords(request_repaint), get_top_level_component());
}

void GUIComponent::request_repaint()
{
	request_repaint(get_size());
}

void GUIComponent::set_cliprect(Canvas &canvas, const Rect &rect)
{
	Rect windcliprect = component_to_window_coords(rect);
	GUIComponent *toplevel = get_top_level_component();
	GUITopLevelWindow *window = impl->gui_manager_impl->get_toplevel_window(toplevel);
	impl->gui_manager_impl->window_manager.set_cliprect(window, canvas, windcliprect);
}

void GUIComponent::reset_cliprect(Canvas &canvas)
{
	GUIComponent *toplevel = get_top_level_component();
	GUITopLevelWindow *window = impl->gui_manager_impl->get_toplevel_window(toplevel);
	impl->gui_manager_impl->window_manager.reset_cliprect(window, canvas);
}

void GUIComponent::push_cliprect(Canvas &canvas, const Rect &rect)
{
	Rect windcliprect = component_to_window_coords(rect);
	GUIComponent *toplevel = get_top_level_component();
	GUITopLevelWindow *window = impl->gui_manager_impl->get_toplevel_window(toplevel);
	impl->gui_manager_impl->window_manager.push_cliprect(window, canvas, windcliprect);
}

void GUIComponent::pop_cliprect(Canvas &canvas)
{
	GUIComponent *toplevel = get_top_level_component();
	GUITopLevelWindow *window = impl->gui_manager_impl->get_toplevel_window(toplevel);
	impl->gui_manager_impl->window_manager.pop_cliprect(window, canvas);
}

void GUIComponent::delete_child_components()
{
	while (impl->last_child)
	{
		delete impl->last_child;
	}

	impl->first_child = 0;
	impl->last_child = 0;
}

void GUIComponent::set_parent_component(GUIComponent *new_parent)
{
	if (new_parent == impl->parent)
	{
		return;
	}

	// 1. Remove this component from parents child list.
	if (impl->parent->impl->first_child == impl->parent->impl->last_child)
	{
		impl->parent->impl->first_child = 0;
		impl->parent->impl->last_child = 0;
	}
	else if (impl->parent->impl->first_child == this)
	{
		impl->parent->impl->first_child = impl->next_sibling;
		impl->next_sibling->impl->prev_sibling = 0;
	}
	else if (impl->parent->impl->last_child == this)
	{
		impl->prev_sibling->impl->next_sibling = 0;
		impl->parent->impl->last_child = impl->prev_sibling;
	}
	else
	{
		impl->prev_sibling->impl->next_sibling = impl->next_sibling;
		impl->next_sibling->impl->prev_sibling = impl->prev_sibling;
	}

	// 2. Set this component as last child of new parent.
	if (new_parent->impl->first_child == 0)
	{
		new_parent->impl->first_child = this;
		impl->prev_sibling = 0;
	}
	else if (new_parent->impl->last_child != 0)
	{
		new_parent->impl->last_child->impl->next_sibling = this;
		impl->prev_sibling = new_parent->impl->last_child;
	}
	impl->next_sibling = 0;
	new_parent->impl->last_child = this;
	impl->parent = new_parent;
}

void GUIComponent::set_layout(GUILayout &layout)
{
	impl->layout = layout;
}

void GUIComponent::set_cursor(const Cursor &cursor)
{
	GUITopLevelWindow *toplevel = impl->gui_manager.lock()->get_toplevel_window(this);
	impl->gui_manager.lock()->window_manager.set_cursor(toplevel, cursor);
}

void GUIComponent::set_cursor(StandardCursor type)
{
	GUITopLevelWindow *toplevel = impl->gui_manager.lock()->get_toplevel_window(this);
	impl->gui_manager.lock()->window_manager.set_cursor(toplevel, type);
}

void GUIComponent::set_clip_children(bool clip, const Rect &rect)
{
	impl->clip_children = clip;
	if (rect.get_width() == 0 || rect.get_height() == 0)
		impl->clip_children_rect = get_size();
	else
		impl->clip_children_rect = rect;
}

void GUIComponent::focus_next()
{
	GUIComponent *c = this;

	// Skip over components in the same group:
	if (get_focus_policy() == focus_group)
	{
		while (c->impl->next_sibling && c->get_component_group_name() == this->get_component_group_name())
		{
			c = c->impl->next_sibling;
		}
	}
	else
		c = c->get_next_component_in_tree();

	while (c != this)
	{
		if ((c->get_focus_policy() == focus_local || c->get_focus_policy() == focus_group) && c->is_visible() && c->is_enabled())
		{
			if (c->get_focus_policy() == focus_group)
			{
				GUIComponent *selected_comp = c->get_group_selected_component();
				if (selected_comp)
				{
					selected_comp->set_focus(true);
				}
				else
				{
					c->set_focus(true);
				}
			}
			else
			{
				c->set_focus(true);
			}

			break;
		}

		c = c->get_next_component_in_tree();
	}
}

void GUIComponent::focus_previous()
{
	GUIComponent *c = this;

	// Skip over components in the same group:
	if (get_focus_policy() == focus_group)
	{
		while (c->impl->prev_sibling && c->get_component_group_name() == this->get_component_group_name())
		{
			c = c->impl->prev_sibling;
		}
	}
	else
		c = c->get_previous_component_in_tree();

	while (c != this)
	{
		if ((c->get_focus_policy() == focus_local || c->get_focus_policy() == focus_group) && c->is_visible() && c->is_enabled())
		{
			if (c->get_focus_policy() == focus_group)
			{
				GUIComponent *selected_comp = c->get_group_selected_component();
				if (selected_comp)
				{
					selected_comp->set_focus(true);
				}
				else
				{
					c->set_focus(true);
				}
			}
			else
			{
				c->set_focus(true);
			}

			break;
		}

		c = c->get_previous_component_in_tree();
	}
}	

GUIComponent *GUIComponent::get_next_component_in_tree()
{
	if (has_child_components())
		return get_first_child();

	if (impl->next_sibling)
		return impl->next_sibling;

	GUIComponent *parent = impl->parent;
	while (parent)
	{
		if (parent->get_next_sibling())
			return parent->get_next_sibling();
		parent = parent->get_parent_component();
	}

	// Reached end of tree. Return first item.
	return get_top_level_component();
}


GUIComponent *GUIComponent::get_previous_component_in_tree()
{
	if (impl->prev_sibling)
	{
		// return last grand-child of sibling.
		if (impl->prev_sibling->has_child_components())
		{
			GUIComponent *last = impl->prev_sibling->impl->last_child;
			while (last->has_child_components())
			{
				last = last->get_last_child();
			}
			return last;
		}
		else
		{
			// sibling has no children, return sibling.
			return impl->prev_sibling;
		}
	}

	// no previous sibling, return parent.
	if (impl->parent)
		return impl->parent;

	// No parent, must be top-level component. Find last child.
	GUIComponent *last = impl->last_child;

    // No child, must be a lonely top-level component.
    if(last == 0)
        return this;

	while (last->has_child_components())
	{
		last = last->get_last_child();
	}
	return last;
}

void GUIComponent::set_default(bool value)
{
	impl->default_handler = value;
	request_repaint();
}

void GUIComponent::set_cancel(bool value)
{
	impl->cancel_handler = value; 
}

void GUIComponent::set_double_click_enabled(bool enable)
{
	impl->double_click_enabled = enable;
}

void GUIComponent::set_blocks_default_action(bool block)
{
	impl->blocks_default_action_when_focused = block;
}

void GUIComponent::set_constant_repaint(bool enable)
{
	impl->constant_repaint = enable;
}

void GUIComponent::set_selected_in_component_group(bool selected)
{
	impl->is_selected_in_group = selected;
}

Rect GUIComponent::get_render_text_span_box( Canvas &canvas, const std::string &str, const Rect &content_rect ) const
{
	Font font = impl->element.get_font(canvas, get_resources());
	SpanLayout span = impl->element.create_span_layout(canvas, font, str, content_rect);
	return span.get_rect();
}

Rect GUIComponent::get_render_text_box( Canvas &canvas, const std::string &str ) const
{
	return impl->element.get_render_text_box(canvas, str, get_content_box(), get_resources());
}

Size GUIComponent::get_render_text_size( Canvas &canvas, const std::string &str ) const
{
	return impl->element.get_render_text_box(canvas, str, get_content_box(), get_resources()).get_size();
}

VerticalTextPosition GUIComponent::get_vertical_text_align(Canvas &canvas)
{
	return get_vertical_text_align(canvas, get_content_box());
}

VerticalTextPosition GUIComponent::get_vertical_text_align(Canvas &canvas, const Rect &content_rect)
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

Rect GUIComponent::render_text_span( Canvas &canvas, const std::string &text, const Rect &content_rect )
{
	Font font = impl->element.get_font(canvas, get_resources());
	SpanLayout span = impl->element.create_span_layout(canvas,  font, text, content_rect);
	span.draw_layout(canvas);
	return span.get_rect();
}

Rect GUIComponent::render_text( Canvas &canvas, const std::string &text )
{
	Rect content_box = get_content_box();
	Font font = impl->element.get_font(canvas, get_resources());
	int baseline = content_box.top + font.get_font_metrics().get_ascent();
	return impl->element.render_text(canvas, font, text, content_box, baseline, false );
}

Rect GUIComponent::render_text( Canvas &canvas, const std::string &text, int xpos, int baseline )
{
	Rect content_box = get_content_box();
	content_box.left += xpos;
	content_box.top += baseline;
	Font font = impl->element.get_font(canvas, get_resources());
	return impl->element.render_text(canvas, font, text, content_box, baseline, false );
}

Rect GUIComponent::get_content_shrink_box() const
{
	Rect shrink_box;
	shrink_box.left = impl->css_used_values.border.left + impl->css_used_values.padding.left;
	shrink_box.right = impl->css_used_values.border.right + impl->css_used_values.padding.right;
	shrink_box.top = impl->css_used_values.border.top + impl->css_used_values.padding.top;
	shrink_box.bottom = impl->css_used_values.border.bottom + impl->css_used_values.padding.bottom;
	return shrink_box;
}

std::string GUIComponent::get_property(const std::string &property, const std::string &default_value) const
{
	return impl->element.get_property(property, default_value);
}

int GUIComponent::get_property_int(const std::string &property, const std::string &default_value) const
{
	return StringHelp::text_to_int(get_property(property, default_value));
}

/////////////////////////////////////////////////////////////////////////////
// GUIComponent Implementation:

GUIComponent::GUIComponent(GUIComponent &other)
{
	throw Exception("GUIComponent copy construction disallowed");
}

GUIComponent &GUIComponent::operator =(const GUIComponent &other)
{
	throw Exception("GUIComponent operator = disallowed");
}

}
