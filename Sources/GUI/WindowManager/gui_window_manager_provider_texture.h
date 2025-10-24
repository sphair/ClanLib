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
**    Kenneth Gangstoe
**    Mark Page
*/

/// \addtogroup clanGUI_System clanGUI System
/// \{

#pragma once

#include "API/Core/Signals/slot_container.h"
#include "API/Core/Signals/callback_v0.h"
#include "API/Core/Signals/callback_v2.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Render/texture.h"
#include "API/Display/Render/frame_buffer.h"
#include "API/Display/Render/render_buffer.h"
#include "API/Display/2D/texture_group.h"
#include "API/Display/2D/subtexture.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/Providers/gui_window_manager_provider.h"
#include <map>

class CL_GUITopLevelWindow;
class CL_GUIWindowManagerSite;
class CL_InputEvent;
class CL_InputState;
class CL_DisplayWindowMessage;
class CL_GUIWindowManagerTextureWindow;

class CL_GUITopLevelWindowTexture
{
public:
	CL_GUITopLevelWindowTexture(CL_GUITopLevelWindow *window) : window(window), enabled(true), visible(true), dirty(true), owner_window(NULL) { }

	CL_GUITopLevelWindow *window;	// The window that this texture belongs to
	CL_Subtexture subtexture;
	CL_RenderBuffer stencil_buffer;
	CL_RenderBuffer depth_buffer;
	CL_Rect geometry;
	bool enabled;
	bool visible;
	bool dirty;

	CL_GUITopLevelWindowTexture *owner_window;
	std::vector<CL_GUITopLevelWindowTexture *> child_windows_zorder;	// Beginning is at the top

	std::vector<CL_Rect> update_region_list;		// Only valid when "dirty" is set to true
};

class CL_GUIWindowManagerProvider_Texture : public CL_GUIWindowManagerProvider
{
/// \name Construction
/// \{
public:
	CL_GUIWindowManagerProvider_Texture(CL_DisplayWindow &display_window);
	~CL_GUIWindowManagerProvider_Texture();

/// \}
/// \name Attributes
/// \{
public:

	std::vector<CL_GUIWindowManagerTextureWindow> get_windows(bool only_visible = true) const;

	/// \brief Retrieves the provider.
	CL_GUIWindowManagerProvider *get_provider();

	CL_GUIWindowManager::CL_WindowManagerType get_window_manager_type() const;

	CL_GUIWindowManagerSite *site;
	std::map<CL_GUITopLevelWindow *, CL_GUITopLevelWindowTexture *> window_map;
	std::vector<CL_GUITopLevelWindowTexture *> root_window_z_order;	// Beginning is at the top
	CL_Callback_v0 func_repaint;
	CL_Callback_v1<CL_FrameBuffer &> func_setup_framebuffer;
	CL_Callback_v2<CL_InputEvent &, CL_InputState &> func_input_intercept;
	CL_GUITopLevelWindow *activated_window;
	CL_GUITopLevelWindow *capture_mouse_window;
	CL_DisplayWindow display_window;
	CL_SlotContainer slots;
	CL_TextureGroup texture_group;
	CL_FrameBuffer frame_buffer;
	bool frame_buffer_initial_setup;
	bool frame_buffer_stencil_attached;
	bool frame_buffer_depth_attached;
	CL_Texture frame_buffer_texture_attached;	// A copy of the last texture that is attached to the frame buffer
	bool setup_painting_called; // True when setup_painting() was called in CL_GUIWindowManagerTexture
	bool painting_set;	// True when the frame buffer is active (used by setup_painting() )

/// \}
/// \name Operations
/// \{
public:

	void destroy();

	void set_texture_group(CL_TextureGroup &new_texture_group);

	void on_displaywindow_window_close();

	void on_input(const CL_InputEvent &event, const CL_InputState &input_state);
	void on_input_mouse_up(const CL_InputEvent &event, const CL_InputState &input_state);
	void on_input_mouse_down(const CL_InputEvent &event, const CL_InputState &input_state);
	void on_input_mouse_move(const CL_InputEvent &event, const CL_InputState &input_state);

