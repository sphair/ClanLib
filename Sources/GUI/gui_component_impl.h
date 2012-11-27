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

#pragma once

#include "API/Core/Signals/callback_2.h"
#include "API/Core/Signals/signal_v1.h"
#include "API/GUI/gui_component.h"
#include "API/CSSLayout/css_layout.h"
#include "API/CSSLayout/css_layout_element.h"
#include "API/CSSLayout/css_box_properties.h"
#include <vector>
#include <map>
#include <memory>
#include "API/Core/Math/rect.h"
#include "css_clan_box_used_values.h"

namespace clan
{

class GUIManager_Impl;
class GUILayout;
class CSSClanBoxVisitor;

class GUIComponent_Impl
{
public:
	GUIComponent_Impl(const std::shared_ptr<GUIManager_Impl> &gui_manager_impl, GUIComponent *parent, bool toplevel);
	~GUIComponent_Impl();

	static GUIComponent_Impl *create_from_parent(GUIComponent *parent);
	static GUIComponent_Impl *create_from_owner(GUIComponent *owner);
	static GUIComponent_Impl *create_from_manager(GUIManager *manager);

	std::weak_ptr<GUIManager_Impl> gui_manager;

	// We keep a second pointer to GUIComponent_Impl because the first one becomes null at shutdown,
	// and we need to shutdown correctly during this phase.
	GUIManager_Impl *gui_manager_impl;

	GUIComponent *component;
	GUIComponent *parent;
	GUIComponent *prev_sibling;
	GUIComponent *next_sibling;
	GUIComponent *first_child;
	GUIComponent *last_child;
	GUIComponent::FocusPolicy focus_policy;
	GUILayout layout;
	Callback_v2<Canvas &, const Rect &> func_render;
	Callback_v1<std::shared_ptr<GUIMessage> &> func_process_message;
	Callback_0<bool> func_close;
	Callback_0<bool> func_activated;
	Callback_0<bool> func_deactivated;
	Callback_0<bool> func_focus_lost;
	Callback_0<bool> func_focus_gained;
	Callback_0<bool> func_pointer_enter;
	Callback_0<bool> func_pointer_exit;
	Callback_v1<CSSBoxProperties &> func_default_properties;
	Callback_1<bool, const InputEvent &> func_input;
	Callback_1<bool, const InputEvent &> func_input_pressed;
	Callback_1<bool, const InputEvent &> func_input_released;
	Callback_1<bool, const InputEvent &> func_input_doubleclick;
	Callback_1<bool, const InputEvent &> func_input_pointer_moved;
	Signal_v1<const CSSPropertyList &> sig_style_changed;
	Callback_v0 func_enablemode_changed;
	Callback_v0 func_resized;
	Callback_v1<std::shared_ptr<GUIMessage> &> func_filter_message;
	Callback_2<GUIComponent*, GUIComponent*, std::string> func_create_custom_component;
	Callback_v1<bool> func_visibility_change;
	std::string tag_name;
	std::string id;
	std::string class_string;
	std::vector<std::string> pseudo_classes;
	bool allow_resize;
	bool clip_children;
	Rect clip_children_rect;
	bool enabled;
	bool visible;
	bool activated;
	bool click_through;
	Rect geometry;
	mutable std::string element_name;
	bool default_handler;
	bool cancel_handler;
	bool constant_repaint;
	bool blocks_default_action_when_focused;
	bool is_selected_in_group;
	bool pointer_inside_component;
	std::string group_name;
	CSSLayout css_layout;
	CSSLayoutElement css_element;
	bool double_click_enabled;

	CSSBoxProperties css_properties;
	CSSClanBoxUsedValues css_used_values;

	void set_geometry(Rect new_geometry, bool client_area);
	void geometry_updated(bool geometry_was_resized);
	void invoke_enablemode_changed();

	void layout_content();

	void visit_css(CSSClanBoxVisitor *visitor, bool base_visitor);

	void on_process_message(std::shared_ptr<GUIMessage> &msg);
	void update_style();

private:
	Image on_css_layout_get_image(Canvas &canvas, const std::string &url);

};

}
