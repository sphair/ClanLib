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

#pragma once

#include "API/Core/Signals/callback.h"
#include "API/Core/Signals/signal.h"
#include "API/Core/Math/rect.h"
#include "API/GUI/gui_component.h"
#include "Layout/gui_css_used_values.h"
#include <vector>
#include <map>
#include <memory>
#include "gui_element.h"

namespace clan
{

class GUIManager_Impl;
class GUILayout;
class GUICSSBoxVisitor;
class GUIElement;
class CSSToken;
class SpanLayout;

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
	Callback<void(Canvas &, const Rect &)> func_render;
	Callback<void(std::shared_ptr<GUIMessage> &)> func_process_message;
	Callback<bool()> func_close;
	Callback<bool()> func_activated;
	Callback<bool()> func_deactivated;
	Callback<bool()> func_focus_lost;
	Callback<bool()> func_focus_gained;
	Callback<bool()> func_pointer_enter;
	Callback<bool()> func_pointer_exit;
	
	Callback<bool(const InputEvent &)> func_input;
	Callback<bool(const InputEvent &)> func_input_pressed;
	Callback<bool(const InputEvent &)> func_input_released;
	Callback<bool(const InputEvent &)> func_input_doubleclick;
	Callback<bool(const InputEvent &)> func_input_pointer_moved;
	Callback<void()> func_enablemode_changed;
	Callback<void()> func_resized;
	Callback<void(std::shared_ptr<GUIMessage> &)> func_filter_message;
	Callback<GUIComponent*(GUIComponent*, std::string)> func_create_custom_component;
	Callback<void(bool)> func_visibility_change;
	bool allow_resize;
	bool clip_children;
	Rect clip_children_rect;
	bool enabled;
	bool visible;
	bool activated;
	bool click_through;
	mutable std::string element_name;
	bool default_handler;
	bool cancel_handler;
	bool constant_repaint;
	bool blocks_default_action_when_focused;
	bool is_selected_in_group;
	bool pointer_inside_component;
	std::string group_name;
	bool double_click_enabled;

	Rect geometry;
	bool use_auto_geometry;

	void set_window_geometry(Rect geometry, bool client_area);
	void set_auto_geometry(Rect geometry);
	void set_manual_geometry(Rect geometry);
	void reset_geometry();
	void window_resized();

	GUIElement element;
	GUICSSUsedValues css_used_values;




	void geometry_updated(bool geometry_was_resized);
	void invoke_enablemode_changed();

	void visit_children(GUICSSBoxVisitor *visitor, bool recursive);
	void layout_content();

	void on_process_message(std::shared_ptr<GUIMessage> &msg);
	static CSSToken next_token(size_t &pos, const std::vector<CSSToken> &tokens, bool skip_whitespace = true);

};

}
