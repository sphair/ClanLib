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

#ifdef _MSC_VER
#pragma warning (disable:4355) // 'this' used in base member initializer list
#endif

#include "precomp.h"
#include "API/Display/display.h"
#include "API/Core/System/log.h"
#include "API/GUI/component_style.h"
#include "API/Core/XML/dom_element.h"
#include "component_generic.h"
#include "gui_manager_generic.h"

CL_Component_Generic::CL_Component_Generic(
	CL_Component *_owner,
	const CL_Rect &pos,
	CL_Component *_parent,
	CL_StyleManager *style)
:
	owner(_owner),
	parent(_parent),
	client_area(_owner),
	style_manager(style),
	position(pos),
	minimum_size(-1,-1),
	maximum_size(-1,-1),
	preferred_size(-1,-1),
	vertical_policy(layout_expanding),
	horizontal_policy(layout_expanding),
	gui(NULL),
	focusable(true),
	visible(true),
	enabled(true),
	mouse_over(false),
	event_passing(true),
	topmost_flag(false),
	clipping(false),
	tab_id(-1),
	style(NULL), delete_style(false)
{
	if (style_manager == NULL && parent)
		style_manager = parent->get_style_manager();

	slots.connect(sig_set_options, this, &CL_Component_Generic::on_set_options);
	slots.connect(sig_begin_paint, this, &CL_Component_Generic::begin_paint);
	slots.connect(sig_paint_children, this, &CL_Component_Generic::paint_children);
	slots.connect(sig_end_paint, this, &CL_Component_Generic::end_paint);
	slots.connect(sig_input_down, this, &CL_Component_Generic::on_input_down);
	slots.connect(sig_input_up, this, &CL_Component_Generic::on_input_up);
	slots.connect(sig_key_down, this, &CL_Component_Generic::on_key_down);
	slots.connect(sig_key_up, this, &CL_Component_Generic::on_key_up);
	slots.connect(sig_mouse_down, this, &CL_Component_Generic::on_mouse_down);
	slots.connect(sig_mouse_up, this, &CL_Component_Generic::on_mouse_up);
	slots.connect(sig_mouse_dblclk, this, &CL_Component_Generic::on_mouse_dblclk);
	slots.connect(sig_mouse_move, this, &CL_Component_Generic::on_mouse_move);
	slots.connect(sig_close, this, &CL_Component_Generic::on_close);
}

CL_Component_Generic::~CL_Component_Generic()
{
	// If we got a parent, disconnect ourselves from its lists of children:
	if (parent)
	{
		parent->impl->children.remove(owner);
		parent->impl->delete_children.remove(owner);
		parent->update();
	}

	// Clean up component style controlled by us.
	if(delete_style)
		delete style;

	// Delete all children marked for deletion.
	while (!delete_children.empty())
	{
		CL_Component *child = delete_children.front();
		delete_children.pop_front();
		delete child;
	}

	remove_focus();
}

CL_GUIManager *CL_Component_Generic::get_gui_manager() const
{
	if (gui != NULL) return gui;
	if (parent != NULL) return parent->get_gui_manager();
	return NULL;
}

void CL_Component_Generic::remove_focus()
{
	// Make sure component doesn't have the focus:
	CL_GUIManager *gui = owner->get_gui_manager();
	if (gui && gui != owner && gui->impl) gui->impl->component_deleted(owner);
}

void CL_Component_Generic::find_preferred_size()
{
	CL_Size size(0,0);
	owner->sig_get_preferred_size()(size);
	preferred_size = size;
	owner->set_size(size.width, size.height);
}

void CL_Component_Generic::on_set_options(const CL_DomElement &options)
{
	int x = 0, y = 0, width = 0, height = 0;

	if (options.has_attribute("x")) x = CL_String::to_int(options.get_attribute("x"));
	if (options.has_attribute("y")) y = CL_String::to_int(options.get_attribute("y"));
	if (options.has_attribute("width"))  width = CL_String::to_int(options.get_attribute("width"));
	if (options.has_attribute("height")) height = CL_String::to_int(options.get_attribute("height"));

	owner->set_position(CL_Rect(x, y, x + width, y + height));

	if (options.has_attribute("visible")) owner->show(CL_String::to_bool(options.get_attribute("visible")));
	if (options.has_attribute("enabled")) owner->enable(CL_String::to_bool(options.get_attribute("enabled")));
	if (options.has_attribute("tab_id")) owner->set_tab_id(CL_String::to_int(options.get_attribute("tab_id")));

	owner->update();
}

