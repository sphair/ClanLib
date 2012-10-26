/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
*/

/// \addtogroup clanDisplay_Window clanDisplay Window
/// \{

#pragma once

#include "../api_display.h"
#include "../../Core/Signals/signal_v0.h"
#include "../../Core/Signals/signal_v1.h"
#include "../../Core/Signals/signal_v2.h"
#include "../../Core/Signals/callback_v1.h"
#include "../../Core/Signals/callback_0.h"
#include "../../Core/Signals/callback_4.h"
#include "../display_target.h"
#include <memory>

#if !defined(WIN32) && !defined(__APPLE__)
#include <X11/Xlib.h>
#endif

namespace clan
{

class Size;
class Rect;
class Display;
class GraphicContext;
class DisplayWindowMode;
class DisplayWindowDescription;
class InputContext;
class Point;
class Cursor;
class DisplayWindowProvider;
class DisplayWindow_Impl;
class PixelBuffer;

/// \brief Standard Cursor
///
/// \xmlonly !group=Display/Window! !header=display.h! \endxmlonly
enum StandardCursor
{
	cursor_arrow,
	cursor_appstarting,
	cursor_cross,
	cursor_hand,
	cursor_ibeam,
	cursor_no,
	cursor_size_all,
	cursor_size_nesw,
	cursor_size_ns,
	cursor_size_nwse,
	cursor_size_we,
	cursor_uparrow,
	cursor_wait
};

/// \brief Top-level window class.
///
/// \xmlonly !group=Display/Window! !header=display.h! \endxmlonly
class CL_API_DISPLAY DisplayWindow
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a null instance.
	DisplayWindow();

	/// \brief Constructs a window.
	///
	/// \param title = Titlebar text.
	/// \param width = Width in pixels of window.
	/// \param height = Height in pixels of window.
	/// \param start_fullscreen = If true, window will go fullscreen.
	/// \param allow_resize = If true, window will have resize grabbers and can be resized.
	/// \param flipping_buffers = Number of flipping buffers in system. Default is that there is a front buffer and a back buffer.
	/// \param target = Display target used to create the window.
	DisplayWindow(
		const std::string &title,
		int width,
		int height,
		bool start_fullscreen = false,
		bool allow_resize = false,
		int flipping_buffers = 2,
		DisplayTarget target = DisplayTarget());

	/// \brief Constructs a window.
	///
	/// \param description = Structure that describes how to create the display window.
	/// \param target = Display target used to create the window.
	DisplayWindow(
		const DisplayWindowDescription &description,
		DisplayTarget target = DisplayTarget());

	/// \brief Constructs a window.
	///
	/// \param provider = Display target implementation object.
	DisplayWindow(DisplayWindowProvider *provider);

	~DisplayWindow();

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Returns the position and size of the window frame.
	Rect get_geometry() const;

	/// \brief Returns the drawable area of the window (excluding window frame).
	Rect get_viewport() const;

	/// \brief Returns true if window is currently running fullscreen.
	bool is_fullscreen() const;

	/// \brief Returns true if window has focus.
	bool has_focus() const;

	/// \brief Return the graphic context for the window.
	GraphicContext& get_gc() const;

	/// \brief Return the input context for the window.
	InputContext get_ic() const;

	/// \brief Signal emitted when window lost focus.
	Signal_v0 &sig_lost_focus();

	/// \brief Signal emitted when window gain focus.
	Signal_v0 &sig_got_focus();

	/// \brief Signal emitted when window is resized.
	Signal_v2<int, int> &sig_resize();

	/// \brief Signal emitted when an area of the window is invalidated.
	Signal_v1<const Rect &> &sig_paint();

	/// \brief Signal emitted when window is closed.
	Signal_v0 &sig_window_close();

	/// \brief Signal emitted when window is destroyed.
	Signal_v0 &sig_window_destroy();

	/// \brief Signal emitted when window is minimized.
	Signal_v0 &sig_window_minimized();

	/// \brief Signal emitted when window is maximized.
	Signal_v0 &sig_window_maximized();

	/// \brief Signal emitted when window is restored.
	Signal_v0 &sig_window_restored();

	/// \brief Signal emitted after a window has been moved.
	Signal_v0 &sig_window_moved();

	/// \brief Callback called when a window is being resized.
	Callback_v1<Rect &> &func_window_resize();

	/// \brief Callback called when a window is asked to minimize itself.
	Callback_0<bool> &func_minimize_clicked();

#ifdef WIN32
	/// \brief Callback called when a windows messages is received.
	Callback_4<bool, HWND, UINT, WPARAM, LPARAM> &func_window_message();
#endif

	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief returns true if this display window is visible
	bool is_visible() const;

	/// \brief Returns true if the window is minimized.
	bool is_minimized() const;

	/// \brief Returns true if the window is maximized.
	bool is_maximized() const;

