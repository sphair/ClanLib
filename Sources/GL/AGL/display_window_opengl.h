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

#ifndef header_display_window_opengl
#define header_display_window_opengl

#if _MSC_VER > 1000
#pragma once
#endif

#include <map>
#include <stack>
#include <set>

#include "API/Core/System/keep_alive.h"

#include "API/Display/graphic_context.h"
#include "API/Display/pixel_buffer.h"
#include "API/Display/input_device.h"
#include "API/Display/input_context.h"
#include "API/Display/display_window_description.h"

#include "Display/display_window_generic.h"

#include <AGL/agl.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>

class CL_DisplayWindow_OpenGL : public CL_DisplayWindow_Generic, CL_KeepAlive
{
//! Construction:
public:
	CL_DisplayWindow_OpenGL();

	virtual ~CL_DisplayWindow_OpenGL();

//! Attributes:
public:
	//: Returns the width of this window.
	virtual int get_width() const;

	//: Returns the height of this window.
	virtual int get_height() const;

	//: Returns true if fullscreen window.
	virtual bool is_fullscreen() const;

	//: Returns true if window got focus.
	virtual bool has_focus() const;

	//: Returns pixelbuffer interfaces for flipping buffers.
	virtual CL_PixelBuffer get_buffer(int i) const;

	//: Returns the amount of flipping buffers.
	virtual int get_buffer_count() const;

	//: Returns the graphic context of this window.
	virtual CL_GraphicContext *get_gc();

	//: Returns the input context of this window.
	virtual CL_InputContext *get_ic() { return &input_context; }

	//: Returns the maximum size of a surface this displaywindow supports.
	CL_Size get_max_surface_size() const;
	
	//: Returns the AGLDrawable for this window.
	AGLDrawable get_drawable() { return GetWindowPort(window_ref); }
	
	//: Returns the window handle.
	WindowRef get_window_ref() { return window_ref; }

	//: Returns the AGL rendering context for this window.
	AGLContext get_context() { return context; }

	//: Input context for all input devices associated with this window.
	CL_InputContext input_context;

	//: Input device for the keyboard.
	CL_InputDevice keyboard;

	//: Input device for the mouse.
	CL_InputDevice mouse;

	//: Converts a MacOSX keycode into a CL_KEY_XX.
	static int keycode_to_clkey(int keycode);

	//: Converts a MacOSX modifier keycode into a CL_KEY_XX.
	static std::set<int> modifiercode_to_clkeys(int modifiercode);

	//: Converts a CL_KEY_XX to a MacOSX keycode.
	static int clkey_to_keycode(int clkey);	

	//: State information about misc key modifiers.
	bool left_ctrl_down, left_alt_down, left_shift_down;
	bool right_ctrl_down, right_alt_down, right_shift_down;

	//: handle of the first created context. Used for sharing
	//: textures and display lists between rendering contexts.
	static AGLContext share_context;

	// Current mouse position
	CL_Point mouse_pos;

	// Current mouse button states
	bool mouse_states[5];

//! Operations:
public:
	//: Set window to fullscreen.
	virtual void set_fullscreen(int width, int height, int bpp, int refresh_rate);

	//: Restore to windowed mode.
	virtual void set_windowed();

	//: Change title on window.
	virtual void set_title(const std::string &title);

	//: Set window position and size.
	virtual void set_position(const CL_Rect &pos);

	//: Set window position.
	virtual void set_position(int x, int y);

	//: Set window size.
	virtual void set_size(int width, int height);

	//: Change the buffer count in the flipping system.
	virtual void set_buffer_count(int flipping_buffers);

	//: Copy a region of the backbuffer to the frontbuffer.
	virtual void update(const CL_Rect &rect);

	//: Flip the backbuffer to front.
	virtual void flip(int interval);

	//: Make system cursor visible over window.
	virtual void show_system_cursor();

	//: Make system cursor invisible over window.
	virtual void hide_system_cursor();

	//: Create a window.
	virtual void create_window(const CL_DisplayWindowDescription &desc);

	//: Called by ClanLib keep alive pump.
	virtual void keep_alive();

//! Signals:
public:

//! Implementation:
private:
	//: Signal that a new cursor CL_Sprite is available
	void cursor_changed();

	//: Destroy current window handles.
	void destroy_window();
	
	//: Event handler for window.
	static OSStatus on_window_event(EventHandlerCallRef call_ref, EventRef event_ref, void *user_data);

	void install_event_handler(EventTargetRef targ_ref, int remove_only=0);
	
	//: True if currently in full screen mode.
	bool fullscreen;

	//: Width and height, if in full screen mode.
	int fullscreen_width, fullscreen_height;

	//: Saved position of window when going fullscreen. This is the
	//: position the window receives when leaving fullscreen again.
	CL_Rect saved_position;

	// OpenGL compatible gc.
	CL_GraphicContext gc;

	//: AGL rendering context handle.
	AGLContext context;
	
	//: Window mode context.
	AGLContext win_context;
	
	//: Fullscreen context.
	AGLContext fs_context;
	
	//: Window reference.
	WindowRef window_ref;
	
	//: Target for EventDispatcher
	EventTargetRef target_ref;
	
	static int disp_ref_count;

	//: Whether we have window focus or not
	bool focus;
	
	//: The previous state of the modifier keys
	std::set<int> prev_modifier_keys;
};

#endif

