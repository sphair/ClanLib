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
*/

#include "GUI/precomp.h"
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/gui_theme.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_layout_corners.h"
#include "API/GUI/gui_window_manager.h"
#include "API/Core/CSS/css_document.h"
#include "API/Core/XML/dom_document.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/XML/dom_text.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Window/input_context.h"
#include "API/Display/Render/blend_mode.h"
#include "API/CSSLayout/css_layout.h"
#include "API/CSSLayout/css_layout_element.h"
#include "API/CSSLayout/css_layout_text.h"
#include "API/CSSLayout/css_document2.h"
#include "API/CSSLayout/dom_select_node.h"
#include "API/CSSLayout/css_property_list2.h"
#include "gui_component_impl.h"
#include "gui_manager_impl.h"
#include "gui_component_description_impl.h"
#include "gui_xml_loader_version_1_0.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GUIComponent Construction:

CL_GUIComponent::CL_GUIComponent(CL_GUIComponent *parent)
: impl(CL_GUIComponent_Impl::create_from_parent(parent))
{
	impl->component = this;
	impl->type_name = "component";
	impl->geometry = CL_Rect(0,0,0,0);

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
}

CL_GUIComponent::CL_GUIComponent(CL_GUIManager *manager, CL_GUITopLevelDescription description)
: impl(CL_GUIComponent_Impl::create_from_manager(manager))
{
	impl->component = this;
	impl->allow_resize = description.get_allow_resize();
	impl->visible = description.is_visible();
	impl->gui_manager.lock()->add_component(this, 0, description);
	impl->type_name = "component";
	impl->geometry = impl->gui_manager.lock()->window_manager.get_geometry(impl->gui_manager.lock()->get_toplevel_window(this), true);
	request_repaint();
}

CL_GUIComponent::CL_GUIComponent(CL_GUIComponent *owner, CL_GUITopLevelDescription description)
: impl(CL_GUIComponent_Impl::create_from_owner(owner))
{
	impl->component = this;
	impl->allow_resize = description.get_allow_resize();
	impl->visible = description.is_visible();
	impl->gui_manager.lock()->add_component(this, owner, description);
	impl->type_name = "component";
	impl->geometry = impl->gui_manager.lock()->window_manager.get_geometry(impl->gui_manager.lock()->get_toplevel_window(this), true);
	request_repaint();
}

