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
#include <map>

class CL_GUITopLevelWindow;
class CL_GUIWindowManagerSite;
class CL_InputEvent;
class CL_InputState;
class CL_DisplayWindowMessage;

class CL_GUITopLevelWindowTexture
{
public:
	CL_GUITopLevelWindowTexture(CL_GUITopLevelWindow *window) : window(window), enabled(true), visible(true), dirty(true) { }

	CL_GUITopLevelWindow *window;	// The window that this texture belongs to
	CL_Subtexture subtexture;
	CL_RenderBuffer stencil_buffer;
	CL_RenderBuffer depth_buffer;
	CL_Rect geometry;
	bool enabled;
	bool visible;
	bool dirty;
	std::vector<CL_Rect> update_region_list;		// Only valid when "dirty" is set to true
};

class CL_GUIWindowManagerTexture_Impl
{
/// \name Construction
/// \{
public:
	CL_GUIWindowManagerTexture_Impl(CL_DisplayWindow display_window);

	~CL_GUIWindowManagerTexture_Impl();

/// \}
/// \name Attributes
/// \{
public:
	CL_GUIWindowManagerSite *site;

	std::map<CL_GUITopLevelWindow *, CL_GUITopLevelWindowTexture *> window_map;

	std::vector<CL_GUITopLevelWindowTexture *> z_order;

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
	void on_displaywindow_window_close();

	void on_input(const CL_InputEvent &event, const CL_InputState &input_state);
	void on_input_mouse_down(const CL_InputEvent &event, const CL_InputState &input_state);
	void on_input_mouse_move(const CL_InputEvent &event, const CL_InputState &input_state);

	void default_repaint();
	void update_paint();

	void clear_frame_buffer(CL_GUITopLevelWindowTexture *toplevel_window );
	void setup_frame_buffer(CL_GUITopLevelWindowTexture *toplevel_window );

	void bring_to_front(CL_GUITopLevelWindow *handle);

/// \}
/// \name Implementation
/// \{
private:
	CL_GUITopLevelWindow *get_window_at_point(const CL_Point &point);

	void invoke_input_received(CL_GUITopLevelWindow *window, const CL_InputEvent &input_event, const CL_InputState &input_state);

/// \}
};

/// \}
