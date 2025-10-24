/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "precomp.h"
#include "API/Core/System/event_trigger.h"
#include "API/Core/System/clanstring.h"
#include "API/Core/System/log.h"
#include "API/GUI/gui_manager.h"
#include "API/Core/XML/dom_element.h"
#include "component_generic.h"
#include "component_manager_generic.h"
#include "API/Core/System/cl_assert.h"
#include <vector>

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_Component::CL_Component(
	CL_Component *parent,
	CL_StyleManager *style)
:
	impl(NULL)
{
	impl = new CL_Component_Generic(this, CL_Rect(0,0,0,0), parent, style);

	if (get_style_manager() != NULL)
		get_style_manager()->connect_styles("component", this);

	if (parent != NULL) parent->add_child(this, true); //SETH: Changed delete child to true
}

CL_Component::CL_Component(const CL_Rect &pos, CL_Component *parent, CL_StyleManager *style)
: impl(NULL)
{
	impl = new CL_Component_Generic(this, pos, parent, style);

	if (get_style_manager() != NULL)
		get_style_manager()->connect_styles("component", this);

	if (parent != NULL) parent->add_child(this, true); //SETH: Changed delete child to true);
}

CL_Component::~CL_Component()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

CL_Component *CL_Component::get_client_area() const
{
	return impl->client_area;
}

CL_Component *CL_Component::get_parent() const
{
	return impl->parent;
}

CL_Component *CL_Component::get_root_parent()
{
	if(impl->parent)
		return impl->parent->get_root_parent();
	else
		return this;
}

CL_GUIManager *CL_Component::get_gui_manager() const
{
	return impl->get_gui_manager();
}

CL_StyleManager *CL_Component::get_style_manager() const
{
	return impl->style_manager;
}

int CL_Component::get_width() const
{
	return impl->position.right - impl->position.left;
}

int CL_Component::get_height() const
{
	return impl->position.bottom - impl->position.top;
}

int CL_Component::get_screen_x() const
{
	if (impl->parent == NULL)
	{
		return impl->position.left;
	}
	else
	{
		int x = impl->parent->get_screen_x();
		return x + impl->position.left;
	}
}

int CL_Component::get_screen_y() const
{
	if (impl->parent == NULL)
	{
		return impl->position.top;
	}
	else
	{
		int y = impl->parent->get_screen_y();
		return y + impl->position.top;
	}
}

int CL_Component::get_client_x() const
{
	return impl->position.left;
}

int CL_Component::get_client_y() const
{
	return impl->position.top;
}

CL_Rect CL_Component::get_screen_rect() const
{
	if (impl->parent == NULL)
	{
		return impl->position;
	}
	else
	{
		CL_Rect r = impl->parent->get_screen_rect();

		return CL_Rect(
			r.left + impl->position.left, 
			r.top + impl->position.top,
			r.left + impl->position.right, 
			r.top + impl->position.bottom);
	}
}

CL_Rect CL_Component::client_to_screen(const CL_Rect & rect)
{
	int x = get_screen_x();
	int y = get_screen_y();

	return CL_Rect(rect.left + x, rect.top + y, rect.right + x, rect.bottom + y);
}

CL_Point CL_Component::client_to_screen(const CL_Point & point)
{
	int x = get_screen_x();
	int y = get_screen_y();

	return CL_Point(x + point.x, y + point.y);
}

CL_Rect CL_Component::screen_to_client(const CL_Rect & rect)
{
	int x = get_screen_x();
	int y = get_screen_y();

	return CL_Rect(rect.left - x, rect.top - y, rect.right - x, rect.bottom - y);
}

CL_Point CL_Component::screen_to_client(const CL_Point & point)
{
	int x = get_screen_x();
	int y = get_screen_y();

	return CL_Point(point.x - x, point.y - y);
}

const CL_Rect &CL_Component::get_position() const
{
	return impl->position;
}

//isn't there a better global place to put all these?
#define cl_min(a,b) ((a < b) ? a : b)
#define cl_max(a,b) ((a > b) ? a : b)