CL_GUIComponent::~CL_GUIComponent()
{
	if (!impl->gui_manager_impl->theme.is_null())
		impl->gui_manager_impl->theme.component_destroyed(this);
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIComponent Attributes:

CL_Rect CL_GUIComponent::get_geometry() const
{
	if (impl->parent == 0)
	{
		return impl->gui_manager.lock()->window_manager.get_geometry(
			impl->gui_manager.lock()->get_toplevel_window(this), true);
	}
	else
	{
		return impl->geometry;
	}
}

CL_Rect CL_GUIComponent::get_window_geometry() const
{
	if (impl->parent == 0)
	{
		return impl->gui_manager.lock()->window_manager.get_geometry(
			impl->gui_manager.lock()->get_toplevel_window(this), false);
	}
	else
	{
		return impl->geometry;
	}
}

int CL_GUIComponent::get_width() const
{
	return impl->geometry.get_width();
}

int CL_GUIComponent::get_height() const
{
	return impl->geometry.get_height();
}

CL_Size CL_GUIComponent::get_size() const
{
	return impl->geometry.get_size();
}

CL_StringRef CL_GUIComponent::get_type_name() const
{
	return impl->type_name;
}
	
CL_StringRef CL_GUIComponent::get_class_name() const
{
	return impl->class_name;
}

CL_StringRef CL_GUIComponent::get_id_name() const
{
	return impl->id_name;
}

CL_StringRef CL_GUIComponent::get_element_name() const
{
	if (!impl->element_name.empty())
		return impl->element_name;

	CL_String element_name = impl->type_name;
	if (!impl->class_name.empty())
		element_name += "." + impl->class_name;
	if (!impl->id_name.empty())
		element_name += "#" + impl->id_name;
	if (impl->parent)
	{
		CL_String path_name = impl->parent->get_element_name();
		if (!path_name.empty())
			element_name = path_name + " " + element_name;
	}

	impl->element_name = element_name;	
	return impl->element_name;
}

bool CL_GUIComponent::has_focus() const
{
	return impl->gui_manager.lock()->has_focus(this);
}

CL_GUIComponent::FocusPolicy CL_GUIComponent::get_focus_policy() const
{
	return impl->focus_policy;
}

CL_StringRef CL_GUIComponent::get_component_group_name() const
{
	return impl->group_name;
}

bool CL_GUIComponent::is_selected_in_group() const
{
	return impl->is_selected_in_group;
}

bool CL_GUIComponent::get_blocks_default_action() const
{
	return impl->blocks_default_action_when_focused;
}

CL_ResourceManager CL_GUIComponent::get_resources() const
{
	return get_gui_manager().get_theme().get_resources();
}

CL_GUIManager CL_GUIComponent::get_gui_manager() const
{
	return CL_GUIManager(impl->gui_manager.lock());
}

CL_GUITheme CL_GUIComponent::get_theme() const
{
	return impl->gui_manager.lock()->theme;
}

const CL_GUIComponent *CL_GUIComponent::get_parent_component() const
{
	return impl->parent;
}

CL_GUIComponent *CL_GUIComponent::get_parent_component()
{
	return impl->parent;
}

const CL_GUIComponent *CL_GUIComponent::get_owner_component() const
{
	return impl->gui_manager.lock()->get_owner_component(this);
}

CL_GUIComponent *CL_GUIComponent::get_owner_component()
{
	return impl->gui_manager.lock()->get_owner_component(this);
}

std::vector<CL_GUIComponent *> CL_GUIComponent::get_child_components() const
{
	std::vector<CL_GUIComponent *> children;
	CL_GUIComponent *cur_child = impl->first_child;
	while (cur_child)
	{
		children.push_back(cur_child);
		cur_child = cur_child->get_next_sibling();
	}
	return children;
}

std::vector<CL_GUIComponent*> CL_GUIComponent::get_child_component_group(const CL_String &group_name) const
{
	std::vector<CL_GUIComponent *> group;
	CL_GUIComponent *cur_child = impl->first_child;
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

CL_GUIComponent *CL_GUIComponent::get_group_selected_component()
{
	CL_GUIComponent *parent = get_parent_component();

	if (parent)
	{
		std::vector<CL_GUIComponent*> group = parent->get_child_component_group(get_component_group_name());
		std::vector<CL_GUIComponent*>::size_type i;
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


const CL_GUIComponent *CL_GUIComponent::get_first_child() const
{
	return impl->first_child;
}

CL_GUIComponent *CL_GUIComponent::get_first_child()
{
	return impl->first_child;
}

const CL_GUIComponent *CL_GUIComponent::get_last_child() const
{
	return impl->last_child;
}

CL_GUIComponent *CL_GUIComponent::get_last_child()
{
	return impl->last_child;
}

const CL_GUIComponent *CL_GUIComponent::get_previous_sibling() const
{
	return impl->prev_sibling;
}

CL_GUIComponent *CL_GUIComponent::get_previous_sibling()
{
	return impl->prev_sibling;
}

const CL_GUIComponent *CL_GUIComponent::get_next_sibling() const
{
	return impl->next_sibling;
}

bool CL_GUIComponent::is_descendant_of(CL_GUIComponent *component)
{
	if(impl->parent == 0)
		return false;

	if(impl->parent == component)
		return true;

	return impl->parent->is_descendant_of(component);
}

bool CL_GUIComponent::is_ancestor_of(CL_GUIComponent *component)
{
	CL_GUIComponent *cur_child = impl->first_child;
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

CL_GUIComponent *CL_GUIComponent::get_next_sibling()
{
	return impl->next_sibling;
}

bool CL_GUIComponent::has_child_components() const
{
	return (impl->first_child != 0);
}

CL_GraphicContext& CL_GUIComponent::get_gc()
{
	const CL_GUIComponent *root_component = get_top_level_component();

	std::vector<CL_GUITopLevelWindow>::size_type pos, size;
	size = impl->gui_manager.lock()->root_components.size();
	for (pos = 0; pos < size; pos++)
	{
		CL_GUITopLevelWindow *cur = impl->gui_manager.lock()->root_components[pos];
		if (cur->component == root_component)
			return impl->gui_manager.lock()->window_manager.get_gc(cur);
	}
	return dummy_gc;
}

CL_InputContext& CL_GUIComponent::get_ic()
{
	const CL_GUIComponent *root_component = get_top_level_component();

	std::vector<CL_GUITopLevelWindow>::size_type pos, size;
	size = impl->gui_manager.lock()->root_components.size();
	for (pos = 0; pos < size; pos++)
	{
		CL_GUITopLevelWindow *cur = impl->gui_manager.lock()->root_components[pos];
		if (cur->component == root_component)
			return impl->gui_manager.lock()->window_manager.get_ic(cur);
	}
	return dummy_ic;
}


bool CL_GUIComponent::is_enabled() const
{
	const CL_GUIComponent *parent = get_parent_component();
	if (parent)
	{
		if (parent->is_enabled() == false)
			return false;
	}
	return impl->enabled;
}

bool CL_GUIComponent::is_visible() const
{
	const CL_GUIComponent *parent = get_parent_component();
	if (parent)
	{
		if (parent->is_visible() == false)
			return false;
	}
	return impl->visible;
}

bool CL_GUIComponent::is_active() const
{
	return impl->activated;
}

CL_GUIComponent *CL_GUIComponent::get_component_at(const CL_Point &point)
{
	if (is_visible() == false)
	{
		return 0;
	}
	else
	{
		CL_Rect parent_rect = get_size();
		if( parent_rect.contains(point) )  
		{
			std::vector<CL_GUIComponent *> children = get_child_components();
			std::size_t pos, size = children.size();

			for( pos=size; pos>0; pos-- )
			{
				CL_GUIComponent *child = children[pos-1];
				if(child->is_visible())
				{
					if (child->get_geometry().contains(point))
					{
						CL_Point P = point;
						P.x -= child->get_geometry().left;
						P.y -= child->get_geometry().top;
						CL_GUIComponent *comp = child->get_component_at(P);
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

CL_Point CL_GUIComponent::window_to_component_coords(const CL_Point &point) const
{
	CL_Point P = point;

	const CL_GUIComponent *current = this;
	while (current->get_parent_component())
	{
		CL_Rect g = current->get_geometry();
		P.x -= g.left;
		P.y -= g.top;
		current = current->get_parent_component();
	}

	return P;
}

CL_Rect CL_GUIComponent::window_to_component_coords(const CL_Rect &rect) const
{
	CL_Point tl = window_to_component_coords(rect.get_top_left());
	CL_Point br = window_to_component_coords(rect.get_bottom_right());
	return CL_Rect(tl.x, tl.y, br.x, br.y);
}

CL_Point CL_GUIComponent::component_to_window_coords(const CL_Point &point) const
{
	CL_Point P = point;

	const CL_GUIComponent *current = this;
	while (current->get_parent_component())
	{
		CL_Rect g = current->get_geometry();
		P.x += g.left;
		P.y += g.top;
		current = current->get_parent_component();
	}

	return P;
}

CL_Rect CL_GUIComponent::component_to_window_coords(const CL_Rect &rect) const
{
	CL_Point tl = component_to_window_coords(rect.get_top_left());
	CL_Point br = component_to_window_coords(rect.get_bottom_right());
	return CL_Rect(tl.x, tl.y, br.x, br.y);
}

CL_Point CL_GUIComponent::screen_to_component_coords(const CL_Point &screen_point) const
{
	CL_GUITopLevelWindow *toplevel_window = impl->gui_manager.lock()->get_toplevel_window(this);
	CL_Point client_point = impl->gui_manager.lock()->window_manager.screen_to_window(toplevel_window, screen_point, true);
	return window_to_component_coords(client_point);
}

CL_Point CL_GUIComponent::component_to_screen_coords(const CL_Point &component_point) const
{
	CL_GUITopLevelWindow *toplevel_window = impl->gui_manager.lock()->get_toplevel_window(this);
	CL_Point client_point = component_to_window_coords(component_point);
	return impl->gui_manager.lock()->window_manager.window_to_screen(toplevel_window, client_point, true);
}

CL_GUIComponent *CL_GUIComponent::get_top_level_component()
{
	CL_GUIComponent *parent = this;

	while (true)
	{
		if (parent->get_parent_component())
			parent = parent->get_parent_component();
		else 
			break;
	}

	return parent;
}

const CL_GUIComponent *CL_GUIComponent::get_top_level_component() const
{
	const CL_GUIComponent *parent = this;

	while (true)
	{
		if (parent->get_parent_component())
			parent = parent->get_parent_component();
		else 
			break;
	}

	return parent;
}

bool CL_GUIComponent::get_allow_resize() const
{
	return impl->allow_resize;
}

bool CL_GUIComponent::get_clip_children() const
{
	return impl->clip_children;
}

CL_Size CL_GUIComponent::get_preferred_size() const
{
	throw CL_Exception("CL_GUIComponent::get_preferred_size shall not be called directly! Derived classes should override this function, if needed!");
	return CL_Size();
}

int CL_GUIComponent::get_preferred_width() const
{
	return get_preferred_size().width;
}

int CL_GUIComponent::get_preferred_height() const
{
	return get_preferred_size().height;
}

CL_GUILayout CL_GUIComponent::get_layout() const
{
	return impl->layout;
}

CL_DisplayWindow CL_GUIComponent::get_display_window() const
{
	CL_GUITopLevelWindow *toplevel = impl->gui_manager.lock()->get_toplevel_window(this);
	return impl->gui_manager.lock()->window_manager.get_display_window(toplevel);
}

bool CL_GUIComponent::get_constant_repaint() const
{
	return impl->constant_repaint;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIComponent Events:

CL_Callback_v2<CL_GraphicContext &, const CL_Rect &> &CL_GUIComponent::func_render()
{
	return impl->func_render;
}

CL_Callback_v1<CL_GUIMessage &> &CL_GUIComponent::func_process_message()
{
	return impl->func_process_message;
}

CL_Callback_0<bool> &CL_GUIComponent::func_close()
{
	return impl->func_close;
}

CL_Callback_0<bool> &CL_GUIComponent::func_activated()
{
	return impl->func_activated;
}

CL_Callback_0<bool> &CL_GUIComponent::func_deactivated()
{
	return impl->func_deactivated;
}

CL_Callback_0<bool> &CL_GUIComponent::func_focus_lost()
{
	return impl->func_focus_lost;
}

CL_Callback_v1<CL_GUIMessage&> &CL_GUIComponent::func_filter_message()
{
	return impl->func_filter_message;
}

CL_Callback_0<bool> &CL_GUIComponent::func_focus_gained()
{
	return impl->func_focus_gained;
}

CL_Callback_0<bool> &CL_GUIComponent::func_pointer_enter()
{
	return impl->func_pointer_enter;
}

CL_Callback_0<bool> &CL_GUIComponent::func_pointer_exit()
{
	return impl->func_pointer_exit;
}

CL_Callback_1<bool, const CL_InputEvent &> &CL_GUIComponent::func_input()
{
	return impl->func_input;
}

CL_Callback_1<bool, const CL_InputEvent &> &CL_GUIComponent::func_input_pressed()
{
	return impl->func_input_pressed;
}

CL_Callback_1<bool, const CL_InputEvent &> &CL_GUIComponent::func_input_released()
{
	return impl->func_input_released;
}

CL_Callback_1<bool, const CL_InputEvent &> &CL_GUIComponent::func_input_doubleclick()
{
	return impl->func_input_doubleclick;
}

CL_Callback_1<bool, const CL_InputEvent &> &CL_GUIComponent::func_input_pointer_moved()
{
	return impl->func_input_pointer_moved;
}

CL_Callback_v1<bool> &CL_GUIComponent::func_visibility_change()
{
	return impl->func_visibility_change;
}

CL_Callback_v0 &CL_GUIComponent::func_style_changed()
{
	return impl->func_style_changed;
}

CL_Callback_v0 &CL_GUIComponent::func_enablemode_changed()
{
	return impl->func_enablemode_changed;
}

CL_Callback_v0 &CL_GUIComponent::func_resized()
{
	return impl->func_resized;
}

CL_Callback_2<CL_GUIComponent*,CL_GUIComponent*, CL_String> &CL_GUIComponent::func_create_custom_component()
{
	return impl->func_create_custom_component;
}

bool CL_GUIComponent::is_default()
{
	return impl->default_handler;
}

bool CL_GUIComponent::is_cancel()
{
	return impl->cancel_handler;
}

bool CL_GUIComponent::is_double_click_enabled() const
{
	return impl->double_click_enabled;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIComponent Operations:

void CL_GUIComponent::render(CL_GraphicContext &gc, const CL_Rect &clip_rect, bool include_children)
{
	if (!impl->visible)
		return;

	if (!impl->css_layout.is_null())
	{
		impl->css_layout.layout(gc, get_size());
		impl->css_layout.render(gc, this);
	}

	if (impl->func_render.is_null() == false)
	{
		impl->func_render.invoke(gc, clip_rect);
	}
	else
	{
		CL_GUIThemePart part(this);
		CL_Rect geometry = get_size();
		part.render_box(gc, CL_RectPS(0, 0, geometry.get_width(), geometry.get_height()), clip_rect);
	}

	if (include_children)
	{
		if (impl->clip_children)
		{
			push_cliprect(gc, impl->clip_children_rect);
		}

		CL_GUIComponent *cur = impl->first_child;
		while (cur)
		{
			CL_Rect cur_geometry = cur->get_geometry();

			CL_Rect update_rect = component_to_window_coords(clip_rect);
			update_rect.overlap(component_to_window_coords(cur_geometry));
			if (update_rect.get_width() > 0 && update_rect.get_height() > 0)
			{
				CL_Rect child_dirty_rect = cur->window_to_component_coords(update_rect);

				gc.push_translate((float)cur_geometry.left, (float)cur_geometry.top);
				cur->render(gc, child_dirty_rect, true);
				gc.pop_modelview();
			}
			cur = cur->impl->next_sibling;
		}

		if (impl->clip_children)
		{
			pop_cliprect(gc);
		}
	}
}

void CL_GUIComponent::paint()
{
	paint(get_size());
}

void CL_GUIComponent::paint(const CL_Rect &clip_rect)
{
	CL_Rect update_region = component_to_window_coords(clip_rect);
	CL_GUIComponent *toplevel_component = get_top_level_component();
	CL_GUITopLevelWindow *toplevel_window = impl->gui_manager.lock()->get_toplevel_window(this);

	CL_GraphicContext gc = impl->gui_manager.lock()->window_manager.begin_paint(toplevel_window, update_region);
	toplevel_component->render(gc, update_region, true);
	impl->gui_manager.lock()->window_manager.end_paint(toplevel_window, update_region);
}

int CL_GUIComponent::exec()
{
	CL_GUIComponent *owner_component = get_owner_component();
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

void CL_GUIComponent::exit_with_code(int exit_code)
{
	get_gui_manager().exit_with_code(exit_code);
}

void CL_GUIComponent::set_geometry(CL_Rect geometry)
{
	impl->set_geometry(geometry, true);
}

void CL_GUIComponent::set_window_geometry(CL_Rect geometry)
{
	impl->set_geometry(geometry, false);
}

void CL_GUIComponent::capture_mouse(bool capture)
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

void CL_GUIComponent::capture_proximity(bool capture)
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

void CL_GUIComponent::set_type_name(const CL_StringRef &name)
{
	impl->type_name = name;
	if (!impl->func_style_changed.is_null())
		impl->func_style_changed.invoke();
}
	
void CL_GUIComponent::set_class_name(const CL_StringRef &name)
{
	impl->class_name = name;
	impl->element_name = CL_String(); // force update of cached element name 
	if (!impl->func_style_changed.is_null())
		impl->func_style_changed.invoke();
}

void CL_GUIComponent::set_id_name(const CL_StringRef &name)
{
	impl->id_name = name;
	impl->element_name = CL_String(); // force update of cached element name 
	if (!impl->func_style_changed.is_null())
		impl->func_style_changed.invoke();
}

void CL_GUIComponent::set_enabled(bool enable)
{
	if (impl->enabled != enable)
	{
		impl->enabled = enable;
		impl->invoke_enablemode_changed();
		if (impl->parent == 0)
			impl->gui_manager.lock()->set_enabled(this, enable);
	}
}

void CL_GUIComponent::set_visible(bool visible, bool activate_root_win)
{
	if (visible != impl->visible)
	{
		request_repaint();
		if (!impl->func_visibility_change.is_null())
			impl->func_visibility_change.invoke(visible);
	}

	impl->visible = visible;
	if (impl->parent == 0)
	{
		CL_GUITopLevelWindow *toplevel_window = impl->gui_manager.lock()->get_toplevel_window(this);
		impl->gui_manager.lock()->window_manager.set_visible(toplevel_window, visible, activate_root_win);
	}
}

void CL_GUIComponent::set_focus(bool enable)
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
	
void CL_GUIComponent::set_focus_policy(FocusPolicy policy)
{
	impl->focus_policy = policy;
}


void CL_GUIComponent::set_component_group_name(const CL_StringRef &str)
{
	impl->group_name = str;
}

CL_GUIComponent *CL_GUIComponent::get_named_item(const CL_StringRef &id)
{
	CL_GUIComponent *cur = impl->first_child;
	while (cur && cur != this)
	{
		if (cur->get_id_name() == id)
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

void CL_GUIComponent::create_components(const CL_DomDocument &gui_xml)
{
	CL_DomDocument const_hack = gui_xml;

	// Check if loaded document uses namespaces and if its a clanlib resources xml document:
	CL_DomElement doc_element = const_hack.get_document_element();
	if (doc_element.get_namespace_uri() == "http://clanlib.org/xmlns/gui-1.0")
	{
		if (doc_element.get_local_name() != "gui")
			throw CL_Exception("ClanLib GUI XML documents must begin with a gui element.");

		CL_GUIXMLLoaderVersion_1_0 loader(this, impl->layout);
		loader.set_create_custom_callback(&impl->func_create_custom_component);
		loader.load(const_hack);
	}
	else
	{
		throw CL_Exception("XML document is not a ClanLib GUI XML document.");
	}
}

void CL_GUIComponent::create_components(const CL_StringRef &filename, const CL_VirtualDirectory &dir)
{
	CL_IODevice device;
	device = dir.open_file_read(filename);
	create_components(device);
}

void CL_GUIComponent::create_components(CL_IODevice &file)
{
	CL_DomDocument doc;
	doc.load(file); 
	create_components(doc);
}

void CL_GUIComponent::create_components(const CL_StringRef &fullname)
{
	CL_String path = CL_PathHelp::get_fullpath(fullname, CL_PathHelp::path_type_file);
	CL_String filename = CL_PathHelp::get_filename(fullname, CL_PathHelp::path_type_file);
	CL_VirtualFileSystem vfs(path);
	create_components(filename, vfs.get_root_directory());
}

void CL_GUIComponent::request_repaint(CL_Rect request_repaint)
{
	get_gui_manager().request_repaint(component_to_window_coords(request_repaint), get_top_level_component());
}

void CL_GUIComponent::request_repaint()
{
	request_repaint(get_size());
}

void CL_GUIComponent::set_cliprect(CL_GraphicContext &gc, const CL_Rect &rect)
{
	CL_Rect windcliprect = component_to_window_coords(rect);
	CL_GUIComponent *toplevel = get_top_level_component();
	CL_GUITopLevelWindow *window = impl->gui_manager_impl->get_toplevel_window(toplevel);
	impl->gui_manager_impl->window_manager.set_cliprect(window, gc, windcliprect);
}

void CL_GUIComponent::reset_cliprect(CL_GraphicContext &gc)
{
	CL_GUIComponent *toplevel = get_top_level_component();
	CL_GUITopLevelWindow *window = impl->gui_manager_impl->get_toplevel_window(toplevel);
	impl->gui_manager_impl->window_manager.reset_cliprect(window, gc);
}

void CL_GUIComponent::push_cliprect(CL_GraphicContext &gc, const CL_Rect &rect)
{
	CL_Rect windcliprect = component_to_window_coords(rect);
	CL_GUIComponent *toplevel = get_top_level_component();
	CL_GUITopLevelWindow *window = impl->gui_manager_impl->get_toplevel_window(toplevel);
	impl->gui_manager_impl->window_manager.push_cliprect(window, gc, windcliprect);
}

void CL_GUIComponent::pop_cliprect(CL_GraphicContext &gc)
{
	CL_GUIComponent *toplevel = get_top_level_component();
	CL_GUITopLevelWindow *window = impl->gui_manager_impl->get_toplevel_window(toplevel);
	impl->gui_manager_impl->window_manager.pop_cliprect(window, gc);
}

void CL_GUIComponent::delete_child_components()
{
	while (impl->last_child)
	{
		delete impl->last_child;
	}

	impl->first_child = 0;
	impl->last_child = 0;
}

void CL_GUIComponent::set_parent_component(CL_GUIComponent *new_parent)
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

void CL_GUIComponent::set_layout(CL_GUILayout &layout)
{
	impl->layout = layout;
}

void CL_GUIComponent::set_cursor(const CL_Cursor &cursor)
{
	CL_GUITopLevelWindow *toplevel = impl->gui_manager.lock()->get_toplevel_window(this);
	impl->gui_manager.lock()->window_manager.set_cursor(toplevel, cursor);
}

void CL_GUIComponent::set_cursor(CL_StandardCursor type)
{
	CL_GUITopLevelWindow *toplevel = impl->gui_manager.lock()->get_toplevel_window(this);
	impl->gui_manager.lock()->window_manager.set_cursor(toplevel, type);
}

void CL_GUIComponent::set_clip_children(bool clip, const CL_Rect &rect)
{
	impl->clip_children = clip;
	if (rect.get_width() == 0 || rect.get_height() == 0)
		impl->clip_children_rect = get_size();
	else
		impl->clip_children_rect = rect;
}

void CL_GUIComponent::focus_next()
{
	CL_GUIComponent *c = this;

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
				CL_GUIComponent *selected_comp = c->get_group_selected_component();
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

void CL_GUIComponent::focus_previous()
{
	CL_GUIComponent *c = this;

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
				CL_GUIComponent *selected_comp = c->get_group_selected_component();
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

CL_GUIComponent *CL_GUIComponent::get_next_component_in_tree()
{
	if (has_child_components())
		return get_first_child();

	if (impl->next_sibling)
		return impl->next_sibling;

	CL_GUIComponent *parent = impl->parent;
	while (parent)
	{
		if (parent->get_next_sibling())
			return parent->get_next_sibling();
		parent = parent->get_parent_component();
	}

	// Reached end of tree. Return first item.
	return get_top_level_component();
}


CL_GUIComponent *CL_GUIComponent::get_previous_component_in_tree()
{
	if (impl->prev_sibling)
	{
		// return last grand-child of sibling.
		if (impl->prev_sibling->has_child_components())
		{
			CL_GUIComponent *last = impl->prev_sibling->impl->last_child;
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
	CL_GUIComponent *last = impl->last_child;

    // No child, must be a lonely top-level component.
    if(last == 0)
        return this;

	while (last->has_child_components())
	{
		last = last->get_last_child();
	}
	return last;
}


void CL_GUIComponent::set_default(bool value)
{
	impl->default_handler = value;
	impl->func_style_changed.invoke();
}

void CL_GUIComponent::set_cancel(bool value)
{
	impl->cancel_handler = value; 
}

void CL_GUIComponent::set_double_click_enabled(bool enable)
{
	impl->double_click_enabled = enable;
}

void CL_GUIComponent::set_blocks_default_action(bool block)
{
	impl->blocks_default_action_when_focused = block;
}

void CL_GUIComponent::set_constant_repaint(bool enable)
{
	impl->constant_repaint = enable;
}

void CL_GUIComponent::set_selected_in_component_group(bool selected)
{
	impl->is_selected_in_group = selected;
}

CL_CSSLayout CL_GUIComponent::get_css_layout()
{
	return get_top_level_component()->impl->css_layout;
}

CL_CSSLayoutElement CL_GUIComponent::get_css_element()
{
	return impl->css_element;
}

void CL_GUIComponent::load_css_layout(const CL_String &xml_filename, const CL_String &css_filename)
{
	CL_CSSDocument2 css_document;
	css_document.add_sheet(css_filename);

	CL_File file(xml_filename);
	CL_DomDocument dom(file, false);
	CL_DomNode cur = dom.get_document_element().get_first_child();
	std::vector<CL_CSSLayoutElement> element_stack;

	{
		CL_DomSelectNode select_node(dom.get_document_element());
		impl->css_element.apply_properties(css_document.select(&select_node));
		impl->css_element.apply_properties(dom.get_document_element().get_attribute("style"));
		impl->css_element.set_col_span(dom.get_document_element().get_attribute_int("colspan", 1));
		impl->css_element.set_row_span(dom.get_document_element().get_attribute_int("rowspan", 1));
	}

	element_stack.push_back(impl->css_element);
	while (!cur.is_null())
	{
		CL_CSSLayoutElement child_css_element;
		if (cur.is_element())
		{
			CL_DomElement cur_element = cur.to_element();
			CL_DomSelectNode select_node(cur_element);
			child_css_element = element_stack.back().create_element(cur_element.get_tag_name());
			child_css_element.apply_properties(css_document.select(&select_node));
			child_css_element.apply_properties(cur_element.get_attribute("style"));
			child_css_element.set_col_span(cur_element.get_attribute_int("colspan", 1));
			child_css_element.set_row_span(cur_element.get_attribute_int("rowspan", 1));
		}
		else if (cur.is_text())
		{
			CL_DomText cur_text = cur.to_text();
			element_stack.back().create_text(cur_text.get_node_value());
		}

		CL_DomNode next = cur.get_first_child();
		if (next.is_null())
		{
			next = cur.get_next_sibling();
			if (next.is_null())
			{
				next = cur.get_parent_node();
				while (!next.is_null() && next.get_next_sibling().is_null())
					next = next.get_parent_node();
				if (!next.is_null())
					next = next.get_next_sibling();
			}
		}
		else
		{
			element_stack.push_back(child_css_element);
		}
		cur = next;
	}

	CL_GraphicContext gc = get_gc();
	impl->css_layout.layout(gc, get_size());
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIComponent Implementation:

CL_GUIComponent::CL_GUIComponent(CL_GUIComponent &other)
{
	throw CL_Exception("CL_GUIComponent copy construction disallowed");
}

CL_GUIComponent &CL_GUIComponent::operator =(const CL_GUIComponent &other)
{
	throw CL_Exception("CL_GUIComponent operator = disallowed");
}


