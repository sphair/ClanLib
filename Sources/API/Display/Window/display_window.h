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
*/

/// \addtogroup clanDisplay_Window clanDisplay Window
/// \{

#pragma once

#include "../api_display.h"
#include "../../Core/System/sharedptr.h"
#include "../../Core/System/weakptr.h"
#include "../../Core/Signals/signal_v0.h"
#include "../../Core/Signals/signal_v1.h"
#include "../../Core/Signals/signal_v2.h"
#include "../../Core/Signals/callback_v1.h"
#include "../../Core/Signals/callback_0.h"
#include "../../Core/Text/string_types.h"
#include "../display_target.h"

#if !defined(WIN32) && !defined(__APPLE__)
#include <X11/Xlib.h>
#endif

class CL_Size;
class CL_Rect;
class CL_Display;
class CL_GraphicContext;
class CL_DisplayWindowMode;
class CL_DisplayWindowDescription;
class CL_InputContext;
class CL_Point;
class CL_Cursor;
class CL_DisplayWindowProvider;
class CL_DisplayWindow_Impl;
class CL_PixelBuffer;

/// \brief Standard Cursor
///
/// \xmlonly !group=Display/Window! !header=display.h! \endxmlonly
enum CL_StandardCursor
{
	cl_cursor_arrow,
	cl_cursor_appstarting,
	cl_cursor_cross,
	cl_cursor_hand,
	cl_cursor_ibeam,
	cl_cursor_no,
	cl_cursor_size_all,
	cl_cursor_size_nesw,
	cl_cursor_size_ns,
	cl_cursor_size_nwse,
	cl_cursor_size_we,
	cl_cursor_uparrow,
	cl_cursor_wait
};

/// \brief Top-level window class.
///
/// \xmlonly !group=Display/Window! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_DisplayWindow
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a null instance.
	CL_DisplayWindow();

	/// \brief Constructs a window.
	///
	/// \param title = Titlebar text.
	/// \param width = Width in pixels of window.
	/// \param height = Height in pixels of window.
	/// \param start_fullscreen = If true, window will go fullscreen.
	/// \param allow_resize = If true, window will have resize grabbers and can be resized.
	/// \param flipping_buffers = Number of flipping buffers in system. Default is that there is a front buffer and a back buffer.
	/// \param target = Display target used to create the window.
	CL_DisplayWindow(
		const CL_StringRef &title,
		int width,
		int height,
		bool start_fullscreen = false,
		bool allow_resize = false,
		int flipping_buffers = 2,
		CL_DisplayTarget target = CL_DisplayTarget());

	/// \brief Constructs a window.
	///
	/// \param description = Structure that describes how to create the display window.
	/// \param target = Display target used to create the window.
	CL_DisplayWindow(
		const CL_DisplayWindowDescription &description,
		CL_DisplayTarget target = CL_DisplayTarget());

	/// \brief Constructs a window.
	///
	/// \param provider = Display target implementation object.
	CL_DisplayWindow(CL_DisplayWindowProvider *provider);

	~CL_DisplayWindow();

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Returns the position and size of the window frame.
	CL_Rect get_geometry() const;

	/// \brief Returns the drawable area of the window (excluding window frame).
	CL_Rect get_viewport() const;

	/// \brief Returns true if window is currently running fullscreen.
	bool is_fullscreen() const;

	/// \brief Returns true if window has focus.
	bool has_focus() const;

	/// \brief Return the graphic context for the window.
	CL_GraphicContext& get_gc() const;

	/// \brief Return the input context for the window.
	CL_InputContext& get_ic() const;

	/// \brief Signal emitted when window lost focus.
	CL_Signal_v0 &sig_lost_focus();

	/// \brief Signal emitted when window gain focus.
	CL_Signal_v0 &sig_got_focus();

	/// \brief Signal emitted when window is resized.
	CL_Signal_v2<int, int> &sig_resize();

	/// \brief Signal emitted when an area of the window is invalidated.
	CL_Signal_v1<const CL_Rect &> &sig_paint();

	/// \brief Signal emitted when window is closed.
	CL_Signal_v0 &sig_window_close();

	/// \brief Signal emitted when window is destroyed.
	CL_Signal_v0 &sig_window_destroy();

	/// \brief Signal emitted when window is minimized.
	CL_Signal_v0 &sig_window_minimized();

	/// \brief Signal emitted when window is maximized.
	CL_Signal_v0 &sig_window_maximized();

	/// \brief Signal emitted when window is restored.
	CL_Signal_v0 &sig_window_restored();

	/// \brief Signal emitted after a window has been moved.
	CL_Signal_v0 &sig_window_moved();

	/// \brief Callback called when a window is being resized.
	CL_Callback_v1<CL_Rect &> &func_window_resize();

	/// \brief Callback called when a window is asked to minimize itself.
	CL_Callback_0<bool> &func_minimize_clicked();

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
	CL_DisplayWindowProvider *get_provider() const;

	/// \brief Returns true if text is available in the clipboard.
	bool is_clipboard_text_available() const;

	/// \brief Returns true if an image is available in the clipboard.
	bool is_clipboard_image_available() const;

	/// \brief Returns the text stored in the clipboard.
	CL_String get_clipboard_text() const;

	/// \brief Returns an image stored in the clipboard. 
	/// <p>Returns a null pixelbuffer if no image is available.</p>
	CL_PixelBuffer get_clipboard_image() const;

	/// \brief Returns the minimum size the window can be resized to by the application user.
	CL_Size get_minimum_size(bool client_area=false);

	/// \brief Returns the maximum size the window can be resized to by the application user.
	CL_Size get_maximum_size(bool client_area=false);

	/// \brief Returns the window title.
	CL_String get_title() const;

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
	CL_Point client_to_screen(const CL_Point &client);

	/// \brief Convert from screen coordinates to client coordinates.
	CL_Point screen_to_client(const CL_Point &screen);

	/// \brief Capture/Release the mouse.
	void capture_mouse(bool capture);

	/// \brief Invalidates a region of a screen, causing a repaint.
	void request_repaint(const CL_Rect &rect);

	/// \brief Change window title.
	void set_title(const CL_StringRef &title);

	/// \brief Set window position and size.
	///
	/// \param pos = Window position and size.
	/// \param client_area = true - Position relative to window client area
	void set_position(const CL_Rect &pos, bool client_area);

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
	void update(const CL_Rect &rect);

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
	void set_cursor(const CL_Cursor &cursor);

	/// \brief Set cursor
	///
	/// \param type = Standard Cursor
	void set_cursor(CL_StandardCursor type);

	/// \brief Hides the mouse cursor.
	void hide_cursor();

	/// \brief Stores text in the clipboard.
	void set_clipboard_text(const CL_StringRef &text);

	/// \brief Stores an image in the clipboard.
	void set_clipboard_image(const CL_PixelBuffer &buf);

	/// \brief Sets the large icon used for this window.
	void set_large_icon(const CL_PixelBuffer &image);

	/// \brief Sets the small icon used for this window.
	void set_small_icon(const CL_PixelBuffer &image);

/// \}
/// \name Implementation
/// \{
private:
	CL_SharedPtr<CL_DisplayWindow_Impl> impl;

/// \}
};

/// \}