CL_Rect CL_Component::get_position_with_children()
{
	CL_Rect r;
	CL_Rect rChildren = get_children_rect();
		
	r.top = cl_min (impl->position.top, rChildren.top + impl->position.top);
	r.left = cl_min (impl->position.left, rChildren.left + impl->position.left);

	r.right = cl_max(impl->position.right, rChildren.right + impl->position.left);
	r.bottom = cl_max(impl->position.bottom, rChildren.bottom + impl->position.top);

	return r;
}

CL_Size CL_Component::get_minimum_size() const
{
	return impl->minimum_size;
}

CL_Size CL_Component::get_maximum_size() const
{
	return impl->maximum_size;
}

CL_Size CL_Component::get_preferred_size() const
{
	return impl->preferred_size;
}

const CL_LayoutPolicy &CL_Component::get_vertical_policy() const
{
	return impl->vertical_policy;
}

const CL_LayoutPolicy &CL_Component::get_horizontal_policy() const
{
	return impl->horizontal_policy;
}

const CL_Layout &CL_Component::get_layout() const
{
	return impl->layout;
}

bool CL_Component::get_event_passing() const
{
	return impl->event_passing;
}

bool CL_Component::has_child(CL_Component *component) const
{
	for (
		std::list<CL_Component *>::iterator it = impl->children.begin();
		it != impl->children.end();
		it++)
	{
		if ((*it) == component || (*it)->has_child(component)) return true;
	}

	return false;
}

const std::list<CL_Component *> &CL_Component::get_children() const
{
	return impl->children;
}

CL_Rect CL_Component::get_children_rect() const
{
	CL_Rect rect(0, 0, 0, 0);

	if (impl->clipping)
	{
		//we clip our children, so their rect can't be larger than ours, fixes selection problem in treeview -SAR April 27th 2007
		return get_position();
	}

	for (
		std::list<CL_Component *>::iterator it = impl->children.begin();
		it != impl->children.end();
		it++)
	{
		
		CL_Component *comp = (*it);

	   if (!comp->is_visible(false)) continue;
	   if (!comp->is_focusable()) continue;

		CL_Rect childrect = comp->get_position_with_children();

		if(childrect.left < rect.left)
			rect.left = childrect.left;
		if(childrect.top < rect.top)
			rect.top = childrect.top;
		if(childrect.right > rect.right)
			rect.right = childrect.right;
		if(childrect.bottom > rect.bottom)
			rect.bottom = childrect.bottom;
	}

	return rect;
}

CL_Component *CL_Component::get_component_at(int pos_x, int pos_y)
{
	std::list<CL_Component *>::iterator it;
	for (it = impl->children.begin(); it != impl->children.end(); it++)
	{
		CL_Component *cur_component = (*it);

		if(cur_component->is_visible(false))
		{
			const CL_Rect r = cur_component->get_position_with_children();

			if (pos_x >= r.left && pos_x < r.right &&
				pos_y >= r.top && pos_y < r.bottom)
			{
				return cur_component->get_component_at(pos_x - r.left, pos_y - r.top);
			}
		}
	}

	return this;
}

CL_Component *CL_Component::get_focus() const
{
	return get_gui_manager()->get_focus();
}

bool CL_Component::has_focus() const
{
	return get_focus() == this;
}

bool CL_Component::is_focusable() const
{
	return impl->focusable;
}

bool CL_Component::has_mouse_over() const
{
	return impl->mouse_over;
}

bool CL_Component::is_visible(bool check_parents) const
{
	if(check_parents)
	{
		if(impl->visible == false)
			return false;
		if(impl->parent)
			return impl->parent->is_visible(true);
		else
			return impl->visible;
	}
	else
		return impl->visible;
}

bool CL_Component::is_enabled(bool check_parents) const
{
	if(check_parents)
	{
		if(impl->enabled == false)
			return false;
		if(impl->parent)
			return impl->parent->is_enabled(true);
		else
			return impl->enabled;
	}
	else
		return impl->enabled;
}

bool CL_Component::is_mouse_captured() const
{
	CL_GUIManager *gui = get_gui_manager();
	if (gui != NULL)
	{
		return gui->get_capture() == this;
	}
	return false;
}

