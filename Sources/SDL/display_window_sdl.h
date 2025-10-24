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

#ifndef header_display_window_sdl
#define header_display_window_sdl

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

#include <SDL/SDL.h>

#ifdef WIN32
#include <windows.h>
#else
#ifndef __APPLE__
#include <X11/Xlib.h>
#endif
#endif

class CL_DisplayWindow_SDL : public CL_DisplayWindow_Generic, CL_KeepAlive
{
//! Construction:
public:
	CL_DisplayWindow_SDL();

	virtual ~CL_DisplayWindow_SDL();

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

	//: Input context for all input devices associated with this window.
	CL_InputContext input_context;

	//: Input device for the keyboard.
	CL_InputDevice keyboard;

	//: Input device for the mouse.
	CL_InputDevice mouse;

#ifdef WIN32
	//: Returns the Win32 window handle.
	HWND get_wnd();
#else
#ifndef __APPLE__
	//: Returns the X11 window handle.
	Display *get_wnd();
#endif
#endif

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
	//: Signal emitted when SDL mouse events are to be processed.
	CL_Signal_v1<SDL_Event &> sig_sdl_mouse_event;

	//: Signal emitted when SDL keyboard events are to be processed.
	CL_Signal_v1<SDL_Event &> sig_sdl_keyboard_event;

	//: Signal emitted when SDL joystick events are to be processed.
	CL_Signal_v1<SDL_Event &> sig_sdl_joystick_event;

//! Implementation:
private:
	//: Signal that a new cursor CL_Sprite is available
	void cursor_changed();

	//: Destroy current window handles.
	void destroy_window();
	
	// SDL compatible gc.
	CL_GraphicContext gc;

	// main SDL_Surface display window representation object (bot that much an object as SDL is C-code)
	SDL_Surface *screen;

	// event object
	SDL_Event event;

	// manage fullscreen state
	bool fullscreen;
};

#endif

