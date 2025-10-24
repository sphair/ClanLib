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

#include "API/Core/System/keep_alive.h"

#include "API/Display/graphic_context.h"
#include "API/Display/pixel_buffer.h"
#include "API/Display/input_device.h"
#include "API/Display/input_context.h"
#include "API/Display/display_window_description.h"

#include "Display/display_window_generic.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

#include <X11/extensions/xf86vmode.h>
#include <X11/extensions/XInput.h>

#include <GL/glu.h>
#include <GL/glx.h>

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

	//: Returns const versions of the pixelbuffer interfaces.
	virtual CL_PixelBuffer get_buffer(int i) const;

	//: Returns the amount of flipping buffers.
	virtual int get_buffer_count() const;

	//: Returns the graphic context of this window.
	virtual CL_GraphicContext *get_gc();

	//: Returns the input context of this window.
	virtual CL_InputContext *get_ic() { return &input_context; }

	//: Returns the maximum size of a surface this displaywindow supports.
	CL_Size get_max_surface_size() const;

	//: Returns the GLX rendering context for this window.
	GLXContext get_context() { return context; }

	//: Returns the X11 display handle.
	static Display *get_display() { return disp; }

	//: Handle to X11 window handle.
	Window get_window() { return window; }

	//: Input context for all input devices associated with this window.
	CL_InputContext input_context;

	//: Input device for the keyboard.
	CL_InputDevice keyboard;

	//: Input device for the mouse.
	CL_InputDevice mouse;

	//: State information about misc key modifiers.
	bool left_ctrl_down, left_alt_down, left_shift_down;
	bool right_ctrl_down, right_alt_down, right_shift_down;

	//: handle of the first created context. Used for sharing
	//: textures and display lists between rendering contexts.
	static GLXContext share_context;

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

	//: Return the X display handle
	virtual Display *get_hwnd();

//! Signals:
public:
	//: Signal emitted when x11 events are to be processed.
	CL_Signal_v1<XEvent &> sig_xevent;

	CL_Signal_v1<XEvent &> sig_unknown_xevent;

//! Implementation:
private:
	//: Initializes all joysticks
	void setup_joysticks();
	
	//: Initializes additional USB mice
	void setup_usb_mice();

	//: Initialize extended X11 input devices (graphic tablet, etc.)
	void setup_xinput();
	
	//: Initialize /dev/input/event* devices
	void setup_event();

	Bool xinput_is_present();

	//: Signal that a new cursor CL_Sprite is available
	void cursor_changed();

	//: Destroy current window handles.
	void destroy_window();

	//: X Event handler for the window
	void on_xevent(XEvent &event);
	
	//: Change resolution for fullscreen mode
	void set_resolution();

	// OpenGL compatible gc.
	CL_GraphicContext gc;

	//: GLX rendering context handle.
	GLXContext context;
	
	//: X11 Display handle.
	static Display *disp;

	static int disp_ref_count;

	// We use 3 windows:
	// -fs_window (for fullscreen)
	// -wm_window (window managed)
	// -window    (the real window)
	// 2 of which will be created in create_window: wm_window and window.
	// The fullscreen window gets (re)created when needed, because reusing
	// it causes trouble.
	// The real window uses wm_window as parent initially and will be
	// reparened to the (freshly created) fullscreen window when requested
	// and reparented back again when going back to windowed mode.
	// Idea / concept of 3 windows borrowed from SDL. But somehow SDL
	// manages to reuse the fullscreen window too.
	Window window;
	Window wm_window;
	Window fs_window;
	
	// width and height of the drawing window as specified by
	// the caller, or as reported after a resize by the windowmanager
	int width;
	int height;

	//: Whether we have window focus or not
	bool focus;
	
	//: Is resizing allowed?
	bool allow_resize;

	//: Attributes to switch between windowed and fullscreen
	int dotclock;
	XF86VidModeModeInfo old_mode;
	int old_x, old_y;
	XVisualInfo *vi;
	Colormap cmap;

	//: X Event slot
	CL_Slot slot_xevent;
	
	Cursor system_cursor;
	Cursor hidden_cursor;
	Pixmap cursor_bitmap;
	
};

#endif