	void default_repaint();
	void update_paint();

	void clear_frame_buffer(CL_GUITopLevelWindowTexture *toplevel_window );
	void setup_frame_buffer(CL_GUITopLevelWindowTexture *toplevel_window );

	void set_site(CL_GUIWindowManagerSite *site);
	void create_window(
		CL_GUITopLevelWindow *handle,
		CL_GUITopLevelWindow *owner,
		CL_GUIComponent *component,
		CL_GUITopLevelDescription description);

	void destroy_window(CL_GUITopLevelWindow *handle);
	void enable_window(CL_GUITopLevelWindow *handle, bool enable);
	bool has_focus(CL_GUITopLevelWindow *handle) const;
	void set_visible(CL_GUITopLevelWindow *handle, bool visible, bool activate_root_win);
	void set_geometry(CL_GUITopLevelWindow *handle, const CL_Rect &geometry, bool client_area);
	CL_Rect get_geometry(CL_GUITopLevelWindow *handle, bool client_area) const;
	CL_Point screen_to_window(CL_GUITopLevelWindow *handle, const CL_Point &screen_point, bool client_area) const;
	CL_Point window_to_screen(CL_GUITopLevelWindow *handle, const CL_Point &window_point, bool client_area) const;
	CL_GraphicContext& get_gc(CL_GUITopLevelWindow *handle) const;
	CL_InputContext& get_ic(CL_GUITopLevelWindow *handle) const;
	CL_GraphicContext begin_paint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region);
	void set_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc, const CL_Rect &rect);
	void reset_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc);
	void push_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc, const CL_Rect &rect);
	void pop_cliprect(CL_GUITopLevelWindow *handle, CL_GraphicContext &gc);
	void end_paint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region);
	void request_repaint(CL_GUITopLevelWindow *handle, const CL_Rect &update_region);
	void bring_to_front(CL_GUITopLevelWindow *handle);
	bool is_minimized(CL_GUITopLevelWindow *handle) const;
	bool is_maximized(CL_GUITopLevelWindow *handle) const;
	void capture_mouse(CL_GUITopLevelWindow *handle, bool state);
	CL_DisplayWindow get_display_window(CL_GUITopLevelWindow *handle) const;
	void set_cursor(CL_GUITopLevelWindow *handle, const CL_Cursor &cursor);
	void set_cursor(CL_GUITopLevelWindow *handle, enum CL_StandardCursor type);

	/// \brief Perform any updates.
	///
	/// This is called by CL_GUIManager::exec(), after all messages has been processed
	void update();

	/// \brief Initial setup for painting all top level windows
	void setup_painting();

	/// \brief Final completion for painting all top level windows
	void complete_painting();
	void finalise_painting();

	void process();

	void draw_windows(CL_GraphicContext &gc);

/// \}
/// \name Implementation
/// \{
private:
	CL_GUITopLevelWindow *get_window_at_point(const CL_Point &point);
	CL_GUITopLevelWindow *get_window_at_point(const CL_Point &point, const std::vector<CL_GUITopLevelWindowTexture *> &z_order);

	CL_GUITopLevelWindowTexture *get_window_texture(CL_GUITopLevelWindow *handle);
	void get_all_windows_zorder(bool only_visible, std::vector<CL_GUIWindowManagerTextureWindow> &windows_dest_list, const std::vector<CL_GUITopLevelWindowTexture *> &z_order) const;
	void draw_all_windows(CL_GraphicContext &gc, std::vector<CL_GUITopLevelWindowTexture *> &z_order);
	void invoke_input_received(CL_GUITopLevelWindow *window, const CL_InputEvent &input_event, const CL_InputState &input_state);
	void invalidate_constant_repaint_components();
	bool is_constant_repaint_enabled(CL_GUIComponent *component) const;
/// \}
};

/// \}
