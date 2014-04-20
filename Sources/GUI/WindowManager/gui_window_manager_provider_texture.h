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
**    Kenneth Gangstoe
**    Mark Page
*/


#pragma once

#include "API/Core/Signals/callback_v0.h"
#include "API/Core/Signals/callback_v2.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Render/texture_2d.h"
#include "API/Display/Render/frame_buffer.h"
#include "API/Display/Render/render_buffer.h"
#include "API/Display/Render/blend_state.h"
#include "API/Display/2D/texture_group.h"
#include "API/Display/2D/subtexture.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/Providers/gui_window_manager_provider.h"
#include <map>
#include "API/Display/2D/canvas.h"

namespace clan
{
/// \addtogroup clanGUI_System clanGUI System
/// \{

class GUITopLevelWindow;
class GUIWindowManagerSite;
class InputEvent;
class DisplayWindowMessage;
class GUIWindowManagerTextureWindow;

class GUITopLevelWindowTexture
{
public:
	GUITopLevelWindowTexture(GUITopLevelWindow *window) : window(window), enabled(true), visible(true), dirty(true), owner_window(NULL) { }

	GUITopLevelWindow *window;	// The window that this texture belongs to
	Subtexture subtexture;
	RenderBuffer stencil_buffer;
	RenderBuffer depth_buffer;
	Rect geometry;
	bool enabled;
	bool visible;
	bool dirty;

	GUITopLevelWindowTexture *owner_window;
	std::vector<GUITopLevelWindowTexture *> child_windows_zorder;	// Beginning is at the top

	std::vector<Rect> update_region_list;		// Only valid when "dirty" is set to true
};

class GUIWindowManagerProvider_Texture : public GUIWindowManagerProvider
{
/// \name Construction
/// \{
public:
	GUIWindowManagerProvider_Texture(DisplayWindow &display_window);
	~GUIWindowManagerProvider_Texture();

/// \}
/// \name Attributes
/// \{
public:

	std::vector<GUIWindowManagerTextureWindow> get_windows(bool only_visible = true) const;

	/// \brief Retrieves the provider.
	GUIWindowManagerProvider *get_provider();

	GUIWindowManager::WindowManagerType get_window_manager_type() const;

	GUIWindowManagerSite *site;
	std::map<GUITopLevelWindow *, GUITopLevelWindowTexture *> window_map;
	std::vector<GUITopLevelWindowTexture *> root_window_z_order;	// Beginning is at the top
	Callback_v0 func_repaint;
	Callback_v1<FrameBuffer &> func_setup_framebuffer;
	Callback_v1<InputEvent &> func_input_intercept;
	GUITopLevelWindow *activated_window;
	GUITopLevelWindow *capture_mouse_window;
	DisplayWindow display_window;
	Canvas canvas_window;
	TextureGroup texture_group;
	FrameBuffer frame_buffer;
	bool frame_buffer_initial_setup;
	bool frame_buffer_stencil_attached;
	bool frame_buffer_depth_attached;
	Texture2D frame_buffer_texture_attached;	// A copy of the last texture that is attached to the frame buffer

/// \}
/// \name Operations
/// \{
public:
	void set_texture_group(TextureGroup &new_texture_group);

	void on_displaywindow_window_close();

	void on_input(const InputEvent &event);
	void on_input_mouse_up(const InputEvent &event);
	void on_input_mouse_down(const InputEvent &event);
	void on_input_mouse_move(const InputEvent &event);

	void update_paint();

	void clear_frame_buffer(GUITopLevelWindowTexture *toplevel_window );
	void setup_frame_buffer(GUITopLevelWindowTexture *toplevel_window );

	void set_site(GUIWindowManagerSite *site);
	void create_window(
		GUITopLevelWindow *handle,
		GUITopLevelWindow *owner,
		GUIComponent *component,
		GUITopLevelDescription description);

	void destroy_window(GUITopLevelWindow *handle);
	void enable_window(GUITopLevelWindow *handle, bool enable);
	bool has_focus(GUITopLevelWindow *handle) const;
	void set_visible(GUITopLevelWindow *handle, bool visible, bool activate_root_win);
	void set_geometry(GUITopLevelWindow *handle, const Rect &geometry, bool client_area);
	Rect get_geometry(GUITopLevelWindow *handle, bool client_area) const;
	Point screen_to_window(GUITopLevelWindow *handle, const Point &screen_point, bool client_area) const;
	Point window_to_screen(GUITopLevelWindow *handle, const Point &window_point, bool client_area) const;
	Canvas& get_canvas(GUITopLevelWindow *handle);
	InputContext get_ic(GUITopLevelWindow *handle) const;
	Canvas begin_paint(GUITopLevelWindow *handle, const Rect &update_region);
	void set_cliprect(GUITopLevelWindow *handle, Canvas &canvas, const Rect &rect);
	void reset_cliprect(GUITopLevelWindow *handle, Canvas &canvas);
	void push_cliprect(GUITopLevelWindow *handle, Canvas &canvas, const Rect &rect);
	void pop_cliprect(GUITopLevelWindow *handle, Canvas &canvas);
	void end_paint(Canvas &canvas, GUITopLevelWindow *handle, const Rect &update_region);
	void request_repaint(GUITopLevelWindow *handle, const Rect &update_region);
	void bring_to_front(GUITopLevelWindow *handle);
	bool is_minimized(GUITopLevelWindow *handle) const;
	bool is_maximized(GUITopLevelWindow *handle) const;
	void capture_mouse(GUITopLevelWindow *handle, bool state);
	DisplayWindow get_display_window(GUITopLevelWindow *handle) const;
	void set_cursor(GUITopLevelWindow *handle, const Cursor &cursor);
	void set_cursor(GUITopLevelWindow *handle, enum StandardCursor type);

	/// \brief Perform any updates.
	///
	/// This is called by GUIManager::exec(), after all messages has been processed
	void update();

	void process();

	void draw_windows(Canvas &canvas);

/// \}
/// \name Implementation
/// \{
private:
	GUITopLevelWindow *get_window_at_point(const Point &point);
	GUITopLevelWindow *get_window_at_point(const Point &point, const std::vector<GUITopLevelWindowTexture *> &z_order);

	GUITopLevelWindowTexture *get_window_texture(GUITopLevelWindow *handle);
	void get_all_windows_zorder(bool only_visible, std::vector<GUIWindowManagerTextureWindow> &windows_dest_list, const std::vector<GUITopLevelWindowTexture *> &z_order) const;
	void draw_all_windows(Canvas &canvas, std::vector<GUITopLevelWindowTexture *> &z_order);
	void invoke_input_received(GUITopLevelWindow *window, const InputEvent &input_event);
	void invalidate_constant_repaint_components();
	bool is_constant_repaint_enabled(GUIComponent *component) const;

	BlendState default_blend;
	BlendState opaque_blend;
	BlendState premultiplied_alpha_blend;
/// \}
};

}

/// \}