	/// \brief Returns the display window provider
	DisplayWindowProvider *get_provider() const;

	/// \brief Returns true if text is available in the clipboard.
	bool is_clipboard_text_available() const;

	/// \brief Returns true if an image is available in the clipboard.
	bool is_clipboard_image_available() const;

	/// \brief Returns the text stored in the clipboard.
	std::string get_clipboard_text() const;

	/// \brief Returns an image stored in the clipboard. 
	/// <p>Returns a null pixelbuffer if no image is available.</p>
	PixelBuffer get_clipboard_image() const;

	/// \brief Returns the minimum size the window can be resized to by the application user.
	Size get_minimum_size(bool client_area=false);

	/// \brief Returns the maximum size the window can be resized to by the application user.
	Size get_maximum_size(bool client_area=false);

	/// \brief Returns the window title.
	std::string get_title() const;

#ifdef WIN32

	/// \brief Get Hwnd
	///
	/// \return hwnd
	HWND get_hwnd() const;

#elif defined(__APPLE__)
	// nothing
#else
	/// \brief Returns the X11 display handle.
	Display *get_display() const;

	/// \brief Handle to X11 window handle.
	Window get_window() const;
#endif

/// \}
/// \name Operations
/// \{
public:
	/// \brief Convert from window client coordinates to screen coordinates.
	Point client_to_screen(const Point &client);

	/// \brief Convert from screen coordinates to client coordinates.
	Point screen_to_client(const Point &screen);

	/// \brief Capture/Release the mouse.
	void capture_mouse(bool capture);

	/// \brief Invalidates a region of a screen, causing a repaint.
	void request_repaint(const Rect &rect);

	/// \brief Change window title.
	void set_title(const std::string &title);

	/// \brief Set window position and size.
	///
	/// \param pos = Window position and size.
	/// \param client_area = true - Position relative to window client area
	void set_position(const Rect &pos, bool client_area);

	/// \brief Set window position and size.
	///
	/// \param x Window x position on desktop.
	/// \param y Window y position on desktop.
	void set_position(int x, int y);

	/// \brief Set enabled
	///
	/// \param enable = bool
	void set_enabled(bool enable);

	/// \brief Set visible
	///
	/// \param visible = bool
	/// \param activate = bool
	void set_visible(bool visible, bool activate);

	/// \brief Resize window.
	///
	/// \param width = New width of window in pixels.
	/// \param height = New height of window in pixels.
	/// \param client_area = true - relative to the window client area
	void set_size(int width, int height, bool client_area);

	/// \brief Minimum size a window can be resized to by the application user.
	void set_minimum_size(int width, int height, bool client_area);

	/// \brief Maximum size a window can be resized to by the application user.
	void set_maximum_size(int width, int height, bool client_area);

	/// \brief Minimizes the window.
	void minimize();

	/// \brief Restores the window.
	void restore();

	/// \brief Maximizes the window.
	void maximize();

	/// \brief Displays the window in its current size and position.
	void show(bool activate = true);

	/// \brief Hides the window.
	void hide();

	/// \brief Raises the window on top of other windows.
	void bring_to_front();

	/// \brief Copy the specified rectangle area from back buffer to front buffer.
	///
	/// \param rect Area that should be copied.
	void update(const Rect &rect);

	/// \brief Flip back buffer to front, making changes visible on screen.
	///
	/// <p>The parameter interval specifies the minimum number of video frames
	/// that are displayed before a buffer swap will occur.</p>
	/// <p>If interval is set to a value of 0, buffer swaps are not synchronized
	/// to a video frame. </p>
	/// <p>If interval is set to a value of -1 (the default), then it will use
	/// the buffer swap used for previous flip. If its the first flip, it will
	/// use the system default.</p>
	///
	/// \param interval = See note
	void flip(int interval = -1);

	/// \brief Shows the mouse cursor.
	void show_cursor();

	/// \brief Sets the current cursor icon.
	void set_cursor(const Cursor &cursor);

	/// \brief Set cursor
	///
	/// \param type = Standard Cursor
	void set_cursor(StandardCursor type);

#ifdef WIN32
	/// \brief Sets the current cursor handle (win32 only)
	void set_cursor_handle(HCURSOR cursor);
#endif

	/// \brief Hides the mouse cursor.
	void hide_cursor();

	/// \brief Stores text in the clipboard.
	void set_clipboard_text(const std::string &text);

	/// \brief Stores an image in the clipboard.
	void set_clipboard_image(const PixelBuffer &buf);

	/// \brief Sets the large icon used for this window.
	void set_large_icon(const PixelBuffer &image);

	/// \brief Sets the small icon used for this window.
	void set_small_icon(const PixelBuffer &image);

/// \}
/// \name Implementation
/// \{
private:
	std::shared_ptr<DisplayWindow_Impl> impl;

/// \}
};

}

/// \}
