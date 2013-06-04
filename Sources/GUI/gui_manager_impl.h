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
**    Mark Page
*/

#pragma once

#include "API/Core/Signals/callback_1.h"
#include "API/Core/Signals/callback_v2.h"
#include "API/Core/Resources/xml_resource_document.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/CSSLayout/CSSDocument/css_document.h"
#include "API/CSSLayout/CSSDocument/css_property.h"
#include "API/Core/System/timer.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_description.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/accelerator_table.h"
#include "API/GUI/gui_window_manager.h"
#include "CSSLayout/css_resource_cache.h"
#include <vector>
#include <map>

namespace clan
{

class GUIMessage;
class GUIWindowManager;
class GUIComponent_Impl;
class InputEvent;
class Font;
class Image;
class GUITopLevelWindow_Alive;

class GUITopLevelWindow
{
public:
	GUITopLevelWindow() : alive(new int) {}

	GUIComponent *component;
	GUIComponent *focused_component;
	GUIComponent *owner;
	GUIComponent *proximity_component;
	std::vector<Rect> update_regions;

private:
	std::shared_ptr<int> alive;	// Shared Pointer, used to determine if this class is active
	friend class GUITopLevelWindow_Alive;
};

/// Helper class, used to determine when a top level window has been destroyed
class GUITopLevelWindow_Alive
{
/// \name Construction
/// \{
public:
	/// \brief Creates a alive object
	///
	/// \param component = The component (NULL or a valid component)
	GUITopLevelWindow_Alive(GUITopLevelWindow *window);

public:
	/// \brief Determines if the component is destroyed
	///
	/// \return true if destroyed
	bool is_null() const;

private:
	std::weak_ptr<int> window_alive;
};

class GUIManager_Impl
{
/// \name Construction
/// \{
public:
	GUIManager_Impl();
	~GUIManager_Impl();

/// \}
/// \name Attributes
/// \{
public:
	GUIComponent *get_focus_component();
	GUIComponent *get_owner_component(const GUIComponent *component);
	const GUIComponent *get_owner_component(const GUIComponent *component) const;
	bool has_focus(const GUIComponent *component) const;
	GUITopLevelWindow *get_toplevel_window(const GUIComponent * const component) const;

	/// \brief Search children for component with is_cancel() set.
	GUIComponent *get_cancel_component(GUIComponent *comp=0);

	/// \brief Search children for component with is_default() set.
	GUIComponent *get_default_component(GUIComponent *comp=0);

	std::vector<GUITopLevelWindow *> root_components;
	Signal_v1<std::shared_ptr<GUIMessage> &> sig_filter_message;
	Callback_0<int> func_exec_handler;
	mutable CSSDocument css_document;

	ResourceManager resources;
	CSSResourceCache resource_cache;

	GUIComponent *mouse_capture_component;
	GUIWindowManager window_manager;
	bool exit_flag;
	int exit_code;
	AcceleratorTable accel_table;
	GUIWindowManagerSite wm_site;
	Callback_v1<GUITopLevelWindow *> func_focus_lost;
	Callback_v1<GUITopLevelWindow *> func_focus_gained;
	Callback_v2<GUITopLevelWindow *, const Size &> func_resize;
	Callback_v2<GUITopLevelWindow *, const Rect &> func_paint;
	Callback_v1<GUITopLevelWindow *> func_close;
	Callback_v1<GUITopLevelWindow *> func_destroy;
	Callback_v2<GUITopLevelWindow *, const InputEvent &> func_input_received;

/// \}
/// \name Operations
/// \{
public:
	void add_component(GUIComponent *component, GUIComponent *owner, const GUITopLevelDescription &desc);
	void remove_component(GUIComponent_Impl *component_impl);
	void gain_focus(GUIComponent *component);
	void loose_focus(GUIComponent *component);
	void set_enabled(GUIComponent *component, bool enable);
	void dispatch_message(std::shared_ptr<GUIMessage> message);

	bool is_constant_repaint_enabled() const;
	bool is_constant_repaint_enabled(GUIComponent *component) const;
	void invalidate_constant_repaint_components();

	void process_standard_gui_keys(std::shared_ptr<GUIMessage> &message);

/// \}
/// \name Implementation
/// \{
private:
	void deliver_message(std::shared_ptr<GUIMessage> &message);
	void dispatch_message_to_component(GUIComponent *target, const InputEvent &input_event);
	void process_pointer_moved_at(GUIComponent *this_component, const Point &point, bool force_pointer_enter);

	void on_focus_lost(GUITopLevelWindow *toplevel_window);
	void on_focus_gained(GUITopLevelWindow *toplevel_window);
	void on_resize(GUITopLevelWindow *toplevel_window, const Size &new_size);
	void on_paint(GUITopLevelWindow *toplevel_window, const Rect &update_rect);
	void on_close(GUITopLevelWindow *toplevel_window);
	void on_destroy(GUITopLevelWindow *toplevel_window);
	void on_input_received(GUITopLevelWindow *toplevel_window, const InputEvent &input_event);

	Image on_resource_cache_get_image(Canvas &canvas, const std::string &url);

/// \}
};

}
