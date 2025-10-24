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
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/gui_theme.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_layout_corners.h"
#include "API/GUI/gui_window_manager.h"
#include "API/Core/CSS/css_ruleset.h"
#include "API/Core/CSS/css_document.h"
#include "API/Core/XML/dom_document.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Window/input_context.h"
#include "API/Display/Render/blend_mode.h"
#include "gui_component_impl.h"
#include "gui_manager_impl.h"
#include "gui_component_description_impl.h"
#include "gui_xml_loader_version_1_0.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GUIComponent Construction:

CL_GUIComponent::CL_GUIComponent(CL_GUIComponent *parent)
: impl(CL_GUIComponent_Impl::create_from_parent(parent))
{
	impl->type_name = cl_text("component");
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

CL_GUIComponent::CL_GUIComponent(const CL_Rect &position, CL_GUIManager *manager, CL_GUITopLevelDescription description, bool temporary)
: impl(CL_GUIComponent_Impl::create_from_manager(manager))
{
	impl->allow_resize = description.get_allow_resize();
	impl->visible = description.is_visible();
	description.set_position(position, false); 
	impl->gui_manager->add_component(this, 0, description, temporary);
	impl->type_name = cl_text("component");
	impl->geometry = impl->gui_manager->window_manager->get_geometry(impl->gui_manager->get_toplevel_window(this), true);
	invalidate_rect();
}

CL_GUIComponent::CL_GUIComponent(const CL_Rect &position, CL_GUIComponent *owner, CL_GUITopLevelDescription description)
: impl(CL_GUIComponent_Impl::create_from_owner(owner))
{
	impl->allow_resize = description.get_allow_resize();
	impl->visible = description.is_visible();
	description.set_position(position, false); 
	impl->gui_manager->add_component(this, owner, description, false);
	impl->type_name = cl_text("component");
	impl->geometry = impl->gui_manager->window_manager->get_geometry(impl->gui_manager->get_toplevel_window(this), true);
	invalidate_rect();
}

CL_GUIComponent::~CL_GUIComponent()
{
	if (impl->gui_manager_impl->theme)
		impl->gui_manager_impl->theme->component_destroyed(this);
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIComponent Attributes:

CL_Rect CL_GUIComponent::get_geometry() const
{
	return impl->geometry;
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

bool CL_GUIComponent::is_clickthrough() const
{
	return impl->click_through;
}

CL_Rect CL_GUIComponent::get_window_geometry() const
{
	if (impl->parent == 0)
	{
		return impl->gui_manager->window_manager->get_geometry(
			impl->gui_manager->get_toplevel_window(this), false);
	}
	return impl->geometry;
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
		element_name += cl_text(".") + impl->class_name;
	if (!impl->id_name.empty())
		element_name += cl_text("#") + impl->id_name;
	if (impl->parent)
	{
		CL_String path_name = impl->parent->get_element_name();
		if (!path_name.empty())
			element_name = path_name + cl_text(" ") + element_name;
	}

	impl->element_name = element_name;	
	return impl->element_name;
}

bool CL_GUIComponent::has_focus() const
{
	return impl->gui_manager->has_focus(this);
}

CL_GUIComponent::FocusPolicy CL_GUIComponent::get_focus_policy() const
{
	return focus_refuse;
}

CL_GUIConsumedKeys CL_GUIComponent::get_consumed_keys() const
{
	return impl->consumed_keys;
}

CL_ResourceManager CL_GUIComponent::get_resources() const
{
	return get_gui_manager().get_theme()->get_resources();
}

CL_GUIManager CL_GUIComponent::get_gui_manager() const
{
	return CL_GUIManager(impl->gui_manager);
}

CL_GUITheme *CL_GUIComponent::get_theme() const
{
	return impl->gui_manager->theme;
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
	return impl->gui_manager->get_owner_component(this);
}

CL_GUIComponent *CL_GUIComponent::get_owner_component()
{
	return impl->gui_manager->get_owner_component(this);
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

CL_GUIComponent *CL_GUIComponent::get_next_sibling()
{
	return impl->next_sibling;
}

bool CL_GUIComponent::has_child_components() const
{
	return (impl->first_child != 0);
}

CL_GraphicContext CL_GUIComponent::get_gc() const
{
	const CL_GUIComponent *root_component = get_top_level_component();

	std::vector<CL_GUITopLevelWindow>::size_type pos, size;
	size = impl->gui_manager->root_components.size();
	for (pos = 0; pos < size; pos++)
	{
		CL_GUITopLevelWindow *cur = impl->gui_manager->root_components[pos];
		if (cur->component == root_component)
			return impl->gui_manager->window_manager->get_gc(cur);
	}
	return CL_GraphicContext();
}

CL_InputContext CL_GUIComponent::get_ic() const
{
	const CL_GUIComponent *root_component = get_top_level_component();

	std::vector<CL_GUITopLevelWindow>::size_type pos, size;
	size = impl->gui_manager->root_components.size();
	for (pos = 0; pos < size; pos++)
	{
		CL_GUITopLevelWindow *cur = impl->gui_manager->root_components[pos];
		if (cur->component == root_component)
			return impl->gui_manager->window_manager->get_ic(cur);
	}
	return CL_InputContext();
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
	if (is_visible() == false || is_clickthrough())
	{
		return 0;
	}

	CL_GUIComponent *comp = 0;

	CL_Rect rect = get_geometry();

	if( rect.contains(point) )  
	{
		std::vector<CL_GUIComponent *> children = get_child_components();
		std::size_t pos, size = children.size();

		for( pos=size; pos>0; pos-- )
		{
			CL_GUIComponent *child = children[pos-1];
			CL_Rect parent_rect = get_geometry();
			CL_Point P = point;
			P.x -= parent_rect.left;
			P.y -= parent_rect.top;
			comp = child->get_component_at(P);
			if( comp != 0 )
				break;
		}

		if( comp == 0 )
			comp = this;
	}

	return comp;
}

CL_Point CL_GUIComponent::window_to_component_coords(const CL_Point &point) const
{
	CL_Point P = point;

	const CL_GUIComponent *current = this;
	while (current)
	{
		CL_Rect g = current->get_geometry();
		P.x -= g.left;
		P.y -= g.top;
		current = current->get_parent_component();
	}

	return P;
}

CL_Point CL_GUIComponent::component_to_window_coords(const CL_Point &point) const
{
	CL_Point P = point;

	const CL_GUIComponent *current = this;
	while (current)
	{
		CL_Rect g = current->get_geometry();
		P.x += g.left;
		P.y += g.top;
		current = current->get_parent_component();
	}

	return P;
}

CL_Point CL_GUIComponent::screen_to_component_coords(const CL_Point &screen_point) const
{
	CL_GUITopLevelWindow *toplevel_window = impl->gui_manager->get_toplevel_window(this);
	CL_Point client_point = impl->gui_manager->window_manager->screen_to_window(toplevel_window, screen_point, true);
	return window_to_component_coords(client_point);
}

CL_Point CL_GUIComponent::component_to_screen_coords(const CL_Point &component_point) const
{
	CL_GUITopLevelWindow *toplevel_window = impl->gui_manager->get_toplevel_window(this);
	CL_Point client_point = component_to_window_coords(component_point);
	return impl->gui_manager->window_manager->window_to_screen(toplevel_window, client_point, true);
}

CL_GUIComponent *CL_GUIComponent::get_top_level_component()
{
	CL_GUIComponent *parent = this;

	while( true )
	{
		if( parent->get_parent_component() )
			parent = parent->get_parent_component();
		else 
			break;
	}

	return parent;
}

const CL_GUIComponent *CL_GUIComponent::get_top_level_component() const
{
	const CL_GUIComponent *parent = this;

	while( true )
	{
		if( parent->get_parent_component() )
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
	throw CL_Exception(cl_text("CL_GUIComponent::get_preferred_size shall not be called directly! Derived classes should override this function, if needed!"));
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

CL_GUILayout *CL_GUIComponent::get_layout() const
{
	return impl->layout;
}

CL_DisplayWindow CL_GUIComponent::get_display_window() const
{
	CL_GUITopLevelWindow *toplevel = impl->gui_manager->get_toplevel_window(this);
	return impl->gui_manager->window_manager->get_display_window(toplevel);
}

bool CL_GUIComponent::is_tab_order_controller() const
{
	return impl->is_tab_order_controller;
}

int CL_GUIComponent::get_tab_order() const
{
	return impl->component_tab_index;
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

/////////////////////////////////////////////////////////////////////////////
// CL_GUIComponent Operations:

void CL_GUIComponent::render(CL_GraphicContext &gc, const CL_Rect &clip_rect, bool include_children)
{
	if (!impl->visible)
		return;

	if (impl->func_render.is_null() == false)
	{
		impl->func_render.invoke(gc, clip_rect);
	}
	else
	{
		CL_GUIThemePart part(this);
		CL_Rect geometry = get_geometry();
		part.render_box(gc, CL_RectPS(0, 0, geometry.get_width(), geometry.get_height()), clip_rect);
	}

	if (include_children)
	{
		if (impl->clip_children)
		{
			set_cliprect(get_geometry().get_size());
		}

		CL_GUIComponent *cur = impl->first_child;
		while (cur)
		{
			CL_Rect rect = cur->get_geometry();
			gc.push_translate((float)rect.left, (float)rect.top);
			CL_Rect urect = clip_rect;
			urect.overlap(rect);
			if (urect.get_width() > 0 && urect.get_height() > 0)
			{
				CL_Rect child_clip_rect(
					urect.left - rect.left,
					urect.top - rect.top,
					urect.right - rect.left,
					urect.bottom - rect.top);
				cur->render(gc, child_clip_rect, true);
			}
			gc.pop_modelview();
			cur = cur->impl->next_sibling;
		}

		if (impl->clip_children)
		{
			reset_cliprect();
		}
	}
}

void CL_GUIComponent::paint()
{
	paint(CL_Rect(CL_Point(0, 0), get_geometry().get_size()));
}

void CL_GUIComponent::paint(const CL_Rect &clip_rect)
{
	if (impl->parent)
	{
		CL_Rect geometry = get_geometry();
		CL_Rect rect(
			geometry.left + clip_rect.left,
			geometry.top + clip_rect.top,
			geometry.left + clip_rect.right,
			geometry.top + clip_rect.bottom);
		impl->parent->paint(rect);
	}
	else
	{
		CL_GUITopLevelWindow *toplevel_window = 0;
		std::vector<CL_GUITopLevelWindow>::size_type pos, size;
		size = impl->gui_manager->root_components.size();
		for (pos = 0; pos < size; pos++)
		{
			CL_GUITopLevelWindow *cur = impl->gui_manager->root_components[pos];
			if (cur->component == this)
			{
				toplevel_window = cur;
				break;
			}
		}
		if (toplevel_window == 0)
			return;

		CL_GraphicContext gc = impl->gui_manager->window_manager->begin_paint(toplevel_window, clip_rect);
		render(gc, clip_rect, true);
		impl->gui_manager->window_manager->end_paint(toplevel_window, clip_rect);
	}
}

int CL_GUIComponent::exec()
{
	CL_GUIComponent *owner_component = get_owner_component();
	if (owner_component)
		owner_component->get_top_level_component()->set_enabled(false);
	CL_AcceleratorTable accel_table;
	get_gui_manager().exec(accel_table);
	if (owner_component)
		owner_component->get_top_level_component()->set_enabled(true);
	return get_gui_manager().get_exit_code();
}

void CL_GUIComponent::post_message(const CL_GUIMessage &message)
{
	get_gui_manager().post_message(message);
}

void CL_GUIComponent::send_message(CL_GUIMessage &message)
{
	get_gui_manager().send_message(message);
}

void CL_GUIComponent::exit_with_code(int exit_code)
{
	get_gui_manager().exit_with_code(exit_code);
}

void CL_GUIComponent::set_geometry(const CL_Rect &geometry)
{
	invalidate_rect(impl->geometry.get_size()); // invalidate current geometry.

	impl->geometry = geometry;

	if (impl->layout)
		impl->layout->set_geometry(geometry);

	if (!impl->func_resized.is_null())
		impl->func_resized.invoke();

	invalidate_rect(impl->geometry.get_size()); // invalidate new geometry.
}

void CL_GUIComponent::set_window_geometry(const CL_Rect &geometry, bool client_area)
{
	if (impl->parent == 0)
	{
		CL_GUITopLevelWindow *handle = impl->gui_manager->get_toplevel_window(this);
		impl->gui_manager->window_manager->set_geometry(handle, geometry, client_area);
		CL_Rect geom = impl->gui_manager->window_manager->get_geometry(handle, true);
		set_geometry(geom);
	}
	else
	{
		set_geometry(geometry);
	}
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

		if (!impl->func_enablemode_changed.is_null())
			impl->func_enablemode_changed.invoke();

		if (impl->parent == 0)
			impl->gui_manager->set_enabled(this, enable);
	}
}

void CL_GUIComponent::set_visible(bool visible, bool activate_root_win)
{
	if (visible != impl->visible)
		invalidate_rect();

	impl->visible = visible;
	if (impl->parent == 0)
	{
		CL_GUITopLevelWindow *toplevel_window = impl->gui_manager->get_toplevel_window(this);
		impl->gui_manager->window_manager->set_visible(toplevel_window, visible, activate_root_win);
	}
}

void CL_GUIComponent::set_focus(bool enable)
{
	if (enable)
	{
		impl->gui_manager->gain_focus(this);
	}
	else
	{
		impl->gui_manager->loose_focus(this);
	}
}
	
void CL_GUIComponent::set_focus_policy(FocusPolicy policy)
{
	impl->focus_policy = policy;
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
	if (doc_element.get_namespace_uri() == cl_text("http://clanlib.org/xmlns/gui-1.0"))
	{
		if (doc_element.get_local_name() != cl_text("gui"))
			throw CL_Exception(cl_text("ClanLib GUI XML documents must begin with a gui element."));

		CL_GUIXMLLoaderVersion_1_0 loader(this, impl->layout);
		loader.set_create_custom_callback(&impl->func_create_custom_component);
		loader.load(const_hack);
	}
	else
	{
		throw CL_Exception(cl_text("XML document is not a ClanLib GUI XML document."));
	}
}

void CL_GUIComponent::create_components(const CL_StringRef &filename, CL_VirtualDirectory dir)
{
	CL_DomDocument doc;
	CL_IODevice device;
	device = dir.open_file(filename, CL_File::open_existing, CL_File::access_read, CL_File::share_read);
	doc.load(device); 
	create_components(doc);
}

void CL_GUIComponent::invalidate_rect(CL_Rect invalidate_rect)
{
	CL_Rect rect = get_geometry();
	CL_Point root_pos(rect.left, rect.top);
	CL_GUIComponent *parent = get_parent_component();

	while( parent )
	{
		rect = parent->get_geometry();
		root_pos.x += rect.left;
		root_pos.y += rect.top;
		if( parent->get_parent_component() )
			parent = parent->get_parent_component();
		else
			break; // gui_manager.invalidate_rect() needs the root parent.
	}

	invalidate_rect.left += root_pos.x;
	invalidate_rect.right += root_pos.x;
	invalidate_rect.top += root_pos.y;
	invalidate_rect.bottom += root_pos.y;

	if (parent == 0)
		get_gui_manager().invalidate_rect(invalidate_rect, this);
	else
		get_gui_manager().invalidate_rect(invalidate_rect, parent);
}

void CL_GUIComponent::invalidate_rect()
{
	invalidate_rect(CL_Rect(CL_Point(0, 0), get_geometry().get_size()));
}

CL_Timer CL_GUIComponent::create_timer()
{
	CL_Timer timer = impl->gui_manager->window_manager->create_timer(impl->gui_manager->get_toplevel_window(get_top_level_component()));
	return timer;
}

void CL_GUIComponent::set_cliprect(const CL_Rect &rect)
{
	CL_GUIComponent *parent = this;
	CL_Size size = rect.get_size();
	CL_Rect R = rect;

	// Is this the correct method of setting the clip rect ?
/*  
	Old code here
	while (parent != 0)
	{
		CL_Rect parent_rect = parent->get_geometry();
		R.top += parent_rect.top;
		R.left += parent_rect.left;
		parent = parent->get_parent_component();
	}
*/
	CL_GraphicContext gc = get_gc();

	// Replaced with these 3 lines
	CL_Mat4f modelview_matrix = gc.get_modelview();
	R.left += modelview_matrix.matrix[(4*3) + 0];	// Translation X
	R.top += modelview_matrix.matrix[(4*3) + 1];	// Translation Y

	R.right = R.left + size.width;
	R.bottom = R.top + size.height;

	gc.push_cliprect(R); 
}

void CL_GUIComponent::reset_cliprect()
{
	get_gc().pop_cliprect();
}

void CL_GUIComponent::delete_child_components()
{
	while (impl->last_child)
	{
		delete impl->last_child;
	}
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

void CL_GUIComponent::set_layout(CL_GUILayout *layout)
{
	if (impl->layout)
		delete impl->layout;

	impl->layout = layout;
}

void CL_GUIComponent::set_cursor(const CL_Cursor &cursor)
{
	CL_GUITopLevelWindow *toplevel = impl->gui_manager->get_toplevel_window(this);
	impl->gui_manager->window_manager->set_cursor(toplevel, cursor);
}

void CL_GUIComponent::set_cursor(CL_StandardCursor type)
{
	CL_GUITopLevelWindow *toplevel = impl->gui_manager->get_toplevel_window(this);
	impl->gui_manager->window_manager->set_cursor(toplevel, type);
}

void CL_GUIComponent::set_clickthrough(bool enabled)
{
	impl->click_through = enabled;
}

void CL_GUIComponent::set_clip_children(bool clip)
{
	impl->clip_children = clip;
}

void CL_GUIComponent::set_tab_order_controller(bool enabled)
{
	impl->is_tab_order_controller = enabled;
}

void CL_GUIComponent::set_tab_order(int index)
{
	CL_GUIComponent *tab_controller = get_tab_order_controller();
	if (tab_controller == 0)
		throw CL_Exception(cl_text("CL_GUIComponent must have a parent set as tab order controller before calling set_tab_order()"));

	if (index > tab_controller->impl->tab_order_controller_last_index)
		tab_controller->impl->tab_order_controller_last_index = index;

	impl->component_tab_index = index;
}

CL_GUIComponent *CL_GUIComponent::get_tab_order_controller()
{
	CL_GUIComponent *test = this;
	while (test != 0)
	{
		if (test->is_tab_order_controller())
			return test;
		test = test->get_parent_component();
	}

	return 0;
}

void CL_GUIComponent::focus_next()
{
	if (is_tab_order_controller())
	{
		impl->tab_order_controller_current_index++;
		CL_GUIComponent *next = get_tab_order_component(impl->tab_order_controller_current_index);

		if (next)
		{
			if (next->is_enabled())
                next->set_focus();
			else
			{
				int start_index = impl->tab_order_controller_current_index;
				int search_index = start_index + 1;
				while (search_index  != start_index)
				{
					next = get_tab_order_component(search_index);
					if (next && next->is_enabled())
					{
						next->set_focus();
						impl->tab_order_controller_current_index = next->get_tab_order();
						break;
					}
					search_index++;
					if (search_index > impl->tab_order_controller_last_index)
						search_index = 0;
				}
			}
		}
		else if (impl->tab_order_controller_current_index > impl->tab_order_controller_last_index )
		{
			// back to the start
			CL_GUIComponent *first = get_tab_order_component(0);
			if (first)
			{
				first->set_focus();
				impl->tab_order_controller_current_index = 0;
			}
		}
	}
	else
	{
	    CL_GUIComponent *tab_order_controller = get_tab_order_controller();

		if (tab_order_controller)
			tab_order_controller->focus_next();
	}
}

void CL_GUIComponent::focus_previous()
{
	if (is_tab_order_controller())
	{
		impl->tab_order_controller_current_index--;
		CL_GUIComponent *prev = get_tab_order_component(impl->tab_order_controller_current_index);

		if (prev)
		{
			if (prev->is_enabled())
				prev->set_focus();
			else
			{
				int start_index = impl->tab_order_controller_current_index;
				int search_index = start_index - 1;
				while (search_index  != start_index)
				{
					prev = get_tab_order_component(search_index);
					if (prev && prev->is_enabled())
					{
						prev->set_focus();
						impl->tab_order_controller_current_index = prev->get_tab_order();
						break;
					}
					search_index--;
					if (search_index < 0)
						search_index = impl->tab_order_controller_last_index;
				}
			}
		}
		else if (impl->tab_order_controller_current_index < 0 && impl->tab_order_controller_last_index != -1) 
		{
			// go to last item
			CL_GUIComponent *last = get_tab_order_component(impl->tab_order_controller_last_index);
			if (last)
			{
				last->set_focus();
				impl->tab_order_controller_current_index = impl->tab_order_controller_last_index;
			}
		}
	}
	else
	{
		CL_GUIComponent *tab_order_controller = get_tab_order_controller();

		if (tab_order_controller)
			tab_order_controller->focus_previous();
	}
}

CL_GUIComponent *CL_GUIComponent::get_tab_order_component(int index, CL_GUIComponent *comp)
{
	if (comp == 0 && !is_tab_order_controller())
	{
		CL_GUIComponent *tab_controller = get_tab_order_controller();
		if (tab_controller)
			return tab_controller->get_tab_order_component(index);
		else
			return 0;
	}

	if (comp == 0 && is_tab_order_controller())
		comp = this;

	CL_GUIComponent *child = comp->get_first_child();

	while (child != 0)
	{
		if (child->get_tab_order() == index)
			return child;

		if (child->has_child_components())
		{
			CL_GUIComponent *test = child->get_tab_order_component(index, child);
			if (test != 0)
				return test;
		}
		
		child = child->get_next_sibling();
	}

	return 0;
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

void CL_GUIComponent::set_consumed_keys(CL_GUIConsumedKeys &keys)
{
	impl->consumed_keys = keys;
}

void CL_GUIComponent::set_constant_repaint(bool enable)
{
	impl->constant_repaint = enable;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIComponent Implementation:

CL_GUIComponent::CL_GUIComponent(CL_GUIComponent &other)
{
	throw CL_Exception(cl_text("CL_GUIComponent copy construction disallowed"));
}

CL_GUIComponent &CL_GUIComponent::operator =(const CL_GUIComponent &other)
{
	throw CL_Exception(cl_text("CL_GUIComponent operator = disallowed"));
}

