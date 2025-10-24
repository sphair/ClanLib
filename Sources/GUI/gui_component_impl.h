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

#pragma once

#include "API/Core/System/weakptr.h"
#include "API/Core/Signals/callback_2.h"
#include "API/GUI/gui_component.h"
#include "API/CSSLayout/css_layout.h"
#include "API/CSSLayout/css_layout_element.h"
#include <vector>
#include <map>
#include "API/Core/Math/rect.h"

class CL_GUIManager_Impl;
class CL_GUILayout;

class CL_GUIComponent_Impl
{
/// \name Construction
/// \{

public:
	CL_GUIComponent_Impl(const CL_SharedPtr<CL_GUIManager_Impl> &gui_manager_impl, CL_GUIComponent *parent, bool toplevel);
	~CL_GUIComponent_Impl();

	static CL_GUIComponent_Impl *create_from_parent(CL_GUIComponent *parent);
	static CL_GUIComponent_Impl *create_from_owner(CL_GUIComponent *owner);
	static CL_GUIComponent_Impl *create_from_manager(CL_GUIManager *manager);

/// \}
/// \name Attributes
/// \{

public:
	CL_WeakPtr<CL_GUIManager_Impl> gui_manager;

	// We keep a second pointer to CL_GUIComponent_Impl because the first one becomes null at shutdown,
	// and we need to shutdown correctly during this phase.
	CL_GUIManager_Impl *gui_manager_impl;

	CL_GUIComponent *component;
	CL_GUIComponent *parent;
	CL_GUIComponent *prev_sibling;
	CL_GUIComponent *next_sibling;
	CL_GUIComponent *first_child;
	CL_GUIComponent *last_child;
	CL_GUIComponent::FocusPolicy focus_policy;
	CL_GUILayout layout;
	CL_Callback_v2<CL_GraphicContext &, const CL_Rect &> func_render;
	CL_Callback_v1<CL_GUIMessage &> func_process_message;
	CL_Callback_0<bool> func_close;
	CL_Callback_0<bool> func_activated;
	CL_Callback_0<bool> func_deactivated;
	CL_Callback_0<bool> func_focus_lost;
	CL_Callback_0<bool> func_focus_gained;
	CL_Callback_0<bool> func_pointer_enter;
	CL_Callback_0<bool> func_pointer_exit;
	CL_Callback_1<bool, const CL_InputEvent &> func_input;
	CL_Callback_1<bool, const CL_InputEvent &> func_input_pressed;
	CL_Callback_1<bool, const CL_InputEvent &> func_input_released;
	CL_Callback_1<bool, const CL_InputEvent &> func_input_doubleclick;
	CL_Callback_1<bool, const CL_InputEvent &> func_input_pointer_moved;
	CL_Callback_v0 func_style_changed;
	CL_Callback_v0 func_enablemode_changed;
	CL_Callback_v0 func_resized;
	CL_Callback_v1<CL_GUIMessage &> func_filter_message;
	CL_Callback_2<CL_GUIComponent*, CL_GUIComponent*, CL_String> func_create_custom_component;
	CL_Callback_v1<bool> func_visibility_change;
	CL_String type_name;
	CL_String class_name;
	CL_String id_name;
	bool allow_resize;
	bool clip_children;
	CL_Rect clip_children_rect;
	bool enabled;
	bool visible;
	bool activated;
	bool click_through;
	CL_Rect geometry;
	mutable CL_String element_name;
	bool default_handler;
	bool cancel_handler;
	bool constant_repaint;
	bool blocks_default_action_when_focused;
	bool is_selected_in_group;
	CL_String group_name;
	CL_CSSLayout css_layout;
	CL_CSSLayoutElement css_element;

/// \}
/// \name Operations
/// \{

public:
	void set_geometry(CL_Rect new_geometry, bool client_area);
	void geometry_updated();

/// \}
/// \name Implementation
/// \{

private:
	CL_Image on_css_layout_get_image(CL_GraphicContext &gc, const CL_String &url);
/// \}
};


