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
**  Trigve Siver
**  Mark Page
*/

#pragma once


#include "../opengl_window_provider.h"
#include "Display/X11/x11_window.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

#include <X11/extensions/xf86vmode.h>
#include <X11/extensions/XInput.h>

#include <GL/glu.h>
#include <GL/glx.h>

typedef int (*ptr_glXSwapIntervalSGI)(int interval);
typedef int (*ptr_glXSwapIntervalMESA)(int interval);


class CL_OpenGLWindowProvider_GLX : public CL_OpenGLWindowProvider
{
/// \name Construction
/// \{

public:
	CL_OpenGLWindowProvider_GLX();

	~CL_OpenGLWindowProvider_GLX();


/// \}
/// \name Attributes
/// \{

public:
	CL_Rect get_geometry() const {return x11_window.get_geometry();}

	CL_Rect get_viewport() const {return x11_window.get_viewport();}

	bool is_fullscreen() const {return x11_window.is_fullscreen();}

	bool has_focus() const {return x11_window.has_focus();}

	bool is_minimized() const {return x11_window.is_minimized();}

	bool is_maximized() const {return x11_window.is_maximized();}

	bool is_visible() const {return x11_window.is_visible();}

	bool is_clipboard_text_available() const { return x11_window.is_clipboard_text_available(); }

	/// \brief Returns the X11 display handle.
	Display *get_display() { return x11_window.get_display(); }

	/// \brief Handle to X11 window handle.
	Window get_window() { return x11_window.get_window(); }

	/// \brief Returns the GLX rendering context for this window.
	GLXContext get_opengl_context() { return opengl_context; }

	CL_GraphicContext get_gc() const { return gc; }

	CL_InputContext get_ic() const { return x11_window.get_ic(); }

	CL_GraphicContext gc;


/// \}
/// \name Operations
/// \{

public:
	CL_Point client_to_screen(const CL_Point &client) { return x11_window.client_to_screen(client); }

	CL_Point screen_to_client(const CL_Point &screen) { return x11_window.screen_to_client(screen); }

	void create(CL_DisplayWindowSite *site, const CL_DisplayWindowDescription &description);

	void show_system_cursor() { x11_window.show_system_cursor(); }

	CL_CursorProvider *create_cursor(const CL_SpriteDescription &sprite_description, const CL_Point &hotspot);

	void set_cursor(CL_CursorProvider *cursor);

	void set_cursor(CL_StandardCursor type) { x11_window.set_cursor(type); }

	void hide_system_cursor()  { x11_window.hide_system_cursor(); }

	void set_title(const CL_StringRef &new_title) { x11_window.set_title(new_title); }

	void set_position(const CL_Rect &pos, bool client_area) { return x11_window.set_position(pos, client_area); };

	void set_size(int width, int height, bool client_area)  { return x11_window.set_size(width, height, client_area); }

	void set_minimum_size(int width, int height, bool client_area) { return x11_window.set_minimum_size(width, height, client_area); }

	void set_maximum_size( int width, int height, bool client_area) { return x11_window.set_maximum_size(width, height, client_area); }

	void set_enabled(bool enable) { return x11_window.set_enabled(enable); }

	void minimize() { x11_window.minimize(); }

	void restore() { x11_window.restore(); }

	void maximize() { x11_window.maximize(); }

	void show(bool activate)  { x11_window.show(activate); }

	void hide() { x11_window.hide(); }

	void bring_to_front() { x11_window.bring_to_front(); }

	/// \brief Flip opengl buffers.
	void flip(int interval);

	/// \brief Copy a region of the backbuffer to the frontbuffer.
	void update(const CL_Rect &rect);

	/// \brief Capture/Release the mouse.
	void capture_mouse(bool capture) { x11_window.capture_mouse(capture); }

	void process_messages();

	/// \brief Set the window timer
	void set_timer(CL_TimerProvider *timer) { x11_window.set_timer(timer); }

	/// \brief Stop the window timer
	void kill_timer(CL_TimerProvider *timer) { x11_window.kill_timer(timer); }

	GLXContext create_context();

	/// \brief Check for window messages
	/** \return true when there is a message*/
	bool has_messages() { return x11_window.has_messages(); }

	/// \brief Stores text in the clipboard.
	void set_clipboard_text(const CL_StringRef &text) { x11_window.set_clipboard_text(text); }

	/// \brief Returns the text stored in the clipboard.
	CL_String get_clipboard_text() const { return x11_window.get_clipboard_text(); }

	void invalidate_rect(const CL_Rect &rect) { x11_window.invalidate_rect(rect); }

	CL_TimerProvider *alloc_timer(CL_DisplayWindow &disp_window);

/// \}
/// \name Implementation
/// \{

private:

	void on_window_resized();

	bool is_glx_extension_supported(const char *ext_name);

	void setup_swap_interval_pointers();

	CL_X11Window x11_window;

	/// \brief GLX rendering context handle.
	GLXContext opengl_context;

	CL_DisplayWindowSite *site;

	XVisualInfo *opengl_visual_info;

	ptr_glXSwapIntervalSGI glXSwapIntervalSGI;
	ptr_glXSwapIntervalMESA glXSwapIntervalMESA;
/// \}
};