void CL_Component_Generic::begin_paint()
{
	paint_clipping = false;
	if (clipping)
	{
		CL_Rect r = owner->get_screen_rect();
		CL_Display::push_cliprect(CL_Rect(r.left, r.top, r.right, r.bottom));
		paint_clipping = true;
	}
}

void CL_Component_Generic::paint_children()
{
	// we draw the children back to front because then all non-focus input
	// will be routed to the topmost window.
	for (
		std::list<CL_Component *>::reverse_iterator it = children.rbegin();
		it != children.rend(); it++
		)
	{
		if((*it)->is_visible(false)) (*it)->paint();
	}
}

void CL_Component_Generic::end_paint()
{
	if (paint_clipping)
	{
		CL_Display::pop_cliprect();
	}
	paint_clipping = false;
}

void CL_Component_Generic::on_input_down(const CL_InputEvent &key)
{
	if (event_passing == false) return;

	CL_InputEvent translated_key = key;
	translated_key.mouse_pos.x += position.left;
	translated_key.mouse_pos.y += position.top;
	if (parent) parent->sig_input_down()(translated_key);

	owner->update();
}

void CL_Component_Generic::on_input_up(const CL_InputEvent &key)
{
	if (event_passing == false) return;

	CL_InputEvent translated_key = key;
	translated_key.mouse_pos.x += position.left;
	translated_key.mouse_pos.y += position.top;
	if (parent) parent->sig_input_up()(translated_key);

	owner->update();
}

void CL_Component_Generic::on_key_down(const CL_InputEvent &key)
{
	if (event_passing == false) return;

	CL_InputEvent translated_key = key;
	translated_key.mouse_pos.x += position.left;
	translated_key.mouse_pos.y += position.top;
	if (parent) parent->sig_key_down()(translated_key);

	owner->update();
}

void CL_Component_Generic::on_key_up(const CL_InputEvent &key)
{
	if (event_passing == false) return;

	CL_InputEvent translated_key = key;
	translated_key.mouse_pos.x += position.left;
	translated_key.mouse_pos.y += position.top;
	if (parent) parent->sig_key_up()(translated_key);

	owner->update();
}

void CL_Component_Generic::on_mouse_down(const CL_InputEvent &key)
{
	if (event_passing == false) return;

	CL_InputEvent translated_key = key;
	translated_key.mouse_pos.x += position.left;
	translated_key.mouse_pos.y += position.top;
	if (parent) parent->sig_mouse_down()(translated_key);

	owner->update();
}

void CL_Component_Generic::on_mouse_up(const CL_InputEvent &key)
{
	if (event_passing == false) return;

	CL_InputEvent translated_key = key;
	translated_key.mouse_pos.x += position.left;
	translated_key.mouse_pos.y += position.top;
	if (parent) parent->sig_mouse_up()(translated_key);

	owner->update();
}

void CL_Component_Generic::on_mouse_dblclk(const CL_InputEvent &key)
{
	if (event_passing == false) return;

	CL_InputEvent translated_key = key;
	translated_key.mouse_pos.x += position.left;
	translated_key.mouse_pos.y += position.top;
	translated_key.repeat_count = 2;
	if (parent) parent->sig_mouse_dblclk()(translated_key);

	owner->update();
}

void CL_Component_Generic::on_mouse_move(const CL_InputEvent &key)
{
	if (event_passing == false) return;

	CL_InputEvent translated_key = key;
	translated_key.mouse_pos.x += position.left;
	translated_key.mouse_pos.y += position.top;
	if (parent) parent->sig_mouse_move()(translated_key);

	owner->update();
}

void CL_Component_Generic::on_close()
{
	owner->update();

	//	show(false);
	if(get_gui_manager() != NULL)
		get_gui_manager()->quit();
	else
		CL_Log::log("debug", "CL_Component::close: gui manager pointer is null!");

	if(parent != NULL)
		parent->remove_child(owner);
	else
		CL_Log::log("debug", "CL_Component::close: parent pointer is null!");
	//	set_gui_manager(NULL);
}