bool CL_Component::is_modal() const
{
	CL_GUIManager *gui = get_gui_manager();
	if (gui != NULL)
	{
		return gui->get_modal_component() == this;
	}
	return false;
}
	
bool CL_Component::has_modal_parent() const
{
	CL_GUIManager *gui = get_gui_manager();
	if (gui != NULL)
	{
		if (impl->parent)
			if (impl->parent == gui->get_modal_component())
				return true;
		else
			return impl->parent->has_modal_parent();
	}
	return false;
}

int CL_Component::get_tab_id() const
{
	return impl->tab_id;
}

CL_ComponentStyle *CL_Component::get_style() const
{
	return impl->style;
}

bool CL_Component::is_topmost() const
{
	return impl->topmost_flag;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_Component::set_tab_id(int id)
{
	impl->tab_id = id;
}

void CL_Component::set_position(const CL_Rect &new_pos)
{
	int old_x = impl->position.left;
	int old_y = impl->position.top;

	int old_width = get_width();
	int old_height = get_height();

	impl->position = new_pos;
	impl->layout.set_position(CL_Rect(CL_Point(0,0), impl->position.get_size()));

	if(old_x != impl->position.left || old_y != impl->position.top)
		impl->sig_move(old_x, old_y);
	if(old_width != get_width() || old_height != get_height())
		impl->sig_resize(old_width, old_height);

	update();
}

void CL_Component::set_position(int new_x, int new_y)
{
	int old_x = impl->position.left;
	int old_y = impl->position.top;

	impl->position.right = get_width() + new_x;
	impl->position.bottom = get_height() + new_y;
	impl->position.left = new_x;
	impl->position.top = new_y;
	impl->layout.set_position(CL_Rect(CL_Point(0,0), impl->position.get_size()));

	if(old_x != impl->position.left || old_y != impl->position.top)
		impl->sig_move(old_x, old_y);

	update();
}

void CL_Component::set_size(int new_width, int new_height)
{
	int old_width = get_width();
	int old_height = get_height();

	impl->position.right = impl->position.left + new_width;
	impl->position.bottom = impl->position.top + new_height;
	impl->layout.set_position(CL_Rect(CL_Point(0,0), impl->position.get_size()));

	if(old_width != get_width() || old_height != get_height())
		impl->sig_resize(old_width, old_height);

	update();
}

void CL_Component::set_width(int width)
{
	int old_width = get_width();

	impl->position.right = impl->position.left + width;
	impl->layout.set_position(CL_Rect(CL_Point(0,0), impl->position.get_size()));

	if(old_width != get_width())
		impl->sig_resize(old_width, get_height());

	update();
}

void CL_Component::set_height(int height)
{
	int old_height = get_height();

	impl->position.bottom = impl->position.top + height;
	impl->layout.set_position(CL_Rect(CL_Point(0,0), impl->position.get_size()));

	if(old_height != get_height())
		impl->sig_resize(get_width(), old_height);

	update();
}

void CL_Component::set_minimum_size(const CL_Size &size)
{
	impl->minimum_size = size;

	update();
}

void CL_Component::set_maximum_size(const CL_Size &size)
{
	impl->maximum_size = size;

	update();
}

void CL_Component::set_preferred_size(const CL_Size &size)
{
	impl->preferred_size = size;

	update();
}

void CL_Component::set_vertical_policy(const CL_LayoutPolicy &policy)
{
	impl->vertical_policy = policy;
}

void CL_Component::set_horizontal_policy(const CL_LayoutPolicy &policy)
{
	impl->horizontal_policy = policy;
}

void CL_Component::set_layout(const CL_Layout &layout)
{
	impl->layout = layout;
}

void CL_Component::set_event_passing(bool enable)
{
	impl->event_passing = enable;
}

void CL_Component::set_parent(CL_Component *parent)
{
	impl->parent = parent;
}

void CL_Component::set_gui_manager(CL_GUIManager *gui)
{
	impl->gui = gui;
}

void CL_Component::show(bool show)
{
	impl->visible = show;
	update();
}

void CL_Component::enable(bool enable)
{
	impl->enabled = enable;
	update();
}

void CL_Component::set_focusable(bool focusable)
{
	impl->focusable = focusable;
	update();
}

void CL_Component::set_focus()
{
	get_gui_manager()->set_focus(this);
}

void CL_Component::raise()
{
	
	if (impl->parent == 0) return;

	std::list<CL_Component *> &children = impl->parent->impl->children;
	children.remove(this);
	if (impl->topmost_flag)
	{
		children.push_front(this);
	}
	else
	{
		std::list<CL_Component *>::iterator it;
		for (it = children.begin(); it != children.end(); ++it)
		{
			CL_Component *cur = *it;
			if (cur->impl->topmost_flag == false)
			{
				children.insert(it, this);
				return;
			}
		}
		children.push_back(this);
	}

	update();
}

void CL_Component::lower()
{
	
	if (impl->parent == 0) return;

	std::list<CL_Component *> &children = impl->parent->impl->children;
	children.remove(this);
	if (impl->topmost_flag)
	{
		std::list<CL_Component *>::iterator it;
		for (it = children.begin(); it != children.end(); ++it)
		{
			CL_Component *cur = *it;
			if (cur->impl->topmost_flag == false)
			{
				children.insert(it, this);
				return;
			}
		}
		children.push_back(this);
	}
	else
	{
		children.push_back(this);
	}

	update();
}

void CL_Component::capture_mouse()
{
	CL_GUIManager *gui = get_gui_manager();
	if (gui != NULL)
	{
		gui->gui_capture_mouse(this);
	}
}

void CL_Component::release_mouse()
{
	CL_GUIManager *gui = get_gui_manager();
	if (gui != NULL)
	{
		gui->gui_release_mouse();
	}
}

void CL_Component::update()
{
	CL_GUIManager *gui_manager = get_gui_manager();
	if (gui_manager && !gui_manager->is_shutting_down()) gui_manager->get_update_trigger()->set_flag();
}

void CL_Component::paint()
{
	impl->sig_begin_paint();
	impl->sig_paint();
	impl->sig_paint_children();
	impl->sig_end_paint();
}

void CL_Component::run()
{
	update();
	get_gui_manager()->run(this);
}

void CL_Component::quit()
{
	get_gui_manager()->quit();
}

void CL_Component::close()
{
	cl_assert(!is_modal() && "Use quit, not close() on modal dialogs!  Otherwise the parent member gets nulled, causing problems with focus when the children are deleted later");
	impl->sig_close();
	update();
}

void CL_Component::add_child(CL_Component *child, bool delete_child)
{
	if(has_child(child))
		return;

	impl->sig_child_before_add(child);

	child->set_parent(this);

	impl->children.push_front(child);
	if (!child->is_topmost()) child->raise();

	if (delete_child)
		impl->delete_children.push_front(child);

	impl->sig_child_add(child);

	update();
}

void CL_Component::delete_child (CL_Component *child)
{
	if (!has_child (child))
		return;

	impl->delete_children.push_front(child);
}

void CL_Component::remove_child(CL_Component *child)
{
	impl->sig_child_before_remove(child);

	child->impl->remove_focus();
	
	child->set_parent(NULL);

	impl->children.remove(child);
	impl->delete_children.remove(child);

	impl->sig_child_remove(child);

	update();
}

void CL_Component::set_style(CL_ComponentStyle *style, bool delete_style)
{
	if (impl->delete_style && NULL != impl->style)
		delete impl->style;

	impl->delete_style = delete_style;
	impl->style = style;
}

void CL_Component::find_preferred_size()
{
	impl->find_preferred_size();
	update();
}

CL_Component *CL_Component::find_tab_id(int tab_id)
{
	if (impl->tab_id == tab_id) return this;

	std::list<CL_Component *>::iterator it;
	for (it = impl->children.begin(); it != impl->children.end(); it++)
	{
		CL_Component *comp = (*it)->find_tab_id(tab_id);
		if (comp) return comp;
	}
	return 0;
}

void CL_Component::set_client_area(CL_Component *component)
{
	impl->client_area = component;
	update();
}

void CL_Component::set_topmost_flag(bool topmost)
{
	if (impl->topmost_flag == topmost) return;

	impl->topmost_flag = topmost;
	if (topmost) lower();
	else raise();
}

void CL_Component::set_clipping(bool enable)
{
	impl->clipping = enable;
}

/////////////////////////////////////////////////////////////////////////////
// Signals:

CL_Signal_v1<const CL_DomElement &> &CL_Component::sig_set_options()
{
	return impl->sig_set_options;
}

CL_Signal_v0 &CL_Component::sig_paint()
{
	return impl->sig_paint;
}

CL_Signal_v0 &CL_Component::sig_paint_children()
{
	return impl->sig_paint_children;
}

CL_Signal_v0 &CL_Component::sig_begin_paint()
{
	return impl->sig_begin_paint;
}

CL_Signal_v0 &CL_Component::sig_end_paint()
{
	return impl->sig_end_paint;
}

CL_Signal_v1<const CL_InputEvent &> &CL_Component::sig_input_down()
{
	return impl->sig_input_down;
}

CL_Signal_v1<const CL_InputEvent &> &CL_Component::sig_input_up()
{
	return impl->sig_input_up;
}

CL_Signal_v1<const CL_InputEvent &> &CL_Component::sig_key_down()
{
	return impl->sig_key_down;
}

CL_Signal_v1<const CL_InputEvent &> &CL_Component::sig_key_up()
{
	return impl->sig_key_up;
}

CL_Signal_v1<const CL_InputEvent &> &CL_Component::sig_mouse_down()
{
	return impl->sig_mouse_down;
}

CL_Signal_v1<const CL_InputEvent &> &CL_Component::sig_mouse_up()
{
	return impl->sig_mouse_up;
}

CL_Signal_v1<const CL_InputEvent &> &CL_Component::sig_mouse_dblclk()
{
	return impl->sig_mouse_dblclk;
}

CL_Signal_v1<const CL_InputEvent &> &CL_Component::sig_mouse_move()
{
	return impl->sig_mouse_move;
}

CL_Signal_v0 &CL_Component::sig_mouse_enter()
{
	return impl->sig_mouse_enter;
}

CL_Signal_v0 &CL_Component::sig_mouse_leave()
{
	return impl->sig_mouse_leave;
}

CL_Signal_v0 &CL_Component::sig_got_focus()
{
	return impl->sig_got_focus;
}

CL_Signal_v0 &CL_Component::sig_lost_focus()
{
	return impl->sig_lost_focus;
}

CL_Signal_v0 &CL_Component::sig_close()
{
	return impl->sig_close;
}

CL_Signal_v2<int, int> &CL_Component::sig_resize()
{
	return impl->sig_resize;
}

CL_Signal_v2<int, int> &CL_Component::sig_move()
{
	return impl->sig_move;
}

CL_Signal_v2<int &, int &> &CL_Component::sig_transform_coords()
{
	return impl->sig_transform_coords;
}

CL_Signal_v1<CL_Component *> &CL_Component::sig_child_add()
{
	return impl->sig_child_add;
}

CL_Signal_v1<CL_Component *> &CL_Component::sig_child_remove()
{
	return impl->sig_child_remove;
}

CL_Signal_v1<CL_Component *> &CL_Component::sig_child_before_add()
{
	return impl->sig_child_before_add;
}

CL_Signal_v1<CL_Component *> &CL_Component::sig_child_before_remove()
{
	return impl->sig_child_before_remove;
}

CL_Signal_v1<CL_Size &> &CL_Component::sig_get_minimum_size()
{
	return impl->sig_get_minimum_size;
}

CL_Signal_v1<CL_Size &> &CL_Component::sig_get_maximum_size()
{
	return impl->sig_get_maximum_size;
}

CL_Signal_v1<CL_Size &> &CL_Component::sig_get_preferred_size()
{
	return impl->sig_get_preferred_size;
}
