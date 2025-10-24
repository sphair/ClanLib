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
**    Mark Page
*/

#pragma once

#include "API/Core/Signals/callback_1.h"
#include "API/Core/Signals/callback_v2.h"
#include "API/Core/CSS/css_document.h"
#include "API/Core/CSS/css_property.h"
#include "API/Core/System/timer.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_description.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/accelerator_table.h"
#include "API/GUI/gui_window_manager.h"
#include "API/GUI/gui_theme.h"
#include "gui_font_cache.h"
#include "Theme/gui_theme_layout_manager.h"
#include <vector>
#include <map>

class CL_GUIMessage;
class CL_GUITheme;
class CL_GUIWindowManager;
class CL_GUIComponent_Impl;
class CL_InputEvent;
class CL_InputState;
class CL_Font;
class CL_GUITopLevelWindow_Alive;

class CL_GUITopLevelWindow
{
public:
	CL_GUITopLevelWindow() : alive(new int) {}

	CL_GUIComponent *component;
	CL_GUIComponent *focused_component;
	CL_GUIComponent *owner;
	CL_GUIComponent *proximity_component;
	std::vector<CL_Rect> update_regions;

private:
	CL_SharedPtr<int> alive;	// Shared Pointer, used to determine if this class is active
	friend class CL_GUITopLevelWindow_Alive;
};

/// Helper class, used to determine when a top level window has been destroyed
class CL_GUITopLevelWindow_Alive
{
/// \name Construction
/// \{
public:
	/// \brief Creates a alive object
	///
	/// \param component = The component (NULL or a valid component)
	CL_GUITopLevelWindow_Alive(CL_GUITopLevelWindow *window);

public:
	/// \brief Determines if the component is destroyed
	///
	/// \return true if destroyed
	bool is_null() const;

private:
	CL_WeakPtr<int> window_alive;
};

class CL_GUIManager_Impl
{
/// \name Construction
/// \{
public:
	CL_GUIManager_Impl();
	~CL_GUIManager_Impl();

/// \}
/// \name Attributes
/// \{
public:
	CL_GUIComponent *get_focus_component();
	CL_GUIComponent *get_owner_component(const CL_GUIComponent *component);
	const CL_GUIComponent *get_owner_component(const CL_GUIComponent *component) const;
	bool has_focus(const CL_GUIComponent *component) const;
	CL_GUITopLevelWindow *get_toplevel_window(const CL_GUIComponent * const component) const;

	/// \brief Search children for component with is_cancel() set.
	CL_GUIComponent *get_cancel_component(CL_GUIComponent *comp=0);

	/// \brief Search children for component with is_default() set.
	CL_GUIComponent *get_default_component(CL_GUIComponent *comp=0);

	CL_Font get_registered_font(const CL_FontDescription &desc);

	std::vector<CL_GUITopLevelWindow *> root_components;
	CL_Signal_v1<CL_GUIMessage &> sig_filter_message;
	CL_Callback_0<int> func_exec_handler;
	CL_CSSDocument css_document;
	CL_GUIThemeLayoutManager layout_manager;

	CL_GUIComponent *mouse_capture_component;
	CL_GUIComponent *mouse_over_component;
	CL_GUITheme theme;
	CL_GUIWindowManager window_manager;
	bool exit_flag;
	int exit_code;
	CL_AcceleratorTable accel_table;
	CL_GUIWindowManagerSite wm_site;
	CL_Callback_v1<CL_GUITopLevelWindow *> func_focus_lost;
	CL_Callback_v1<CL_GUITopLevelWindow *> func_focus_gained;
	CL_Callback_v2<CL_GUITopLevelWindow *, const CL_Size &> func_resize;
	CL_Callback_v2<CL_GUITopLevelWindow *, const CL_Rect &> func_paint;
	CL_Callback_v1<CL_GUITopLevelWindow *> func_close;
	CL_Callback_v1<CL_GUITopLevelWindow *> func_destroy;
	CL_Callback_v3<CL_GUITopLevelWindow *, const CL_InputEvent &, const CL_InputState &> func_input_received;
	CL_GUIFontCache font_cache;

/// \}
/// \name Operations
/// \{
public:
	void add_component(CL_GUIComponent *component, CL_GUIComponent *owner, CL_GUITopLevelDescription desc);
	void remove_component(CL_GUIComponent_Impl *component_impl);
	void gain_focus(CL_GUIComponent *component);
	void loose_focus(CL_GUIComponent *component);
	void set_enabled(CL_GUIComponent *component, bool enable);
	void dispatch_message(CL_GUIMessage &message);

	std::vector<CL_CSSProperty> &get_properties(const CL_StringRef &element_name) const;
	void reset_properties();

	void register_font(const CL_Font &font, const CL_FontDescription &desc);

	bool is_constant_repaint_enabled() const;
	bool is_constant_repaint_enabled(CL_GUIComponent *component) const;
	void invalidate_constant_repaint_components();

	void process_standard_gui_keys(CL_GUIMessage &message);

/// \}
/// \name Implementation
/// \{
private:
	void deliver_message(CL_GUIMessage &message);

	void on_focus_lost(CL_GUITopLevelWindow *toplevel_window);
	void on_focus_gained(CL_GUITopLevelWindow *toplevel_window);
	void on_resize(CL_GUITopLevelWindow *toplevel_window, const CL_Size &new_size);
	void on_paint(CL_GUITopLevelWindow *toplevel_window, const CL_Rect &update_rect);
	void on_close(CL_GUITopLevelWindow *toplevel_window);
	void on_destroy(CL_GUITopLevelWindow *toplevel_window);
	void on_input_received(CL_GUITopLevelWindow *toplevel_window, const CL_InputEvent &input_event, const CL_InputState &input_state);
	void on_gc_destruction_imminent();
	mutable std::map< CL_String, std::vector<CL_CSSProperty> > properties_cache;

	struct NamedFontCacheEntry
	{
		CL_Font font;
		CL_FontDescription desc;
	};

	CL_Slot slot_gc_destruction_imminent;
	bool destroy_signal_connected;

	std::vector<NamedFontCacheEntry> named_font_cache;

/// \}
};
